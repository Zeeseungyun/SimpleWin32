#pragma once
#include <type_traits>
#include <utility>
#include "../math/vec/vec_decl.h"

namespace zee {
	template<bool A, bool B> struct logical_xor;
	template<> struct logical_xor<false, false> : std::false_type {};
	template<> struct logical_xor<false, true > : std::true_type  {};
	template<> struct logical_xor<true , false> : std::true_type  {};
	template<> struct logical_xor<true , true > : std::false_type {};

namespace impl {
	template<typename BindType, BindType BindValue, template<BindType, typename> typename T>
	struct bind_value_btu_param0 {
		template<typename ParamT>
		using type =  T<BindValue, ParamT>;
	};

	template<typename BindType, BindType BindValue, template<typename, BindType> typename T>
	struct bind_value_btu_param1 {
		template<typename ParamT>
		using type = T<ParamT, BindValue>;
	};

	template<typename BindType, template<typename, typename> typename T>
	struct bind_type_btu_param0 {
		template<typename ParamT>
		using type = T<BindType, ParamT>;
	};

	template<typename BindType, template<typename, typename> typename T>
	struct bind_type_btu_param1 {
		template<typename ParamT>
		using type = T<ParamT, BindType>;
	};

}//namespace zee::impl 

namespace advance {
namespace unary {
	template<
		template <typename> typename A,
		typename T>
		struct is_A
		: std::conditional_t< A<T>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A,
		typename T>
		struct is_not_A
		: std::conditional_t< !A<T>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T>
		struct is_A_or_B
		: std::conditional_t< A<T>::value || B<T>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T>
		struct is_A_and_B
		: std::conditional_t< A<T>::value && B<T>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T>
		struct is_A_xor_B
		: std::conditional_t< logical_xor<A<T>::value, B<T>::value>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T>
		struct is_A_not_B
		: std::conditional_t< A<T>::value && !B<T>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A,
		typename T, typename... Args>
		struct are_all_A
		: std::conditional_t< are_all_A<A, T>::value && are_all_A<A, Args...>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A,
		typename T>
		struct are_all_A<A, T>
		: std::conditional_t<A<T>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A,
		typename T, typename... Args>
		struct are_not_all_A
		: std::conditional_t< are_not_all_A<A, T>::value && are_not_all_A<A, Args...>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A,
		typename T>
		struct are_not_all_A<A, T>
		: std::conditional_t< is_not_A<A, T>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A,
		typename T, typename... Args>
		struct are_any_A
		: std::conditional_t< are_any_A<A, T>::value || are_any_A<A, Args...>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A, 
		typename T>
	struct are_any_A<A, T>
		: std::conditional_t<A<T>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A,
		typename T, typename... Args>
		struct are_not_any_A
		: std::conditional_t< are_not_any_A<A, T>::value || are_not_any_A<A, Args...>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A,
		typename T>
		struct are_not_any_A<A, T>
		: std::conditional_t< is_not_A<A, T>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T, typename... Args>
		struct are_all_A_not_B
		: std::conditional_t< are_all_A_not_B<A, B, T>::value && are_all_A_not_B<A, B, Args...>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T>
		struct are_all_A_not_B<A, B, T>
		: std::conditional_t< is_A_not_B<A, B, T>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T, typename... Args>
		struct are_any_A_not_B
		: std::conditional_t< is_A_not_B<A, B, T>::value || are_any_A_not_B<A, B, Args...>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T>
		struct are_any_A_not_B<A, B, T>
		: std::conditional_t< is_A_not_B<A, B, T>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T, typename... Args>
		struct are_all_A_or_B
		: std::conditional_t< is_A_or_B<A, B, T>::value && are_all_A_or_B<A, B, Args...>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T>
		struct are_all_A_or_B<A, B, T>
		: std::conditional_t< is_A_or_B<A, B, T>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T, typename... Args>
		struct are_any_A_or_B
		: std::conditional_t< is_A_or_B<A, B, T>::value || are_any_A_or_B<A, B, Args...>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T>
		struct are_any_A_or_B<A, B, T>
		: std::conditional_t< is_A_or_B<A, B, T>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T, typename... Args>
		struct are_all_A_and_B
		: std::conditional_t< is_A_and_B<A, B, T>::value && are_all_A_and_B<A, B, Args...>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T>
		struct are_all_A_and_B<A, B, T>
		: std::conditional_t< is_A_and_B<A, B, T>::value , std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T, typename... Args>
		struct are_any_A_and_B
		: std::conditional_t< is_A_and_B<A, B, T>::value || are_any_A_and_B<A, B, Args...>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T>
		struct are_any_A_and_B<A, B, T>
		: std::conditional_t< is_A_and_B<A, B, T>::value, std::true_type, std::false_type> {
	};

}//namespace zee::advance::unary

namespace binary {
namespace impl {
	template<size_t Size>
	struct is_odd : std::conditional_t<Size % 2 == 0, std::true_type, std::false_type> { };
	struct dummy_empty_struct { };
}//namespace zee::advance::binary::impl
	template<
		template <typename, typename> typename  A,
		typename T, typename U>
		struct is_A
		: std::conditional_t< A<T, U>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename, typename> typename A,
		typename T, typename U>
		struct is_not_A
		: std::conditional_t< !A<T, U>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U>
		struct is_A_or_B
		: std::conditional_t< A<T, U>::value || B<T, U>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U>
		struct is_A_and_B
		: std::conditional_t< A<T, U>::value && B<T, U>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U>
		struct is_A_xor_B
		: std::conditional_t< logical_xor<A<T, U>::value, B<T, U>::value>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U>
		struct is_A_not_B
		: std::conditional_t<A<T, U>::value && !B<T, U>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename, typename> typename A,
		typename T, typename U, typename... Args>
		struct are_all_A
		: std::conditional_t< impl::is_odd<sizeof...(Args)>::value,
		std::conditional_t< is_A<A, T, U>::value && are_all_A<A, Args...>::value, std::true_type, std::false_type>,
		impl::dummy_empty_struct
		> {
	};

