#ifndef FN_H
#define FN_H

#include "_void.h"
#include "more_type_traits.h"
#include "error.h"
#include "va_handler.h"

#define enable_if std::enable_if_t
#define decay std::decay_t

namespace x
{

	template<typename...>
	class Callable;

	template<typename Ret, typename... Args>
	class Callable<Ret(Args...)>
	{
	public:
		enum ErrorNum { empty_fn };

		virtual Callable<Ret(Args...)>* copy() const abstract;
		virtual Ret call(Args...) const abstract;
		virtual operator bool() const abstract;
		virtual bool empty() const abstract;
		//virtual void clear() abstract;
	};

	template<typename...>
	class Fn;

	template<typename Ret, typename... Args>
	class Fn<Ret(Args...)>: public Callable<Ret(Args...)>
	{
		struct FnEraser_
		{
			virtual Ret operator()(Args...) const abstract;
			virtual FnEraser_* copy() const abstract;
		};

		template<typename...>
		struct FnConcrete_;

		template<typename F>
		struct FnConcrete_<F> final: public FnEraser_
		{
			mutable F fn;

			template<typename G>
			FnConcrete_(G&& fn):
				fn{std::forward<G>(fn)}
			{
			}

			Ret operator()(Args... args) const final override
			{
				return (Ret)fn(args...);
			}

			FnEraser_* copy() const final override
			{
				return new FnConcrete_<F>{fn};
			}
		};

		template<class T, typename F>
		struct FnConcrete_<T, F> final: public FnEraser_
		{
			mutable T* obj;
			mutable F fn;

			template<typename G>
			FnConcrete_(T* obj, G&& fn):
				obj{obj},
				fn{std::forward<G>(fn)}
			{
			}

			Ret operator()(Args... args) const final override
			{
				return (Ret)(obj->*fn)(args...);
			}

			FnEraser_* copy() const final override
			{
				return new FnConcrete_<T, F>{obj,fn};
			}
		};

		FnEraser_* fn_;

	public:
		Fn():
			fn_{nullptr}
		{
		}

		template<typename F, typename = enable_if<!std::is_same<decay<F>, Fn<Ret(Args...)>>::value>>
		Fn(F&& fn) :
			fn_{new FnConcrete_<decay<F>>{std::forward<F>(fn)}}
		{
		}

		template<typename T, typename F, typename = enable_if<!std::is_same<decay<F>, Fn<Ret(Args...)>>::value>>
		Fn(T* obj, F&& fn) :
			fn_{new FnConcrete_<T,decay<F>>{obj,std::forward<F>(fn)}}
		{
		}

		Fn(Fn<Ret(Args...)> const& other):
			fn_{other.fn_ ? other.fn_->copy() : nullptr}
		{ }

		Fn<Ret(Args...)>& operator=(Fn<Ret(Args...)> const& other)
		{
			if (fn_) delete fn_;
			fn_ = other.fn_->copy();
			return *this;
		}

		Callable<Ret(Args...)>* copy() const override
		{
			return new Fn<Ret(Args...)>{*this};
		}

		template<typename F>
		void set(F&& fn)
		{
			if (fn_) delete fn_;
			fn_ = new FnConcrete_<decay<F>>{std::forward<F>(fn)};
		}

		__forceinline Ret operator()(Args... args) const
		{
			return (Ret)(*fn_)(std::forward<Args>(args)...);
		}

		__forceinline operator bool() const
		{
			return fn_;
		}

		__forceinline bool empty() const
		{
			return !(bool)fn_;
		}

		virtual Ret call(Args... args) const final override
		{
			if (fn_)
				return (Ret)(*fn_)(std::forward<Args>(args)...);
			else
				throw error<Fn<Ret(Args...)>>{empty_fn, "Function is empty."};
		}

		/*void clear()
		{
			if (fn_) {
				delete fn_;
				fn_ = nullptr;
			}
		}*/

		~Fn()
		{
			if (fn_)
				delete fn_;
		}
	};

