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

	template<typename _Ret, typename... _Args>
	class Callable<_Ret(_Args...)>
	{
	public:
		enum ErrorNum { empty_fn };

		virtual Callable<_Ret(_Args...)>* copy() const abstract;
		virtual _Ret call(_Args...) const abstract;
		//virtual _Ret except_call(_Args...) const abstract;
		//virtual operator bool() const abstract;
		virtual bool empty() const abstract;
		//virtual void clear() abstract;
	};

	template<typename...>
	class Fn;

	template<typename _Ret, typename... _Args>
	class Fn<_Ret(_Args...)>: public Callable<_Ret(_Args...)>
	{
		struct FnEraser_
		{
			virtual _Ret operator()(_Args&&...) const abstract;
			virtual FnEraser_* copy() const abstract;
		};

		template<typename...>
		struct FnConcrete_;

		template<typename _Func>
		struct FnConcrete_<_Func> final: public FnEraser_
		{
			mutable _Func fn;

			template<typename G>
			FnConcrete_(G&& fn):
				fn{std::forward<G>(fn)}
			{
			}

			_Ret operator()(_Args&&... args) const final override
			{
				return (_Ret)fn(args...);
			}

			FnEraser_* copy() const final override
			{
				return new FnConcrete_<_Func>{fn};
			}
		};

		template<class T, typename _Func>
		struct FnConcrete_<T, _Func> final: public FnEraser_
		{
			mutable T* obj;
			mutable _Func fn;

			template<typename G>
			FnConcrete_(T* obj, G&& fn):
				obj{obj},
				fn{std::forward<G>(fn)}
			{
			}

			_Ret operator()(_Args... args) const final override
			{
				return (_Ret)(obj->*fn)(args...);
			}

			FnEraser_* copy() const final override
			{
				return new FnConcrete_<T, _Func>{obj,fn};
			}
		};

		FnEraser_* fn_;

	public:
		Fn():
			fn_{nullptr}
		{
		}

		template<typename _Func, typename = enable_if<!std::is_same<decay<_Func>, Fn<_Ret(_Args...)>>::value>>
		Fn(_Func&& fn) :
			fn_{new FnConcrete_<decay<_Func>>{std::forward<_Func>(fn)}}
		{
		}

		template<typename T, typename _Func, typename = enable_if<!std::is_same<decay<_Func>, Fn<_Ret(_Args...)>>::value>>
		Fn(T* obj, _Func&& fn) :
			fn_{new FnConcrete_<T,decay<_Func>>{obj,std::forward<_Func>(fn)}}
		{
		}

		Fn(Fn<_Ret(_Args...)> const& other):
			fn_{other.fn_ ? other.fn_->copy() : nullptr}
		{ }

		Fn<_Ret(_Args...)>& operator=(Fn<_Ret(_Args...)> const& other)
		{
			if (fn_) delete fn_;
			fn_ = other.fn_->copy();
			return *this;
		}

		Callable<_Ret(_Args...)>* copy() const override
		{
			return new Fn<_Ret(_Args...)>{*this};
		}

		template<typename _Func>
		void set(_Func&& fn)
		{
			if (fn_) delete fn_;
			fn_ = new FnConcrete_<decay<_Func>>{std::forward<_Func>(fn)};
		}

		__forceinline _Ret operator()(_Args... args) const
		{
			return (_Ret)(*fn_)((std::remove_reference_t<_Args>&&)args...);
		}

		__forceinline operator bool() const
		{
			return fn_ != nullptr;
		}

		__forceinline bool empty() const
		{
			return fn_ == nullptr;
		}

		virtual _Ret call(_Args... args) const final override
		{
			//if (fn_)
				return (_Ret)(*fn_)(std::forward<_Args>(args)...);
			//else
				//throw error<Fn<_Ret(_Args...)>>{"Function is empty."};
		}

		~Fn()
		{
			if (fn_)
				delete fn_;
		}
	};

	template<class...>
	class _Function;

	template<class _Func, class _DeclRet, class... _DeclArgs>
	class _Function<_Func, _DeclRet(_DeclArgs...)>
	{
		_Func* fn_;

	public:
		_Function():
			fn_{nullptr}
		{
		}

		_Function(_Func& fn) :
			fn_{x::pointer(fn)}
		{
		}

		_Function(_Function<_Func> const& other):
			fn_{other.fn_}
		{
		}

		_Function<_Func>& operator=(_Function<_Func> const& other)
		{
			fn_ = other.fn_;
			return *this;
		}

		_Function<_Func>* copy() const
		{
			return new _Function<_Func>{*this};
		}

		template<typename _Func>
		void set(_Func&& fn)
		{
			fn_ = x::pointer(fn);
		}

		virtual _DeclRet operator()(_DeclArgs... args) const
		{
			return (*fn_)(std::forward<_DeclArgs>(args)...);
		}

		__forceinline operator bool() const
		{
			return fn_;
		}

		__forceinline bool empty() const
		{
			return !(bool)fn_;
		}

		
		~_Function()
		{
			
		}
	};

	/*template<class _Func, class... _Rest>
	_Function<x::naked<_Func>, _Rest...> func(_Func&&);*/

	/*template<class...>
	void func();*/
	template<class...>
	struct _FuncFactory;

	template<class _Ret, class... _Args, class _Func>
	struct _FuncFactory<_Ret(_Args...), _Func>
	{
		__forceinline static _Function<x::naked<_Func>, _Ret(_Args...)> create(_Func&& func)
		{
			return _Function<x::naked<_Func>, _Ret(_Args...)>{func};
		}
	};

	/*template<class _DeclFunc, class _Func>
	__forceinline auto func(_Func&& func)
	{
		return _FuncFactory<_DeclFunc, _Func>::create(std::forward<_Func>(func));
	}*/

	template<class _Owner, typename _Ret, typename... _Args>
	class Fn<_Owner, _Ret(_Args...)>: public Callable<_Ret(_Args...)>
	{
		using FnType_ = _Ret(_Owner::*)(_Args...);

		FnType_ fn_;
		mutable _Owner* obj_;

	public:
		Fn():
			fn_{nullptr},
			obj_{nullptr}
		{
		}

		Fn(_Owner* obj, FnType_ fn):
			fn_{fn},
			obj_{obj}
		{
		}

		Fn(Fn<_Owner, _Ret(_Args...)> const& other):
			fn_{other.fn_},
			obj_{other.obj_}
		{ }

		Fn<_Owner, _Ret(_Args...)>& operator=(Fn<_Owner, _Ret(_Args...)> const& other)
		{
			fn_ = other.fn_->copy();
			return *this;
		}

		Callable<_Ret(_Args...)>* copy() const override
		{
			return new Fn<_Owner, _Ret(_Args...)>{*this};
		}

		void set(_Owner* obj, FnType_ fn)
		{
			fn_ = fn;
			obj_ = obj;
		}

		__forceinline _Ret operator()(_Args... args) const
		{
			return (_Ret)(obj_->*fn_)(std::forward<_Args>(args)...);
		}

		__forceinline operator bool() const
		{
			return fn_;
		}

		__forceinline bool empty() const
		{
			return !(bool)fn_ || !(bool)obj_;
		}

		virtual _Ret call(_Args... args) const final override
		{
			if (fn_ && obj_)
				return (_Ret)(obj_->*fn_)(std::forward<_Args>(args)...);
			else
				throw error<Fn<_Owner, _Ret(_Args...)>>{"Function is empty."};
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

	template<typename _Ret, typename... _Args>
	class Fn<_Ret(*)(_Args...)>: public Callable<_Ret(_Args...)>
	{
	public:
		using FnType_ = _Ret(*)(_Args...);

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

		Fn(Fn<_Ret(*)(_Args...)> const& other):
			fn_{other.fn_}
		{ }

		Fn<_Ret(*)(_Args...)>& operator=(Fn<_Ret(*)(_Args...)> const& other)
		{
			fn_ = other.fn_->copy();
			return *this;
		}

		Callable<_Ret(_Args...)>* copy() const override
		{
			return new Fn<_Ret(*)(_Args...)>{*this};
		}

		void set(FnType_ fn)
		{
			fn_ = fn;
		}

		__forceinline _Ret operator()(_Args... args) const
		{
			return (_Ret)(*fn_)(std::forward<_Args>(args)...);
		}

		__forceinline operator bool() const
		{
			return fn_;
		}

		__forceinline bool empty() const
		{
			return !(bool)fn_;
		}

		virtual _Ret call(_Args... args) const final override
		{
			if (fn_)
				return (_Ret)(*fn_)(std::forward<_Args>(args)...);
			else
				throw error<Fn<_Ret(*)(_Args...)>>{"Function is empty."};
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

	template<typename _Ret, typename... _Args>
	class FnCall<_Ret(_Args...)>: public Callable<_Ret()>
	{
	public:
		static constexpr unsigned size = sizeof...(_Args);
	private:
		using FnPtr_ = _Ret(*)(_Args...);

		FnPtr_ fn_;
		mutable x::va::pack<_Args...> args_;

		template<int... I>
		__forceinline _Ret expandArgs_(seq<I...>&) const
		{
			return (_Ret)fn_(args_.get<I>()...);
		}

	public:
		FnCall() = delete;

		FnCall(FnPtr_ fn, _Args&&... args):
			args_{std::forward<_Args>(args)...},
			fn_{fn}
		{
		}

		FnCall(FnCall<_Ret(_Args...)> const& other):
			fn_{other.fn_},
			args_{other.args_}
		{
		}

		virtual Callable<_Ret()>* copy() const override
		{
			return new FnCall<_Ret(_Args...)>{*this};
		}

		virtual _Ret call() const override
		{
			if (fn_)
				return (_Ret)expandArgs_(gen_seq<size>{});
			else
				throw error<FnCall<_Ret(_Args...)>>{"Function is empty."};

		}

		__forceinline operator bool() const
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

		__forceinline _Ret operator()() const
		{
			return (_Ret)expandArgs_(gen_seq<size>{});
		}

	};

	//template<typename _Ret, typename... _Args>
	//class FnCall<_Ret(_Args...)>: public Callable<_Ret()>
	//{
	//public:
	//	static constexpr unsigned size = sizeof...(_Args);
	//private:
	//	using FnPtr_ = _Ret(*)(_Args...);
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
	//	__forceinline _Ret expandArgs_(x::seq<i...>&) const
	//	{
	//		return (_Ret)fn_(static_cast<ArgConcrete_<x::select_t<i+1, _Args...>>*>(args_[i])->value...);
	//	}
	//
	//public:
	//	FnCall(FnPtr_ fn, _Args const&... args):
	//		args_{(new ArgConcrete_<_Args>{args})...},
	//		fn_{fn}
	//	{
	//	}
	//
	//	virtual Callable<_Ret()>* copy() const override
	//	{
	//		return new FnCall<_Ret(_Args...)>{*this};
	//	}
	//
	//	virtual _Ret call() const override
	//	{
	//		if (fn_)
	//			return (_Ret)expandArgs_(gen_seq<size>{});
	//		else
	//			throw x::error<FnCall<_Ret(_Args...)>>{empty_fn, "Function is empty."};
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
	//	__forceinline _Ret operator()() const
	//	{
	//		return (_Ret)expandArgs_(gen_seq<size>{});
	//	}
	//
	//};

	template<class C, typename _Ret, typename... _Args>
	class FnCall<C, _Ret(_Args...)>: public Callable<_Ret()>
	{
	public:
		static constexpr unsigned size = sizeof...(_Args);
	private:
		using FnPtr_ = _Ret(C::*)(_Args...);
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
		mutable x::va::pack<_Args...> args_;
		//ArgEraser_* args_[size];

		template<int... I>
		__forceinline _Ret expandArgs_(seq<I...>&) const
		{
			return (_Ret)(obj_->*fn_)(args_.get<I>()...);
			//return (_Ret)(obj_->*fn_)(static_cast<ArgConcrete_<x::select_t<I+1, _Args...>>*>(args_[I])->value...);
		}

	public:
		FnCall() = delete;

		FnCall(C* obj, FnPtr_ fn, _Args&&... args):
			args_{std::forward<_Args>(args)...},
			obj_{obj},
			fn_{fn}
		{
		}

		FnCall(FnCall<C, _Ret(_Args...)> const& other):
			obj_{other.obj_},
			fn_{other.fn_},
			args_{other.args_}
		{
		}

		virtual Callable<_Ret()>* copy() const override
		{
			return new FnCall<C, _Ret(_Args...)>{*this};
		}

		virtual _Ret call() const override
		{
			if (fn_)
				return (_Ret)expandArgs_(gen_seq<size>{});
			else
				throw error<FnCall<C, _Ret(_Args...)>>{"Function is empty."};

		}

		__forceinline operator bool() const
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

		__forceinline _Ret operator()() const
		{
			return (_Ret)expandArgs_(gen_seq<size>{});
		}

	};

	template<typename _Ret, typename... _Args>
	class FnCall<Fn<_Ret(_Args...)>>: public Callable<_Ret()>
	{
	public:
		static constexpr unsigned size = sizeof...(_Args);
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

		Fn<_Ret(_Args...)> fn_;
		mutable x::va::pack<_Args...> args_;
		//ArgEraser_* args_[size];

		template<int... I>
		__forceinline _Ret expandArgs_(seq<I...>&) const
		{
			return (_Ret)fn_(args_.get<I>()...);
			//return (_Ret)fn_(static_cast<ArgConcrete_<x::select_t<I+1, _Args...>>*>(args_[I])->value...);
		}

	public:
		FnCall() = delete;

		FnCall(Fn<_Ret(_Args...)> const& fn, _Args&&... args):
			args_{std::forward<_Args>(args)...},
			fn_{fn}
		{
		}

		FnCall(FnCall<Fn<_Ret(_Args...)>> const& other):
			fn_{other.fn_},
			args_{other.args_}
		{
		}

		virtual Callable<_Ret()>* copy() const override
		{
			return new FnCall<Fn<_Ret(_Args...)>>{*this};
		}

		virtual _Ret call() const override
		{
			if (fn_)
				return (_Ret)expandArgs_(gen_seq<size>{});
			else
				throw error<FnCall<Fn<_Ret(_Args...)>>>{"Function is empty."};

		}

		__forceinline operator bool() const
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

		__forceinline _Ret operator()() const
		{
			return (_Ret)expandArgs_(gen_seq<size>{});
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
//template<typename... _Args>
//class Callable
//{
//public:
//	enum ErrorNum { empty_fn };
//
//	virtual Callable<_Args...>* copy() const abstract;
//	virtual void call(_Args...) abstract;
//	virtual operator bool() const abstract;
//	virtual bool empty() const abstract;
//	virtual void clear() abstract;
//};
//
//template<typename...>
//class Fn;
//
//template<typename _Ret, typename... _Args>
//class Fn<_Ret(_Args...)>: public Callable<_Args...>
//{
//	struct FnEraser_
//	{
//		virtual _Ret operator()(_Args...) const abstract;
//		virtual FnEraser_* virtual_copy() const abstract;
//	};
//
//	template<typename...>
//	struct FnConcrete_;
//
//	template<typename _Func>
//	struct FnConcrete_<_Func> final: public FnEraser_
//	{
//		_Func fn;
//
//		template<typename G>
//		FnConcrete_(G&& fn): 
//			fn{std::forward<G>(fn)} 
//		{
//		}
//
//		_Ret operator()(_Args... args) const final override
//		{
//			return (_Ret)fn(args...);
//		}
//
//		FnEraser_* virtual_copy() const final override
//		{
//			return new FnConcrete_<_Func>{fn};
//		}
//	};
//
//	template<class T, typename _Func>
//	struct FnConcrete_<T,_Func> final: public FnEraser_
//	{
//		T* obj;
//		_Func fn;
//
//		template<typename G>
//		FnConcrete_(T* obj, G&& fn): 
//			obj{obj},
//			fn{std::forward<G>(fn)} 
//		{
//		}
//
//		_Ret operator()(_Args... args) const final override
//		{
//			return (_Ret)(obj->*fn)(args...);
//		}
//
//		FnEraser_* virtual_copy() const final override
//		{
//			return new FnConcrete_<T,_Func>{obj,fn};
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
//	template<typename _Func, typename = enable_if<!std::is_same<decay<_Func>, Fn<_Ret(_Args...)>>::value>>
//	Fn(_Func&& fn) :
//		fn_{new FnConcrete_<decay<_Func>>{std::forward<_Func>(fn)}}
//	{
//	}
//
//	template<typename T, typename _Func, typename = enable_if<!std::is_same<decay<_Func>, Fn<_Ret(_Args...)>>::value>>
//	Fn(T* obj, _Func&& fn) :
//		fn_{new FnConcrete_<T,decay<_Func>>{obj,std::forward<_Func>(fn)}}
//	{
//	}
//
//	Fn(Fn<_Ret(_Args...)> const& other):
//		fn_{other.fn_ ? other.fn_->virtual_copy() : nullptr}
//	{ }
//
//	Fn<_Ret(_Args...)>& operator=(Fn<_Ret(_Args...)> const& other)
//	{
//		if (fn_) delete fn_;
//		fn_ = other.fn_->virtual_copy();
//		return *this;
//	}
//
//	Callable<_Args...>* copy() const override
//	{
//		return new Fn<_Ret(_Args...)>{*this};
//	}
//
//	template<typename _Func>
//	void set(_Func&& fn)
//	{
//		if (fn_) delete fn_;
//		fn_ = new FnConcrete_<decay<_Func>>{std::forward<_Func>(fn)};
//	}
//
//	__forceinline _Ret operator()(_Args... args)
//	{
//		/*return */(_Ret)(*fn_)(std::forward<_Args>(args)...);
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
//	virtual void call(_Args... args) final override
//	{
//		if (fn_)
//			return (_Ret)(*fn_)(std::forward<_Args>(args)...);
//		else
//			throw x::error<Fn<_Ret(_Args...)>>{empty_fn, "Function is empty."};
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
//template<class T, typename _Ret, typename... _Args>
//class Fn<T,_Ret(_Args...)>: public Callable<_Args...>
//{
//	using FnType_ = _Ret(T::*)(_Args...);
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
//	Fn(Fn<T, _Ret(_Args...)> const& other):
//		fn_{other.fn_},
//		obj_{other.obj_}
//	{ }
//
//	Fn<T,_Ret(_Args...)>& operator=(Fn<T,_Ret(_Args...)> const& other)
//	{
//		fn_ = other.fn_->virtual_copy();
//		return *this;
//	}
//
//	Callable<_Args...>* copy() const override
//	{
//		return new Fn<T,_Ret(_Args...)>{*this};
//	}
//
//	void set(T* obj, FnType_ fn)
//	{
//		fn_ = fn;
//		obj_ = obj;
//	}
//
//	__forceinline _Ret operator()(_Args... args)
//	{
//		return (_Ret)(obj_->*fn_)(std::forward<_Args>(args)...);
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
//	virtual void call(_Args... args) final override
//	{
//		if (fn_ && obj_)
//			return (_Ret)(obj_->*fn_)(std::forward<_Args>(args)...);
//		else
//			throw x::error<Fn<T,_Ret(_Args...)>>{empty_fn, "Function is empty."};
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
//template<typename _Ret, typename... _Args>
//class Fn<_Ret(*)(_Args...)>: public Callable<_Args...>
//{
//	using FnType_ = _Ret(*)(_Args...);
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
//	Fn(Fn<_Ret(*)(_Args...)> const& other):
//		fn_{other.fn_}
//	{ }
//
//	Fn<_Ret(*)(_Args...)>& operator=(Fn<_Ret(*)(_Args...)> const& other)
//	{
//		fn_ = other.fn_->virtual_copy();
//		return *this;
//	}
//
//	Callable<_Args...>* copy() const override
//	{
//		return new Fn<_Ret(*)(_Args...)>{*this};
//	}
//
//	void set(FnType_ fn)
//	{
//		fn_ = fn;
//	}
//
//	__forceinline _Ret operator()(_Args... args)
//	{
//		return (_Ret)(*fn_)(std::forward<_Args>(args)...);
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
//	virtual void call(_Args... args) final override
//	{
//		if (fn_)
//			return (_Ret)(*fn_)(std::forward<_Args>(args)...);
//		else
//			throw x::error<Fn<_Ret(*)(_Args...)>>{empty_fn, "Function is empty."};
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
//template<typename _Ret, typename... _Args>
//class FnCall<_Ret(_Args...)>: public Callable<>
//{
//public:
//	static constexpr unsigned size = sizeof...(_Args);
//private:
//	using FnPtr_ = _Ret(*)(_Args...);
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
//	__forceinline _Ret expandArgs_(seq<i...>&) const
//	{
//		return fn_(static_cast<ArgConcrete_<x::select_t<i+1, _Args...>>*>(args_[i])->value...);
//	}
//
//public:
//	FnCall(FnPtr_ fn, _Args const&... args):
//		args_{(new ArgConcrete_<_Args>{args})...},
//		fn_{fn}
//	{
//	}
//
//	virtual Callable<>* copy() const override
//	{
//		return new FnCall<_Ret(_Args...)>{*this};
//	}
//
//	virtual void call() override
//	{
//		if (fn_)
//			expandArgs_(gen_seq<size>{});
//		else
//			throw x::error<FnCall<_Ret(_Args...)>>{empty_fn, "Function is empty."};
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
//	__forceinline _Ret operator()() const
//	{
//		return expandArgs_(gen_seq<size>{});
//	}
//
//};
//
//template<class C, typename _Ret, typename... _Args>
//class FnCall<C, _Ret(_Args...)>: public Callable<>
//{
//public:
//	static constexpr unsigned size = sizeof...(_Args);
//private:
//	using FnPtr_ = _Ret(C::*)(_Args...);
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
//	__forceinline _Ret expandArgs_(seq<i...>&) const
//	{
//		return (obj_->*fn_)(static_cast<ArgConcrete_<x::select_t<i+1, _Args...>>*>(args_[i])->value...);
//	}
//
//public:
//	FnCall(C* obj, FnPtr_ fn, _Args const&... args):
//		args_{(new ArgConcrete_<_Args>{args})...},
//		obj_{obj}, fn_{fn}
//	{
//	}
//
//	virtual Callable<>* copy() const override
//	{
//		return new FnCall<C,_Ret(_Args...)>{*this};
//	}
//
//	virtual void call() override
//	{
//		if (fn_)
//			expandArgs_(gen_seq<size>{});
//		else
//			throw x::error<FnCall<C,_Ret(_Args...)>>{empty_fn, "Function is empty."};
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
//	__forceinline _Ret operator()() const
//	{
//		return expandArgs_(gen_seq<size>{});
//	}
//
//};
//
//template<typename _Ret, typename... _Args>
//class FnCall<Fn<_Ret(_Args...)>>: public Callable<>
//{
//public:
//	static constexpr unsigned size = sizeof...(_Args);
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
//	Fn<_Ret(_Args...)> fn_;
//	ArgEraser_* args_[size];
//
//	template<int... i>
//	__forceinline _Ret expandArgs_(seq<i...>&) const
//	{
//		return fn_(static_cast<ArgConcrete_<x::select_t<i+1, _Args...>>*>(args_[i])->value...);
//	}
//
//public:
//	FnCall(Fn<_Ret(_Args...)> const& fn, _Args const&... args):
//		args_{(new ArgConcrete_<_Args>{args})...},
//		fn_{fn}
//	{
//	}
//
//	virtual Callable<>* copy() const override
//	{
//		return new FnCall<Fn<_Ret(_Args...)>>{*this};
//	}
//
//	virtual void call() override
//	{
//		if (fn_)
//			expandArgs_(gen_seq<size>{});
//		else
//			throw x::error<FnCall<Fn<_Ret(_Args...)>>>{empty_fn, "Function is empty."};
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
//	__forceinline _Ret operator()() const
//	{
//		return expandArgs_(gen_seq<size>{});
//	}
//};
//
//#undef enable_if
//#undef decay
//
//#endif //FN_H