#ifndef FN_HELPER_H
#define FN_HELPER_H

#include "more_type_traits.h"

#define enable_if typename std::enable_if_t

//template<typename...>
//class fn_helper;
//
//// Member functions
//template<typename T, typename R, typename... A>
//class fn_helper<T,R(A...)>
//{
//	template<R(T::* const fn_ptr)(A...)>
//	struct FnData_
//	{
//		static unsigned n_calls;
//		static unsigned n_failed;
//		static bool called;
//	};
//
//public:
//	template<bool test, R(T::* const fn_ptr)(A...)>
//	static __forceinline enable_if<test, 
//		void> call_if(T* obj, A... args)
//	{
//		(obj->*fn_ptr)(args...);
//	}
//
//	template<bool test, R(T::* const fn_ptr)(A...)>
//	static __forceinline enable_if<!test,
//		void> call_if(T* obj, A... args)
//	{
//	}
//
//	template<R(T::* const fn_ptr)(A...), bool count = false>
//	static inline enable_if<count,
//		void> call_once(T* obj, A... args)
//	{
//		if (!FnData_<fn_ptr>::n_calls < 1) {
//			(obj->*fn_ptr)(args...);
//			++FnData_<fn_ptr>::n_calls;
//		}
//		else {
//			++FnData_<fn_ptr>::n_failed;
//		}
//	}
//
//	template<R(T::* const fn_ptr)(A...), bool count = false>
//	static inline enable_if<!count,
//		void> call_once(T* obj, A... args)
//	{
//		if (!FnData_<fn_ptr>::called) {
//			(obj->*fn_ptr)(args...);
//			FnData_<fn_ptr>::called = true;
//		}
//	}
//
//	template<unsigned n, R(T::* const fn_ptr)(A...)>
//	static inline void call_times(T* obj, A... args)
//	{
//		if (FnData_<fn_ptr>::n_calls < n) {
//			(obj->*fn_ptr)(args...);
//			FnData_<fn_ptr>::called = true;
//			++FnData_<fn_ptr>::n_calls;
//		}
//		else {
//			++FnData_<fn_ptr>::n_failed;
//		}
//	}
//
//	template<unsigned n, R(T::* const fn_ptr)(A...)>
//	static inline void call_after(T* obj, A... args)
//	{
//		if (FnData_<fn_ptr>::n_failed >= n)	{
//			(obj->*fn_ptr)(args...);
//			++FnData_<fn_ptr>::n_calls;
//		}
//		else {
//			++FnData_<fn_ptr>::n_failed;
//		}
//	}
//
//	template<R(T::* const fn_ptr)(A...)>
//	static inline void reset()
//	{
//		FnData_<fn_ptr>::called = false;
//	}
//};
//
//template<typename T, typename R, typename... A>
//template<R(T::* const fn_ptr)(A...)>
//bool fn_helper<T, R(A...)>::FnData_<fn_ptr>::called = false;
//
//template<typename T, typename R, typename... A>
//template<R(T::* const fn_ptr)(A...)>
//unsigned fn_helper<T, R(A...)>::FnData_<fn_ptr>::n_calls = 0;
//
//template<typename T, typename R, typename... A>
//template<R(T::* const fn_ptr)(A...)>
//unsigned fn_helper<T, R(A...)>::FnData_<fn_ptr>::n_failed = 0;
//
//// Non member functions
//template<typename R, typename... A>
//class fn_helper<R(A...)>
//{
//	static bool called_;
//
//	template<R(* const fn_ptr)(A...)>
//	struct FnData_
//	{
//		static unsigned n_calls;
//		static unsigned n_failed;
//		static bool called;
//	};
//
//public:
//	template<bool test, R(* const fn_ptr)(A...)>
//	static __forceinline enable_if<test,
//		void> call_if(A... args)
//	{
//		(*fn_ptr)(args...);
//	}
//
//	template<bool test, R(* const fn_ptr)(A...)>
//	static __forceinline enable_if<!test,
//		void> call_if(A... args)
//	{
//	}
//
//	template<R(* const fn_ptr)(A...), bool count = false>
//	static inline enable_if<count,
//		void> call_once(A... args)
//	{
//		if (!FnData_<fn_ptr>::called) {
//			(*fn_ptr)(args...);
//			FnData_<fn_ptr>::called = true;
//			++FnData_<fn_ptr>::n_calls;
//		}
//		else {
//			++FnData_<fn_ptr>::n_failed;
//		}
//	}
//
//	template<R(* const fn_ptr)(A...), bool count = false>
//	static inline enable_if<!count,
//		void> call_once(A... args)
//	{
//		if (!FnData_<fn_ptr>::called) {
//			(*fn_ptr)(args...);
//			FnData_<fn_ptr>::called = true;
//		}
//	}
//
//	template<unsigned n, R(* const fn_ptr)(A...)>
//	static inline void call_times(A... args)
//	{
//		if (FnData_<fn_ptr>::n_calls < n) {
//			(*fn_ptr)(args...);
//			FnData_<fn_ptr>::called = true;
//			++FnData_<fn_ptr>::n_calls;
//		}
//		else {
//			++FnData_<fn_ptr>::n_failed;
//		}
//	}
//
//	template<R(* const fn_ptr)(A...)>
//	static inline void reset()
//	{
//		FnData_<fn_ptr>::called = false;
//	}
//};
//
//template<typename R, typename... A>
//template<R(*const fn_ptr)(A...)>
//bool fn_helper<R(A...)>::FnData_<fn_ptr>::called = false;
//
//template<typename R, typename... A>
//template<R(*const fn_ptr)(A...)>
//unsigned fn_helper<R(A...)>::FnData_<fn_ptr>::n_calls = 0;
//
//template<typename R, typename... A>
//template<R(*const fn_ptr)(A...)>
//unsigned fn_helper<R(A...)>::FnData_<fn_ptr>::n_failed = 0;
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//


