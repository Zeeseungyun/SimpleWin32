#pragma once
#include <type_traits>
namespace zee {
namespace interfaces {
	struct tickable {
		virtual void tick(float delta_time) = 0;
	};
}//namespace zee::interfaces

template<typename T>
struct is_tickable : std::conditional_t<
	std::is_base_of<interfaces::tickable, T>::value,
	std::true_type,
	std::false_type
> {
};
}//namespace zee