	template<class Owner, typename Ret, typename... Args>
	class Fn<Owner, Ret(Args...)>: public Callable<Ret(Args...)>
	{
		using FnType_ = Ret(Owner::*)(Args...);

		FnType_ fn_;
		mutable Owner* obj_;

	public:
		Fn():
			fn_{nullptr},
			obj_{nullptr}
		{
		}

		Fn(Owner* obj, FnType_ fn):
			fn_{fn},
			obj_{obj}
		{
		}

		Fn(Fn<Owner, Ret(Args...)> const& other):
			fn_{other.fn_},
			obj_{other.obj_}
		{ }

		Fn<Owner, Ret(Args...)>& operator=(Fn<Owner, Ret(Args...)> const& other)
		{
			fn_ = other.fn_->copy();
			return *this;
		}

		Callable<Ret(Args...)>* copy() const override
		{
			return new Fn<Owner, Ret(Args...)>{*this};
		}

		void set(Owner* obj, FnType_ fn)
		{
			fn_ = fn;
			obj_ = obj;
		}

		__forceinline Ret operator()(Args... args) const
		{
			return (Ret)(obj_->*fn_)(std::forward<Args>(args)...);
		}

		__forceinline operator bool() const
		{
			return fn_;
		}

		__forceinline bool empty() const
		{
			return !(bool)fn_ || !(bool)obj_;
		}

		virtual Ret call(Args... args) const final override
		{
			if (fn_ && obj_)
				return (Ret)(obj_->*fn_)(std::forward<Args>(args)...);
			else
				throw error<Fn<Owner, Ret(Args...)>>{empty_fn, "Function is empty."};
		}

		/*void clear()
		{
			fn_ = nullptr;
			obj_ = nullptr;
		}*/

		~Fn()
		{
		}
	};

	template<typename Ret, typename... Args>
	class Fn<Ret(*)(Args...)>: public Callable<Ret(Args...)>
	{
	public:
		using FnType_ = Ret(*)(Args...);

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

		Fn(Fn<Ret(*)(Args...)> const& other):
			fn_{other.fn_}
		{ }

		Fn<Ret(*)(Args...)>& operator=(Fn<Ret(*)(Args...)> const& other)
		{
			fn_ = other.fn_->copy();
			return *this;
		}

		Callable<Ret(Args...)>* copy() const override
		{
			return new Fn<Ret(*)(Args...)>{*this};
		}

		void set(FnType_ fn)
		{
			fn_ = fn;
		}

		__forceinline Ret operator()(Args... args) const
		{
			return (Ret)(*fn_)(std::forward<Args>(args)...);
		}

		__forceinline operator bool() const
		{
			return fn_;
		}

		__forceinline bool empty() const
		{
			return !(bool)fn_;
		}

		virtual Ret call(Args... args) const final override
		{
			if (fn_)
				return (Ret)(*fn_)(std::forward<Args>(args)...);
			else
				throw error<Fn<Ret(*)(Args...)>>{empty_fn, "Function is empty."};
		}

		/*void clear()
		{
			fn_ = nullptr;
		}*/

		~Fn()
		{
		}
	};

#pragma region FN_CALL

	template<typename...>
	class FnCall;

	template<typename Ret, typename... Args>
	class FnCall<Ret(Args...)>: public Callable<Ret()>
	{
	public:
		static constexpr unsigned size = sizeof...(Args);
	private:
		using FnPtr_ = Ret(*)(Args...);

		FnPtr_ fn_;
		mutable x::va::pack<Args...> args_;

		template<int... I>
		__forceinline Ret expandArgs_(seq<I...>&) const
		{
			return (Ret)fn_(args_.get<I>()...);
		}

	public:
		FnCall() = delete;

		FnCall(FnPtr_ fn, Args&&... args):
			args_{std::forward<Args>(args)...},
			fn_{fn}
		{
		}

		FnCall(FnCall<Ret(Args...)> const& other):
			fn_{other.fn_},
			args_{other.args_}
		{
		}

		virtual Callable<Ret()>* copy() const override
		{
			return new FnCall<Ret(Args...)>{*this};
		}

