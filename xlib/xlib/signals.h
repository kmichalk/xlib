#pragma once

#include "more_type_traits.h"
#include "xarr.h"

//#define __extern static
#define forceinline __forceinline

#define always false


template<typename... A>
class _Slot
{
public:
	forceinline virtual void operator()(A...) = 0;
	forceinline virtual bool operator==(const _Slot&) const = 0;
	virtual ~_Slot(){}
};

template<typename... T>
class Slot;
//{
//	Slot() = delete;
//	Slot(const Slot&) = delete;
//	//static_assert(std::is_constructible<Slot<T...>, T...>::value,
//	//	"Arguments for Slot constructor does not match any valid .");
//};
//{
	//static_assert(always, "Slot specialization constructed with given arguments has not beed defined.");
	/*Slot() = delete;
	Slot(const Slot&) = delete;
	Slot(T... args) = delete;*/
//};

template<typename T, typename R, typename... A>
class Slot<T,R(A...)> : public _Slot<A...>
{
	using fn_type = R(T::*)(A...);
	fn_type fn_;
	T* obj_;
public:
	Slot() : 
		fn_{nullptr}, 
		obj_{nullptr} 
	{}
	Slot(T* obj, fn_type fn) :
		fn_{fn}, 
		obj_{obj} 
	{}
	Slot(const Slot& other) :
		fn_{other.fn_}, 
		obj_{other.obj_} 
	{}
	forceinline void operator()(A... args) override
	{
		(obj_->*fn_)(args...);
	}
	forceinline bool operator==(const _Slot<A...>& other) const override
	{
		return fn_ == static_cast<const Slot<T, R(A...)>&>(other).fn_ &&
			obj_ == static_cast<const Slot<T, R(A...)>&>(other).obj_;
	}
	~Slot(){}
};

template<typename R, typename... A>
class Slot<R(A...)> : public _Slot<A...>
{
	using fn_type = R(*)(A...);
	fn_type fn_;
public:
	Slot() :
		fn_{nullptr} 
	{}
	Slot(fn_type fn) :
		fn_{fn} 
	{}
	Slot(const Slot& other) :
		fn_{other.fn_} 
	{}
	forceinline void operator()(A... args) override
	{
		(*fn_)(args...);
	}
	forceinline bool operator==(const _Slot<A...>& other) const override
	{
		return fn_ == static_cast<const Slot<R(A...)>&>(other).fn_;
	}
	~Slot(){}
};
//template<typename T, typename R, typename... A>
//class Slot<T*, R(T::*)(A...)> : public _Slot<A...>
//{
//	using fn_type = R(T::*)(A...);
//	fn_type fn_;
//	T* obj_;
//public:
//	Slot() : fn_{nullptr}, obj_{nullptr} {}
//	Slot(T* obj, fn_type fn) :fn_{fn}, obj_{obj} {}
//	Slot(const Slot& other) :fn_{other.fn_}, obj_{other.obj_} {}
//	forceinline void operator()(A... args) override
//	{
//		(obj_->*fn_)(args...);
//	}
//	forceinline bool operator==(const _Slot<A...>& other) const override
//	{
//		return fn_ == static_cast<const Slot<T*, R(T::*)(A...)>&>(other).fn_ &&
//			obj_ == static_cast<const Slot<T*, R(T::*)(A...)>&>(other).obj_;
//	}
//	~Slot() {}
//};
//
//template<typename R, typename... A>
//class Slot<R(*)(A...)> : public _Slot<A...>
//{
//	using fn_type = R(*)(A...);
//	fn_type fn_;
//public:
//	Slot() : fn_{nullptr} {}
//	Slot(fn_type fn) :fn_{fn} {}
//	Slot(const Slot& other) :fn_{other.fn_} {}
//	forceinline void operator()(A... args) override
//	{
//		(*fn_)(args...);
//	}
//	forceinline bool operator==(const _Slot<A...>& other) const override
//	{
//		return fn_ == static_cast<const Slot<R(*)(A...)>&>(other).fn_;
//	}
//	~Slot() {}
//};

template<typename... A>
class Signal
{
	x::vector_<_Slot<A...>*> slots;
public:
	Signal():
		slots(1)
	{}
	forceinline void operator()(A... args)
	{
		for (auto&& s : slots) (*s)(args...);
	}
	////////////////////////////////////////////////////////////////////////////
	/*template<typename... T>
	forceinline void attach(T... args)
	{
	static_assert(std::is_constructible<Slot<T...>, T...>::value,
	"Slot specialization with template parameters: "typeid(T).name()" constructed with given arguments has not beed defined.");
	slots.push_back(new Slot<T...>(args...));
	}
	template<typename... T>
	forceinline void detach(T... args)
	{
	Slot<T...> s(args...);
	slots.remove<vector_opt::PTR_DELETE, vector_opt::PTR_DEREF>(&s, 1);
	}*/
	////////////////////////////////////////////////////////////////////////////
	template<typename T, typename R>
	forceinline void attach(T* obj, R(T::*fn)(A...))
	{
		this->slots.push_back(new Slot<T, R(A...)>(obj, fn));
	}
	template<typename R>
	forceinline void attach(R(*fn)(A...))
	{
		this->slots.push_back(new Slot<R(A...)>(fn));
	}
	template<typename T, typename R>
	forceinline void detach(T* obj, R(T::*fn)(A...))
	{
		Slot<T,R(A...)> s(obj, fn);
		slots.remove<vector_opt::PTR_DELETE, vector_opt::PTR_DEREF>(&s, 1);
	}
	template<typename R>
	forceinline void detach(R(*fn)(A...))
	{
		Slot<R(A...)> s(obj, fn);
		slots.remove<vector_opt::PTR_DELETE, vector_opt::PTR_DEREF>(&s, 1);
	}
	////////////////////////////////////////////////////////////////////////////
	/*template<typename T1, typename T2, typename R>
	forceinline void attach(T1* obj, R(T2::*fn)(A...))
	{
		static_assert(always,
			"Types of referenced object and member owner do not match.");
	}
	template<typename T, typename R>
	forceinline void attach(T* obj, R(T::*fn)(A...))
	{
		this->slots.push_back(new Slot<T, R(A...)>(obj, fn));
	}
	template<typename T, typename R>
	forceinline void attach(R(T::*fn)(A...))
	{
		static_assert(always,
			"For non-static member function, pointer to_ an instance of object must be specified.");
	}
	template<typename R>
	forceinline void attach(R(*fn)(A...))
	{
		this->slots.push_back(new Slot<R(A...)>(fn));
	}
	template<typename T1, typename T2, typename R>
	forceinline void detach(T1* obj, R(T2::*fn)(A...))
	{
		static_assert(always,
			"Types of referenced object and member owner do not match.");
	}
	template<typename T, typename R>
	forceinline void detach(T* obj, R(T::*fn)(A...))
	{
		Slot<T, R(A...)> s(obj, fn);
		slots.remove<vector_opt::PTR_DELETE, vector_opt::PTR_DEREF>(&s, 1);
	}
	template<typename T, typename R>
	forceinline void detach(R(T::*fn)(A...))
	{
		static_assert(always,
			"For non-static member function, pointer to_ an instance of object must be specified.");
	}
	template<typename R>
	forceinline void detach(R(*fn)(A...))
	{
		Slot<R(A...)> s(obj, fn);
		slots.remove<vector_opt::PTR_DELETE, vector_opt::PTR_DEREF>(&s, 1);
	}*/
	~Signal()
	{
		slots.erase<vector_opt::PTR_DELETE>();
	}
};

#define signal(...) Signal<__VA_ARGS__>
#define emit

#undef forceinline