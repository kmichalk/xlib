#pragma once

#include "more_type_traits.h"
#include "xvector.h"
#include "_void.h"
#include "fn.h"

#define forceinline __forceinline

template<typename... A>
class Signal;

template<typename... A>
class _Slot
{
	forceinline virtual _Slot<A...>* virtual_copy_() const abstract;
	friend class Signal<A...>;
public:
	forceinline virtual void operator()(A...) abstract;
	forceinline virtual bool operator==(const _Slot&) const abstract;

	virtual ~_Slot() {}
};

template<typename...>
class Slot;

template<typename T, typename R, typename... A>
class Slot<T, R(A...)> : public _Slot<A...>
{
	using fn_type = R(T::*)(A...);
	fn_type fn_;
	T* obj_;

	forceinline _Slot<A...>* virtual_copy_() const override
	{
		return new Slot<T, R(A...)>(obj_, fn_);
	}

	friend class Signal<A...>;
public:
	explicit Slot() :
		fn_{nullptr},
		obj_{nullptr}
	{}
	explicit Slot(T* obj, fn_type fn) :
		fn_{fn},
		obj_{obj}
	{}
	explicit Slot(const Slot& other) :
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

	~Slot() {}
};

template<typename R, typename... A>
class Slot<R(A...)> : public _Slot<A...>
{
	using fn_type = R(*)(A...);
	fn_type fn_;

	forceinline _Slot<A...>* virtual_copy_() const override
	{
		return new Slot<R(A...)>(fn_);
	}

	friend class Signal<A...>;
public:
	explicit Slot() :
		fn_{nullptr}
	{}
	explicit Slot(fn_type fn) :
		fn_{fn}
	{}
	explicit Slot(const Slot& other) :
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

	~Slot() {}
};

class _Signal {};

template<typename... A>
class Signal: public Callable<A...>
{
	x::vector_<Callable<A...>*> slots_;

public:
	Signal() :
		slots_(1)
	{
	}

	Signal(const Signal<A...>& other)
	{
		for (auto s = other.slots_.cbegin(),
			cend = other.slots_.cend();
			s!=cend; ++s)
			slots_.push_back((*s)->copy());
	}

	Signal(std::initializer_list<Callable<A...>*> slots):
		slots_{slots}
	{
	}
	/*template<typename... T>
	Signal(T*... slots):
		slots_{slots...}
	{
		static_assert(
			all_true<std::is_base_of<_Slot<A...>, T>::value...>::value,
			"At least one argument is not a function or member function pointer");
	}*/

	Signal<A...>& operator=(const Signal<A...>& other)
	{
		slots_.clear<x::vector_opt::PTR_DELETE>();
		for (auto s = other.slots_.cbegin(),
			cend = other.slots_.cend();
			s!=cend; ++s)
			slots_.push_back((*s)->copy());
		return *this;
	}

	Callable<A...>* copy() const override
	{
		return new Signal<A...>{*this};
	}

	_void<_Signal> operator()(A... args)
	{
		for (auto&& s : slots_) s->call(args...);
		return _void<_Signal>{};
	}

	virtual void call(A... args) final override
	{
		for (auto&& s : slots_) s->call(args...);
	}

	template<typename T, typename R>
	inline void attach(T* obj, R(T::*fn)(A...))
	{
		slots_.push_back(new Fn<T, R(A...)>(obj, fn));
	}
	template<typename R>
	inline void attach(R(*fn)(A...))
	{
		slots_.push_back(new Fn<R(A...)>(fn));
	}
	template<typename R>
	inline void attach(Fn<R(A...)>* slot)
	{
		slots_.push_back(slot);
	}
	template<typename T, typename R>
	inline void attach(Fn<T, R(A...)>* slot)
	{
		slots_.push_back(slot);
	}
	inline void attach(Signal<A...> const& other)
	{
		slots_.append(other.slots_);
	}

	template<typename T, typename R>
	void detach(T* obj, R(T::*fn)(A...))
	{
		Fn<T, R(A...)> s(obj, fn);
		slots_.remove<x::vector_opt::PTR_DELETE, x::vector_opt::PTR_DEREF>(&s, 1);
	}
	template<typename R>
	void detach(R(*fn)(A...))
	{
		Fn<R(A...)> s(obj, fn);
		slots_.remove<x::vector_opt::PTR_DELETE, x::vector_opt::PTR_DEREF>(&s, 1);
	}
	void clear()
	{
		slots_.erase<x::vector_opt::PTR_DELETE>();
	}

	operator bool() const
	{
		return !slots_.empty();
	}

	bool empty() const
	{
		return slots_.empty();
	}

	~Signal()
	{
		slots_.erase<x::vector_opt::PTR_DELETE>();
	}
};

#define signal(...) Signal<__VA_ARGS__>
#define emit _void<_Signal> __emitResult__ =

#undef forceinline

