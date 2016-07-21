#ifndef FN_H
#define FN_H

#include "_void.h"

#define enable_if std::enable_if_t
#define decay std::decay_t

template<typename... A>
class Callable
{
public:
	virtual Callable<A...>* copy() const abstract;
	virtual void call(A...) abstract;
	virtual operator bool() const abstract;
	virtual bool empty() const abstract;
	virtual void clear() abstract;
};

template<typename...>
class Fn;

template<typename R, typename... A>
class Fn<R(A...)>: public Callable<A...>
{
	struct FnEraser_
	{
		virtual R operator()(A...) const abstract;
		virtual FnEraser_* virtual_copy() const abstract;
	};

	template<typename...>
	struct FnConcrete_;

	template<typename F>
	struct FnConcrete_<F> final: public FnEraser_
	{
		F fn;

		template<typename G>
		FnConcrete_(G&& fn): 
			fn{std::forward<G>(fn)} 
		{
		}

		R operator()(A... args) const final override
		{
			return (R)fn(args...);
		}

		FnEraser_* virtual_copy() const final override
		{
			return new FnConcrete_<F>{fn};
		}
	};

	template<class T, typename F>
	struct FnConcrete_<T,F> final: public FnEraser_
	{
		T* obj;
		F fn;

		template<typename G>
		FnConcrete_(T* obj, G&& fn): 
			obj{obj},
			fn{std::forward<G>(fn)} 
		{
		}

		R operator()(A... args) const final override
		{
			return (R)(obj->*fn)(args...);
		}

		FnEraser_* virtual_copy() const final override
		{
			return new FnConcrete_<T,F>{obj,fn};
		}
	};

	enum error_num { empty_fn };

	FnEraser_* fn_;

public:
	Fn():
		fn_{nullptr}
	{
	}

	template<typename F, typename = enable_if<!std::is_same<decay<F>, Fn<R(A...)>>::value>>
	Fn(F&& fn) :
		fn_{new FnConcrete_<decay<F>>{std::forward<F>(fn)}}
	{
	}

	template<typename T, typename F, typename = enable_if<!std::is_same<decay<F>, Fn<R(A...)>>::value>>
	Fn(T* obj, F&& fn) :
		fn_{new FnConcrete_<T,decay<F>>{obj,std::forward<F>(fn)}}
	{
	}

	Fn(Fn<R(A...)> const& other):
		fn_{other.fn_ ? other.fn_->virtual_copy() : nullptr}
	{ }

	Fn<R(A...)>& operator=(Fn<R(A...)> const& other)
	{
		if (fn_) delete fn_;
		fn_ = other.fn_->virtual_copy();
		return *this;
	}

	Callable<A...>* copy() const override
	{
		return new Fn<R(A...)>{*this};
	}

	template<typename F>
	void set(F&& fn)
	{
		if (fn_) delete fn_;
		fn_ = new FnConcrete_<decay<F>>{std::forward<F>(fn)};
	}

	__forceinline R operator()(A... args)
	{
		/*return */(R)(*fn_)(std::forward<A>(args)...);
	}

	__forceinline operator bool() const
	{
		return fn_;
	}

	__forceinline bool empty() const
	{
		return !(bool)fn_;
	}

	virtual void call(A... args) final override
	{
		if (fn_)
			return (R)(*fn_)(std::forward<A>(args)...);
		else
			throw x::error<Fn<R(A...)>>{empty_fn, "Function is empty."};
	}

	void clear()
	{
		if (fn_) {
			delete fn_;
			fn_ = nullptr;
		}
	}

	~Fn()
	{
		delete fn_;
	}
};

template<class T, typename R, typename... A>
class Fn<T,R(A...)>: public Callable<A...>
{
	using FnType_ = R(T::*)(A...);
	enum error_num { empty_fn };

	FnType_ fn_;
	mutable T* obj_;

public:
	Fn():
		fn_{nullptr},
		obj_{nullptr}
	{
	}

	Fn(T* obj, FnType_ fn) :
		fn_{fn},
		obj_{obj}
	{
	}

	Fn(Fn<T, R(A...)> const& other):
		fn_{other.fn_},
		obj_{other.obj_}
	{ }

	Fn<T,R(A...)>& operator=(Fn<T,R(A...)> const& other)
	{
		fn_ = other.fn_->virtual_copy();
		return *this;
	}

	Callable<A...>* copy() const override
	{
		return new Fn<T,R(A...)>{*this};
	}

	void set(T* obj, FnType_ fn)
	{
		fn_ = fn;
		obj_ = obj;
	}

	__forceinline R operator()(A... args)
	{
		return (R)(obj_->*fn_)(std::forward<A>(args)...);
	}

	__forceinline operator bool() const
	{
		return fn_;
	}

