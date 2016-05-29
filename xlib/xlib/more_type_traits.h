#ifndef more_type_traits_
#define more_type_traits_

#include <type_traits>

#define IS_SAME(T1, T2) std::is_same<T1,T2>::value

//////////////////////////////////////////////////////////////////////////////

template<typename T, const bool test, const T a, const T b>
struct condition;
template<typename T, const T a, const T b>
struct condition<T, true, a, b>
{
	static constexpr T value = a;
};
template<typename T, const T a, const T b>
struct condition<T, false, a, b>
{
	static constexpr T value = b;
};

template<typename T>
constexpr T _if(const bool test, const T a, const T b)
{
	return condition<T, test, a, b>::value;
}

//////////////////////////////////////////////////////////////////////////////

template<typename T>
struct is_free_fn_ptr
{
	static constexpr bool value = false;
};

template<typename R, typename... A>
struct is_free_fn_ptr<R(*)(A...)>
{
	static constexpr bool value = true;
};

//////////////////////////////////////////////////////////////////////////////

template<typename T>
struct is_member_fn_ptr
{
	static constexpr bool value = false;
};

template<typename R, typename T, typename... A>
struct is_member_fn_ptr<R(T::*)(A...)>
{
	static constexpr bool value = true;
};

//////////////////////////////////////////////////////////////////////////////

template<typename T>
struct is_fn_ptr
{
	static constexpr bool value =
		is_free_fn_ptr<T>::value ||
		is_member_fn_ptr<T>::value;
};

//////////////////////////////////////////////////////////////////////////////

template<typename T, typename... Y_>
struct is_any_of {};

template<typename T, typename Y, typename... Y_>
struct is_any_of<T,Y,Y_...>
{
	static constexpr bool value = std::is_same<T, Y>::value ||
		is_any_of<T, Y_...>::value;
};
template<typename T, typename Y_>
struct is_any_of<T, Y_>
{
	static constexpr bool value = std::is_same<T, Y_>::value;
};

#define IS_ANY_OF(...) is_any_of<__VA_ARGS__>::value

//////////////////////////////////////////////////////////////////////////////

struct __True { __True() {} };
struct __False { __False(__True) {} };

//////////////////////////////////////////////////////////////////////////////

template<typename T, typename... Y>
using select_any = typename 
	std::conditional<is_any_of<T,Y...>::value, 
	T, __False>::type;

//////////////////////////////////////////////////////////////////////////////

template<typename R1, typename T, typename R2, typename... A>
constexpr bool is_return_type_of(R2(T::*const)(A...))
{
	return std::is_same<R1, R2>::value;
};
template<typename R1, typename T, typename R2>
constexpr bool is_return_type_of(R2(T::*const)(...))
{
	return std::is_same<R1, R2>::value;
};
template<typename R1, typename R2, typename... A>
constexpr bool is_return_type_of(R2(*const)(A...))
{
	return std::is_same<R1, R2>::value;
};
template<typename R1, typename R2>
constexpr bool is_return_type_of(R2(*const)(...))
{
	return std::is_same<R1, R2>::value;
};
template<typename T, typename Y>
constexpr bool is_return_type_of(Y)
{
	return std::is_same<T, Y>::value;
};

//////////////////////////////////////////////////////////////////////////////

template<typename T, typename C, typename R, typename... A>
constexpr bool is_argument_of(R(C::* const)(A...))
{
	return is_any_of<T, A...>::value;
}
template<typename T, typename C, typename R>
constexpr bool is_argument_of(R(C::*const)())
{
	return false;
}
template<typename T, typename R, typename... A>
constexpr bool is_argument_of(R(*const)(A...))
{
	return is_any_of<T, A...>::value;
}
template<typename T, typename R>
constexpr bool is_argument_of(R(*const)())
{
	return false;
}

//////////////////////////////////////////////////////////////////////////////

template<bool...>
struct any_true;

template<bool b, bool... b_>
struct any_true<b, b_...>
{
	static constexpr bool value = b || any_true<b_...>::value;
};

