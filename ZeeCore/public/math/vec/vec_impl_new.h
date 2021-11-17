#pragma once
#include "vec_decl_new.h"
#include "../../util/type_supports.h"
#include "../common.h"

namespace zee {
namespace math {
namespace impl {
	struct legal_vec {

	};

	struct illegal_vec {
		static constexpr size_t component_size = 0;
		typedef void element_type;
	};

	template<size_t CompSize, typename T>
	struct vec_impl : legal_vec {
		static constexpr size_t component_size = CompSize;
		typedef T element_type;

		element_type data[component_size];

		template<typename... Args>
		constexpr vec_impl(Args&&... args) noexcept 
			: data ((element_type)(args)...) {

		}

		template<typename OtherElemT>
		constexpr vec_impl(const vec_impl<component_size, OtherElemT>& other) noexcept
			: vec_impl() {
			for (size_t i = 0; i != component_size; ++i) {
				data[i] = (element_type)other.data[i];
			}
		}

		vec_impl& operator=(const vec_impl<component_size, element_type>& other) noexcept {
			if (this != &other) {
				for (size_t i = 0; i != component_size; ++i) {
					data[i] = other.data[i];
				}
			}
			return *this;
		}

		template<typename OtherElemT>
		vec_impl& operator=(const vec_impl<component_size, OtherElemT>& other) noexcept {
			for (size_t i = 0; i != component_size; ++i) {
				data[i] = (element_type)other.data[i];
			}
			return *this;
		}

		element_type& at(size_t idx) noexcept { return data[idx]; }
		constexpr const element_type& at(size_t idx) const noexcept { return data[idx]; }

		element_type& operator[](size_t idx) noexcept { return at(idx); }
		constexpr const element_type& operator[](size_t idx) const noexcept { return at(idx); }

		constexpr bool is_zero() const noexcept {
			for (const auto& elem : data) {
				if (!is_zero(elem)) {
					return false;
				}
			}
			return true;
		}

		template<typename OtherElemT>
		constexpr bool is_equal(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			for (size_t i = 0; i != component_size; ++i) {
				if (data[i] != other.data[i]) {
					return false;
				}
			}
			return true;
		}

		template<typename EpsT = element_type>
		bool is_near_zero(EpsT eps = epsilon<EpsT>()) const noexcept {
			for (const auto& elem : data) {
				if (!is_near_zero(elem, eps)) {
					return false;
				}
			}
			return true;
		}

