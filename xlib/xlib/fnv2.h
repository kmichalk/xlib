#ifndef _FN_V2_H_
#define _FN_V2_H_

#include "_void.h"
#include "more_type_traits.h"
#include "error.h"
#include "va_handler.h"
#include "finally.h"

#define enable_if std::enable_if_t
#define decay std::decay_t


	template<class...>
	class Executable;

	template<class _Ret, class... _Args>
	class Executable<_Ret(_Args...)>
	{
	public:
		virtual _Ret execute(_Args...) abstract;
		virtual Executable<_Ret(_Args...)>* copy() const abstract;
		//virtual _Ret except_call(_Args...) const abstract;
		//virtual operator bool() const abstract;
		//virtual bool empty() const abstract;
		//virtual void clear() abstract;
	};


	template<class...>
	class FnEraser_;

	template<class _Ret, class... _Args>
	class FnEraser_<_Ret(_Args...)>: public Executable<_Ret(_Args...)>
	{
	protected:

	public:
		/*FnEraser_():
		func_{nullptr},
		object_{nullptr}
		{
		}*/

		virtual FnEraser_<_Ret(_Args...)>* copy() const override
		{
			std::cout << "non\n";
			return nullptr;
		}
	};

	template<class...>
	struct FnConcrete_;

	template<class _Ret, class... _Args, class _Func>
	struct FnConcrete_<_Ret(_Args...), _Func>: public FnEraser_<_Ret(_Args...)>
	{
		using FuncType = x::naked<_Func>;
		_Func* func;
		void* object;

		template<class _ArgFunc>
		FnConcrete_(_ArgFunc&& func):
			func{x::pointer(func)},
			object{nullptr}
		{
			std::cout << (void*)x::pointer(func) << std::endl;
		}

		virtual _Ret execute(_Args... args) final override
		{
			//displn "Conc exe";
			//std::cout << sizeof(x::naked<_Func>) << " " << typeid(FuncType).name() << std::endl;
			return (*func)((std::remove_reference_t<_Args>&&)args...);
		}

		virtual FnEraser_<_Ret(_Args...)>* copy() const override
		{
			//std::cout << "Conc copy";
			return new FnConcrete_<_Ret(_Args...), _Func>{*this};
		}

		//virtual _Ret operator()(_Args... args) final override
		//{
		//	//////displn "Concrete op()";
		//	return (_Ret)(*func)((std::remove_reference_t<_Args>&&)args...);
		//}
	};

	template<class...>
	class MemberFnConcrete_;

	template<class _Ret, class... _Args, class _Type>
	struct MemberFnConcrete_<_Ret(_Args...), _Type> final: public FnEraser_<_Ret(_Args...)>
	{
		_Ret(_Type::*func)(_Args...);
		_Type* object;

		struct TypeMask: public Executable<_Ret(_Args...)>
		{
			_Ret(__thiscall _Type::*func_)(_Args...);
			_Type* object_;
		};

		template<class _ArgFunc>
		MemberFnConcrete_(_Type* object, _ArgFunc&& func):
			func{func},
			object{object}
		{
		}

		//#pragma optimize("", off)
		virtual _Ret execute(_Args... args) final override
		{
			//displn "Conc exe";
			//_Type* obj = reinterpret_cast<_Type*>(object_);
			//auto fn = reinterpret_cast<TypeMask*>(this)->func_;
			return (object->*func)((std::remove_reference_t<_Args>&&)args...);
			//return (_Ret)(((_Type*)object->*(reinterpret_cast<TypeMask*>(this)->func_))((std::remove_reference_t<_Args>&&)args...);
		}
		//#pragma optimize("", on)
		virtual FnEraser_<_Ret(_Args...)>* copy() const final override
		{
			////displn "Conc copy";
			return new MemberFnConcrete_<_Ret(_Args...), _Type>{*this};
		}

		/*	virtual _Ret operator()(_Args... args) final override
		{
		return (_Ret)(obj->*func)(args...);
		}*/
	};


	template<class...>
	class Function;

	template<class _Ret, class... _Args>
	class Function<_Ret(_Args...)>: public FnEraser_<_Ret(_Args...)>
	{
		void* func_;
		void* object_;
	public:
		template<class _Func>
		struct Overrider
		{
			_Func onDestruct;
			inline Overrider(_Func const& func):
				onDestruct{func}
			{
			}

			inline ~Overrider()
			{
				onDestruct();
			}
		};

	public:
		Function():
			FnEraser_<_Ret(_Args...)>{}
		{
		}

		template<class _Func, class = enable_if<!std::is_same<decay<_Func>, Function<_Ret(_Args...)>>::value>>
		Function(_Func&& func)
		{
			auto overriderAction = [&, this] {new (this) FnConcrete_<_Ret(_Args...), _Func>{func}; };
			overriderAction();
			//new (this) FnConcrete_<_Ret(_Args...), _Func>{func};
			//overriderAction();
			//execute();
			//new (this) FnConcrete_<_Ret(_Args...), _Func>{func};
			//Overrider<decltype(overriderAction)> overrider{overriderAction};
		}

		template<typename _Type, class _Func, class = enable_if<!std::is_same<decay<_Func>, Function<_Ret(_Args...)>>::value>>
		Function(_Type* obj, _Func&& func)
		{
			auto overriderAction = [&, this] {new (this) MemberFnConcrete_<_Ret(_Args...), _Type>{obj, func}; };
			overriderAction();
			//Overrider<decltype(overriderAction)> overrider{overriderAction};
		}

		Function(Function<_Ret(_Args...)> const& other)
		{
			x::assign(this, &other);
		}

		Function(Function<_Ret(_Args...)> && other)
		{
			x::assign(this, &other);
		}

		Function<_Ret(_Args...)>& operator=(Function<_Ret(_Args...)> const& other)
		{
			x::assign(this, &other);
			return *this;
		}

		virtual Function<_Ret(_Args...)>* copy() const override
		{
			////displn "func copy";
			return new Function<_Ret(_Args...)>{*this};
		}

		virtual __forceinline _Ret execute(_Args... args) override
		{
			this->execute((std::remove_reference_t<_Args>&&)args...);
			////displn func_;
			////displn object_;
			////displn "func exe";
		}

		__forceinline _Ret operator()(_Args... args)
		{
			return this->execute((std::remove_reference_t<_Args>&&)args...);
			//displn "Function op()";
		}

		inline operator bool() const
		{
			return func_ != nullptr;
		}

		inline bool operator!() const
		{
			return func_ == nullptr;
		}

		inline bool empty() const
		{
			return func_ == nullptr;
		}


		~Function()
		{
			/*if (func_)
			delete func_;*/
		}
	};



