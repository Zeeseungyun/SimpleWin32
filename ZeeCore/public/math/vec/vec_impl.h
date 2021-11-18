#pragma once
#include "vec_decl.h"
#include "../../util/type_supports.h"
#include "../common.h"

namespace zee {
namespace math {
namespace impl {
	struct axis_idx {
		enum {
			x,y,z,w
		};
	};

	struct legal_vec { };
	
	struct illegal_vec {
	private:
		struct dummy {};

	public:
		static constexpr size_t component_size = 0;
		typedef void element_type;

	private:
		illegal_vec(dummy) {}
	};

	template<size_t CompSize, typename T, bool IsValidElem = is_vec_argument<CompSize, T>::value>	struct vec_impl;

	template<size_t CompSize, typename T>
	constexpr const vec_impl<CompSize, T, true>& 
		operator+(const vec_impl<CompSize, T, true>& v) noexcept {
		return v;
	}

	template<size_t CompSize, typename T> 
	constexpr vec_impl<CompSize, T> 
		operator-(const vec_impl<CompSize, T, true>& v) noexcept {
		vec_impl<CompSize, T, true> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret.data[i] = -v.data[i];
		}
		return ret;
	}

	template<typename T>
	constexpr vec_impl<2, T>
		operator-(const vec_impl<2, T, true>& v) noexcept {
		return {
			-v.data[0],
			-v.data[1]
		};
	}

	template<typename T>
	constexpr vec_impl<3, T>
		operator-(const vec_impl<3, T, true>& v) noexcept {
		return {
			-v.data[0],
			-v.data[1],
			-v.data[2]
		};
	}

	template<typename T>
	constexpr vec_impl<4, T>
		operator-(const vec_impl<4, T, true>& v) noexcept {
		return {
			-v.data[0],
			-v.data[1],
			-v.data[2],
			-v.data[3]
		};
	}

	template<size_t CompSize, typename T, typename U>
	constexpr bool
		operator==(const vec_impl<CompSize, T, true>& l , const vec_impl<CompSize, U, true>& r) noexcept {
		return l.is_equal(r);
	}

	template<size_t CompSize, typename T, typename U>
	constexpr bool
		operator!=(const vec_impl<CompSize, T, true>& l, const vec_impl<CompSize, U, true>& r) noexcept {
		return !(l == r);
	}

	template<size_t CompSize, typename T, typename U>
	constexpr vec_impl<CompSize, promotion_t<T, U>>
		operator-(const vec_impl<CompSize, T, true>& l, const vec_impl<CompSize, U, true>& r) noexcept {
		return l.sub(r);
	}

	template<size_t CompSize, typename T, typename U>
	constexpr vec_impl<CompSize, promotion_t<T, U>> 
		operator+(const vec_impl<CompSize, T, true>& l, const vec_impl<CompSize, U, true>& r) noexcept {
		return l.add(r);
	}

	template<size_t CompSize, typename T, typename U>
	constexpr vec_impl<CompSize, promotion_t<T, U>> 
		operator*(const vec_impl<CompSize, T, true>& l, const vec_impl<CompSize, U, true>& r) noexcept {
		return l.mul(r);
	}

	template<size_t CompSize, typename T, typename U>
	constexpr std::enable_if_t<is_vec_element<U>::value, vec_impl<CompSize, promotion_t<T, U>>>
		operator*(const vec_impl<CompSize, T, true>& l, const U& r) noexcept {
		return l.mul(r);
	}
	
	template<size_t CompSize, typename T, typename U>
	constexpr std::enable_if_t<is_vec_element<T>::value, vec_impl<CompSize, promotion_t<T, U>>>
		operator*(const T& l, const vec_impl<CompSize, U, true>& r) noexcept {
		return r.mul(l);
	}

	template<size_t CompSize, typename T, typename U>
	constexpr vec_impl<CompSize, promotion_t<T, U>>
		operator/(const vec_impl<CompSize, T, true>& l, const vec_impl<CompSize, U, true>& r) noexcept {
		return l.div(r);
	}

	template<size_t CompSize, typename T, typename U>
	constexpr std::enable_if_t<is_vec_element<U>::value, vec_impl<CompSize, promotion_t<T, U>>>
		operator/(const vec_impl<CompSize, T, true>& l, const U& r) noexcept {
		return l.div(r);
	}

	template<size_t CompSize, typename T, typename U>
	constexpr std::enable_if_t<is_vec_element<T>::value, vec_impl<CompSize, promotion_t<T, U>>>
		operator/(const T& l, const vec_impl<CompSize, U, true>& r) noexcept {
		vec_impl<CompSize, promotion_t<T, U>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret.data[i] = l / r.data[i];
		}
		return ret;
	}

	template<typename T, typename U>
	constexpr std::enable_if_t<is_vec_element<T>::value, vec_impl<4, promotion_t<T, U>>>
		operator/(const T& l, const vec_impl<4, U, true>& r) noexcept {
		return {
			l / r.data[0],
			l / r.data[1],
			l / r.data[2],
			l / r.data[3]
		};
	}

	template<typename T, typename U>
	constexpr std::enable_if_t<is_vec_element<T>::value, vec_impl<3, promotion_t<T, U>>>
		operator/(const T& l, const vec_impl<3, U, true>& r) noexcept {
		return {
			l / r.data[0],
			l / r.data[1],
			l / r.data[2]
		};
	}

	template<typename T, typename U>
	constexpr std::enable_if_t<is_vec_element<T>::value, vec_impl<2, promotion_t<T, U>>>
		operator/(const T& l, const vec_impl<2, U, true>& r) noexcept {
		return {
			l / r.data[0],
			l / r.data[1]
		};
	}