		template<typename OtherElemT, typename EpsT = promotion_t<element_type, OtherElemT>>
		bool is_near_equal(const vec_impl<component_size, OtherElemT>& other, EpsT eps = epsilon<EpsT>()) const noexcept {
			for (size_t i = 0; i != component_size; ++i) {
				if (!is_near_equal(data[i], other.data[i], eps) {
					return false;
				}
			}
			return true;
		}

		template<typename OtherElemT>
		promotion_t<OtherElemT, element_type> 
			dot(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			promotion_t<OtherElemT, element_type> ret = 0;
			for (size_t i = 0; i != component_size; ++i) {
				ret += data[i] * other.data[i];
			}
			return ret;
		}

		element_type length_sq() const noexcept {
			return dot(*this);
		}

		promotion_t<default_floating_point_t, element_type> 
			length() const noexcept {
			return sqrt((promotion_t<default_floating_point_t, element_type>)length_sq());
		}

		constexpr vec_impl<component_size, promotion_t<default_floating_point_t, element_type>>
			reciprocal() const noexcept {
			typedef promotion_t<default_floating_point_t, element_type> promotion_t;
			vec_impl<component_size, promotion_t> ret;
			for (size_t i = 0; i != component_size; ++i) {
				ret.data[i] = (promotion_t)1 / data[i];
			}
			return ret;
		}

		vec_impl<component_size, promotion_t<default_floating_point_t, element_type>>
			normalize() const noexcept {
			typedef promotion_t<default_floating_point_t, element_type> promotion_t;
			return mul(reciprocal(length()));
		}
		
		bool is_normalize() const noexcept {
			return length_sq() == 1;
		}
		
		template<typename EpsT = element_type>
		bool is_near_normalize(EpsT eps = epsilon<EpsT>()) const noexcept {
			return is_near_equal(length_sq(), (EpsT)1, eps);
		}

		template<typename OtherElemT>
		constexpr vec_impl<component_size, promotion_t<element_type, OtherElemT>> 
			add(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			typedef promotion_t<element_type, OtherElemT> promotion_t;
			vec_impl<component_size, promotion_t> ret;
			for (size_t i = 0; i != component_size; ++i) {
				ret[i] = data[i] + other[i];
			}
			return ret;
		}

		template<typename OtherElemT>
		constexpr vec_impl<component_size, promotion_t<element_type, OtherElemT>> 
			sub(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			typedef promotion_t<element_type, OtherElemT> promotion_t;
			vec_impl<component_size, promotion_t> ret;
			for (size_t i = 0; i != component_size; ++i) {
				ret[i] = data[i] - other[i];
			}
			return ret;
		}

		template<typename OtherElemT>
		constexpr vec_impl<component_size, promotion_t<element_type, OtherElemT>> 
			mul(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			typedef promotion_t<element_type, OtherElemT> promotion_t;
			vec_impl<component_size, promotion_t> ret;
			for (size_t i = 0; i != component_size; ++i) {
				ret[i] = data[i] * other[i];
			}
			return ret;
		}

		template<typename OtherT>
		constexpr std::enable_if_t<is_vec_element<OtherT>::value, vec_impl<component_size, promotion_t<element_type, OtherT>>>
			mul(const OtherT& other) const noexcept {
			typedef promotion_t<element_type, OtherT> promotion_t;
			vec_impl<component_size, promotion_t> ret;
			for (size_t i = 0; i != component_size; ++i) {
				ret[i] = data[i] * other;
			}
			return ret;
		}

		template<typename OtherElemT>
		constexpr vec_impl<component_size, promotion_t<element_type, OtherElemT>> 
			div(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			typedef promotion_t<element_type, OtherElemT> promotion_t;
			vec_impl<component_size, promotion_t> ret;
			for (size_t i = 0; i != component_size; ++i) {
				ret[i] = data[i] / other[i];
			}
			return ret;
		}

		template<typename OtherT>
		constexpr std::enable_if_t<is_vec_element<OtherT>::value, vec_impl<component_size, promotion_t<element_type, OtherT>>>
			div(const OtherT& other) const noexcept {
			typedef promotion_t<element_type, OtherT> promotion_t;
			return mul(reciprocal((promotion_t)other));
		}

		template<typename OtherElemT>
		promotion_t<element_type, OtherElemT> 
			distance_sq(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return sub(other).length_sq();
		}

		template<typename OtherElemT>
		promotion_t<default_floating_point_t, element_type, OtherElemT> 
			distance(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return sqrt(distance_sq(other));
		}

		template<typename OtherElemT>
		promotion_t<default_floating_point_t, element_type, OtherElemT> 
			reflect(const vec_impl<component_size, OtherElemT>& nor) const noexcept {
			//v - 2 * dot(v, n) * n
			return sub(nor.mul(dot(nor)).mul(2));
		}
	};

	template<typename T>
	struct vec_impl<2, T> : legal_vec {
		static constexpr size_t component_size = 2;
		typedef T element_type;

		union {
			struct {
				element_type x, y;
			};
			element_type data[component_size];
		};

		template<typename OtherElemT, std::enable_if_t<is_vec_element<OtherElemT>::value, int> = 0>
		constexpr vec_impl(OtherElemT new_x, OtherElemT new_y) noexcept
			: data{ (element_type)new_x, (element_type)new_y } {
		}

		template<typename OtherElemT>
		constexpr vec_impl(const vec_impl<component_size, OtherElemT>& other) noexcept
			: vec_impl(other.data[0], other.data[1]) {
		}

		vec_impl& operator=(const vec_impl<component_size, element_type>& other) noexcept {
			if (this != &other) {
				for (size_t i = 0; i != component_size; ++i) {
					data[i] = other.data[i];
				}
			}
			return *this;
		}

		template<typename OtherElemT>
		vec_impl& operator=(const vec_impl<component_size, OtherElemT>& other) noexcept {
			for (size_t i = 0; i != component_size; ++i) {
				data[i] = (element_type)other.data[i];
			}
			return *this;
		}

		element_type& at(size_t idx) noexcept { return data[idx]; }
		constexpr const element_type& at(size_t idx) const noexcept { return data[idx]; }

		element_type& operator[](size_t idx) noexcept { return at(idx); }
		constexpr const element_type& operator[](size_t idx) const noexcept { return at(idx); }

		constexpr bool is_zero() const noexcept {
			return data[0] == 0 
				&& data[1] == 0;
		}

