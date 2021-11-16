#pragma once
#include <type_traits>
#include <utility>

namespace zee {
	template<bool A, bool B> struct logical_xor;
	template<> struct logical_xor<false, false> : std::false_type {};
	template<> struct logical_xor<false, true > : std::true_type  {};
	template<> struct logical_xor<true , false> : std::true_type  {};
	template<> struct logical_xor<true , true > : std::false_type {};

namespace impl {
	template<typename BindType, BindType BindValue, template<BindType, typename> typename T>
	struct bind_bin_to_uni_non_type_param0 {
		template<typename ParamT>
		using type =  T<BindValue, ParamT>;
	};

	template<typename BindType, BindType BindValue, template<typename, BindType> typename T>
	struct bind_bin_to_uni_non_type_param1 {
		template<typename ParamT>
		using type = T<ParamT, BindValue>;
	};

	template<typename BindType, template<typename, typename> typename T>
	struct bind_bin_to_uni_type_param0 {
		template<typename ParamT>
		using type = T<BindType, ParamT>;
	};

	template<typename BindType, template<typename, typename> typename T>
	struct bind_bin_to_uni_type_param1 {
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
		struct is_all_A
		: std::conditional_t< is_all_A<A, T>::value&& is_all_A<A, Args...>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A,
		typename T>
		struct is_all_A<A, T>
		: std::conditional_t<A<T>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A,
		typename T, typename... Args>
		struct is_not_all_A
		: std::conditional_t< is_not_all_A<A, T>::value && is_not_all_A<A, Args...>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A,
		typename T>
		struct is_not_all_A<A, T>
		: std::conditional_t< is_not_A<A, T>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A,
		typename T, typename... Args>
		struct is_any_A
		: std::conditional_t< is_any_A<A, T>::value || is_any_A<A, Args...>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A, 
		typename T>
	struct is_any_A<A, T>
		: std::conditional_t<A<T>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A,
		typename T, typename... Args>
		struct is_not_any_A
		: std::conditional_t< is_not_any_A<A, T>::value || is_not_any_A<A, Args...>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A,
		typename T>
		struct is_not_any_A<A, T>
		: std::conditional_t< is_not_A<A, T>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T, typename... Args>
		struct is_all_A_not_B
		: std::conditional_t< is_all_A_not_B<A, B, T>::value && is_all_A_not_B<A, B, Args...>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T>
		struct is_all_A_not_B<A, B, T>
		: std::conditional_t< is_A_not_B<A, B, T>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T, typename... Args>
		struct is_any_A_not_B
		: std::conditional_t< is_A_not_B<A, B, T>::value || is_any_A_not_B<A, B, Args...>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T>
		struct is_any_A_not_B<A, B, T>
		: std::conditional_t< is_A_not_B<A, B, T>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T, typename... Args>
		struct is_all_A_or_B
		: std::conditional_t< is_A_or_B<A, B, T>::value && is_all_A_or_B<A, B, Args...>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T>
		struct is_all_A_or_B<A, B, T>
		: std::conditional_t< is_A_or_B<A, B, T>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T, typename... Args>
		struct is_any_A_or_B
		: std::conditional_t< is_A_or_B<A, B, T>::value || is_any_A_or_B<A, B, Args...>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T>
		struct is_any_A_or_B<A, B, T>
		: std::conditional_t< is_A_or_B<A, B, T>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T, typename... Args>
		struct is_all_A_and_B
		: std::conditional_t< is_A_and_B<A, B, T>::value && is_all_A_and_B<A, B, Args...>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T>
		struct is_all_A_and_B<A, B, T>
		: std::conditional_t< is_A_and_B<A, B, T>::value , std::true_type, std::false_type> {
	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T, typename... Args>
		struct is_any_A_and_B
		: std::conditional_t< is_A_and_B<A, B, T>::value || is_any_A_and_B<A, B, Args...>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename> typename A,
		template <typename> typename B,
		typename T>
		struct is_any_A_and_B<A, B, T>
		: std::conditional_t< is_A_and_B<A, B, T>::value, std::true_type, std::false_type> {
	};

}//namespace zee::advance::unary

namespace binary {
namespace impl {
	template<size_t Size>
	struct is_odd : std::conditional_t<Size % 2 == 0, std::true_type, std::false_type> { };
	struct dummy_ { };
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
		struct is_all_A
		: std::conditional_t< impl::is_odd<sizeof...(Args)>::value,
		std::conditional_t< is_A<A, T, U>::value && is_all_A<A, Args...>::value, std::true_type, std::false_type>,
		impl::dummy_
		> {
	};