	__forceinline bool empty() const
	{
		return !(bool)fn_ || !(bool)obj_;
	}

	virtual void call(A... args) final override
	{
		if (fn_ && obj_)
			return (R)(obj_->*fn_)(std::forward<A>(args)...);
		else
			throw x::error<Fn<T,R(A...)>>{empty_fn, "Function is empty."};
	}

	void clear()
	{
		fn_ = nullptr;
		obj_ = nullptr;
	}

	~Fn()
	{
	}
};

template<typename R, typename... A>
class Fn<R(*)(A...)>: public Callable<A...>
{
	using FnType_ = R(*)(A...);
	enum error_num { empty_fn };

	FnType_ fn_;

public:
	Fn():
		fn_{nullptr}
	{
	}

	Fn(FnType_ fn):
		fn_{fn}
	{
	}

	Fn(Fn<R(*)(A...)> const& other):
		fn_{other.fn_}
	{ }

	Fn<R(*)(A...)>& operator=(Fn<R(*)(A...)> const& other)
	{
		fn_ = other.fn_->virtual_copy();
		return *this;
	}

	Callable<A...>* copy() const override
	{
		return new Fn<R(*)(A...)>{*this};
	}

	void set(FnType_ fn)
	{
		fn_ = fn;
	}

	__forceinline R operator()(A... args)
	{
		return (R)(*fn_)(std::forward<A>(args)...);
	}

	__forceinline operator bool() const
	{
		return fn_;
	}

	__forceinline bool empty() const
	{
		return !(bool)fn_;
	}

	virtual void call(A... args) final override
	{
		if (fn_)
			return (R)(*fn_)(std::forward<A>(args)...);
		else
			throw x::error<Fn<R(*)(A...)>>{empty_fn, "Function is empty."};
	}

	void clear()
	{
		fn_ = nullptr;
	}

	~Fn()
	{
	}
};

template<typename...>
class FnCall;

template<typename R, typename... A>
class FnCall<R(A...)>
{
public:
	static constexpr unsigned size = sizeof...(A);
private:
	using fnptr = R(*)(A...);
	class ArgEraser_ {};

	template<typename T>
	class ArgConcrete_: public ArgEraser_
	{
	public:
		T value;
		ArgConcrete_(T const& value):
			value{value}
		{
		}
	};

	fnptr fn_;
	ArgEraser_* args_[size];

	template<int... i>
	__forceinline R call_(seq<i...>&) const
	{
		return fn_(static_cast<ArgConcrete_<x::select_t<i+1, A...>>*>(args_[i])->value...);
	}

public:
	FnCall(fnptr fn, A const&... args):
		args_{(new ArgConcrete_<A>{args})...},
		fn_{fn}
	{
	}

	inline R operator()() const
	{
		return call_(gen_seq<size>{});
	}

};

template<class C, typename R, typename... A>
class FnCall<C, R(A...)>
{
public:
	static constexpr unsigned size = sizeof...(A);
private:
	using fnptr = R(C::*)(A...);
	class ArgEraser_ {};

	template<typename T>
	class ArgConcrete_: public ArgEraser_
	{
	public:
		T value;
		ArgConcrete_(T const& value):
			value{value}
		{
		}
	};

	C* obj_;
	fnptr fn_;
	ArgEraser_* args_[size];

	template<int... i>
	__forceinline R call_(seq<i...>&) const
	{
		return (obj_->*fn_)(static_cast<ArgConcrete_<x::select_t<i+1, A...>>*>(args_[i])->value...);
	}

public:
	FnCall(C* obj, fnptr fn, A const&... args):
		args_{(new ArgConcrete_<A>{args})...},
		obj_{obj}, fn_{fn}
	{
	}

	inline R operator()() const
	{
		return call_(gen_seq<size>{});
	}

};

template<typename R, typename... A>
class FnCall<Fn<R(A...)>>
{
public:
	static constexpr unsigned size = sizeof...(A);
private:
	class ArgEraser_ {};

	template<typename T>
	class ArgConcrete_: public ArgEraser_
	{
	public:
		T value;
		ArgConcrete_(T const& value):
			value{value}
		{
		}
	};

	Fn<R(A...)> fn_;
	ArgEraser_* args_[size];

	template<int... i>
	__forceinline R call_(seq<i...>&) const
	{
		return fn_(static_cast<ArgConcrete_<x::select_t<i+1, A...>>*>(args_[i])->value...);
	}

public:
	FnCall(Fn<R(A...)> const& fn, A const&... args):
		args_{(new ArgConcrete_<A>{args})...},
		fn_{fn}
	{
		//assign_(gen_seq<size>{}, args...);
	}

	inline R operator()() const
	{
		return call_(gen_seq<size>{});
	}
};


#undef enable_if
#undef decay

#endif //FN_H