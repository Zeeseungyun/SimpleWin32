#pragma once
#include <type_traits>
namespace zee {
namespace interfaces {
	template<typename CloneObjectT>
	struct cloneable {
		virtual bool clone(CloneObjectT& out_object) = 0;
		virtual ~cloneable() = default;
	};

}//namespace zee::interfaces
	
	template<typename CloneObjectT, typename T>
	struct is_cloneable : std::conditional_t<
		std::is_base_of<interfaces::cloneable<CloneObjectT>, T>::value,
		std::true_type, 
		std::false_type
		> {
	};

}//namespace zee