template<bool b>
struct any_true<b>
{
	static constexpr bool value = b;
};

#define ANY_TRUE(...) any_true<__VA_ARGS__>::value

//////////////////////////////////////////////////////////////////////////////

template<bool...>
struct all_true;

template<bool b, bool... b_>
struct all_true<b, b_...>
{
	static constexpr bool value = b && all_true<b_...>::value;
};

template<bool b>
struct all_true<b>
{
	static constexpr bool value = b;
};

#define ALL_TRUE(...) all_true<__VA_ARGS__>::value

//////////////////////////////////////////////////////////////////////////////

template<typename...>
struct every_is;

template<typename C, typename T, typename... Y>
struct every_is<C, T, Y...> {
	static constexpr bool value =
		std::is_same<C, T>::value &&
		every_is<C, Y...>::value;
};

template<typename C, typename T>
struct every_is<C, T> {
	static constexpr bool value =
		std::is_same<C, T>::value;
};

//////////////////////////////////////////////////////////////////////////////

template<typename... T>
struct all_same {
	static constexpr bool value =
		every_is<typename select<1, T...>::type, T...>::value;
};

//template<typename T, typename Y, typename... U>
//struct all_same<T, Y, U...> {
//	static constexpr bool value =
//		std::is_same<T, Y>::value &&
//		all_same<Y, U...>::value;
//};
//
//template<typename T, typename Y>
//struct all_same<T, Y> {
//	static constexpr bool value =
//		std::is_same<T, Y>::value;
//};
//
//template<typename T>
//struct all_same<T> {
//	static constexpr bool value =
//		true;
//};
//
//template<>
//struct all_same<> {
//	static constexpr bool value =
//		false;
//};

//////////////////////////////////////////////////////////////////////////////

template<size_t n, typename... T_>
struct select;
template<size_t n, typename T, typename... T_>
struct select<n, T, T_...>
{
	static_assert(n > 0,
		"Given iterator value is 0, which is not valid and equivalent to error-type.");
	typedef typename select<n - 1, T_...>::type type;

	static T value(T v, T_... v_)
	{
		static_assert(all_true<std::is_same<T, T_>::value...>::value,
			"Value types must be the same.");
		return select<n-1, T_...>::value(v_...);
	}

	template<T v, T_... v_>
	struct elem
	{
		static_assert(all_true<std::is_same<T, T_>::value...>::value,
			"Value types must be the same.");
		static constexpr T value = select<n-1, T_...>::elem<v_...>::value;
	};
};
template<size_t n, typename T>
struct select<n, T>
{
	static_assert(n < 2, 
		"Type selection iterator exceeds the parameter pack.");
	typename T type;

	static T value(T v)
	{
		return v;
	}

	template<T v>
	struct elem
	{
		static constexpr T value = v;
	};
};
template<typename T, typename... T_>
struct select<1, T, T_...>
{
	typedef T type;

	static T value(T v, T_... v_)
	{
		return v;
	}

	template<T v, T_... v_>
	struct elem
	{
		static constexpr T value = v;
	};
};
//template<typename... T>
//struct select<0, T...>
//{
//};

//////////////////////////////////////////////////////////////////////////////

template<typename, size_t, typename...>
struct find_;

template<typename T, size_t current, typename Y, typename... Y_>
struct find_<T, current, Y, Y_...>
{
	static constexpr size_t value =
		condition<size_t,
		std::is_same<T, Y>::value,
		current,
		find_<T, current + 1, Y_...>::value>::value;

};

template<typename T, size_t current, typename Y>
struct find_<T, current, Y>
{
	static constexpr size_t value =
		condition<size_t,
		std::is_same<T, Y>::value,
		current,
		0>::value;
};

template<typename T>
struct find_type
{
	template<typename... Y_>
	struct in
	{
		static constexpr size_t value =
			find_<T, 1, Y_...>::value;
	};	
};

//////////////////////////////////////////////////////////////////////////////

