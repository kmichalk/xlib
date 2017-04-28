#ifndef _EVENT_
#define _EVENT_H_

#include "va_handler.h"
#include "more_type_traits.h"

namespace x
{
	template<class... _Args>
	class event
	{
	public:
		class FuncEraser
		{
		public:
			static thread_local x::va::pack<_Args...> argPack;

			static inline void makePack(_Args... args)
			{
				argPack = x::va::make_pack(std::forward<_Args>(args)...);
			}

			_void<> virtual call() const abstract;
		};

		template<class _Owner>
		class FuncConcrete:
			public FuncEraser
		{
			using FuncType = void(_Owner::*)(_Args...);

			_Owner* objectPtr_;
			FuncType member_;

			template<int... _i>
			__forceinline void expandArgs_(x::seq<_i...>&) const
			{
				(objectPtr_->*member_)(FuncEraser::argPack.get<_i>()...);
			}

		public:
			template<class _Ret>
			FuncConcrete(_Owner* objectPtr, _Ret(_Owner::*member)(_Args...)) :
				objectPtr_	{objectPtr},
				member_		{(FuncType)member}
			{
			}

			FuncConcrete()								= delete;
			FuncConcrete(FuncConcrete<_Owner> const&)	= default;
			FuncConcrete(FuncConcrete<_Owner>&&)		= default;

			_void<> virtual call() const override
			{
				expandArgs_(x::gen_seq<sizeof...(_Args)>{});
				return _void<>{};
			}
		};

		class FuncPackEraser
		{
		public:
			void virtual call() const abstract;
		};

		template<size_t _size>
		class FuncPackConcrete:
			public FuncPackEraser
		{
			FuncEraser* funcPack_[_size];

			template<int... _i>
			__forceinline void expandFuncs_(x::seq<_i...>&) const
			{
				x::va::expand(funcPack_[_i]->call()...);
			}

		public:
			template<class... _CtorArgs>
			FuncPackConcrete(_CtorArgs... funcs) :
				funcPack_{funcs...}
			{
			}

			void virtual call() const override
			{
				expandFuncs_(x::gen_seq<_size>{});
			}
		};
	public:
		FuncPackEraser* funcPack_;

		template<class _Ret, class _Owner>
		struct F
		{
			_Owner* obj;
			_Ret(_Owner::*member)(_Args...);

			F(_Owner* obj,
			_Ret(_Owner::*member)(_Args...)): obj{obj, member{member}}{}
		};
	
		event()
		{

		}

		__forceinline void operator()(_Args... args)
		{
			//FuncEraser::makePack(args...);
			funcPack_->call();
		}
	};

	template<class... _Args>
	thread_local x::va::pack<_Args...> event<_Args...>::FuncEraser::argPack = {0,0.0f};
}
#endif //_EVENT_H_
