#pragma once
#include <type_traits>
#include "core/string.h"

namespace zee {
	using std::is_arithmetic;

	template<typename T>
	struct is_arithmetic_with_decay {

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

	template<typename ArithmeticT, typename...Args>
	struct promotion_type {
	private:
		typedef std::decay_t<ArithmeticT> left_type;
		typedef typename promotion_type<Args...>::type right_type;
	
	public:
		typedef typename impl::promotion_impl<is_arithmetic<left_type, Args...>::value,
			left_type, right_type>::type type;
	};
	
	template<typename ArithmeticT>
	struct promotion_type<ArithmeticT> {
	private:
		typedef std::decay_t<ArithmeticT> arithmetic_t;

	public:
		typedef typename impl::promotion_impl<is_arithmetic<arithmetic_t>::value, arithmetic_t, arithmetic_t>::type type;
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

}//namespace zee