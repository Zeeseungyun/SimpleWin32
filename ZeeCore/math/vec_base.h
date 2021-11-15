#pragma once
#include "../core/core_base.h"
#include "common.h"

namespace zee {
namespace math {
namespace impl {
	template<size_t CompSize, typename VecElemT>
	struct vec_base {
		typedef VecElemT element_type;
		static constexpr size_t component_size = CompSize;
		union {
			element_type data[component_size];
		};

		element_type& operator[](size_t idx) noexcept { return data[idx]; }
		constexpr const element_type& operator[](size_t idx) const noexcept { return data[idx]; }

		template<typename...Args>
		constexpr vec_base(Args&&...args) noexcept
			: data{std::forward<Args>(args)...} {
			static_assert(sizeof...(args) <= component_size, "invalid component size.");
		}

		template<typename...Args>
		constexpr vec_base() noexcept
			: data { 0 } {
		}

		template<typename OtherElemT>
		vec_base(const vec_base<component_size, OtherElemT>& other) noexcept
			: vec_base() {
			for (size_t i = 0; i != component_size; ++i) {
				data[i] = other.data[i];
			}
		}

		template<typename OtherElemT>
		vec_base& operator=(const vec_base<component_size, OtherElemT>& other) noexcept {
			for (size_t i = 0; i != component_size; ++i) {
				data[i] = other.data[i];
			}
			return *this;
		}
		
		vec_base& operator=(const vec_base& other) noexcept {
			if (this != &other) {
				for (size_t i = 0; i != component_size; ++i) {
					data[i] = other.data[i];
				}
			}
			return *this;
		}

		struct constants;
	};

	template<size_t CompSize, typename VecElemT>
	struct vec_base<CompSize, VecElemT>::constants {
		static constexpr vec_base<CompSize, VecElemT> zero = {};
	};

	template<typename VecElemT>
	struct vec_base<2, VecElemT> {
		typedef VecElemT element_type;
		static constexpr size_t component_size = 2;
		union {
			element_type x, y;
			element_type data[component_size];
		};

		constexpr element_type& operator[](size_t idx) noexcept { return data[idx]; }
		constexpr const element_type& operator[](size_t idx) const noexcept { return data[idx]; }

		constexpr vec_base() noexcept
			: vec_base(0, 0) {
		}

		constexpr vec_base(element_type new_x, element_type new_y) noexcept
			: data{ new_x, new_y } {
		}

		template<typename OtherElemT>
		constexpr vec_base(const vec_base<component_size, OtherElemT>& other) noexcept 
			: vec_base(other[0], other[1]) {
		}

		template<typename OtherElemT>
		constexpr vec_base& operator=(const vec_base<component_size, OtherElemT>& other) noexcept {
			data[0] = other[0];
			data[1] = other[1];
			return *this;
		}

		constexpr vec_base& operator=(const vec_base& other) noexcept {
			if (this != &other) {
				data[0] = other[0];
				data[1] = other[1];
			}

			return *this;
		}

		struct constants;
	};
	


	template<typename CheckT, typename ...Args>
	struct is_valid_vec : std::conditional_t< is_valid_vec<CheckT>::value && is_valid_vec<Args...>::value, std::true_type, std::false_type> { 
	};

	template<typename CheckT>
	struct is_valid_vec<CheckT> {
	private:
		typedef char yes[2];
		typedef char no[1];

		template<typename T> static
			std::enable_if_t<std::is_signed<typename T::element_type>::value && (T::component_size > 0), yes&>
			check(T*);

		template<typename T> static no& check(...);

	public:
		static constexpr bool value = sizeof(check<CheckT>(0)) == sizeof(yes);
	};

	template<typename CheckT, typename ...Args>
	struct is_valid_vec_elem : std::conditional_t< is_valid_vec_elem<CheckT>::value&& is_valid_vec_elem<Args...>::value, std::true_type, std::false_type> {
	};

