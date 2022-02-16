#pragma once
#include <type_traits>
namespace yjj {
namespace interfaces {
	struct tickable {
		virtual void tick(float delta_time) = 0;
		virtual ~tickable() = default;
	};
}//namespace yjj::interfaces

	template<typename T>
	struct is_tickable : std::conditional_t<
		std::is_base_of<interfaces::tickable, T>::value,
		std::true_type,
		std::false_type
	> {
	};
}//namespace yjj