	template<
		template <typename, typename> typename A, 
		typename T, typename U>
	struct is_all_A<A, T, U>
		: std::conditional_t< is_A<A, T ,U>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename, typename> typename A,
		typename T, typename U, typename... Args>
		struct is_not_all_A
		: std::conditional_t< impl::is_odd<sizeof...(Args)>::value,
		std::conditional_t< is_not_A<A, T, U>::value && is_not_all_A<A, Args...>::value, std::true_type, std::false_type>,
		impl::dummy_
		> {
	};

	template<
		template <typename, typename> typename A,
		typename T, typename U>
		struct is_not_all_A<A, T, U>
		: std::conditional_t< is_not_A<A, T, U>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename, typename> typename A,
		typename T, typename U, typename... Args>
		struct is_any_A
		: std::conditional_t< impl::is_odd<sizeof...(Args)>::value,
		std::conditional_t< is_A<A, T, U>::value || is_any_A<A, Args...>::value, std::true_type, std::false_type>,
		impl::dummy_
		> {
	};
	
	template<
		template <typename, typename> typename A,
		typename T, typename U>
	struct is_any_A<A, T, U>
		: std::conditional_t< is_A<A, T, U>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename, typename> typename A,
		typename T, typename U, typename... Args>
		struct is_not_any_A
		: std::conditional_t< impl::is_odd<sizeof...(Args)>::value,
		std::conditional_t< is_not_A<A, T, U>::value || is_not_any_A<A, Args...>::value, std::true_type, std::false_type>,
		impl::dummy_
		> {
	};

	template<
		template <typename, typename> typename A,
		typename T, typename U>
		struct is_not_any_A<A, T, U>
		: std::conditional_t< is_not_A<A, T, U>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U, typename... Args>
		struct is_all_A_not_B
		: std::conditional_t< impl::is_odd<sizeof...(Args)>::value,
		std::conditional_t< is_A_not_B<A, B, T, U>::value && is_all_A_not_B<A, B, Args...>::value, std::true_type, std::false_type>,
		impl::dummy_
		> {
	};
	
	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U>
		struct is_all_A_not_B<A, B, T, U>
		: std::conditional_t< is_A_not_B<A, B, T, U>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U, typename... Args>
		struct is_any_A_not_B
		: std::conditional_t< impl::is_odd<sizeof...(Args)>::value,
		std::conditional_t< is_A_not_B<A, B, T, U>::value || is_any_A_not_B<A, B, Args...>::value, std::true_type, std::false_type>,
		impl::dummy_
		> {
	};
	
	
	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U>
		struct is_any_A_not_B<A, B, T, U>
		: std::conditional_t< is_A_not_B<A, B, T, U>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U, typename... Args>
		struct is_all_A_or_B
		: std::conditional_t< impl::is_odd<sizeof...(Args)>::value,
		std::conditional_t< is_A_or_B<A, B, T, U>::value && is_all_A_or_B<A, B, Args...>::value, std::true_type, std::false_type>,
		impl::dummy_
		> {
	};
	
	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U>
		struct is_all_A_or_B<A, B, T, U>
		: std::conditional_t< is_A_or_B<A, B, T, U>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U, typename... Args>
		struct is_any_A_or_B
		: std::conditional_t< impl::is_odd<sizeof...(Args)>::value,
		std::conditional_t< is_A_or_B<A, B, T, U>::value || is_any_A_or_B<A, B, Args...>::value, std::true_type, std::false_type>,
		impl::dummy_
		> {
	};

	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U>
		struct is_any_A_or_B<A, B, T, U>
		: std::conditional_t< is_A_or_B<A, B, T, U>::value, std::true_type, std::false_type> {

	};

	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U, typename... Args>
		struct is_all_A_and_B
		: std::conditional_t< impl::is_odd<sizeof...(Args)>::value,
		std::conditional_t< is_A_and_B<A, B, T, U>::value&& is_all_A_and_B<A, B, Args...>::value, std::true_type, std::false_type>,
		impl::dummy_
		> {
	};

	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U>
		struct is_all_A_and_B<A, B, T, U>
		: std::conditional_t< is_A_and_B<A, B, T, U>::value, std::true_type, std::false_type> {
	};

	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U, typename... Args>
		struct is_any_A_and_B
		: std::conditional_t< impl::is_odd<sizeof...(Args)>::value,
		std::conditional_t< is_A_and_B<A, B, T, U>::value || is_any_A_and_B<A, B, Args...>::value, std::true_type, std::false_type>,
		impl::dummy_
		> {
	};

