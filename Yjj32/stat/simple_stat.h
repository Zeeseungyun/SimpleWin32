#pragma once
#include <util/helper_macros.h>
#include <chrono>

namespace yjj {

    struct simple_stat_base {
        typedef typename std::ratio<60, 1>::type min_ratio;
        typedef typename std::ratio<1, 1>::type sec_ratio;
        typedef std::milli milli_sec_ratio;
        typedef std::nano nano_sec_ratio;

    private:
        template<typename T, typename ToRatioT, typename FromRatioT>
        struct from_to_impl {
            static constexpr T op(T v) noexcept {
                return (v * FromRatioT::num / FromRatioT::den) * ToRatioT::den / ToRatioT::num;
            }
        };

        template<typename T, typename SameRatioT>
        struct from_to_impl<T, SameRatioT, SameRatioT> {
            static constexpr T op(T v) noexcept {
                return v;
            }
        };

    public:
        template<typename T, typename ToRatioT, typename FromRatioT>
        static constexpr std::enable_if_t<std::is_arithmetic<T>::value, T>
            from_to(T v) {
            return from_to_impl<T, ToRatioT, FromRatioT>::op(v);
        }

#define ZEE_DEFINE_CONVERT_FROM_TO(from, to)                                    \
        template<typename T>                                                    \
        static constexpr std::enable_if_t<std::is_arithmetic<T>::value, T>      \
            from##_to_##to(T v) {                                               \
                return from_to<T, to##_ratio, from##_ratio>(v);                 \
        }

        ZEE_DEFINE_CONVERT_FROM_TO(nano_sec, min);
        ZEE_DEFINE_CONVERT_FROM_TO(nano_sec, sec);
        ZEE_DEFINE_CONVERT_FROM_TO(nano_sec, milli_sec);
        ZEE_DEFINE_CONVERT_FROM_TO(nano_sec, nano_sec);

        ZEE_DEFINE_CONVERT_FROM_TO(milli_sec, min);
        ZEE_DEFINE_CONVERT_FROM_TO(milli_sec, sec);
        ZEE_DEFINE_CONVERT_FROM_TO(milli_sec, milli_sec);
        ZEE_DEFINE_CONVERT_FROM_TO(milli_sec, nano_sec);

        ZEE_DEFINE_CONVERT_FROM_TO(sec, min);
        ZEE_DEFINE_CONVERT_FROM_TO(sec, sec);
        ZEE_DEFINE_CONVERT_FROM_TO(sec, milli_sec);
        ZEE_DEFINE_CONVERT_FROM_TO(sec, nano_sec);

        ZEE_DEFINE_CONVERT_FROM_TO(min, min);
        ZEE_DEFINE_CONVERT_FROM_TO(min, sec);
        ZEE_DEFINE_CONVERT_FROM_TO(min, milli_sec);
        ZEE_DEFINE_CONVERT_FROM_TO(min, nano_sec);

#undef ZEE_DEFINE_CONVERT_FROM_TO
    };

    template<typename ClockT = std::chrono::steady_clock>
    struct simple_stat : simple_stat_base {
        ZEE_DEFINE_REMOVE_MOVE_AND_COPY_FUNCTION(simple_stat);
        typedef ClockT clock_type;
        typedef float default_return_type;

        simple_stat()
            : started_(clock_type::now()) {
        }

        ~simple_stat() = default;

        void reset() {
            started_ = clock_type::now();
        }

    private:
        template<typename T, typename PeriodT>
        T impl() const {
            return std::chrono::duration_cast<std::chrono::duration<T, PeriodT>>(clock_type::now() - started_).count();
        }

    public:
        template<typename T = default_return_type>
        std::enable_if_t<std::is_arithmetic<T>::value, T>
            nano_sec() const {
            return impl<T, typename simple_stat_base::nano_sec_ratio>();
        }

        template<typename T = default_return_type>
        std::enable_if_t<std::is_arithmetic<T>::value, T>
            mili_sec() const {
            return impl<T, typename simple_stat_base::milli_sec_ratio>();
        }

        template<typename T = default_return_type>
        std::enable_if_t<std::is_arithmetic<T>::value, T>
            sec() const {
            return impl<T, typename simple_stat_base::sec_ratio>();
        }

        template<typename T = default_return_type>
        std::enable_if_t<std::is_arithmetic<T>::value, T>
            min() const {
            return impl<T, typename simple_stat_base::min_ratio>();
        }

        template<typename T = default_return_type, typename PeriodT = typename simple_stat_base::milli_sec_ratio>
        std::chrono::duration<T, PeriodT> duration() const {
            return std::chrono::duration_cast<std::chrono::duration<T, PeriodT>>(clock_type::now() - started_);
        }

    private:
        std::chrono::time_point<clock_type> started_;
    };

}//namespace yjj