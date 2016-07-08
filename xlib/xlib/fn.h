#ifndef FN_H
#define FN_H

#include "_void.h"

#define enable_if std::enable_if_t
#define decay std::decay_t

//template<typename... A>
//__forceinline _void<> expand(A&&...) {}

template<typename...>
class Fn;

template<typename R, typename... A>
class Fn<R(A...)>
{
	struct Eraser_
	{
		virtual R operator()(A...) const abstract;
		virtual Eraser_* virtual_copy() const abstract;
	};

	template<typename F>
	struct Concrete_ final: public Eraser_
	{
		F fn;

		template<typename G>
		Concrete_(G&& fn): fn{std::forward<G>(fn)} {}

		R operator()(A... args) const final override
		{
			return fn(args...);
		}

		Eraser_* virtual_copy() const final override
		{
			return new Concrete_<F>{fn};
		}
	};

	Eraser_* fn_;

public:
	Fn():
		fn_{nullptr}
	{
	}

	template<typename F, typename = enable_if<!std::is_same<decay<F>, Fn<R(A...)>>::value>>
	Fn(F&& fn) :
		fn_{new Concrete_<decay<F>>{std::forward<F>(fn)}}
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

	template<typename F>
	void set(F&& fn)
	{
		if (fn_) delete fn_;
		fn_ = new Concrete_<decay<F>>{std::forward<F>(fn)};
	}

	__forceinline R operator()(A... args) const
	{
		return (*fn_)(std::forward<A>(args)...);
	}

	__forceinline operator bool() const
	{
		return fn_;
	}

	__forceinline bool empty() const
	{
		return (bool)fn_;
	}

	~Fn()
	{
		delete fn_;
	}
};

template<typename...>
class FnCall;

template<typename R, typename... A>
class FnCall<R(A...)>
{
	using fnptr = R(*)(A...);
	static constexpr unsigned nArgs = sizeof...(A);
	class Eraser_ {};

	template<typename T>
	class Argument_: public Eraser_
	{
	public:
		T value;
		Argument_(T const& value):
			value{value}
		{
		}
	};

	fnptr fn_;
	Eraser_** args_;

	template<int... i>
	void assign_(seq<i...>&, A const&... args)
	{
		args_ = new Eraser_*[nArgs] { new Argument_<A>{args}... };
	}

	template<int... i>
	__forceinline R call_(seq<i...>&) const
	{
		return fn_(static_cast<Argument_<x::select_t<i+1, A...>>*>(args_[i])->value...);
	}

public:
	FnCall(fnptr fn, A const&... args):
		fn_{fn}
	{
		assign_(gen_seq<nArgs>{}, args...);
	}

	inline R operator()() const
	{
		return call_(gen_seq<nArgs>{});
	}

};

template<class C, typename R, typename... A>
class FnCall<C, R(A...)>
{
	using fnptr = R(C::*)(A...);
	static constexpr unsigned nArgs = sizeof...(A);
	class Eraser_ {};

	template<typename T>
	class Argument_: public Eraser_
	{
	public:
		T value;
		Argument_(T const& value):
			value{value}
		{
		}
	};

	C* obj_;
	fnptr fn_;
	Eraser_** args_;

	template<int... i>
	void assign_(seq<i...>&, A const&... args)
	{
		args_ = new Eraser_*[nArgs] { new Argument_<A>{args}... };
	}

	template<int... i>
	__forceinline R call_(seq<i...>&) const
	{
		return (obj_->*fn_)(static_cast<Argument_<x::select_t<i+1, A...>>*>(args_[i])->value...);
	}

public:
	FnCall(C* obj, fnptr fn, A const&... args):
		obj_{obj}, fn_{fn}
	{
		assign_(gen_seq<nArgs>{}, args...);
	}

	inline R operator()() const
	{
		return call_(gen_seq<nArgs>{});
	}

};

template<typename R, typename... A>
class FnCall<Fn<R(A...)>>
{
	static constexpr unsigned nArgs = sizeof...(A);
	class Eraser_ {};

	template<typename T>
	class Argument_: public Eraser_
	{
	public:
		T value;
		Argument_(T const& value):
			value{value}
		{
		}
	};

	Fn<R(A...)> fn_;
	Eraser_** args_;

	template<int... i>
	void assign_(seq<i...>&, A const&... args)
	{
		args_ = new Eraser_*[nArgs] { new Argument_<A>{args}... };
	}

	template<int... i>
	__forceinline R call_(seq<i...>&) const
	{
		return fn_(static_cast<Argument_<x::select_t<i+1, A...>>*>(args_[i])->value...);
	}

public:
	FnCall(Fn<R(A...)> const& fn, A const&... args):
		fn_{fn}
	{
		assign_(gen_seq<nArgs>{}, args...);
	}

	inline R operator()() const
	{
		return call_(gen_seq<nArgs>{});
	}
};

//template<typename...>
//class UniFnCall;
//
//template<typename R, typename... A>
//class UniFnCall<R(A...)>
//{
//	static constexpr unsigned nArgs = sizeof...(A);
//
//	class Eraser_
//	{};
//
//	template<typename T>
//	class Argument_ : public Eraser_
//	{
//	public:
//		T value;
//		Argument_(T const& value):
//			value{ value }
//		{
//		}
//	};
//
//	Fn<R(A...)> fn_;
//	Eraser_** args_;
//
//	template<int... i>
//	void assign_(seq<i...>&, A const&... args)
//	{
//		args_ = new Eraser_*[nArgs]{ new Argument_<A>{ args }... };
//	}
//
//	template<int... i>
//	__forceinline R call_(seq<i...>&) const
//	{
//		return fn_(static_cast<Argument_<x::select_t<i+1, A...>>*>(args_[i])->value...);
//	}
//
//public:
//	UniFnCall(Fn<R(A...)> const& fn, A const&... args):
//		fn_{fn}
//	{
//		assign_(gen_seq<nArgs>{}, args...);
//	}
//
//	inline R operator()() const
//	{
//		return call_(gen_seq<nArgs>{});
//	}
//
//};
//
//template<typename R, typename A>
//class UniFnCall<R(A)>
//{
//	static constexpr unsigned nArgs = 1;
//
//	Fn<R(A)> fn_;
//	A arg_;
//
//public:
//	UniFnCall(Fn<R(A)> const& fn, A const& arg):
//		fn_{ fn }, arg_{arg}
//	{
//	}
//
//	inline R operator()() const
//	{
//		return fn_(arg_);
//	}
//};
//
//template<typename R, typename A1, typename A2>
//class UniFnCall<R(A1,A2)>
//{
//	static constexpr unsigned nArgs = 1;
//
//	Fn<R(A1,A2)> fn_;
//	A1 arg1_;
//	A2 arg2_;
//
//public:
//	UniFnCall(Fn<R(A1,A2)> const& fn, A1 const& arg1, A2 const& arg2):
//		fn_{ fn }, arg1_{ arg1 }, arg2_{arg2}
//	{
//	}
//
//	inline R operator()() const
//	{
//		return fn_(arg1_,arg2_);
//	}
//};


//#define function(...) Fn<__VA_ARGS__>

#undef enable_if
#undef decay

#endif //FN_H