	template<size_t CompSize, typename T, typename U>
	vec_impl<CompSize, T>&
		operator*=(vec_impl<CompSize, T, true>& l, const vec_impl<CompSize, U, true>& r) noexcept {
		for (size_t i = 0; i != CompSize; ++i) {
			l.data[i] *= r.data[i];
		}
		return l;
	}

	template<typename T, typename U>
	vec_impl<4, T>&
		operator*=(vec_impl<4, T, true>& l, const vec_impl<4, U, true>& r) noexcept {
		l.data[0] *= r.data[0];
		l.data[1] *= r.data[1];
		l.data[2] *= r.data[2];
		l.data[3] *= r.data[3];
		return l;
	}

	template<typename T, typename U>
	vec_impl<3, T>&
		operator*=(vec_impl<3, T, true>& l, const vec_impl<3, U, true>& r) noexcept {
		l.data[0] *= r.data[0];
		l.data[1] *= r.data[1];
		l.data[2] *= r.data[2];
		return l;
	}

	template<typename T, typename U>
	vec_impl<2, T>&
		operator*=(vec_impl<2, T, true>& l, const vec_impl<2, U, true>& r) noexcept {
		l.data[0] *= r.data[0];
		l.data[1] *= r.data[1];
		return l;
	}

	template<size_t CompSize, typename T, typename U>
	std::enable_if_t<is_vec_element<U>::value, vec_impl<CompSize, T>&>
		operator*=(vec_impl<CompSize, T, true>& l, U r) noexcept {
		for (size_t i = 0; i != CompSize; ++i) {
			l.data[i] *= r;
		}
		return l;
	}

	template<typename T, typename U>
	std::enable_if_t<is_vec_element<U>::value, vec_impl<4, T>&>
		operator*=(vec_impl<4, T, true>& l, U r) noexcept {
		l.data[0] *= r;
		l.data[1] *= r;
		l.data[2] *= r;
		l.data[3] *= r;
		return l;
	}

	template<typename T, typename U>
	std::enable_if_t<is_vec_element<U>::value, vec_impl<3, T>&>
		operator*=(vec_impl<3, T, true>& l, U r) noexcept {
		l.data[0] *= r;
		l.data[1] *= r;
		l.data[2] *= r;
		return l;
	}

	template<typename T, typename U>
	std::enable_if_t<is_vec_element<U>::value, vec_impl<2, T>&>
		operator*=(vec_impl<2, T, true>& l, U r) noexcept {
		l.data[0] *= r;
		l.data[1] *= r;
		return l;
	}

	template<size_t CompSize, typename T, typename U>
	vec_impl<CompSize, T>&
		operator/=(vec_impl<CompSize, T, true>& l, const vec_impl<CompSize, U, true>& r) noexcept {
		for (size_t i = 0; i != CompSize; ++i) {
			l.data[i] /= r.data[i];
		}
		return l;
	}

	template<size_t CompSize, typename T, typename U>
	vec_impl<4, T>&
		operator/=(vec_impl<4, T, true>& l, const vec_impl<4, U, true>& r) noexcept {
		l.data[0] *= r.data[0];
		l.data[1] *= r.data[1];
		l.data[2] *= r.data[2];
		l.data[3] *= r.data[3];
		return l;
	}

	template<size_t CompSize, typename T, typename U>
	vec_impl<3, T>&
		operator/=(vec_impl<3, T, true>& l, const vec_impl<3, U, true>& r) noexcept {
		l.data[0] *= r.data[0];
		l.data[1] *= r.data[1];
		l.data[2] *= r.data[2];
		return l;
	}

	template<size_t CompSize, typename T, typename U>
	vec_impl<2, T>&
		operator/=(vec_impl<2, T, true>& l, const vec_impl<2, U, true>& r) noexcept {
		l.data[0] *= r.data[0];
		l.data[1] *= r.data[1];
		return l;
	}

	template<size_t CompSize, typename T, typename U>
	std::enable_if_t<is_vec_element<U>::value, vec_impl<CompSize, T>&>
		operator/=(vec_impl<CompSize, T, true>& l, U r) noexcept {
		return l *= reciprocal(r);
	}

	template<size_t CompSize, typename T>
	struct vec_impl<CompSize, T, false> : illegal_vec { };

	template<size_t CompSize, typename T>
	struct vec_impl<CompSize, T, true> : legal_vec {
		static constexpr size_t component_size = CompSize;
		typedef T element_type;

		element_type data[component_size];

		template<typename... Args>
		constexpr vec_impl(Args&&... args) noexcept 
			: data{ (element_type)(args)... } {
		}

		template<typename OtherElemT>
		constexpr vec_impl(const vec_impl<component_size, OtherElemT, true>& other) noexcept
			: vec_impl() {
			for (size_t i = 0; i != component_size; ++i) {
				data[i] = (element_type)other.data[i];
			}
		}

		vec_impl& operator=(const vec_impl& other) noexcept {
			if (this != &other) {
				for (size_t i = 0; i != component_size; ++i) {
					data[i] = other.data[i];
				}
			}
			return *this;
		}

		template<typename OtherElemT>
		vec_impl& operator=(const vec_impl<component_size, OtherElemT, true>& other) noexcept {
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
			for (size_t i = 0; i != component_size; ++i) {
				if (!math::is_zero(data[i])) {
					return false;
				}
			}
			return true;
		}

		template<typename OtherElemT>
		constexpr bool is_equal(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
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
				if (!math::is_near_zero(elem, eps)) {
					return false;
				}
			}
			return true;
		}

