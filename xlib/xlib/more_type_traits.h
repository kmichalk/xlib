#ifndef	MORE_TYPE_TRAITS_H
#define MORE_TYPE_TRAITS_H

#include <type_traits>
#include "vamacros.h"

//////////////////////////////////////////////////////////////////////////////

namespace x
{

	struct _True
	{
		constexpr _True() {}
	};
	struct _False
	{
		constexpr _False(_True) {}
	};

	//////////////////////////////////////////////////////////////////////////////

	class triv_cpy {};

//////////////////////////////////////////////////////////////////////////////

	template<typename T, typename Y>
	constexpr bool same = false;

	template<typename T>
	constexpr bool same<T, T> = true;

//////////////////////////////////////////////////////////////////////////////

#define ADVANCED_MEMBER_TEST(TestName, R_template, member, parameters, ...) \
template<typename T_, R_template, __VA_ARGS__>\
class TestName\
{\
protected:\
	template<typename Y_, R_template, __VA_ARGS__>\
	using MemberType_ = typename std::conditional<std::is_class<T_>::value,\
		R_template(Y_::*)(__VA_ARGS__) parameters, x::_True>::type;\
	template<typename Y_> static x::_False test(x::_False);\
	template<typename Y_> static typename std::conditional<std::is_class<T_>::value,\
		decltype(&Y_::member), x::_False>::type test(x::_True);\
public:\
	static constexpr bool value = \
		std::is_same<decltype(test<T_>(x::_True())),\
		MemberType_<T_,R_template,__VA_ARGS__>>::value;\
};

//////////////////////////////////////////////////////////////////////////////

#define EXPLICIT_MEMBER_TEST(TestName, ReturnType, member, arguments, parameters) \
template<typename T_>\
class TestName\
{\
private:\
	template<typename Y_>\
	using MemberType_ = typename std::conditional<std::is_class<T_>::value,\
		ReturnType(Y_::*)##arguments parameters, x::_True>::type;\
	template<typename Y_> static x::_False test(x::_False);\
	template<typename Y_> static typename std::conditional<std::is_class<T_>::value,\
		decltype(&Y_::member), x::_False>::type test(x::_True);\
public:\
	static constexpr bool value = \
		std::is_same<decltype(test<T_>(x::_True())), MemberType_<T_>>::value;\
};

//////////////////////////////////////////////////////////////////////////////

#define MEMBER_TEST(TestName, member) \
template<typename T>\
class TestName\
{\
private:\
	template <typename Y> static x::_False test(x::_False);\
	template <typename Y> static typename std::conditional<std::is_class<T>::value,\
		decltype(&Y::member), x::_False>::type test(x::_True);\
public:\
	static constexpr bool value = \
		!std::is_same<decltype(test<T>(x::_True())), x::_False>::value;\
};

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

	MEMBER_TEST(is_functor, operator())

	//////////////////////////////////////////////////////////////////////////////

		template<typename T>
	struct is_free_func_ptr
	{
		static constexpr bool value = false;
	};

	template<typename R, typename... A>
	struct is_free_func_ptr<R(*)(A...)>
	{
		static constexpr bool value = true;
	};

	//////////////////////////////////////////////////////////////////////////////

	template<typename T>
	struct is_member_func_ptr
	{
		static constexpr bool value = false;
	};

	template<typename R, typename T, typename... A>
	struct is_member_func_ptr<R(T::*)(A...)>
	{
		static constexpr bool value = true;
	};

	//////////////////////////////////////////////////////////////////////////////

	/*template<typename T>
	struct is_func_ptr
	{
		static constexpr bool value =
			is_free_func_ptr<T>::value ||
			is_member_func_ptr<T>::value;
	};*/

	template<class>
	struct is_func;

	//////////////////////////////////////////////////////////////////////////////

	template<class _Type>
	struct is_callable
	{
		static constexpr bool value =
			is_functor<_Type>::value ||
			is_free_func_ptr<_Type>::value ||
			std::is_function<_Type>::value;
	};

	//////////////////////////////////////////////////////////////////////////////

	template<typename T, typename... Y_>
	struct is_any_of {};

	template<typename T, typename Y, typename... Y_>
	struct is_any_of<T, Y, Y_...>
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

	template<typename T, typename... Y>
	using select_any = typename
		std::conditional<is_any_of<T, Y...>::value,
		T, _False>::type;

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

	template<typename R, typename... A>
	constexpr unsigned argnum(R(*)(A...))
	{
		return sizeof...(A);
	}

