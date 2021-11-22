#pragma once
#include <type_traits>
#include "../core/string.h"
namespace zee {
	class log;
namespace interfaces {

	struct loggable {
		//npos로 전달 할 시 문자열 길이 계산.
		virtual void print(const TCHAR* null_terminated_str, size_t length /* tstring::npos */) = 0;
		virtual void on_bind(log& owner_log) = 0;
		virtual void on_unbind() = 0;
		virtual ~loggable() = default;
	};

}//namespace zee::interfaces

	template<typename T>
	struct is_printable : std::conditional_t<
		std::is_base_of<interfaces::loggable, T>::value ,
		std::true_type ,
		std::false_type
	> {
	};

}//namespace zee