		virtual Ret call() const override
		{
			if (fn_)
				return (Ret)expandArgs_(gen_seq<size>{});
			else
				throw error<FnCall<Ret(Args...)>>{empty_fn, "Function is empty."};

		}

		virtual operator bool() const override
		{
			return fn_;
		}

		virtual bool empty() const override
		{
			return !(bool)fn_;
		}

		//virtual void clear() override
		//{
		//	//for (int i = 0; i<size; ++i) delete args_[i];
		//}

		__forceinline Ret operator()() const
		{
			return (Ret)expandArgs_(gen_seq<size>{});
		}

	};

	//template<typename Ret, typename... Args>
	//class FnCall<Ret(Args...)>: public Callable<Ret()>
	//{
	//public:
	//	static constexpr unsigned size = sizeof...(Args);
	//private:
	//	using FnPtr_ = Ret(*)(Args...);
	//	class ArgEraser_ 
	//	{
	//	public:
	//		virtual ArgEraser_* copy() const abstract;
	//	};
	//
	//	template<typename T>
	//	class ArgConcrete_: public ArgEraser_
	//	{
	//	public:
	//		T value;
	//		ArgConcrete_(T const& value):
	//			value{value}
	//		{
	//		}
	//
	//		virtual ArgEraser_* copy() const override
	//		{
	//			return new ArgConcrete_{*this};
	//		}
	//	};
	//
	//	FnPtr_ fn_;
	//	ArgEraser_* args_[size];
	//
	//	template<int... i>
	//	__forceinline Ret expandArgs_(x::seq<i...>&) const
	//	{
	//		return (Ret)fn_(static_cast<ArgConcrete_<x::select_t<i+1, Args...>>*>(args_[i])->value...);
	//	}
	//
	//public:
	//	FnCall(FnPtr_ fn, Args const&... args):
	//		args_{(new ArgConcrete_<Args>{args})...},
	//		fn_{fn}
	//	{
	//	}
	//
	//	virtual Callable<Ret()>* copy() const override
	//	{
	//		return new FnCall<Ret(Args...)>{*this};
	//	}
	//
	//	virtual Ret call() const override
	//	{
	//		if (fn_)
	//			return (Ret)expandArgs_(gen_seq<size>{});
	//		else
	//			throw x::error<FnCall<Ret(Args...)>>{empty_fn, "Function is empty."};
	//
	//	}
	//
	//	virtual operator bool() const override
	//	{
	//		return fn_;
	//	}
	//
	//	virtual bool empty() const override
	//	{
	//		return !(bool)fn_;
	//	}
	//
	//	virtual void clear() override
	//	{
	//		for (int i = 0; i<size; ++i) delete args_[i];
	//	}
	//
	//	__forceinline Ret operator()() const
	//	{
	//		return (Ret)expandArgs_(gen_seq<size>{});
	//	}
	//
	//};

	template<class C, typename Ret, typename... Args>
	class FnCall<C, Ret(Args...)>: public Callable<Ret()>
	{
	public:
		static constexpr unsigned size = sizeof...(Args);
	private:
		using FnPtr_ = Ret(C::*)(Args...);
		/*class ArgEraser_ {};

		template<typename T>
		class ArgConcrete_: public ArgEraser_
		{
		public:
			T value;
			ArgConcrete_(T const& value):
				value{value}
			{
			}
		};*/


		mutable C* obj_;
		FnPtr_ fn_;
		mutable x::va::pack<Args...> args_;
		//ArgEraser_* args_[size];

		template<int... I>
		__forceinline Ret expandArgs_(seq<I...>&) const
		{
			return (Ret)(obj_->*fn_)(args_.get<I>()...);
			//return (Ret)(obj_->*fn_)(static_cast<ArgConcrete_<x::select_t<I+1, Args...>>*>(args_[I])->value...);
		}

	public:
		FnCall() = delete;

		FnCall(C* obj, FnPtr_ fn, Args&&... args):
			args_{std::forward<Args>(args)...},
			obj_{obj},
			fn_{fn}
		{
		}

