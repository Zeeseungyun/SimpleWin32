#pragma once
#include <type_traits>
namespace zee {
namespace interfaces {
	class clonable {
	public:
		virtual void clone(void* out_object_ptr) = 0;
	};
}//namespace zee::interfaces
	
	template<typename T>
	struct is_clonable : std::conditional_t<
		std::is_base_of<interfaces::clonable, T>::value, 
		std::true_type, 
		std::false_type
		> {
	};

}//namespace zee