#pragma once
#include <util/helper_macros.h>
#include <chrono>

namespace zee {
    struct simple_stat {
        ZEE_DEFINE_REMOVE_MOVE_AND_COPY_FUNCTION(simple_stat);
        typedef std::chrono::high_resolution_clock clock_type;

        simple_stat()
            : started_(clock_type::now()) {
        }

        ~simple_stat() = default;

        void reset() {
            started_ = clock_type::now();
        }

    private:
        template<typename T, typename PriodT>
        T impl() const {
            return duration_cast<std::chrono::duration<T, PriodT>>(clock_type::now() - started_).count();
        }

    public:
        template<typename T = size_t>
        std::enable_if_t<std::is_arithmetic<T>::value, T>
            nano_sec() const {
            return impl<T, std::nano>();
        }

        template<typename T = size_t>
        std::enable_if_t<std::is_arithmetic<T>::value, T>
            mili_sec() const {
            return impl<T, std::milli>();
        }

        template<typename T = size_t>
        std::enable_if_t<std::is_arithmetic<T>::value, T>
            sec() const {
            return impl<T, std::ratio<1, 1>>();
        }

        template<typename T = size_t>
        std::enable_if_t<std::is_arithmetic<T>::value, T>
            min() const {
            return impl<T, std::ratio<60, 1>>();
        }

    private:
        std::chrono::time_point<clock_type> started_;
    };

}//namespace zee