		template<typename OtherElemT, typename EpsT = promotion_t<element_type, OtherElemT>>
		bool is_near_equal(const vec_impl<component_size, OtherElemT, true>& other, EpsT eps = epsilon<EpsT>()) const noexcept {
			for (size_t i = 0; i != component_size; ++i) {
				if (!math::is_near_equal(data[i], other.data[i], eps)) {
					return false;
				}
			}
			return true;
		}

		template<typename OtherElemT>
		auto dot(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			promotion_t<OtherElemT, element_type> ret = 0;
			for (size_t i = 0; i != component_size; ++i) {
				ret += data[i] * other.data[i];
			}
			return ret;
		}

		element_type length_sq() const noexcept {
			return dot(*this);
		}

		auto length() const noexcept {
			return sqrt((promotion_t<default_floating_point_t, element_type>)length_sq());
		}

		constexpr auto reciprocal() const noexcept {
			typedef promotion_t<default_floating_point_t, element_type> promotion_t;
			vec_impl<component_size, promotion_t> ret;
			for (size_t i = 0; i != component_size; ++i) {
				ret.data[i] = (promotion_t)1 / data[i];
			}
			return ret;
		}

		auto normalize() const noexcept {
			return mul(math::reciprocal(length()));
		}
		
		bool is_normalize() const noexcept {
			return length_sq() == 1;
		}
		
		template<typename EpsT = element_type>
		bool is_near_normalize(EpsT eps = epsilon<EpsT>()) const noexcept {
			return math::is_near_equal(length_sq(), (EpsT)1, eps);
		}

		template<typename OtherElemT>
		constexpr auto add(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			typedef promotion_t<element_type, OtherElemT> promotion_t;
			vec_impl<component_size, promotion_t> ret;
			for (size_t i = 0; i != component_size; ++i) {
				ret.data[i] = data[i] + other[i];
			}
			return ret;
		}

		template<typename OtherElemT>
		constexpr auto sub(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			typedef promotion_t<element_type, OtherElemT> promotion_t;
			vec_impl<component_size, promotion_t> ret;
			for (size_t i = 0; i != component_size; ++i) {
				ret.data[i] = data[i] - other[i];
			}
			return ret;
		}

		template<typename OtherElemT>
		constexpr auto mul(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			typedef promotion_t<element_type, OtherElemT> promotion_t;
			vec_impl<component_size, promotion_t> ret;
			for (size_t i = 0; i != component_size; ++i) {
				ret.data[i] = data[i] * other[i];
			}
			return ret;
		}

		template<typename OtherT>
		constexpr std::enable_if_t<is_vec_element<OtherT>::value, vec_impl<component_size, promotion_t<element_type, OtherT>>>
			mul(const OtherT& other) const noexcept {
			typedef promotion_t<element_type, OtherT> promotion_t;
			vec_impl<component_size, promotion_t> ret;
			for (size_t i = 0; i != component_size; ++i) {
				ret.data[i] = data[i] * other;
			}
			return ret;
		}

		template<typename OtherElemT>
		constexpr auto div(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			typedef promotion_t<element_type, OtherElemT> promotion_t;
			vec_impl<component_size, promotion_t> ret;
			for (size_t i = 0; i != component_size; ++i) {
				ret.data[i] = data[i] / other[i];
			}
			return ret;
		}

		template<typename OtherT>
		constexpr std::enable_if_t<is_vec_element<OtherT>::value, vec_impl<component_size, promotion_t<element_type, OtherT>>>
			div(const OtherT& other) const noexcept {
			typedef promotion_t<element_type, OtherT> promotion_t;
			return mul(math::reciprocal((promotion_t)other));
		}

		template<typename OtherElemT>
		auto distance_sq(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			return sub(other).length_sq();
		}

		template<typename OtherElemT>
		auto distance(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			typedef promotion_t<default_floating_point_t, element_type, OtherElemT> promotion_t;
			return sqrt((promotion_t)distance_sq(other));
		}

