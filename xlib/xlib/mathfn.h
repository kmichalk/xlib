#ifndef _MATH_FUNCTION_H
#define _MATH_FUNCTION_H_

#include "fn.h"

namespace x::math
{

	template<class _Ret, class... _Args>
	using FuncPtr = _Ret(*)(_Args...);

	template<class... _Types>
	struct Pack
	{
		template<class _Added>
		using add_type = Pack<_Types..., _Added>;
	};

	template<class...>
	struct UnpackAsFuncPtrArgs;

	template<class _Ret, class... _PackedTypes>
	struct UnpackAsFuncPtrArgs<_Ret, Pack<_PackedTypes...>>
	{
		using type = FuncPtr<_Ret, _PackedTypes...>;
	};


	template<class _Type, int _size, class... _Buffer>
	struct UniPackGenerator
	{
		using type = typename UniPackGenerator<_Type, _size - 1, _Buffer..., _Type>::type;
	};

	template<class _Type, class... _Buffer>
	struct UniPackGenerator<_Type, 0, _Buffer...>
	{
		using type = Pack<_Buffer...>;
	};

	template<class _Ret, class _Arg, unsigned _nArg>
	using GenFuncPtr = typename UnpackAsFuncPtrArgs<_Ret, typename UniPackGenerator<_Arg, _nArg>::type>::type;

	template<typename _Type>
	class expr
	{
	public:
		virtual _Type operator()(_Type) const abstract;
		virtual expr<_Type>* copy() const abstract;
	};

	template<class _Type, unsigned _nArg>
	class func: 
		public expr<_Type>
	{
		using FuncPtr = GenFuncPtr<_Type, _Type, _nArg>;

		FuncPtr func_;
		expr<_Type>* args_[_nArg];

		template<size_t... _i>
		__forceinline _Type expandExecute_(_Type arg, x::seq<_i...>&) const
		{
			return func_((args_[_i] ? (*args_[_i])(arg) : arg)...);
		}

		template<size_t... _i>
		__forceinline void expandCopyArgs_(func<_Type, _nArg> const& other, x::seq<_i...>&)
		{
			x::va::expand((args_[_i] = other.args_[_i] ? other.args_[_i]->copy() : nullptr)...);
		}

		template<size_t... _i>
		__forceinline void expandMoveArgs_(func<_Type, _nArg> const& other, x::seq<_i...>&)
		{
			x::va::expand((args_[_i] = other.args_[_i])...);
		}

	public:
		func() = delete;

		func(FuncPtr func):
			func_{func},
			args_{nullptr}
		{
		}

		template<class... _Args>
		func(FuncPtr func, _Args const&... args):
			func_{func},
			args_{args.copy()...}
		{
			static_assert(
				x::all_true<std::is_base_of<expr<_Type>, _Args>::value...>::value,
				"Arguments provided are not expr.");
			static_assert(
				sizeof...(_Args) == _nArg,
				"Too many arguments.");
		}

		template<class... _Args>
		func(FuncPtr func, _Args const*... args):
			func_{func},
			args_{args->copy()...}
		{
			static_assert(
				x::all_true<std::is_base_of<expr<_Type>, _Args>::value...>::value,
				"Arguments provided are not expr.");
			static_assert(
				sizeof...(_Args) == _nArg,
				"Too many arguments.");
		}

		func(func<_Type, _nArg> const& other):
			func_{other.func_},
			args_{nullptr}
		{
			expandCopyArgs_(other, x::gen_seq<_nArg>{});
		}

		func(func<_Type, _nArg> && other):
			func_{other.func_},
			args_{nullptr}
		{
			expandMoveArgs_(other, x::gen_seq<_nArg>{});
		}

		virtual _Type operator()(_Type arg) const override
		{
			return expandExecute_(arg, x::gen_seq<_nArg>{});
		}

		virtual expr<_Type>* copy() const override
		{
			return new func<_Type, _nArg>{*this};
		}
	};

	template<class _Type>
	class cval:
		public expr<_Type>
	{
		_Type value_;

	public:
		cval() = delete;

		cval(_Type value):
			value_{value}
		{
		}

		cval(cval<_Type> const& other):
			value_{other.value_}
		{
		}

		cval(cval<_Type> && other):
			value_{std::move(other.value_)}
		{
		}

		virtual _Type operator()(_Type arg) const override
		{
			return value_;
		}

		virtual expr<_Type>* copy() const override
		{
			return new cval<_Type>{*this};
		}
	};

	template<class _Type>
	class val:
		public expr<_Type>
	{
		_Type const* value_;

	public:
		val() = delete;

		val(_Type const* value):
			value_{value}
		{
		}

		val(_Type value):
			value_{new _Type{value}}
		{
		}

		val(val<_Type> const& other):
			value_{other.value_}
		{
		}

		val(val<_Type>&& other):
			value_{other.value_}
		{
		}

		virtual _Type operator()(_Type arg) const override
		{
			return *value_;
		}

		virtual expr<_Type>* copy() const override
		{
			return new val<_Type>{*this};
		}
	};
}
#endif //_MATH_FUNCTION_H_