		FnCall(FnCall<C, Ret(Args...)> const& other):
			obj_{other.obj_},
			fn_{other.fn_},
			args_{other.args_}
		{
		}

		virtual Callable<Ret()>* copy() const override
		{
			return new FnCall<C, Ret(Args...)>{*this};
		}

		virtual Ret call() const override
		{
			if (fn_)
				return (Ret)expandArgs_(gen_seq<size>{});
			else
				throw error<FnCall<C, Ret(Args...)>>{empty_fn, "Function is empty."};

		}

		virtual operator bool() const override
		{
			return fn_;
		}

		virtual bool empty() const override
		{
			return !(bool)fn_;
		}

		/*virtual void clear() override
		{
			for (int i = 0; i<size; ++i) delete args_[i];
		}*/

		__forceinline Ret operator()() const
		{
			return (Ret)expandArgs_(gen_seq<size>{});
		}

	};

	template<typename Ret, typename... Args>
	class FnCall<Fn<Ret(Args...)>>: public Callable<Ret()>
	{
	public:
		static constexpr unsigned size = sizeof...(Args);
	private:
		/*class ArgEraser_ {};

		template<typename T>
		class ArgConcrete_: public ArgEraser_
		{
		public:
			T value;
			ArgConcrete_(T const& value):
				value{value}
			{
			}
		};*/

		Fn<Ret(Args...)> fn_;
		mutable x::va::pack<Args...> args_;
		//ArgEraser_* args_[size];

		template<int... I>
		__forceinline Ret expandArgs_(seq<I...>&) const
		{
			return (Ret)fn_(args_.get<I>()...);
			//return (Ret)fn_(static_cast<ArgConcrete_<x::select_t<I+1, Args...>>*>(args_[I])->value...);
		}

	public:
		FnCall() = delete;

		FnCall(Fn<Ret(Args...)> const& fn, Args&&... args):
			args_{std::forward<Args>(args)...},
			fn_{fn}
		{
		}

		FnCall(FnCall<Fn<Ret(Args...)>> const& other):
			fn_{other.fn_},
			args_{other.args_}
		{
		}

		virtual Callable<Ret()>* copy() const override
		{
			return new FnCall<Fn<Ret(Args...)>>{*this};
		}

		virtual Ret call() const override
		{
			if (fn_)
				return (Ret)expandArgs_(gen_seq<size>{});
			else
				throw error<FnCall<Fn<Ret(Args...)>>>{empty_fn, "Function is empty."};

		}

		virtual operator bool() const override
		{
			return fn_;
		}

		virtual bool empty() const override
		{
			return !(bool)fn_;
		}

		/*virtual void clear() override
		{
			for (int i = 0; i<size; ++i) delete args_[i];
		}*/

		__forceinline Ret operator()() const
		{
			return (Ret)expandArgs_(gen_seq<size>{});
		}
	};
}
#pragma endregion

#undef enable_if
#undef decay

#endif //FN_H