		template<typename OtherElemT>
		constexpr bool is_equal(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return data[0] == other.data[0]
				&& data[1] == other.data[1];
		}

		template<typename EpsT = element_type>
		bool is_near_zero(EpsT eps = epsilon<EpsT>()) const noexcept {
			return is_near_zero(data[0], eps)
				&& is_near_zero(data[1], eps);
		}

		template<typename OtherElemT, typename EpsT = promotion_t<element_type, OtherElemT>>
		bool is_near_equal(const vec_impl<component_size, OtherElemT>& other, EpsT eps = epsilon<EpsT>()) const noexcept {
			return is_near_equal(data[0], other.data[0], eps)
				&& is_near_equal(data[1], other.data[1], eps);
		}

		template<typename OtherElemT>
		constexpr promotion_t<OtherElemT, element_type> dot(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return data[0] * other.data[0]
				 + data[1] * other.data[1];
		}

		constexpr element_type length_sq() const noexcept {
			return dot(*this);
		}

		promotion_t<default_floating_point_t, element_type> length() const noexcept {
			return sqrt((promotion_t<default_floating_point_t, element_type>)length_sq());
		}

		constexpr vec_impl<component_size, promotion_t<default_floating_point_t, element_type>>
			reciprocal() const noexcept {
			typedef promotion_t<default_floating_point_t, element_type> promotion_t;
			return  {
				(promotion_t)1 / data[0],
				(promotion_t)1 / data[1],
			};
		}

		vec_impl<component_size, promotion_t<default_floating_point_t, element_type>>
			normalize() const noexcept {
			typedef promotion_t<default_floating_point_t, element_type> promotion_t;
			return mul(reciprocal(length()));
		}

		constexpr bool is_normalize() const noexcept {
			return length_sq() == 1;
		}

		template<typename EpsT = element_type>
		bool is_near_normalize(EpsT eps = epsilon<EpsT>()) const noexcept {
			return is_near_equal(length_sq(), (EpsT)1, eps);
		}

		template<typename OtherElemT>
		constexpr vec_impl<component_size, promotion_t<element_type, OtherElemT>> 
			add(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return  {
				data[0] + other.data[0],
				data[1] + other.data[1],
			};
		}

		template<typename OtherElemT>
		constexpr vec_impl<component_size, promotion_t<element_type, OtherElemT>> 
			sub(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return  {
				data[0] - other.data[0],
				data[1] - other.data[1],
			};
		}

		template<typename OtherElemT>
		constexpr vec_impl<component_size, promotion_t<element_type, OtherElemT>> 
			mul(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return  {
				data[0] * other.data[0],
				data[1] * other.data[1],
			};
		}

		template<typename OtherT>
		constexpr std::enable_if_t<is_vec_element<OtherT>::value, vec_impl<component_size, promotion_t<element_type, OtherT>>>
			mul(const OtherT& other) const noexcept {
			return  {
				data[0] * other,
				data[1] * other,
			};
		}

		template<typename OtherElemT>
		constexpr vec_impl<component_size, promotion_t<element_type, OtherElemT>> 
			div(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return  {
				data[0] / other.data[0],
				data[1] / other.data[1],
			};
		}

		template<typename OtherT>
		constexpr std::enable_if_t<is_vec_element<OtherT>::value, vec_impl<component_size, promotion_t<element_type, OtherT>>>
			div(const OtherT& other) const noexcept {
			typedef promotion_t<element_type, OtherT> promotion_t;
			return mul(reciprocal((promotion_t)other));
		}

		template<typename OtherElemT>
		constexpr promotion_t<element_type, OtherElemT> 
			distance_sq(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return sub(other).length_sq();
		}

		template<typename OtherElemT>
		constexpr promotion_t<default_floating_point_t, element_type, OtherElemT> 
			distance(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return sqrt(distance_sq(other));
		}

		template<typename OtherElemT>
		constexpr promotion_t<default_floating_point_t, element_type, OtherElemT> 
			reflect(const vec_impl<component_size, OtherElemT>& nor) const noexcept {
			//v - 2 * dot(v, n) * n
			return sub(nor.mul(dot(nor)).mul(2));
		}
	};

}//namespace zee::math::impl

	template<size_t CompSize, typename T> 
	struct vec : std::conditional_t<is_vec_element<T>::value && (CompSize > 0), impl::vec_impl<CompSize, T>, impl::illegal_vec> { 
		
	};
	
	template<typename T>
	struct vec<0, T> : impl::illegal_vec { };

}//namespace zee::math
}//namespace zee