		template<typename OtherElemT>
		constexpr auto reflect(const vec_impl<component_size, OtherElemT, true>& nor) const noexcept {
			return (*this) - nor * dot(nor) * 2;
		}
	};

	template<typename T>
	struct vec_impl<2, T, true> : legal_vec {
		static constexpr size_t component_size = 2;
		typedef T element_type;

		union {
			struct {
				element_type x, y;
			};
			element_type data[component_size];
		};

		constexpr vec_impl() noexcept
			: vec_impl(0, 0) {
		}

		template<typename OtherElemT, std::enable_if_t<is_vec_element<OtherElemT>::value, int> = 0>
		constexpr vec_impl(OtherElemT new_x, OtherElemT new_y) noexcept
			: data{ (element_type)new_x, (element_type)new_y } {
		}

		template<typename OtherElemT>
		constexpr vec_impl(const vec_impl<component_size, OtherElemT, true>& other) noexcept
			: vec_impl(other.data[0], other.data[1]) {
		}

		vec_impl& operator=(const vec_impl& other) noexcept {
			if (this != &other) {
				data[0] = other.data[0];
				data[1] = other.data[1];
			}
			return *this;
		}

		template<typename OtherElemT>
		vec_impl& operator=(const vec_impl<component_size, OtherElemT, true>& other) noexcept {
			data[0] = other.data[0];
			data[1] = other.data[1];
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
		constexpr bool is_equal(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			return data[0] == other.data[0]
				&& data[1] == other.data[1];
		}

		template<typename EpsT = element_type>
		bool is_near_zero(EpsT eps = epsilon<EpsT>()) const noexcept {
			return is_near_zero(data[0], eps)
				&& is_near_zero(data[1], eps);
		}

		template<typename OtherElemT, typename EpsT = promotion_t<element_type, OtherElemT>>
		bool is_near_equal(const vec_impl<component_size, OtherElemT, true>& other, EpsT eps = epsilon<EpsT>()) const noexcept {
			return is_near_equal(data[0], other.data[0], eps)
				&& is_near_equal(data[1], other.data[1], eps);
		}

		template<typename OtherElemT>
		constexpr auto dot(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			return data[0] * other.data[0]
				 + data[1] * other.data[1];
		}

		constexpr element_type length_sq() const noexcept {
			return dot(*this);
		}

		auto length() const noexcept {
			return sqrt((promotion_t<default_floating_point_t, element_type>)length_sq());
		}

		constexpr auto reciprocal() const noexcept {
			typedef promotion_t<default_floating_point_t, element_type> promotion_t;
			return vec_impl<component_size, promotion_t> {
				(promotion_t)1 / data[0],
				(promotion_t)1 / data[1]
			};
		}

		auto normalize() const noexcept {
			return mul(math::reciprocal(length()));
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
			add(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			return  {
				data[0] + other.data[0],
				data[1] + other.data[1]
			};
		}

		template<typename OtherElemT>
		constexpr 
			vec_impl<component_size, promotion_t<element_type, OtherElemT>>
			sub(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			return  {
				data[0] - other.data[0],
				data[1] - other.data[1]
			};
		}

		template<typename OtherElemT>
		constexpr vec_impl<component_size, promotion_t<element_type, OtherElemT>>
			mul(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			return  {
				data[0] * other.data[0],
				data[1] * other.data[1]
			};
		}

		template<typename OtherT>
		constexpr std::enable_if_t<is_vec_element<OtherT>::value, vec_impl<component_size, promotion_t<element_type, OtherT>>>
			mul(const OtherT& other) const noexcept {
			return  {
				data[0] * other,
				data[1] * other
			};
		}

		template<typename OtherElemT>
		constexpr vec_impl<component_size, promotion_t<element_type, OtherElemT>>
			div(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			return  {
				data[0] / other.data[0],
				data[1] / other.data[1]
			};
		}

		template<typename OtherT>
		constexpr std::enable_if_t<is_vec_element<OtherT>::value, vec_impl<component_size, promotion_t<element_type, OtherT>>>
			div(const OtherT& other) const noexcept {
			typedef promotion_t<element_type, OtherT> promotion_t;
			return mul(math::reciprocal((promotion_t)other));
		}

		template<typename OtherElemT>
		constexpr auto
			distance_sq(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			return sub(other).length_sq();
		}

		template<typename OtherElemT>
		auto distance(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			typedef promotion_t<default_floating_point_t, element_type, OtherElemT> promotion_t;
			return sqrt((promotion_t)distance_sq(other));
		}

		template<typename OtherElemT>
		constexpr auto reflect(const vec_impl<component_size, OtherElemT, true>& nor) const noexcept {
			return (*this) - nor * dot(nor) * 2;
		}

		template<typename OtherElemT>
		constexpr auto ccw(const vec<component_size, OtherElemT>& other) const noexcept {
			return data[0] * other.data[1] - other.data[0] * data[1];
		}
	};
	
	template<typename T>
	struct vec_impl<3, T, true> : legal_vec {
		static constexpr size_t component_size = 3;
		typedef T element_type;

		union {
			struct {
				element_type x, y, z;
			};
			element_type data[component_size];
		};

		constexpr vec_impl() noexcept
			: vec_impl(0, 0, 0) {
		}

		template<typename OtherElemT, std::enable_if_t<is_vec_element<OtherElemT>::value, int> = 0>
		constexpr vec_impl(OtherElemT new_x, OtherElemT new_y, OtherElemT new_z) noexcept
			: data{ (element_type)new_x, (element_type)new_y, (element_type)new_z } {
		}

		template<typename OtherElemT>
		constexpr vec_impl(const vec_impl<component_size, OtherElemT, true>& other) noexcept
			: vec_impl(other.data[0], other.data[1], other.data[2]) {
		}

		vec_impl& operator=(const vec_impl& other) noexcept {
			if (this != &other) {
				data[0] = other.data[0];
				data[1] = other.data[1];
				data[2] = other.data[2];
			}
			return *this;
		}

		template<typename OtherElemT>
		vec_impl& operator=(const vec_impl<component_size, OtherElemT, true>& other) noexcept {
			data[0] = other.data[0];
			data[1] = other.data[1];
			data[2] = other.data[2];
			return *this;
		}

		element_type& at(size_t idx) noexcept { return data[idx]; }
		constexpr const element_type& at(size_t idx) const noexcept { return data[idx]; }

		element_type& operator[](size_t idx) noexcept { return at(idx); }
		constexpr const element_type& operator[](size_t idx) const noexcept { return at(idx); }

		constexpr bool is_zero() const noexcept {
			return data[0] == 0 
				&& data[1] == 0
				&& data[2] == 0;
		}

		template<typename OtherElemT>
		constexpr bool is_equal(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			return data[0] == other.data[0]
				&& data[1] == other.data[1]
				&& data[2] == other.data[2];
		}

		template<typename EpsT = element_type>
		bool is_near_zero(EpsT eps = epsilon<EpsT>()) const noexcept {
			return is_near_zero(data[0], eps)
				&& is_near_zero(data[1], eps)
				&& is_near_zero(data[2], eps);
		}

		template<typename OtherElemT, typename EpsT = promotion_t<element_type, OtherElemT>>
		bool is_near_equal(const vec_impl<component_size, OtherElemT, true>& other, EpsT eps = epsilon<EpsT>()) const noexcept {
			return is_near_equal(data[0], other.data[0], eps)
				&& is_near_equal(data[1], other.data[1], eps)
				&& is_near_equal(data[2], other.data[2], eps);
		}

		template<typename OtherElemT>
		constexpr 
			promotion_t<OtherElemT, element_type> 
			dot(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			return data[0] * other.data[0]
				 + data[1] * other.data[1]
				 + data[2] * other.data[2];
		}

		constexpr element_type length_sq() const noexcept {
			return dot(*this);
		}

		auto length() const noexcept {
			return sqrt((promotion_t<default_floating_point_t, element_type>)length_sq());
		}

		constexpr auto
			reciprocal() const noexcept {
			typedef promotion_t<default_floating_point_t, element_type> promotion_t;
			return  vec_impl<component_size, promotion_t>{
				(promotion_t)1 / data[0],
				(promotion_t)1 / data[1],
				(promotion_t)1 / data[2],
			};
		}

		auto normalize() const noexcept {
			return mul(math::reciprocal(length()));
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
			add(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			return  {
				data[0] + other.data[0],
				data[1] + other.data[1],
				data[2] + other.data[2]
			};
		}

		template<typename OtherElemT>
		constexpr vec_impl<component_size, promotion_t<element_type, OtherElemT>>
			sub(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			return  {
				data[0] - other.data[0],
				data[1] - other.data[1],
				data[2] - other.data[2]
			};
		}

		template<typename OtherElemT>
		constexpr vec_impl<component_size, promotion_t<element_type, OtherElemT>>
			mul(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			return  {
				data[0] * other.data[0],
				data[1] * other.data[1],
				data[2] * other.data[2]
			};
		}

		template<typename OtherT>
		constexpr std::enable_if_t<is_vec_element<OtherT>::value, vec_impl<component_size, promotion_t<element_type, OtherT>>>
			mul(const OtherT& other) const noexcept {
			return  {
				data[0] * other,
				data[1] * other,
				data[2] * other
			};
		}

		template<typename OtherElemT>
		constexpr vec_impl<component_size, promotion_t<element_type, OtherElemT>>
			div(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			return  {
				data[0] / other.data[0],
				data[1] / other.data[1],
				data[2] / other.data[2]
			};
		}

		template<typename OtherT>
		constexpr std::enable_if_t<is_vec_element<OtherT>::value, vec_impl<component_size, promotion_t<element_type, OtherT>>>
			div(const OtherT& other) const noexcept {
			typedef promotion_t<element_type, OtherT> promotion_t;
			return mul(reciprocal((promotion_t)other));
		}

		template<typename OtherElemT>
		constexpr auto
			distance_sq(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			return sub(other).length_sq();
		}
		
		template<typename OtherElemT>
		auto distance(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			typedef promotion_t<default_floating_point_t, element_type, OtherElemT> promotion_t;
			return sqrt((promotion_t)distance_sq(other));
		}

		template<typename OtherElemT>
		constexpr vec_impl<component_size, promotion_t<element_type, OtherElemT>>
			reflect(const vec_impl<component_size, OtherElemT, true>& nor) const noexcept {
			return (*this) - nor * dot(nor) * 2;
		}
	};

	template<typename T>
	struct vec_impl<4, T, true> : legal_vec {
		static constexpr size_t component_size = 4;
		typedef T element_type;

		union {
			struct {
				element_type x, y, z, w;
			};
			element_type data[component_size];
		};

		constexpr vec_impl() noexcept
			: vec_impl(0, 0, 0, 0) {
		}

		template<typename OtherElemT, std::enable_if_t<is_vec_element<OtherElemT>::value, int> = 0>
		constexpr vec_impl(OtherElemT new_x, OtherElemT new_y, OtherElemT new_z, OtherElemT new_w) noexcept
			: data{ (element_type)new_x, (element_type)new_y, (element_type)new_z, (element_type)new_w } {
		}

		template<typename OtherElemT>
		constexpr vec_impl(const vec_impl<component_size, OtherElemT, true>& other) noexcept
			: vec_impl(other.data[0], other.data[1], other.data[2], other.data[3]) {
		}

		vec_impl& operator=(const vec_impl& other) noexcept {
			if (this != &other) {
				data[0] = other.data[0];
				data[1] = other.data[1];
				data[2] = other.data[2];
				data[3] = other.data[3];
			}
			return *this;
		}

		template<typename OtherElemT>
		vec_impl& operator=(const vec_impl<component_size, OtherElemT, true>& other) noexcept {
			data[0] = other.data[0];
			data[1] = other.data[1];
			data[2] = other.data[2];
			data[3] = other.data[3];
			return *this;
		}

		element_type& at(size_t idx) noexcept { return data[idx]; }
		constexpr const element_type& at(size_t idx) const noexcept { return data[idx]; }

		element_type& operator[](size_t idx) noexcept { return at(idx); }
		constexpr const element_type& operator[](size_t idx) const noexcept { return at(idx); }

		constexpr bool is_zero() const noexcept {
			return data[0] == 0
				&& data[1] == 0
				&& data[2] == 0
				&& data[3] == 0;
		}

		template<typename OtherElemT>
		constexpr bool is_equal(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			return data[0] == other.data[0]
				&& data[1] == other.data[1]
				&& data[2] == other.data[2]
				&& data[3] == other.data[3];
		}

		template<typename EpsT = element_type>
		bool is_near_zero(EpsT eps = epsilon<EpsT>()) const noexcept {
			return is_near_zero(data[0], eps)
				&& is_near_zero(data[1], eps)
				&& is_near_zero(data[2], eps)
				&& is_near_zero(data[3], eps);
		}

		template<typename OtherElemT, typename EpsT = promotion_t<element_type, OtherElemT>>
		bool is_near_equal(const vec_impl<component_size, OtherElemT, true>& other, EpsT eps = epsilon<EpsT>()) const noexcept {
			return is_near_equal(data[0], other.data[0], eps)
				&& is_near_equal(data[1], other.data[1], eps)
				&& is_near_equal(data[2], other.data[2], eps)
				&& is_near_equal(data[3], other.data[3], eps);
		}

		template<typename OtherElemT>
		constexpr
			promotion_t<OtherElemT, element_type>
			dot(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			return data[0] * other.data[0]
				+ data[1] * other.data[1]
				+ data[2] * other.data[2]
				+ data[3] * other.data[3];
		}

		constexpr element_type length_sq() const noexcept {
			return dot(*this);
		}

		promotion_t<default_floating_point_t, element_type> length() const noexcept {
			return sqrt((promotion_t<default_floating_point_t, element_type>)length_sq());
		}

		constexpr auto reciprocal() const noexcept {
			typedef promotion_t<default_floating_point_t, element_type> promotion_t;
			return  vec_impl<component_size, promotion_t>{
				(promotion_t)1 / data[0],
				(promotion_t)1 / data[1],
				(promotion_t)1 / data[2],
				(promotion_t)1 / data[3],
			};
		}

		auto normalize() const noexcept {
			return mul(math::reciprocal(length()));
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
			add(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			return  {
				data[0] + other.data[0],
				data[1] + other.data[1],
				data[2] + other.data[2],
				data[3] + other.data[3]
			};
		}

		template<typename OtherElemT>
		constexpr vec_impl<component_size, promotion_t<element_type, OtherElemT>>
			sub(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			return  {
				data[0] - other.data[0],
				data[1] - other.data[1],
				data[2] - other.data[2],
				data[3] - other.data[3]
			};
		}

		template<typename OtherElemT>
		constexpr vec_impl<component_size, promotion_t<element_type, OtherElemT>>
			mul(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			return  {
				data[0] * other.data[0],
				data[1] * other.data[1],
				data[2] * other.data[2],
				data[3] * other.data[3]
			};
		}

		template<typename OtherT>
		constexpr std::enable_if_t<is_vec_element<OtherT>::value, vec_impl<component_size, promotion_t<element_type, OtherT>>>
			mul(const OtherT& other) const noexcept {
			return  {
				data[0] * other,
				data[1] * other,
				data[2] * other,
				data[3] * other
			};
		}

		template<typename OtherElemT>
		constexpr vec_impl<component_size, promotion_t<element_type, OtherElemT>>
			div(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			return  {
				data[0] / other.data[0],
				data[1] / other.data[1],
				data[2] / other.data[2],
				data[3] / other.data[3]
			};
		}

		template<typename OtherT>
		constexpr std::enable_if_t<is_vec_element<OtherT>::value, vec_impl<component_size, promotion_t<element_type, OtherT>>>
			div(const OtherT& other) const noexcept {
			typedef promotion_t<element_type, OtherT> promotion_t;
			return mul(reciprocal((promotion_t)other));
		}

		template<typename OtherElemT>
		constexpr auto distance_sq(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			return sub(other).length_sq();
		}

		template<typename OtherElemT>
		auto distance(const vec_impl<component_size, OtherElemT, true>& other) const noexcept {
			typedef promotion_t<default_floating_point_t, element_type, OtherElemT> promotion_t;
			return sqrt((promotion_t)distance_sq(other));
		}

		template<typename OtherElemT>
		constexpr vec_impl<component_size, promotion_t<element_type, OtherElemT>>
			reflect(const vec_impl<component_size, OtherElemT, true>& nor) const noexcept {
			return (*this) - nor * dot(nor) * 2;
		}
	};

}//namespace zee::math::impl

	template<size_t CompSize, typename T> 
	struct vec : impl::vec_impl<CompSize, T> { 
	private:
		typedef impl::vec_impl<CompSize, T> base_type;

	public:
		typedef T element_type;

		constexpr vec() noexcept = default;

		template<typename... Args>
		constexpr vec(Args&&... args) noexcept
			: base_type(args...) {
		}

		template<typename OtherElemT>
		constexpr vec(const vec<base_type::component_size, OtherElemT>& other) noexcept
			: base_type(other) {
		}

		template<typename OtherElemT>
		constexpr vec(const impl::vec_impl<base_type::component_size, OtherElemT, true>& other) noexcept
			: base_type(other) {
		}

		template<typename OtherElemT>
		vec& operator=(const impl::vec_impl<base_type::component_size, OtherElemT, true>& other) noexcept {
			base_type::operator=(other);
			return *this;
		}

		template<typename OtherElemT>
		vec& operator=(const vec<base_type::component_size, OtherElemT>& other) noexcept {
			base_type::operator=(other);
			return *this;
		}

		struct constants;
	};

	template<size_t CompSize, typename T>
	struct vec<CompSize, T>::constants {
		static constexpr vec<CompSize, T> zero = {};
	};

	template<typename T>
	struct vec<4, T> : impl::vec_impl<4, T> {
	private:
		typedef impl::vec_impl<4, T> base_type;

	public:
		typedef T element_type;

		constexpr vec() noexcept = default;

		template<typename Elem1T, typename Elem2T, typename Elem3T, typename Elem4T, std::enable_if_t<are_all_vec_element<Elem1T, Elem2T, Elem3T, Elem4T>::value, int> = 0>
		constexpr vec(const Elem1T& new_x, const Elem2T& new_y, const Elem3T& new_z, const Elem4T& new_w) noexcept
			: base_type((element_type)new_x, (element_type)new_y, (element_type)new_z, (element_type)new_w) {
		}


		template<typename OtherElemT>
		constexpr vec(const vec<base_type::component_size, OtherElemT>& other) noexcept
			: base_type(other) {
		}

		template<typename OtherElemT>
		constexpr vec(const impl::vec_impl<base_type::component_size, OtherElemT, true>& other) noexcept
			: base_type(other) {
		}

		template<typename OtherElemT>
		vec& operator=(const impl::vec_impl<base_type::component_size, OtherElemT, true>& other) noexcept {
			base_type::operator=(other);
			return *this;
		}

		template<typename OtherElemT>
		vec& operator=(const vec<base_type::component_size, OtherElemT>& other) noexcept {
			base_type::operator=(other);
			return *this;
		}

		struct constants;
	};

	template<typename T>
	struct vec<4, T>::constants {
		static constexpr vec<4, T> zero = {};
		static constexpr vec<4, T> one    = { 1, 1, 1, 1 };
		static constexpr vec<4, T> unit_x = { 1, 0, 0, 0 };
		static constexpr vec<4, T> unit_y = { 0, 1, 0, 0 };
		static constexpr vec<4, T> unit_z = { 0, 0, 1, 0 };
		static constexpr vec<4, T> unit_w = { 0, 0, 0, 1 };
	};

	template<typename T>
	struct vec<3, T> : impl::vec_impl<3, T> {
	private:
		typedef impl::vec_impl<3, T> base_type;

	public:
		typedef T element_type;

		constexpr vec() noexcept = default;

		template<typename Elem1T, typename Elem2T, typename Elem3T, std::enable_if_t<are_all_vec_element<Elem1T, Elem2T, Elem3T>::value, int> = 0>
		constexpr vec(const Elem1T& new_x, const Elem2T& new_y, const Elem3T& new_z) noexcept
			: base_type((element_type)new_x, (element_type)new_y, (element_type)new_z) {
		}

		template<typename OtherElemT>
		constexpr vec(const vec<base_type::component_size, OtherElemT>& other) noexcept
			: base_type(other) {
		}

		template<typename OtherElemT>
		constexpr vec(const impl::vec_impl<base_type::component_size, OtherElemT, true>& other) noexcept
			: base_type(other) {
		}

		template<typename OtherElemT>
		vec& operator=(const impl::vec_impl<base_type::component_size, OtherElemT, true>& other) noexcept {
			base_type::operator=(other);
			return *this;
		}

		template<typename OtherElemT>
		vec& operator=(const vec<base_type::component_size, OtherElemT>& other) noexcept {
			base_type::operator=(other);
			return *this;
		}

		struct constants;
	};
	
	template<typename T>
	struct vec<3, T>::constants {
		static constexpr vec<3, T> zero = {};
		static constexpr vec<3, T> one    = { 1, 1, 1 };
		static constexpr vec<3, T> unit_x = { 1, 0, 0 };
		static constexpr vec<3, T> unit_y = { 0, 1, 0 };
		static constexpr vec<3, T> unit_z = { 0, 0, 1 };
	};

	template<typename T>
	struct vec<2, T> : impl::vec_impl<2, T> {
	private:
		typedef impl::vec_impl<2, T> base_type;

	public:
		typedef T element_type;

		constexpr vec() noexcept = default;

		template<typename Elem1T, typename Elem2T,  std::enable_if_t<are_all_vec_element<Elem1T, Elem2T>::value, int> = 0>
		constexpr vec(const Elem1T& new_x, const Elem2T& new_y) noexcept
			: base_type((element_type)new_x, (element_type)new_y) {
		}

		template<typename OtherElemT>
		constexpr vec(const vec<base_type::component_size, OtherElemT>& other) noexcept
			: base_type(other) {
		}

		template<typename OtherElemT>
		constexpr vec(const impl::vec_impl<base_type::component_size, OtherElemT, true>& other) noexcept
			: base_type(other) {
		}

		template<typename OtherElemT>
		vec& operator=(const impl::vec_impl<base_type::component_size, OtherElemT, true>& other) noexcept {
			base_type::operator=(other);
			return *this;
		}

		template<typename OtherElemT>
		vec& operator=(const vec<base_type::component_size, OtherElemT>& other) noexcept {
			base_type::operator=(other);
			return *this;
		}

		struct constants;
	};
	
	template<typename T>
	struct vec<2, T>::constants {
		static constexpr vec<2, T> zero = {};
		static constexpr vec<2, T> one    = { 1, 1 };
		static constexpr vec<2, T> unit_x = { 1, 0 };
		static constexpr vec<2, T> unit_y = { 0, 1 };
	};

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	promotion_t<LeftElemT, RightElemT>
		dot(const vec<CompSize, LeftElemT>& l, const vec<CompSize, RightElemT>& r) noexcept {
		return l.dot(r);
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr  promotion_t<LeftElemT, RightElemT>
		dot(const vec<2, LeftElemT>& l, const vec<2, RightElemT>& r) noexcept {
		return l.dot(r);
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr  promotion_t<LeftElemT, RightElemT>
		dot(const vec<3, LeftElemT>& l, const vec<3, RightElemT>& r) noexcept {
		return l.dot(r);
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr promotion_t<LeftElemT, RightElemT>
		dot(const vec<4, LeftElemT>& l, const vec<4, RightElemT>& r) noexcept {
		return l.dot(r);
	}

	template<size_t CompSize, typename ElemT>
	vec<CompSize, ElemT>
		abs(const vec<CompSize, ElemT>& v) noexcept {
		vec<CompSize, ElemT> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret.data[i] = abs(v.data[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename ElemT>
	std::enable_if_t<std::is_floating_point<ElemT>::value, vec<CompSize, ElemT>>
		ceil(const vec<CompSize, ElemT>& v) noexcept {
		vec<CompSize, ElemT> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret.data[i] = ceil(v.data[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename ElemT>
	constexpr std::enable_if_t<!std::is_floating_point<ElemT>::value, const vec<CompSize, ElemT>&>
		ceil(const vec<CompSize, ElemT>& v) noexcept {
		return v;
	}

	template<size_t CompSize, typename ElemT>
	std::enable_if_t<std::is_floating_point<ElemT>::value, vec<CompSize, ElemT>>
		floor(const vec<CompSize, ElemT>& v) noexcept {
		vec<CompSize, ElemT> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret.data[i] = floor(v.data[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename ElemT>
	constexpr std::enable_if_t<!std::is_floating_point<ElemT>::value, const vec<CompSize, ElemT>&>
		floor(const vec<CompSize, ElemT>& v) noexcept {
		return v;
	}

	template<size_t CompSize, typename ElemT>
	std::enable_if_t<std::is_floating_point<ElemT>::value, vec<CompSize, ElemT>>
		round(const vec<CompSize, ElemT>& v) noexcept {
		vec<CompSize, ElemT> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret.data[i] = round(v.data[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename ElemT>
	constexpr std::enable_if_t<!std::is_floating_point<ElemT>::value, const vec<CompSize, ElemT>&>
		round(const vec<CompSize, ElemT>& v) noexcept {
		return v;
	}

	template<size_t CompSize, typename ElemT>
	std::enable_if_t<std::is_floating_point<ElemT>::value, vec<CompSize, ElemT>>
		trunc(const vec<CompSize, ElemT>& v) noexcept {
		vec<CompSize, ElemT> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret.data[i] = trunc(v.data[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename ElemT>
	constexpr std::enable_if_t<!std::is_floating_point<ElemT>::value, const vec<CompSize, ElemT>&>
		trunc(const vec<CompSize, ElemT>& v) noexcept {
		return v;
	}

	template<size_t CompSize, typename ElemT, typename RemT>
	std::enable_if_t<are_any_floating_point<ElemT, RemT>::value, vec<CompSize, promotion_t<ElemT, RemT>>>
		fmod(const vec<CompSize, ElemT>& v, RemT r) noexcept {
		vec<CompSize, promotion_t<ElemT, RemT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret.data[i] = fmod(v.data[i], r);
		}
		return ret;
	}

	template<size_t CompSize, typename ElemT, typename RemT>
	constexpr std::enable_if_t<!are_any_floating_point<ElemT, RemT>::value, vec<CompSize, promotion_t<ElemT, RemT>>>
		fmod(const vec<CompSize, ElemT>& v, RemT r) noexcept {
		vec<CompSize, promotion_t<ElemT, RemT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret.data[i] = v.data[i] % r;
		}
		return ret;
	}

	template<size_t CompSize, typename ElemT, typename RemT>
	std::enable_if_t<are_any_floating_point<ElemT, RemT>::value, vec<CompSize, promotion_t<ElemT, RemT>>>
		fmod(const vec<CompSize, ElemT>& v, const vec<CompSize, RemT>& r) noexcept {
		vec<CompSize, promotion_t<ElemT, RemT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret.data[i] = fmod(v.data[i], r.data[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename ElemT, typename RemT>
	constexpr std::enable_if_t<!are_any_floating_point<ElemT, RemT>::value, vec<CompSize, promotion_t<ElemT, RemT>>>
		fmod(const vec<CompSize, ElemT>& v, const vec<CompSize, RemT>& r) noexcept {
		vec<CompSize, promotion_t<ElemT, RemT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret.data[i] = v.data[i] % r.data[i];
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr vec<CompSize, promotion_t<LeftElemT, RightElemT>>
		min(const vec<CompSize, LeftElemT>& l, const vec<CompSize, RightElemT>& r) noexcept {
		vec<CompSize, promotion_t<LeftElemT, RightElemT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret.data[i] = min(l.data[i], r.data[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr vec<CompSize, promotion_t<LeftElemT, RightElemT>>
		max(const vec<CompSize, LeftElemT>& l, const vec<CompSize, RightElemT>& r) noexcept {
		vec<CompSize, promotion_t<LeftElemT, RightElemT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret.data[i] = max(l.data[i], r.data[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename SrcElemT, typename MinElemT, typename MaxElemT>
	constexpr vec<CompSize, promotion_t<SrcElemT, MinElemT, MaxElemT>>
		clamp(const vec<CompSize, SrcElemT>& src_v, const vec<CompSize, MinElemT>& min_v, const vec<CompSize, MaxElemT>& max_v) noexcept {
		return min(max(src_v, min_v), max_v);
	}

	template<size_t CompSize, typename FirstT, typename LastT, typename DeltaT>
	constexpr std::enable_if_t<std::is_floating_point<DeltaT>::value, 
		vec<CompSize, promotion_t<FirstT, LastT, DeltaT>>>
		lerp(const vec<CompSize, FirstT>& f, const vec<CompSize, LastT>& l, DeltaT d) noexcept {
		return f + d * (l - f);
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr auto
		cross(const vec<3, LeftElemT>& l, const vec<3, RightElemT>& r) noexcept {
		return l.cross(r);
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr auto
		ccw(const vec<2, LeftElemT>& l, const vec<2, RightElemT>& r) noexcept {
		return l.ccw(r);
	}

}//namespace zee::math
}//namespace zee