	template<
		template <typename, typename> typename A, 
		typename T, typename U>
	struct are_all_A<A, T, U>
		: std::conditional_t< is_A<A, T ,U>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename, typename> typename A,
		typename T, typename U, typename... Args>
		struct are_not_all_A
		: std::conditional_t< impl::is_odd<sizeof...(Args)>::value,
		std::conditional_t< is_not_A<A, T, U>::value && are_not_all_A<A, Args...>::value, std::true_type, std::false_type>,
		impl::dummy_empty_struct
		> {
	};

	template<
		template <typename, typename> typename A,
		typename T, typename U>
		struct are_not_all_A<A, T, U>
		: std::conditional_t< is_not_A<A, T, U>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename, typename> typename A,
		typename T, typename U, typename... Args>
		struct are_any_A
		: std::conditional_t< impl::is_odd<sizeof...(Args)>::value,
		std::conditional_t< is_A<A, T, U>::value || are_any_A<A, Args...>::value, std::true_type, std::false_type>,
		impl::dummy_empty_struct
		> {
	};
	
	template<
		template <typename, typename> typename A,
		typename T, typename U>
	struct are_any_A<A, T, U>
		: std::conditional_t< is_A<A, T, U>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename, typename> typename A,
		typename T, typename U, typename... Args>
		struct are_not_any_A
		: std::conditional_t< impl::is_odd<sizeof...(Args)>::value,
		std::conditional_t< is_not_A<A, T, U>::value || are_not_any_A<A, Args...>::value, std::true_type, std::false_type>,
		impl::dummy_empty_struct
		> {
	};