	template<typename T, typename R, typename... A>
	constexpr unsigned argnum(R(T::*)(A...))
	{
		return sizeof...(A);
	}

	template<typename _Func>
	unsigned argnum(_Func&&)
	{
		return argnum(&_Func::operator());
	}

	template<typename _Func>
	constexpr unsigned argnum()
	{
		return argnum(&_Func::operator());
	}

	//////////////////////////////////////////////////////////////////////////////

	//template<typename...>
	//struct owner_of {};
	//
	//template<typename T, typename R, typename... A>
	//__forceinline constexpr owner_of<T, R, A...>& _owner_of(R(T::*)(A...))
	//{
	//	return owner_of<T, R, A...>{};
	//}
	//
	//template<

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

//template<int...>
//struct sum;
//
//template<int n, int... rest>
//struct sum<n, rest...>
//{
//	static constexpr int value = n + sum<rest...>::value;
//};
//
//template<int n>
//struct sum<n>
//{
//	static constexpr int value = n;
//};
//
//template<>
//struct sum<>
//{
//	static constexpr int value = 0;
//};

//////////////////////////////////////////////////////////////////////////////

	template<typename...>
	struct every_is;

	template<typename C, typename T, typename... Y>
	struct every_is<C, T, Y...>
	{
		static constexpr bool value =
			std::is_same<C, T>::value &&
			every_is<C, Y...>::value;
	};

	template<typename C, typename T>
	struct every_is<C, T>
	{
		static constexpr bool value =
			std::is_same<C, T>::value;
	};

	//////////////////////////////////////////////////////////////////////////////

	template<typename... T>
	struct all_same
	{
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

		/*static T value(T v, T_... v_)
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
		};*/
	};
	template<size_t n, typename T>
	struct select<n, T>
	{
		static_assert(n < 2,
			"Type selection iterator exceeds the parameter pack.");
		typename T type;

		/*static T value(T v)
		{
			return v;
		}

		template<T v>
		struct elem
		{
			static constexpr T value = v;
		};*/
	};
	template<typename T, typename... T_>
	struct select<1, T, T_...>
	{
		typedef T type;

		/*static T value(T v, T_... v_)
		{
			return v;
		}

		template<T v, T_... v_>
		struct elem
		{
			static constexpr T value = v;
		};*/
	};

	template<size_t n, typename... T_>
	using select_t = typename select<n, T_...>::type;


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

	//template<typename T, typename R, typename... A>
	//std::tuple<A...> args_to_tuple(R(T::*)(A...))
	//{
	//	return std::tuple<A...>;
	//}

	//////////////////////////////////////////////////////////////////////////////

	template<unsigned...>
	struct seq {};

	template<unsigned N, unsigned... Is>
	struct gen_seq: gen_seq<N - 1, N - 1, Is...> {};

	template<unsigned... Is>
	struct gen_seq<0, Is...>: seq<Is...> {};

	template<typename... Ts>
	struct type_seq: gen_seq<sizeof...(Ts)> {};

	//////////////////////////////////////////////////////////////////////////////

	template<typename...>
	struct same_template
	{
		static constexpr bool value = false;
	};

	//template<
	//	template<typename...> typename T1,
	//	template<typename...> typename T2,
	//	typename... Y1,
	//	typename... Y2>
	//struct same_template<T1, T2<Y2...>>
	//{
	//	static constexpr bool value = false;
	//};

	template<
		template<typename...> typename T1,
		typename... Y1,
		typename... Y2>
		struct same_template<T1<Y1...>, T1<Y2...>>
	{
		static constexpr bool value = true;
	};

	//////////////////////////////////////////////////////////////////////////////

	template<typename T, typename... Y>
	struct is_comparable
	{
		ADVANCED_MEMBER_TEST(has_compare_op, typename R, operator==, const, typename C);
		static constexpr bool value = any_true<has_compare_op<T, bool, Y>::value...>::value;
	};

	//////////////////////////////////////////////////////////////////////////////

	template<typename T>
	struct is_trivially_copyable
	{
		static constexpr bool value =
			std::is_trivially_copyable<T>::value ||
			std::is_base_of<triv_cpy, T>::value;
	};

	//////////////////////////////////////////////////////////////////////////////

	template<bool>
	struct _StaticIf;

	template<>
	struct _StaticIf<true>
	{};

	//////////////////////////////////////////////////////////////////////////////

	template<typename _Member>
	struct owner_of;

	template<class _Owner, typename _Ret, typename... _Args>
	struct owner_of<_Ret(_Owner::*)(_Args...)>
	{
		using type = _Owner;
	};

