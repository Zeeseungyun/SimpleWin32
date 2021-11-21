#pragma once
#include <type_traits>
namespace zee {
namespace interfaces {
	template<typename CloneObjectT>
	struct clonable {
		virtual std::decay_t<CloneObjectT> clone() = 0;
		virtual ~clonable() = default;
	};

}//namespace zee::interfaces
	
	template<typename CloneObjectT, typename T>
	struct is_clonable : std::conditional_t<
		std::is_base_of<interfaces::clonable<CloneObjectT>, T>::value,
		std::true_type, 
		std::false_type
		> {
	};

}//namespace zee