	template<
		template <typename, typename> typename A,
		template <typename, typename> typename B,
		typename T, typename U>
		struct is_any_A_and_B<A, B, T, U>
		: std::conditional_t< is_A_and_B<A, B, T, U>::value, std::true_type, std::false_type> {
	};

}//namespace zee::advance::binary
}//namespace zee::advance

namespace impl {
	template<typename T, typename...Args>
	struct promotion_type {
	private:
		typedef typename promotion_type<Args...>::type args_type;

	public:
		typedef decltype(T(0) + args_type(0)) type;
	};

	template<typename FirstT, typename SecondT>
	struct promotion_type<FirstT, SecondT> {
	public:
		typedef decltype(FirstT(0) + SecondT(0)) type;
	};
	
}//namespace zee::impl

	template<typename... Args>
	using promotion_t = typename impl::promotion_type<Args...>::type;

	template<typename ClassT, typename T> struct is_member_function_specified { static constexpr bool value = false; };
	template<typename ClassT, typename RetT, typename...Args> struct is_member_function_specified <ClassT, RetT(ClassT::*)(Args...)> { static constexpr bool value = true; };
	template<typename ClassT, typename RetT, typename...Args> struct is_member_function_specified <const ClassT, RetT(ClassT::*)(Args...) const> { static constexpr bool value = true; };
	template<typename ClassT, typename RetT, typename...Args> struct is_member_function_specified <const ClassT, RetT(ClassT::*)(Args...) const&> { static constexpr bool value = true; };
	template<typename ClassT, typename RetT, typename...Args> struct is_member_function_specified <ClassT, RetT(ClassT::*)(Args...) const> { static constexpr bool value = true; };
	
	template<typename T> struct is_static_function_specified { static constexpr bool value = false; };
	template<typename RetT, typename...Args> struct is_static_function_specified <RetT(*)(Args...)> { static constexpr bool value = true; };

	//Forward declaration
namespace math {
	template<size_t/*CompSize*/, typename/*VecElemT*/> struct vec_base;
}//namespace zee::math

namespace impl {
	template<typename T>
	struct is_vec_elem_impl {
	private:
		typedef char yes[2];
		typedef char no[1];

		template<typename U> static std::enable_if_t<std::is_signed<U>::value, yes&> check(U*);
		template<typename U> static no& check(...);

	public:
		static constexpr bool value = sizeof(check<T>(0)) == sizeof(yes);
	};

	template<typename T>
	struct is_vec_impl {
		static constexpr bool value = false;
	};

	template<size_t CompSize, typename VecElemT>
	struct is_vec_impl<math::vec_base<CompSize, VecElemT>> {
	private:
		typedef char yes[2];
		typedef char no[1];

		template<size_t CompSize, typename T> static std::enable_if_t<is_vec_elem_impl<T>::value && (CompSize > 0), yes&> check(std::nullptr_t);
		template<size_t CompSize, typename T> static no& check(...);