	template<class _Owner, typename _Ret>
	struct owner_of<_Ret(_Owner::*)>
	{
		using type = _Owner;
	};

	template<class _Member>
	using owner_of_t = typename owner_of<_Member>::type;

	template<class...>
	struct function_type_of;

	template<class _Owner, class _Ret, class... _Args>
	struct function_type_of<_Ret(_Owner::*)(_Args...)>
	{
		using type = _Ret(_Args...);
	};

	template<class _Ret, class... _Args>
	struct function_type_of<_Ret(*)(_Args...)>
	{
		using type = _Ret(_Args...);
	};

	template<class _Ret, class... _Args>
	struct function_type_of<_Ret(_Args...)>
	{
		using type = _Ret(_Args...);
	};

	template<class _Member>
	using function_type_of_t = typename function_type_of<_Member>::type;

//#define owner_of(...) typename x::_OwnerOf<decltype(__VA_ARGS__)>::type

	template<typename...>
	struct _result;

	template<typename R, typename... A>
	struct _result<R(A...)>
	{
		using type = R;
	};

	template<class T, typename R, typename... A>
	struct _result<R(T::*)(A...)>
	{
		using type = R;
	};

	template<class _Type>
	using same_type = _Type;

	//////////////////////////////////////////////////////////////////////////////

	template<class _Type>
	struct is_naked
	{
		static constexpr bool value = 
			(std::is_fundamental<_Type>::value || std::is_class<_Type>::value) &&
			!std::is_const<_Type>::value && !std::is_volatile<_Type>::value;
	};

	template<class _Outer>
	using in_type = typename _Outer::type;

	template<class _Outer>
	using in_raw_type = typename _Outer::raw_type;

	template<class _Type>
	struct nested
	{
		using type = _Type;
	};

	template<class _Type>
	struct naked
	{
		using raw_type = _Type;
		using type = std::conditional_t<
			is_naked<_Type>::value,
			_Type, in_type<
				std::conditional_t<is_naked<_Type>::value, 
					nested<void>,
					naked<std::remove_cv_t<std::remove_pointer_t<std::remove_reference_t<_Type>>>>
				>
			>
		>;
	};

	template<class _Type>
	using naked_t = typename naked<_Type>::type;

	/*template<class _Type>
	using naked = std::remove_cv_t<std::remove_pointer_t<std::remove_reference_t<_Type>>>;*/



	//////////////////////////////////////////////////////////////////////////////

	template<class _OnTrue, class _OnFalse>
	struct StaticSelector
	{
		_OnTrue& onTrue;
		_OnFalse& onFalse;

		inline StaticSelector(
			_OnTrue& onTrue,
			_OnFalse& onFalse)
			:
			onTrue{onTrue},
			onFalse{onFalse}
		{
		}

		template<bool _cond>
		__forceinline std::enable_if_t<_cond == true,
			_OnTrue&> select() const
		{
			return onTrue;
		}

		template<bool _cond>
		__forceinline std::enable_if_t<_cond == false,
			_OnFalse&> select() const
		{
			return onFalse;
		}
	};


	///////////////////////////////////////////////////////////////////////////////


	template<class _Type, unsigned _dim>
	struct multi_ptr
	{
		using type = typename multi_ptr<_Type, _dim - 1>::type*;
	};

	template<class _Type>
	struct multi_ptr<_Type, 0>
	{
		using type = _Type;
	};

	template<class _Type, unsigned _dim>
	using multi_ptr_t = typename multi_ptr<_Type, _dim>::type;

	///////////////////////////////////////////////////////////////////////////////

	/*template<class _Base, class _Der>
	class is_virtual_base_of
	{
		template<class = void>
		struct Derived: _Der
		{
		};

		template<class = void>
		struct VirtuallyDerived: virtual _Base, _Der
		{
		};

		static constexpr Derived

	public:
		static constexpr bool value = 
			std::is_base_of<_Base, _Der>::value && 
			sizeof(Derived) == sizeof(VirtuallyDerived);
	};*/

	///////////////////////////////////////////////////////////////////////////////

#define _result(_fn) x::_result<std::remove_pointer_t<decltype(_fn)>>::type
#define _concept class = std::enable_if_t
#define _capture(_Type) class _Type = _Type
#define _suspend template<class _Suspender = void>

#define THIS_TYPE template<class=void> inline constexpr void __member__(){}\
	using this_type = x::owner_of_t<decltype(&__member__<>)>;
}

#endif //MORE_TYPE_TRAITS_H