#pragma region FN_CALL

	//template<class...>
	//class FnCall;

	//template<class _Ret, class... _Args>
	//class FnCall<_Ret(_Args...)>: public Executable<_Ret()>
	//{
	//public:
	//	static constexpr unsigned size = sizeof...(_Args);
	//private:
	//	using FnPtr_ = _Ret(*)(_Args...);

	//	FnPtr_ func_;
	//	mutable x::va::pack<_Args...> args_;

	//	template<int... I>
	//	__forceinline _Ret expandArgs_(seq<I...>&) const
	//	{
	//		return (_Ret)func_(args_.get<I>()...);
	//	}

	//public:
	//	FnCall() = delete;

	//	FnCall(FnPtr_ func, _Args&&... args):
	//		args_{std::forward<_Args>(args)...},
	//		func_{func}
	//	{
	//	}

	//	FnCall(FnCall<_Ret(_Args...)> const& other):
	//		func_{other.func_},
	//		args_{other.args_}
	//	{
	//	}

	//	virtual Executable<_Ret()>* copy() const override
	//	{
	//		return new FnCall<_Ret(_Args...)>{*this};
	//	}

	//	virtual _Ret execute() const override
	//	{
	//		if (func_)
	//			return (_Ret)expandArgs_(gen_seq<size>{});
	//		else
	//			throw error<FnCall<_Ret(_Args...)>>{"Function is empty."};

	//	}

	//	__forceinline operator bool() const
	//	{
	//		return func_;
	//	}

	//	virtual bool empty() const override
	//	{
	//		return !(bool)func_;
	//	}

	//	//virtual void clear() override
	//	//{
	//	//	//for (int i = 0; i<size; ++i) delete args_[i];
	//	//}

	//	__forceinline _Ret operator()() const
	//	{
	//		return (_Ret)expandArgs_(gen_seq<size>{});
	//	}

	//};

	//template<class C, class _Ret, class... _Args>
	//class FnCall<C, _Ret(_Args...)>: public Executable<_Ret()>
	//{
	//public:
	//	static constexpr unsigned size = sizeof...(_Args);
	//private:
	//	using FnPtr_ = _Ret(C::*)(_Args...);
	//	/*class ArgEraser_ {};

	//	template<typename _Type>
	//	class ArgConcrete_: public ArgEraser_
	//	{
	//	public:
	//	_Type value;
	//	ArgConcrete_(_Type const& value):
	//	value{value}
	//	{
	//	}
	//	};*/


	//	mutable C* obj_;
	//	FnPtr_ func_;
	//	mutable x::va::pack<_Args...> args_;
	//	//ArgEraser_* args_[size];

	//	template<int... I>
	//	__forceinline _Ret expandArgs_(seq<I...>&) const
	//	{
	//		return (_Ret)(obj_->*func_)(args_.get<I>()...);
	//		//return (_Ret)(obj_->*func_)(static_cast<ArgConcrete_<x::select_t<I+1, _Args...>>*>(args_[I])->value...);
	//	}

	//public:
	//	FnCall() = delete;

	//	FnCall(C* obj, FnPtr_ func, _Args&&... args):
	//		args_{std::forward<_Args>(args)...},
	//		obj_{obj},
	//		func_{func}
	//	{
	//	}

	//	FnCall(FnCall<C, _Ret(_Args...)> const& other):
	//		obj_{other.obj_},
	//		func_{other.func_},
	//		args_{other.args_}
	//	{
	//	}

	//	virtual Executable<_Ret()>* copy() const override
	//	{
	//		return new FnCall<C, _Ret(_Args...)>{*this};
	//	}

	//	virtual _Ret execute() const override
	//	{
	//		if (func_)
	//			return (_Ret)expandArgs_(gen_seq<size>{});
	//		else
	//			throw error<FnCall<C, _Ret(_Args...)>>{"Function is empty."};

	//	}

	//	__forceinline operator bool() const
	//	{
	//		return func_;
	//	}

	//	virtual bool empty() const override
	//	{
	//		return !(bool)func_;
	//	}

	//	/*virtual void clear() override
	//	{
	//	for (int i = 0; i<size; ++i) delete args_[i];
	//	}*/

	//	__forceinline _Ret operator()() const
	//	{
	//		return (_Ret)expandArgs_(gen_seq<size>{});
	//	}

	//};

	//template<class _Ret, class... _Args>
	//class FnCall<Function<_Ret(_Args...)>>: public Executable<_Ret()>
	//{
	//public:
	//	static constexpr unsigned size = sizeof...(_Args);
	//private:
	//	/*class ArgEraser_ {};

	//	template<typename _Type>
	//	class ArgConcrete_: public ArgEraser_
	//	{
	//	public:
	//	_Type value;
	//	ArgConcrete_(_Type const& value):
	//	value{value}
	//	{
	//	}
	//	};*/

	//	Function<_Ret(_Args...)> func_;
	//	mutable x::va::pack<_Args...> args_;
	//	//ArgEraser_* args_[size];

	//	template<int... I>
	//	__forceinline _Ret expandArgs_(seq<I...>&) const
	//	{
	//		return (_Ret)func_(args_.get<I>()...);
	//		//return (_Ret)func_(static_cast<ArgConcrete_<x::select_t<I+1, _Args...>>*>(args_[I])->value...);
	//	}

	//public:
	//	FnCall() = delete;

	//	FnCall(Function<_Ret(_Args...)> const& func, _Args&&... args):
	//		args_{std::forward<_Args>(args)...},
	//		func_{func}
	//	{
	//	}

	//	FnCall(FnCall<Function<_Ret(_Args...)>> const& other):
	//		func_{other.func_},
	//		args_{other.args_}
	//	{
	//	}

	//	virtual Executable<_Ret()>* copy() const override
	//	{
	//		return new FnCall<Function<_Ret(_Args...)>>{*this};
	//	}

	//	virtual _Ret execute() const override
	//	{
	//		if (func_)
	//			return (_Ret)expandArgs_(gen_seq<size>{});
	//		else
	//			throw error<FnCall<Function<_Ret(_Args...)>>>{"Function is empty."};

	//	}

	//	__forceinline operator bool() const
	//	{
	//		return func_;
	//	}

	//	virtual bool empty() const override
	//	{
	//		return !(bool)func_;
	//	}

	//	/*virtual void clear() override
	//	{
	//	for (int i = 0; i<size; ++i) delete args_[i];
	//	}*/

	//	__forceinline _Ret operator()() const
	//	{
	//		return (_Ret)expandArgs_(gen_seq<size>{});
	//	}
	//};

#pragma endregion

#undef enable_if
#undef decay

#endif //_FN_V2_H_
