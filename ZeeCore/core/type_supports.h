#pragma once
#include <type_traits>
#include <utility>

namespace zee {

	template<typename ArithmeticT, typename... Args>
	struct is_arithmetic : 
		std::conditional_t<
		std::is_arithmetic<ArithmeticT>::value && is_arithmetic<Args...>::value, 
		std::true_type, 
		std::false_type
		> {
	};
	
	template<typename ArithmeticT>
	struct is_arithmetic<ArithmeticT> : 
		std::conditional_t<
		std::is_arithmetic<ArithmeticT>::value, 
		std::true_type, 
		std::false_type
		> {
	};
	
	template<typename ArithmeticT, typename... Args>
	struct is_arithmetic_with_decay : 
		std::conditional_t<
		std::is_arithmetic<std::decay_t<ArithmeticT>>::value && is_arithmetic_with_decay<Args...>::value,
		std::true_type, 
		std::false_type
		> {
	};
	
	template<typename ArithmeticT>
	struct is_arithmetic_with_decay<ArithmeticT> :
		std::conditional_t<
		std::is_arithmetic<std::decay_t<ArithmeticT>>::value, 
		std::true_type, 
		std::false_type
		> {
	};

	template<typename FloatT, typename ... Args>
	struct is_floating_point : std::conditional_t<std::is_floating_point<FloatT>::value && is_floating_point<Args...>::value, std::true_type, std::false_type> {

	};

	template<typename FloatT>
	struct is_floating_point<FloatT> : std::conditional_t<std::is_floating_point<FloatT>::value, std::true_type, std::false_type> {

	};

namespace impl {
	template<bool IsArithmeticBoth, typename Arg1T, typename Arg2T> struct promotion_impl;

	template<typename Arg1T, typename Arg2T>
	struct promotion_impl<true, Arg1T, Arg2T> {
		typedef decltype(std::decay_t<Arg1T>(0) + std::decay_t<Arg2T>(0)) type;
	};

	template<typename Arg1T, typename Arg2T>
	struct promotion_impl<false, Arg1T, Arg2T> {
		typedef void type;
	};

}//namespace zee::impl

	template<typename LeftT, typename...Args>
	struct promotion_type {
	private:
		typedef typename promotion_type<Args...>::type args_type;
	
	public:
		typedef decltype(LeftT(0) + args_type(0)) type;
	};

	template<typename LeftT, typename RightT>
	struct promotion_type<LeftT, RightT> {
	public:
		typedef decltype(LeftT(0) + RightT(0)) type;
	};

	template<typename... Args>
	using promotion_t = typename promotion_type<Args...>::type;

	template<typename ClassT, typename RetT, typename... Args>
	struct has_function_operator_specified {
	private:
		typedef char true_t[1];
		typedef char false_t[2];
	
		template<typename U, RetT(U::*)(Args...) = &U::operator()> static true_t& test_empty(U* v1);
		template<typename U> static false_t& test_empty(...);
	
		template<typename U, RetT(U::*)(Args...) volatile = &U::operator()> static true_t& test_volatile(U* v1);
		template<typename U> static false_t& test_volatile(...);
	
		template<typename U, RetT(U::*)(Args...) const = &U::operator()> static true_t& test_const(U* v1);
		template<typename U> static false_t& test_const(...);
	
		template<typename U, RetT(U::*)(Args...) const& = &U::operator()> static true_t& test_cref(U* v1);
		template<typename U> static false_t& test_cref(...);
	
		template<typename U, RetT(U::*)(Args...) & = &U::operator()> static true_t& test_lref(U* v1);
		template<typename U> static false_t& test_lref(...);
	
		template<typename U, RetT(U::*)(Args...) && = &U::operator()> static true_t& test_rref(U* v1);
		template<typename U> static false_t& test_rref(...);
	
	public:
		static constexpr bool has_empty		= sizeof(test_empty		<ClassT>(0)) == sizeof(true_t);
		static constexpr bool has_volatile	= sizeof(test_volatile	<ClassT>(0)) == sizeof(true_t);
		static constexpr bool has_const		= sizeof(test_const		<ClassT>(0)) == sizeof(true_t);
		static constexpr bool has_cref		= sizeof(test_cref		<ClassT>(0)) == sizeof(true_t);
		static constexpr bool has_lref		= sizeof(test_lref		<ClassT>(0)) == sizeof(true_t);
		static constexpr bool has_rref		= sizeof(test_rref		<ClassT>(0)) == sizeof(true_t);
		static constexpr bool value			= has_empty | has_volatile | has_const | has_cref | has_lref | has_rref;
	};
	
	template<typename ClassT, typename T> struct is_member_function_specified { static constexpr bool value = false; };
	template<typename ClassT, typename RetT, typename...Args> struct is_member_function_specified <ClassT, RetT(ClassT::*)(Args...)> { static constexpr bool value = true; };
	template<typename ClassT, typename RetT, typename...Args> struct is_member_function_specified <const ClassT, RetT(ClassT::*)(Args...) const> { static constexpr bool value = true; };
	template<typename ClassT, typename RetT, typename...Args> struct is_member_function_specified <const ClassT, RetT(ClassT::*)(Args...) const&> { static constexpr bool value = true; };
	template<typename ClassT, typename RetT, typename...Args> struct is_member_function_specified <ClassT, RetT(ClassT::*)(Args...) const> { static constexpr bool value = true; };
	
	template<typename T> struct is_static_function_specified { static constexpr bool value = false; };
	template<typename RetT, typename...Args> struct is_static_function_specified <RetT(*)(Args...)> { static constexpr bool value = true; };

	template<typename T>
	struct is_valid_vector_element_type : std::conditional_t<std::is_signed<T>::value, std::true_type, std::false_type> { };

}//namespace zee