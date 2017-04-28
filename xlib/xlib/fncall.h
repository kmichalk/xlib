#ifndef FNCALL_H
#define FNCALL_H

#include "fn.h"
#include "more_type_traits.h"
#include "va_handler.h"
#include "loops.h"

template<typename... A>
__forceinline void variadic_call(A&&...) {}

namespace x
{

struct Eraser
{};

template<typename T>
struct Value: public Eraser
{
	T value;
	Value(T&& value): value{value} {}
};
}


//template<typename...>
//class FnCall;
//
//template<typename R, typename... A>
//class FnCall<R(A...)>
//{
//
//public:
//	static constexpr unsigned nargs = sizeof...(A);
//	
//
//	x::Eraser* args_[nargs];
//	Fn<R(A...)> fn_;
//
//	template<int... i>
//	__forceinline void assign_(seq<i...>&, A... args)
//	{
//		size_t k = 0;
//		variadic_call((args_[k++] = new Value<A>(args))...);
//	}
//	
//	
//public:
//	template<int... i>
//	__forceinline R call_(seq<i...>&) const
//	{
//		fn_(static_cast<x::Value<x::select_t<i+1, A...>>*>(args_[i])->value ...);
//	}
//
//	__forceinline R operator()() const
//	{
//		call_(gen_seq<nargs>());
//	}
//	
//	template<typename _Func>
//	FnCall(_Func&& fn, A... args):
//		fn_{std::forward<_Func>(fn)}
//	{
//		assign_(gen_seq<nargs>(), args...);
//	}
//
//};

#endif //FNCALL_H