	template<typename CheckT>
	struct is_valid_vec_elem<CheckT> {
	private:
		typedef char yes[2];
		typedef char no[1];

		template<typename T> static
			std::enable_if_t<std::is_signed<T>::value, yes&> 
			check(T*);

		template<typename T> static no& check(...);

	public:
		static constexpr bool value = sizeof(check<CheckT>(0)) == sizeof(yes);
	};

	//Constants definitions

	template<typename VecElemT>
	struct vec_base<2, VecElemT>::constants {
		static constexpr vec_base<2, VecElemT> zero = {};
		static constexpr vec_base<2, VecElemT> one    = { 1, 1 };
		static constexpr vec_base<2, VecElemT> unit_x = { 1, 0 };
		static constexpr vec_base<2, VecElemT> unit_y = { 0, 1 };
	};

	//template<typename VecElemT>
	//struct vec_base<3, VecElemT>::constants {
	//	static constexpr vec_base<3, VecElemT> zero = {};
	//	static constexpr vec_base<3, VecElemT> one    = { 1, 1, 1 };
	//	static constexpr vec_base<3, VecElemT> unit_x = { 1, 0, 0 };
	//	static constexpr vec_base<3, VecElemT> unit_y = { 0, 1, 0 };
	//	static constexpr vec_base<3, VecElemT> unit_z = { 0, 0, 1 };
	//};
	
	//template<typename VecElemT>
	//struct vec_base<4, VecElemT>::constants {
	//	static constexpr vec_base<4, VecElemT> zero = {};
	//	static constexpr vec_base<4, VecElemT> one    = { 1, 1, 1, 1 };
	//	static constexpr vec_base<4, VecElemT> unit_x = { 1, 0, 0, 0 };
	//	static constexpr vec_base<4, VecElemT> unit_y = { 0, 1, 0, 0 };
	//	static constexpr vec_base<4, VecElemT> unit_z = { 0, 0, 1, 0 };
	//	static constexpr vec_base<4, VecElemT> unit_w = { 0, 0, 0, 1 };
	//};

}//namespace zee::math::impl
	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<impl::is_valid_vec_elem<LeftElemT, RightElemT>::value && (CompSize > 0), bool>
		is_equal(const impl::vec_base<CompSize, LeftElemT>& l, const impl::vec_base<CompSize, RightElemT>& r) {
		for (size_t i = 0; i != CompSize; ++i) {
			if (!is_equal(l[i], r[i])) {
				return false;
			}
		}
		 
		return true;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<impl::is_valid_vec_elem<LeftElemT, RightElemT>::value && (CompSize > 0), bool>
		is_not_equal(const impl::vec_base<CompSize, LeftElemT>& l, const impl::vec_base<CompSize, RightElemT>& r) {
		return !is_equal(l, r);
	}

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<impl::is_valid_vec_elem<VecElemT>::value && (CompSize > 0), bool>
		is_zero(const impl::vec_base<CompSize, VecElemT>& v) {
		for (size_t i = 0; i != CompSize; ++i) {
			if (!is_zero(v[i])) {
				return false;
			}
		}

		return true;
	}

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<impl::is_valid_vec_elem<VecElemT>::value && (CompSize > 0), bool>
		is_not_zero(const impl::vec_base<CompSize, VecElemT>& v) {
		return !is_zero(v);
	}

	void test() {
		constexpr impl::vec_base<2, int> v1 = { 1,2 };
		constexpr auto temp = v1[0];

		constexpr impl::vec_base<2, int> v2 = { 1,2 };
		constexpr bool t1 = is_equal(v1, v2);
		constexpr bool t2 = is_not_equal(v1, v2);
		constexpr bool t3 = is_zero(v1);
		constexpr impl::vec_base<2, int> v3 = { 0,0 };
		constexpr bool t4 = is_zero(v3);
	}
}//namespace zee::math
}//namespace zee 