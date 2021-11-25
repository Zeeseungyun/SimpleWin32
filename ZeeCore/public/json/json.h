#pragma once
#include "json.hpp"
#include "../math/vec/vec_decl.h"
#include "../shape/rect_decl.h"
#include "../shape/circle_decl.h"

namespace zee {
	typedef nlohmann::json json;
	
namespace json_helper {
	template<typename PredT, typename... Args>
	auto safety_get_members_with_pred(bool& is_valid, const json& j, const PredT& pred, Args&&... members) noexcept
		-> std::array<json::const_iterator, sizeof...(members)>
	{
		typedef std::array<json::const_iterator, sizeof...(members)> array_type;
		if (!j.is_object()) {
			return array_type{};
		}

		array_type found_members = {
			j.find(members)...
		};

		is_valid = false;
		for (const auto& member : found_members) {
			is_valid = member != j.end();
			if (!is_valid) {
				break;
			}

			is_valid &= pred(member.value());
			if (!is_valid) {
				break;
			}
		}

		return found_members;
	}

	template<typename PredT, typename... Args>
	bool safety_check_has_members_with_pred(const json& j, const PredT& pred, Args&&... members) noexcept {
		typedef std::array<json::const_iterator, sizeof...(members)> array_type;
		if (!j.is_object()) {
			return false;
		}

		array_type found_members = {
			j.find(members)...
		};

		for (const auto& member : found_members) {
			if (member == j.end()) {
				return false;
			}

			if (!pred(member.value())) {
				return false;
			}
		}

		return true;
	}

	template<typename... Args>
	auto safety_get_members(bool& is_valid, const json& j, Args&&... members) noexcept
		-> std::array<json::const_iterator, sizeof...(members)>
	{
		typedef std::array<json::const_iterator, sizeof...(members)> array_type;
		if (!j.is_object()) {
			return array_type{};
		}

		array_type found_members = {
			j.find(members)...
		};

		is_valid = false;
		for (const auto& member : found_members) {
			is_valid = member != j.end();
			if (!is_valid) {
				break;
			}
		}

		return found_members;
	}

	template<typename PredT, typename... Args>
	bool safety_check_has_members(const json& j, const PredT& pred, Args&&... members) noexcept {
		typedef std::array<json::const_iterator, sizeof...(members)> array_type;
		if (!j.is_object()) {
			return false;
		}

		array_type found_members = {
			j.find(members)...
		};

		for (const auto& member : found_members) {
			if (member == j.end()) {
				return false;
			}
		}

		return true;
	}
	
	template<size_t CompSize>
	inline bool is_vec(const json& j) noexcept {
		if (j.is_array() && j.size() == CompSize) {
			for (size_t i = 0; i != CompSize; ++i) {
				if (!j[i].is_number()) {
					return false;
				}
			}

			return true;
		}
		return false;
	}

	inline bool is_vec(const size_t component_size, const json& j) noexcept {
		if (j.is_array() && j.size() == component_size) {
			for (size_t i = 0; i != component_size; ++i) {
				if (!j[i].is_number()) {
					return false;
				}
			}

			return true;
		}
		return false;
	}

	inline bool is_rect(const json& j) noexcept {
		return json_helper::safety_check_has_members(j, [](const json& j) {return j.is_number(); }, "left", "top", "right", "bottom");
	}

	inline bool is_circle(const json& j) noexcept {
		bool is_valid = false;
		auto list = json_helper::safety_get_members(is_valid, j, "origin", "radius");
		if (is_valid) {
			is_valid &= is_vec<2>(list[0].value());
			is_valid &= list[1].value().is_number();
		}

		return is_valid;
	}

	inline bool is_number(const json& j) noexcept { return j.is_number(); }
	inline bool is_array(const json& j) noexcept { return j.is_array(); }
	inline bool is_boolean(const json& j) noexcept { return j.is_boolean(); }

}//namespace zee::json_helper

namespace math {
namespace impl {
	template<size_t CompSize, typename VecElemT>
	void to_json(json& j, const vec_impl<CompSize, VecElemT, true>& v) {
		j.clear();
		for (const VecElemT& e : v) {
			j.push_back(e);
		}
	}

	template<size_t CompSize, typename VecElemT>
	void from_json(const json& j, vec_impl<CompSize, VecElemT, true>& v) {
		if (json_helper::is_vec<CompSize>(j)) {
			for (size_t i = 0; i != CompSize; ++i) {
				v[i] = j[i];
			}
		}
	}

}//namespace zee::math::impl

	template<size_t CompSize, typename VecElemT>
	void to_json(json& j, const vec_base<CompSize, VecElemT>& v) {
		impl::to_json(j, v);
	}
	
	template<size_t CompSize, typename VecElemT>
	void from_json(const json& j, vec_base<CompSize, VecElemT>& v) {
		impl::from_json(j, v);
	}

}//namespace zee::math

namespace shape {
namespace impl {
	template<typename ElemT>
	void to_json(json& j, const rect_base_impl<ElemT, true>& rc) {
		j = json
		{
			{"left"  , rc.left	 },
			{"top"   , rc.top    },
			{"right" , rc.right  },
			{"bottom", rc.bottom }
		};
	}

	template<typename ElemT>
	void from_json(const json& j, rect_base_impl<ElemT, true>& rc) {
		bool is_valid = false;
		const auto list = json_helper::safety_get_members_with_pred(is_valid, j, [](const json& v) { return v.is_number(); }, "left", "top", "right", "bottom");

		if (is_valid) {
			rc.left		= list[0].value();
			rc.top		= list[1].value();
			rc.right	= list[2].value();
			rc.bottom	= list[3].value();
		} else {
			rc = rect_base_impl<ElemT, true>{};
		}
	}

	template<typename ElemT>
	void to_json(json& j, const circle_base_impl<ElemT, true>& cc) {
		j = json 
		{
			{"origin", cc.origin},
			{"radius", cc.radius}
		};
	}

	template<typename ElemT>
	void from_json(const json& j, circle_base_impl<ElemT, true>& cc) {
		bool is_valid = false;
		const auto list = json_helper::safety_get_members(is_valid, j, "origin", "radius");

		if (is_valid) {
			is_valid &= json_helper::is_vec<2>(list[0].value());
			is_valid &= list[1].value().is_number();
		}

		if (is_valid) {
			cc.origin	= list[0].value();
			cc.radius   = list[1].value();

		} else {
			cc = circle_base_impl<ElemT, true>{};
		}
	}

}//namespace zee::shape::impl
}//namespace zee::shape
}//namespace zee