template<size_t size, typename... T>
struct same_packs_;

template<size_t size, typename T>
struct same_packs_<size, T>
{
	static constexpr bool value = false;
};

template<typename... T>
struct same_packs_<0, T...>
{
	static constexpr bool value = false;
};

template<typename... T>
struct same_packs_<1, T...>
{
	static constexpr bool value = false;
};

template<size_t size, typename T, typename... Y>
struct same_packs_<size, T, Y...>
{
	static constexpr bool value =
		std::is_same<T, typename select<size / 2, T, Y...>::type>::value &&
		(sizeof...(Y) <= size / 2 ? true : same_packs_<size, Y...>::value);
};

template<typename... T>
struct same_packs
{
	static constexpr bool value =
		same_packs_<sizeof...(T), T...>::value;
};

//////////////////////////////////////////////////////////////////////////////

template<typename T, typename R, typename... A>
std::tuple<A...> args_to_tuple(R(T::*)(A...))
{
	return std::tuple<A...>;
}

//////////////////////////////////////////////////////////////////////////////

template<unsigned...>
struct seq {};

template<unsigned N, unsigned... Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...> { };

template<unsigned... Is>
struct gen_seq<0, Is...> : seq<Is...> { };

//////////////////////////////////////////////////////////////////////////////

template<typename...>
struct same_template;

template<
	template<typename...> typename T1,
	template<typename...> typename T2,
	typename... Y1,
	typename... Y2>
struct same_template<T1<Y1...>, T2<Y2...>>
{
	static constexpr bool value = false;
};

template<
	template<typename...> typename T1, 
	typename... Y1,
	typename... Y2>
struct same_template<T1<Y1...>, T1<Y2...>>
{
	static constexpr bool value = true;
};

//////////////////////////////////////////////////////////////////////////////

#define ADVANCED_MEMBER_TEST(TestName, R_template, member, parameters, ...) \
template<typename T_, R_template, __VA_ARGS__>\
class TestName\
{\
protected:\
	template<typename Y_, R_template, __VA_ARGS__>\
	using MemberType_ = typename std::conditional<std::is_class<T_>::value,\
		R_template(Y_::*)(__VA_ARGS__) parameters, __True>::type;\
	template<typename Y_> static __False test(__False);\
	template<typename Y_> static typename std::conditional<std::is_class<T_>::value,\
		decltype(&Y_::member), __False>::type test(__True);\
public:\
	static constexpr bool value = \
		std::is_same<decltype(test<T_>(__True())),\
		MemberType_<T_,R_template,__VA_ARGS__>>::value;\
};

#define EXPLICIT_MEMBER_TEST(TestName, ReturnType, member, arguments, parameters) \
template<typename T_>\
class TestName\
{\
private:\
	template<typename Y_>\
	using MemberType_ = typename std::conditional<std::is_class<T_>::value,\
		ReturnType(Y_::*)##arguments parameters, __True>::type;\
	template<typename Y_> static __False test(__False);\
	template<typename Y_> static typename std::conditional<std::is_class<T_>::value,\
		decltype(&Y_::member), __False>::type test(__True);\
public:\
	static constexpr bool value = \
		std::is_same<decltype(test<T_>(__True())), MemberType_<T_>>::value;\
};


#define MEMBER_TEST(TestName, member) \
template<typename T>\
class TestName\
{\
private:\
	template <typename Y> static __False test(__False);\
	template <typename Y> static typename std::conditional<std::is_class<T>::value,\
		decltype(&Y::member), __False>::type test(__True);\
public:\
	static constexpr bool value = \
		!std::is_same<decltype(test<T>(__True())), __False>::value;\
};

//////////////////////////////////////////////////////////////////////////////

template<typename T, typename... Y>
struct is_comparable
{
	ADVANCED_MEMBER_TEST(has_compare_op, typename R, operator==, const, typename C);
	static constexpr bool value = any_true<has_compare_op<T, bool, Y>::value...>::value;
};

#endif //more_type_traits_