	public:
		static constexpr bool value = sizeof(check<CompSize, VecElemT>(nullptr)) == sizeof(yes);
	};

	template<size_t CompSize, typename VecT>
	struct is_vec_with_size_impl {
		static constexpr bool value = false;
	};

	template<size_t CompSize, size_t VecCompSize, typename VecElemT>
	struct is_vec_with_size_impl<CompSize, math::vec_base<VecCompSize, VecElemT>> {
		static constexpr bool value = false;
	};

	template<size_t VecCompSize, typename VecElemT>
	struct is_vec_with_size_impl<VecCompSize, math::vec_base<VecCompSize, VecElemT>> {
		static constexpr bool value = is_vec_impl<math::vec_base<VecCompSize, VecElemT>>::value;
	};

	template<typename... Args>
	struct vec_promoition { 
		typedef void type;
		typedef void elem_type;
	};

	template<size_t CompSize, typename VecElem1T>
	struct vec_promoition <math::vec_base<CompSize, VecElem1T>> {
		typedef math::vec_base<CompSize, VecElem1T> type;
		typedef VecElem1T elem_type;
	};

	template<size_t CompSize, typename VecElem1T, typename... Args>
	struct vec_promoition <math::vec_base<CompSize, VecElem1T>, Args...>  {
		typedef typename vec_promoition<Args...>::elem_type inner_elem_type;
		typedef promotion_t<VecElem1T, inner_elem_type> elem_type;
		typedef math::vec_base<CompSize, promotion_t<VecElem1T, inner_elem_type>> type;
	};

}//namespace zee::impl
	template<typename ...VecArgs>
	using vec_promotion_t = typename impl::vec_promoition< VecArgs...>::type;
	
	template<typename T>
	struct is_vec_elem : std::conditional_t<impl::is_vec_elem_impl<T>::value, std::true_type, std::false_type> { };

	template<typename T>
	struct is_vec : std::conditional_t<impl::is_vec_impl<T>::value, std::true_type, std::false_type> { };

	template<size_t CompSize, typename T>
	struct is_vec_with_size : std::conditional_t<impl::is_vec_with_size_impl<CompSize, T>::value, std::true_type, std::false_type> { };

	template<typename T>
	struct is_vec_elem_not_floating : advance::unary::is_A_not_B<is_vec_elem, std::is_floating_point, T> { };
	
	template<typename ... Args>
	struct is_all_arithemtic : advance::unary::is_all_A<std::is_arithmetic, Args...> { };

	template<typename ... Args>
	struct is_any_arithemtic : advance::unary::is_any_A<std::is_arithmetic, Args...> { };

	template<typename ... Args>
	struct is_all_floating_point : advance::unary::is_all_A<std::is_floating_point, Args...> { };

	template<typename ... Args>
	struct is_any_floating_point : advance::unary::is_any_A<std::is_floating_point, Args...> { };

	template<typename ... Args>	
	struct is_all_arithmetic_not_floating : advance::unary::is_all_A_not_B<std::is_arithmetic, std::is_floating_point, Args...> { };

	template<typename ... Args>
	struct is_all_signed_not_floating : advance::unary::is_all_A_not_B<std::is_signed, std::is_floating_point, Args...> { };

	template<typename ...Args>
	struct is_all_vec_elem : advance::unary::is_all_A<is_vec_elem, Args...> { };

	template<typename... Args>
	struct is_all_vec_elem_not_floating : advance::unary::is_all_A_not_B<is_vec_elem, std::is_floating_point, Args...> { };

	template<typename ...Args>
	struct is_all_vec : advance::unary::is_all_A<is_vec, Args...> { };

	template<size_t CompSize, typename ...Args>
	struct is_all_vec_with_size : advance::unary::is_all_A<typename impl::bind_bin_to_uni_non_type_param0<size_t, CompSize, is_vec_with_size>::type, Args...> { };

}//namespace zee