	template<
		template <typename, typename> typename A,
		typename T, typename U>
		struct are_not_any_A<A, T, U>
		: std::conditional_t< is_not_A<A, T, U>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U, typename... Args>
		struct are_all_A_not_B
		: std::conditional_t< impl::is_odd<sizeof...(Args)>::value,
		std::conditional_t< is_A_not_B<A, B, T, U>::value && are_all_A_not_B<A, B, Args...>::value, std::true_type, std::false_type>,
		impl::dummy_empty_struct
		> {
	};
	
	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U>
		struct are_all_A_not_B<A, B, T, U>
		: std::conditional_t< is_A_not_B<A, B, T, U>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U, typename... Args>
		struct are_any_A_not_B
		: std::conditional_t< impl::is_odd<sizeof...(Args)>::value,
		std::conditional_t< is_A_not_B<A, B, T, U>::value || are_any_A_not_B<A, B, Args...>::value, std::true_type, std::false_type>,
		impl::dummy_empty_struct
		> {
	};
	
	
	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U>
		struct are_any_A_not_B<A, B, T, U>
		: std::conditional_t< is_A_not_B<A, B, T, U>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U, typename... Args>
		struct are_all_A_or_B
		: std::conditional_t< impl::is_odd<sizeof...(Args)>::value,
		std::conditional_t< is_A_or_B<A, B, T, U>::value && are_all_A_or_B<A, B, Args...>::value, std::true_type, std::false_type>,
		impl::dummy_empty_struct
		> {
	};
	
	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U>
		struct are_all_A_or_B<A, B, T, U>
		: std::conditional_t< is_A_or_B<A, B, T, U>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U, typename... Args>
		struct are_any_A_or_B
		: std::conditional_t< impl::is_odd<sizeof...(Args)>::value,
		std::conditional_t< is_A_or_B<A, B, T, U>::value || are_any_A_or_B<A, B, Args...>::value, std::true_type, std::false_type>,
		impl::dummy_empty_struct
		> {
	};

	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U>
		struct are_any_A_or_B<A, B, T, U>
		: std::conditional_t< is_A_or_B<A, B, T, U>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U, typename... Args>
		struct are_all_A_and_B
		: std::conditional_t< impl::is_odd<sizeof...(Args)>::value,
		std::conditional_t< is_A_and_B<A, B, T, U>::value&& are_all_A_and_B<A, B, Args...>::value, std::true_type, std::false_type>,
		impl::dummy_empty_struct
		> {
	};

	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U>
		struct are_all_A_and_B<A, B, T, U>
		: std::conditional_t< is_A_and_B<A, B, T, U>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U, typename... Args>
		struct are_any_A_and_B
		: std::conditional_t< impl::is_odd<sizeof...(Args)>::value,
		std::conditional_t< is_A_and_B<A, B, T, U>::value || are_any_A_and_B<A, B, Args...>::value, std::true_type, std::false_type>,
		impl::dummy_empty_struct
		> {
	};

	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U>
		struct are_any_A_and_B<A, B, T, U>
		: std::conditional_t< is_A_and_B<A, B, T, U>::value, std::true_type, std::false_type> {
	};

}//namespace zee::advance::binary
}//namespace zee::advance

namespace impl {
	template<typename T, typename...Args>
	struct promotion_type_impl {
	private:
		typedef typename promotion_type_impl<Args...>::type args_type;

	public:
		typedef std::conditional_t<
			std::is_arithmetic<T>::value&& std::is_arithmetic<args_type>::value
			, typename promotion_type_impl<T, args_type>::type
			, void
		> type;
	};

	template<typename T, typename U, bool IsValid = std::is_arithmetic<T>::value&& std::is_arithmetic<U>::value>
	struct promotion_if_valid_impl;

	template<typename T, typename U>
	struct promotion_if_valid_impl <T, U, true> {
		typedef decltype(T(0) + U(0)) type;
	};

	template<typename T, typename U>
	struct promotion_if_valid_impl <T, U, false> {
		typedef void type;
	};

	template<typename FirstT, typename SecondT>
	struct promotion_type_impl<FirstT, SecondT> {
	public:
		typedef typename promotion_if_valid_impl<FirstT, SecondT>::type type;
	};

	template<typename SameT>
	struct promotion_type_impl<SameT, SameT> {
	public:
		typedef std::conditional_t<std::is_arithmetic<SameT>::value, SameT, void> type;
	};

	template<typename AloneT>
	struct promotion_type_impl<AloneT> {
		typedef std::conditional_t<std::is_arithmetic<AloneT>::value, AloneT, void> type;
	};

}//namespace zee::impl

	template<typename... Args>
	using promotion_t = typename impl::promotion_type_impl<Args...>::type;

	template<typename ClassT, typename T> struct is_member_function_specified { static constexpr bool value = false; };
	template<typename ClassT, typename RetT, typename...Args> struct is_member_function_specified <ClassT, RetT(ClassT::*)(Args...)> { static constexpr bool value = true; };
	template<typename ClassT, typename RetT, typename...Args> struct is_member_function_specified <const ClassT, RetT(ClassT::*)(Args...) const> { static constexpr bool value = true; };
	template<typename ClassT, typename RetT, typename...Args> struct is_member_function_specified <const ClassT, RetT(ClassT::*)(Args...) const&> { static constexpr bool value = true; };
	template<typename ClassT, typename RetT, typename...Args> struct is_member_function_specified <ClassT, RetT(ClassT::*)(Args...) const> { static constexpr bool value = true; };
	
	template<typename T> struct is_static_function_specified { static constexpr bool value = false; };
	template<typename RetT, typename...Args> struct is_static_function_specified <RetT(*)(Args...)> { static constexpr bool value = true; };