//#pragma once
//
//#include "more_type_traits.h"
//#include "xvector.h"
//#include "_void.h"
//
//#define forceinline __forceinline
//
//template<typename... A>
//class Signal;
//
//template<typename... A>
//class _Slot
//{
//	forceinline virtual _Slot<A...>* virtual_copy_() const abstract;
//	friend class Signal<A...>;
//public:
//	forceinline virtual void operator()(A...) abstract;
//	forceinline virtual bool operator==(const _Slot&) const abstract;
//
//	virtual ~_Slot() {}
//};
//
//template<typename...>
//class Slot;
//
//template<typename T, typename R, typename... A>
//class Slot<T, R(A...)> : public _Slot<A...>
//{
//	using fn_type = R(T::*)(A...);
//	fn_type fn_;
//	T* obj_;
//
//	forceinline _Slot<A...>* virtual_copy_() const override
//	{
//		return new Slot<T, R(A...)>(obj_, fn_);
//	}
//
//	friend class Signal<A...>;
//public:
//	/*explicit Slot() :
//		fn_{nullptr},
//		obj_{nullptr}
//	{}*/
//	explicit Slot(T* obj, fn_type fn) :
//		fn_{fn},
//		obj_{obj}
//	{}
//	explicit Slot(const Slot& other) :
//		fn_{other.fn_},
//		obj_{other.obj_}
//	{}
//	forceinline void operator()(A... args) override
//	{
//		(obj_->*fn_)(args...);
//	}
//	forceinline bool operator==(const _Slot<A...>& other) const override
//	{
//		return fn_ == static_cast<const Slot<T, R(A...)>&>(other).fn_ &&
//			obj_ == static_cast<const Slot<T, R(A...)>&>(other).obj_;
//	}
//
//	~Slot() {}
//};
//
//template<typename F, typename... A>
//class Slot<F, A...> : public _Slot<A...>
//{
//	F* fn_;
//
//	forceinline _Slot<A...>* virtual_copy_() const override
//	{
//		return new Slot<F, A...>(*fn_);
//	}
//
//	friend class Signal<A...>;
//public:
//	explicit Slot(F& fn) :
//		fn_{&fn}
//	{}
//	explicit Slot(const Slot& other) :
//		fn_{other.fn_}
//	{}
//	forceinline void operator()(A... args) override
//	{
//		(*fn_)(args...);
//	}
//	forceinline bool operator==(const _Slot<A...>& other) const override
//	{
//		return fn_ == static_cast<const Slot<F, A...>&>(other).fn_;
//	}
//
//	~Slot() {}
//};

//template<typename R, typename... A>
//class Slot<R(A...)> : public _Slot<A...>
//{
//	using fn_type = R(*)(A...);
//	fn_type fn_;
//
//	forceinline _Slot<A...>* virtual_copy_() const override
//	{
//		return new Slot<R(A...)>(fn_);
//	}
//
//	friend class Signal<A...>;
//public:
//	/*explicit Slot() :
//		fn_{nullptr}
//	{}*/
//	explicit Slot(fn_type fn) :
//		fn_{fn}
//	{}
//	explicit Slot(const Slot& other) :
//		fn_{other.fn_}
//	{}
//	forceinline void operator()(A... args) override
//	{
//		(*fn_)(args...);
//	}
//	forceinline bool operator==(const _Slot<A...>& other) const override
//	{
//		return fn_ == static_cast<const Slot<R(A...)>&>(other).fn_;
//	}
//
//	~Slot() {}
//};

//class _Signal {};
//
//template<typename... A>
//class Signal: public _Signal
//{
//	x::vector_<_Slot<A...>*> slots_;
//
//public:
//	Signal() :
//		slots_(1)
//	{}
//	Signal(const Signal<A...>& other)
//	{
//		for (auto s = other.slots_.cbegin(),
//			cend = other.slots_.cend();
//			s!=cend; ++s)
//			slots_.push_back((*s)->virtual_copy_());
//	}
//	template<typename... T>
//	Signal(T*... slots):
//		slots_{slots...}
//	{
//		static_assert(
//			all_true<std::is_base_of<_Slot<A...>, T>::value...>::value,
//			"At least one argument is not a function or member function pointer");
//	}
//
//	Signal<A...>& operator=(const Signal<A...>& other)
//	{
//		slots_.clear<x::vector_opt::PTR_DELETE>();
//		for (auto s = other.slots_.cbegin(),
//			cend = other.slots_.cend();
//			s!=cend; ++s)
//			slots_.push_back((*s)->virtual_copy_());
//		return *this;
//	}
//
//	forceinline _void<_Signal> operator()(A... args)
//	{
//		for (auto&& s : slots_) (*s)(args...);
//	}
//
//	template<typename T, typename R>
//	forceinline void attach(T* obj, R(T::*fn)(A...))
//	{
//		slots_.push_back(new Slot<T, R(A...)>(obj, fn));
//	}
//	template<typename F>
//	forceinline void attach(F& fn)
//	{
//		slots_.push_back(new Slot<F, A...>(fn));
//	}
//	template<typename R>
//	forceinline void attach(Slot<R(A...)>* slot)
//	{
//		slots_.push_back(slot);
//	}
//	template<typename T, typename R>
//	forceinline void attach(Slot<T, R(A...)>* slot)
//	{
//		slots_.push_back(slot);
//	}
//	forceinline void attach(Signal<A...> const& other)
//	{
//		slots_.append(other.slots_);
//	}
//
//	template<typename T, typename R>
//	inline void detach(T* obj, R(T::*fn)(A...))
//	{
//		Slot<T, R(A...)> s(obj, fn);
//		slots_.remove<x::vector_opt::PTR_DELETE, x::vector_opt::PTR_DEREF>(&s, 1);
//	}
//	template<typename R>
//	inline void detach(R(*fn)(A...))
//	{
//		Slot<R(A...)> s(obj, fn);
//		slots_.remove<x::vector_opt::PTR_DELETE, x::vector_opt::PTR_DEREF>(&s, 1);
//	}
//	void detach()
//	{
//		slots_.erase<x::vector_opt::PTR_DELETE>();
//	}
//
//	~Signal()
//	{
//		slots_.erase<x::vector_opt::PTR_DELETE>();
//	}
//};
//
//#define signal(...) Signal<__VA_ARGS__>
//#define emit _void<_Signal> __emitResult__ =
//
//#undef forceinline