template<typename F>
class fn_helper
{
	F fn;
	unsigned n_calls;
	//unsigned n_failed;
	//bool called;

public:

	fn_helper(F fn):
		fn{fn}, n_calls{0}
	{}
	/*template<bool test, R(T::* const fn_ptr)(A...)>
	static __forceinline enable_if<test,
		void> call_if(T* obj, A... args)
	{
		(obj->*fn_ptr)(args...);
	}

	template<bool test, R(T::* const fn_ptr)(A...)>
	static __forceinline enable_if<!test,
		void> call_if(T* obj, A... args)
	{
	}

	template<R(T::* const fn_ptr)(A...), bool count = false>
	static inline enable_if<count,
		void> call_once(T* obj, A... args)
	{
		if (!FnData_<fn_ptr>::n_calls < 1) {
			(obj->*fn_ptr)(args...);
			++FnData_<fn_ptr>::n_calls;
		}
		else {
			++FnData_<fn_ptr>::n_failed;
		}
	}

	template<R(T::* const fn_ptr)(A...), bool count = false>
	static inline enable_if<!count,
		void> call_once(T* obj, A... args)
	{
		if (!FnData_<fn_ptr>::called) {
			(obj->*fn_ptr)(args...);
			FnData_<fn_ptr>::called = true;
		}
	}

	template<unsigned n, R(T::* const fn_ptr)(A...)>
	static inline void call_times(T* obj, A... args)
	{
		if (FnData_<fn_ptr>::n_calls < n) {
			(obj->*fn_ptr)(args...);
			FnData_<fn_ptr>::called = true;
			++FnData_<fn_ptr>::n_calls;
		}
		else {
			++FnData_<fn_ptr>::n_failed;
		}
	}

	template<unsigned n, R(T::* const fn_ptr)(A...)>
	static inline void call_after(T* obj, A... args)
	{
		if (FnData_<fn_ptr>::n_failed >= n) {
			(obj->*fn_ptr)(args...);
			++FnData_<fn_ptr>::n_calls;
		}
		else {
			++FnData_<fn_ptr>::n_failed;
		}
	}

	template<R(T::* const fn_ptr)(A...)>
	static inline void reset()
	{
		FnData_<fn_ptr>::called = false;
	}*/
};

template<typename F>
fn_helper<F> create(F&& fn)
{
	return fn_helper<F>(fn);
}

#undef enable_if

#endif