//#ifndef FN_H
//#define FN_H
//
//#include "_void.h"
//#include "more_type_traits.h"
//#include "error.h"
//
//#define enable_if std::enable_if_t
//#define decay std::decay_t
//
//template<typename... Args>
//class Callable
//{
//public:
//	enum ErrorNum { empty_fn };
//
//	virtual Callable<Args...>* copy() const abstract;
//	virtual void call(Args...) abstract;
//	virtual operator bool() const abstract;
//	virtual bool empty() const abstract;
//	virtual void clear() abstract;
//};
//
//template<typename...>
//class Fn;
//
//template<typename Ret, typename... Args>
//class Fn<Ret(Args...)>: public Callable<Args...>
//{
//	struct FnEraser_
//	{
//		virtual Ret operator()(Args...) const abstract;
//		virtual FnEraser_* virtual_copy() const abstract;
//	};
//
//	template<typename...>
//	struct FnConcrete_;
//
//	template<typename F>
//	struct FnConcrete_<F> final: public FnEraser_
//	{
//		F fn;
//
//		template<typename G>
//		FnConcrete_(G&& fn): 
//			fn{std::forward<G>(fn)} 
//		{
//		}
//
//		Ret operator()(Args... args) const final override
//		{
//			return (Ret)fn(args...);
//		}
//
//		FnEraser_* virtual_copy() const final override
//		{
//			return new FnConcrete_<F>{fn};
//		}
//	};
//
//	template<class T, typename F>
//	struct FnConcrete_<T,F> final: public FnEraser_
//	{
//		T* obj;
//		F fn;
//
//		template<typename G>
//		FnConcrete_(T* obj, G&& fn): 
//			obj{obj},
//			fn{std::forward<G>(fn)} 
//		{
//		}
//
//		Ret operator()(Args... args) const final override
//		{
//			return (Ret)(obj->*fn)(args...);
//		}
//
//		FnEraser_* virtual_copy() const final override
//		{
//			return new FnConcrete_<T,F>{obj,fn};
//		}
//	};
//
//	FnEraser_* fn_;
//
//public:
//	Fn():
//		fn_{nullptr}
//	{
//	}
//
//	template<typename F, typename = enable_if<!std::is_same<decay<F>, Fn<Ret(Args...)>>::value>>
//	Fn(F&& fn) :
//		fn_{new FnConcrete_<decay<F>>{std::forward<F>(fn)}}
//	{
//	}
//
//	template<typename T, typename F, typename = enable_if<!std::is_same<decay<F>, Fn<Ret(Args...)>>::value>>
//	Fn(T* obj, F&& fn) :
//		fn_{new FnConcrete_<T,decay<F>>{obj,std::forward<F>(fn)}}
//	{
//	}
//
//	Fn(Fn<Ret(Args...)> const& other):
//		fn_{other.fn_ ? other.fn_->virtual_copy() : nullptr}
//	{ }
//
//	Fn<Ret(Args...)>& operator=(Fn<Ret(Args...)> const& other)
//	{
//		if (fn_) delete fn_;
//		fn_ = other.fn_->virtual_copy();
//		return *this;
//	}
//
//	Callable<Args...>* copy() const override
//	{
//		return new Fn<Ret(Args...)>{*this};
//	}
//
//	template<typename F>
//	void set(F&& fn)
//	{
//		if (fn_) delete fn_;
//		fn_ = new FnConcrete_<decay<F>>{std::forward<F>(fn)};
//	}
//
//	__forceinline Ret operator()(Args... args)
//	{
//		/*return */(Ret)(*fn_)(std::forward<Args>(args)...);
//	}
//
//	__forceinline operator bool() const
//	{
//		return fn_;
//	}
//
//	__forceinline bool empty() const
//	{
//		return !(bool)fn_;
//	}
//
//	virtual void call(Args... args) final override
//	{
//		if (fn_)
//			return (Ret)(*fn_)(std::forward<Args>(args)...);
//		else
//			throw x::error<Fn<Ret(Args...)>>{empty_fn, "Function is empty."};
//	}
//
//	void clear()
//	{
//		if (fn_) {
//			delete fn_;
//			fn_ = nullptr;
//		}
//	}
//
//	~Fn()
//	{
//		delete fn_;
//	}
//};
//
//template<class T, typename Ret, typename... Args>
//class Fn<T,Ret(Args...)>: public Callable<Args...>
//{
//	using FnType_ = Ret(T::*)(Args...);
//
//	FnType_ fn_;
//	mutable T* obj_;
//
//public:
//	Fn():
//		fn_{nullptr},
//		obj_{nullptr}
//	{
//	}
//
//	Fn(T* obj, FnType_ fn) :
//		fn_{fn},
//		obj_{obj}
//	{
//	}
//
//	Fn(Fn<T, Ret(Args...)> const& other):
//		fn_{other.fn_},
//		obj_{other.obj_}
//	{ }
//
//	Fn<T,Ret(Args...)>& operator=(Fn<T,Ret(Args...)> const& other)
//	{
//		fn_ = other.fn_->virtual_copy();
//		return *this;
//	}
//
//	Callable<Args...>* copy() const override
//	{
//		return new Fn<T,Ret(Args...)>{*this};
//	}
//
//	void set(T* obj, FnType_ fn)
//	{
//		fn_ = fn;
//		obj_ = obj;
//	}
//
//	__forceinline Ret operator()(Args... args)
//	{
//		return (Ret)(obj_->*fn_)(std::forward<Args>(args)...);
//	}
//
//	__forceinline operator bool() const
//	{
//		return fn_;
//	}
//
//	__forceinline bool empty() const
//	{
//		return !(bool)fn_ || !(bool)obj_;
//	}
//
//	virtual void call(Args... args) final override
//	{
//		if (fn_ && obj_)
//			return (Ret)(obj_->*fn_)(std::forward<Args>(args)...);
//		else
//			throw x::error<Fn<T,Ret(Args...)>>{empty_fn, "Function is empty."};
//	}
//
//	void clear()
//	{
//		fn_ = nullptr;
//		obj_ = nullptr;
//	}
//
//	~Fn()
//	{
//	}
//};
//
//template<typename Ret, typename... Args>
//class Fn<Ret(*)(Args...)>: public Callable<Args...>
//{
//	using FnType_ = Ret(*)(Args...);
//
//	FnType_ fn_;
//
//public:
//	Fn():
//		fn_{nullptr}
//	{
//	}
//
//	Fn(FnType_ fn):
//		fn_{fn}
//	{
//	}
//
//	Fn(Fn<Ret(*)(Args...)> const& other):
//		fn_{other.fn_}
//	{ }
//
//	Fn<Ret(*)(Args...)>& operator=(Fn<Ret(*)(Args...)> const& other)
//	{
//		fn_ = other.fn_->virtual_copy();
//		return *this;
//	}
//
//	Callable<Args...>* copy() const override
//	{
//		return new Fn<Ret(*)(Args...)>{*this};
//	}
//
//	void set(FnType_ fn)
//	{
//		fn_ = fn;
//	}
//
//	__forceinline Ret operator()(Args... args)
//	{
//		return (Ret)(*fn_)(std::forward<Args>(args)...);
//	}
//
//	__forceinline operator bool() const
//	{
//		return fn_;
//	}
//
//	__forceinline bool empty() const
//	{
//		return !(bool)fn_;
//	}
//
//	virtual void call(Args... args) final override
//	{
//		if (fn_)
//			return (Ret)(*fn_)(std::forward<Args>(args)...);
//		else
//			throw x::error<Fn<Ret(*)(Args...)>>{empty_fn, "Function is empty."};
//	}
//
//	void clear()
//	{
//		fn_ = nullptr;
//	}
//
//	~Fn()
//	{
//	}
//};
//
//
//
//template<typename...>
//class FnCall;
//
//template<typename Ret, typename... Args>
//class FnCall<Ret(Args...)>: public Callable<>
//{
//public:
//	static constexpr unsigned size = sizeof...(Args);
//private:
//	using FnPtr_ = Ret(*)(Args...);
//	class ArgEraser_ {};
//
//	template<typename T>
//	class ArgConcrete_: public ArgEraser_
//	{
//	public:
//		T value;
//		ArgConcrete_(T const& value):
//			value{value}
//		{
//		}
//	};
//
//	FnPtr_ fn_;
//	ArgEraser_* args_[size];
//
//	template<int... i>
//	__forceinline Ret expandArgs_(seq<i...>&) const
//	{
//		return fn_(static_cast<ArgConcrete_<x::select_t<i+1, Args...>>*>(args_[i])->value...);
//	}
//
//public:
//	FnCall(FnPtr_ fn, Args const&... args):
//		args_{(new ArgConcrete_<Args>{args})...},
//		fn_{fn}
//	{
//	}
//
//	virtual Callable<>* copy() const override
//	{
//		return new FnCall<Ret(Args...)>{*this};
//	}
//
//	virtual void call() override
//	{
//		if (fn_)
//			expandArgs_(gen_seq<size>{});
//		else
//			throw x::error<FnCall<Ret(Args...)>>{empty_fn, "Function is empty."};
//
//	}
//
//	virtual operator bool() const override
//	{
//		return fn_;
//	}
//
//	virtual bool empty() const override
//	{
//		return !(bool)fn_;
//	}
//
//	virtual void clear() override
//	{
//		for (int i = 0; i<size; ++i) delete args_[i];
//	}
//
//	__forceinline Ret operator()() const
//	{
//		return expandArgs_(gen_seq<size>{});
//	}
//
//};
//
//template<class C, typename Ret, typename... Args>
//class FnCall<C, Ret(Args...)>: public Callable<>
//{
//public:
//	static constexpr unsigned size = sizeof...(Args);
//private:
//	using FnPtr_ = Ret(C::*)(Args...);
//	class ArgEraser_ {};
//
//	template<typename T>
//	class ArgConcrete_: public ArgEraser_
//	{
//	public:
//		T value;
//		ArgConcrete_(T const& value):
//			value{value}
//		{
//		}
//	};
//
//	C* obj_;
//	FnPtr_ fn_;
//	ArgEraser_* args_[size];
//
//	template<int... i>
//	__forceinline Ret expandArgs_(seq<i...>&) const
//	{
//		return (obj_->*fn_)(static_cast<ArgConcrete_<x::select_t<i+1, Args...>>*>(args_[i])->value...);
//	}
//
//public:
//	FnCall(C* obj, FnPtr_ fn, Args const&... args):
//		args_{(new ArgConcrete_<Args>{args})...},
//		obj_{obj}, fn_{fn}
//	{
//	}
//
//	virtual Callable<>* copy() const override
//	{
//		return new FnCall<C,Ret(Args...)>{*this};
//	}
//
//	virtual void call() override
//	{
//		if (fn_)
//			expandArgs_(gen_seq<size>{});
//		else
//			throw x::error<FnCall<C,Ret(Args...)>>{empty_fn, "Function is empty."};
//
//	}
//
//	virtual operator bool() const override
//	{
//		return fn_;
//	}
//
//	virtual bool empty() const override
//	{
//		return !(bool)fn_;
//	}
//
//	virtual void clear() override
//	{
//		for (int i = 0; i<size; ++i) delete args_[i];
//	}
//
//	__forceinline Ret operator()() const
//	{
//		return expandArgs_(gen_seq<size>{});
//	}
//
//};
//
//template<typename Ret, typename... Args>
//class FnCall<Fn<Ret(Args...)>>: public Callable<>
//{
//public:
//	static constexpr unsigned size = sizeof...(Args);
//private:
//	class ArgEraser_ {};
//
//	template<typename T>
//	class ArgConcrete_: public ArgEraser_
//	{
//	public:
//		T value;
//		ArgConcrete_(T const& value):
//			value{value}
//		{
//		}
//	};
//
//	Fn<Ret(Args...)> fn_;
//	ArgEraser_* args_[size];
//
//	template<int... i>
//	__forceinline Ret expandArgs_(seq<i...>&) const
//	{
//		return fn_(static_cast<ArgConcrete_<x::select_t<i+1, Args...>>*>(args_[i])->value...);
//	}
//
//public:
//	FnCall(Fn<Ret(Args...)> const& fn, Args const&... args):
//		args_{(new ArgConcrete_<Args>{args})...},
//		fn_{fn}
//	{
//	}
//
//	virtual Callable<>* copy() const override
//	{
//		return new FnCall<Fn<Ret(Args...)>>{*this};
//	}
//
//	virtual void call() override
//	{
//		if (fn_)
//			expandArgs_(gen_seq<size>{});
//		else
//			throw x::error<FnCall<Fn<Ret(Args...)>>>{empty_fn, "Function is empty."};
//
//	}
//
//	virtual operator bool() const override
//	{
//		return fn_;
//	}
//
//	virtual bool empty() const override
//	{
//		return !(bool)fn_;
//	}
//
//	virtual void clear() override
//	{
//		for (int i = 0; i<size; ++i) delete args_[i];
//	}
//
//	__forceinline Ret operator()() const
//	{
//		return expandArgs_(gen_seq<size>{});
//	}
//};
//
//#undef enable_if
//#undef decay
//
//#endif //FN_H