	template<typename ... Args>
	struct are_all_arithemtic : advance::unary::are_all_A<std::is_arithmetic, Args...> { };

	template<typename ... Args>
	struct are_any_arithemtic : advance::unary::are_any_A<std::is_arithmetic, Args...> { };

	template<typename ... Args>
	struct are_all_floating_point : advance::unary::are_all_A<std::is_floating_point, Args...> { };

	template<typename ... Args>
	struct are_any_floating_point : advance::unary::are_any_A<std::is_floating_point, Args...> { };

	template<typename ... Args>	
	struct are_all_arithmetic_not_floating : advance::unary::are_all_A_not_B<std::is_arithmetic, std::is_floating_point, Args...> { };

	template<typename ... Args>
	struct are_all_signed_not_floating : advance::unary::are_all_A_not_B<std::is_signed, std::is_floating_point, Args...> { };

	template<typename ... Args>
	struct is_arithmetic_not_floating : advance::unary::is_A_not_B<std::is_arithmetic, std::is_floating_point, Args...> { };
	
	template<typename T>
	struct is_vec_element : std::conditional_t<std::is_signed<T>::value, std::true_type, std::false_type> {
	};

	template<typename T>
	struct is_vec_element_not_floating : advance::unary::is_A_not_B<is_vec_element, std::is_floating_point, T> {
	};

	template<typename...Args>
	struct are_all_vec_element : advance::unary::are_all_A<is_vec_element, Args...> {
	};

	template<typename...Args>
	struct are_all_vec_element_not_floating : advance::unary::are_all_A<is_vec_element_not_floating, Args...> {
	};

	template<size_t CompSize, typename VecElemT>
	struct is_vec_argument : 
		std::conditional_t<(CompSize > 0) && std::is_signed<VecElemT>::value, 
		std::true_type, 
		std::false_type> {
	};

	template<typename VecT>
	struct is_vec : std::false_type { };
	
	template<size_t CompSize, typename VecElemT>
	struct is_vec<math::vec_base<CompSize, VecElemT>> : 
		std::conditional_t<
		is_vec_argument<CompSize, VecElemT>::value,
		std::true_type, 
		std::false_type> {
	};

	template<size_t CompSize, typename VecElemT, bool IsValid>
	struct is_vec<math::impl::vec_impl<CompSize, VecElemT, IsValid>> :
		std::conditional_t<
		is_vec_argument<CompSize, VecElemT>::value,
		std::true_type,
		std::false_type> {
	};

	template<typename...Args>
	struct are_all_vec : advance::unary::are_all_A<is_vec, Args...> {
	};

	template<typename T, typename U>
	struct is_same_comp_vec : std::false_type {	};

	template<size_t CompSize, typename T, typename U>
	struct is_same_comp_vec<math::vec_base<CompSize, T>, math::vec_base<CompSize, U>> 
		:  std::conditional_t<are_all_vec_element<T, U>::value && (CompSize > 0), std::true_type, std::false_type> {
	};

namespace impl {
	template<typename T, typename... Args>
	struct vec_promotion_type_impl {
		typedef void type;
	};

	template<size_t CompSize, typename T, typename... Args>
	struct vec_promotion_type_impl<math::vec_base<CompSize, T>, Args...> {
	private:
		typedef typename vec_promotion_type_impl<math::vec_base<CompSize, T>>::type vec1_type;
		typedef typename vec_promotion_type_impl<Args...>::type vec2_type;
	public:
		typedef typename vec_promotion_type_impl<vec1_type, vec2_type>::type type;
	};

	template<size_t CompSize, typename T, typename U>
	struct vec_promotion_type_impl<math::vec_base<CompSize, T>, math::vec_base<CompSize, U>> {
	private:
		typedef std::conditional_t<are_all_vec_element<T, U>::value && (CompSize > 0), promotion_t<T, U>, void> element_type;
	public:
		typedef std::conditional_t<is_vec_element<element_type>::value, math::vec_base<CompSize, element_type>, void> type;
	};

	template<size_t CompSize, typename T>
	struct vec_promotion_type_impl<math::vec_base<CompSize, T>> {
	private:
		typedef std::conditional_t< is_vec_argument<CompSize, T>::value, T, void> element_type;
	public:
		typedef std::conditional_t<is_vec_element<element_type>::value, math::vec_base<CompSize, element_type>, void> type;
	};
}//namespace zee::impl 

	template<typename... Args>
	using vec_promotion_t = typename impl::vec_promotion_type_impl<Args...>::type;

}//namespace zee