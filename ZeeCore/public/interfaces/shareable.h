#pragma once
#include <type_traits>
#include <memory>
namespace zee {
namespace interfaces {
	
	template<typename T>
	using shareable = std::enable_shared_from_this<T>;

}//namespace zee::interfaces
namespace impl {
	template<typename T>
	struct is_sharable_impl {
		typedef char yes[2];
		typedef char no [1];
	};

}//namespace zee::impl
	template<typename T>
	struct is_shareable : std::conditional_t<
		std::is_base_of<interfaces::sharable, T>::value,
		std::true_type, 
		std::false_type
		> {
	};

}//namespace zee
