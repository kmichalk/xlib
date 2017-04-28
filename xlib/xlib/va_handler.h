#ifndef VA_PACK_H
#define VA_PACK_H

#define enable_if std::enable_if_t
#define decay std::decay_t
#include "more_type_traits.h"

namespace x::va
{
	template<typename... Types>
	class pack
	{
	public:
		static constexpr unsigned size = sizeof...(Types);

	private:
		template<int i>
		using NType_ = select_t<i, Types...>;

		class Eraser_
		{
		public:
			virtual Eraser_* copy() const abstract;
		};

		template<typename T>
		class Concrete_: public Eraser_
		{
		public:
			T value;

			Concrete_(T&& value):
				value{std::forward<T>(value)}
			{
			}

			virtual Eraser_* copy() const override
			{
				return new Concrete_{*this};
			}
		};

		Eraser_* args_[size];

		template<int... i>
		__forceinline void copy_(pack<Types...> const& other, seq<i...>&)
		{
			expand((args_[i] = other.args_[i]->copy())...);
		}

		template<typename Expr, size_t... Is>
		__forceinline auto unpackTo_(Expr&& func, seq<Is...>&) const
		{
			return func(static_cast<Concrete_<NType_<Is+1>>*>(args_[Is])->value...);
		}

	public:
		pack(Types&&... args):
			args_{(new Concrete_<decay<Types>>{std::forward<Types>(args)}) ...}
		{
		}

		pack(pack<Types...> const& other)
		{
			copy_(other, gen_seq<size>{});
		}

		/*template<size_t... Is>
		pack(pack<Types...> const& other, seq<Is...>&):
			args_{other.args_[Is]->copy()...}
		{
		}*/

		pack(pack<Types...>&& other):
			args_{other.args_}
		{
		}

		pack<Types...>& operator=(pack<Types...> const& other)
		{
			copy_(other, gen_seq<size>{});
			return *this;
		}

		template<size_t I>
		__forceinline auto& get()
		{
			static_assert(I <= size, "Index exceeds parameter pack size.");
			return static_cast<Concrete_<NType_<I+1>>*>(args_[I])->value;
		}

		template<size_t I>
		__forceinline NType_<I> const& cget() const
		{
			static_assert(I <= size, "Index exceeds parameter pack size.");
			return static_cast<Concrete_<NType_<I+1>>*>(args_[I])->value;
		}

		~pack()
		{
			for (int i = 0; i<size; ++i) delete args_[i];
		}

		template<typename Expr, typename... PackTypes>
		friend auto unpack(Expr&&, pack<PackTypes...> const&);
	};



	template<typename... Types>
	inline pack<Types...> make_pack(Types&&... args)
	{
		return pack<Types...>{std::forward<Types>(args)...};
	}

	/*template<typename... Types>
	inline pack<Types...> make_pack(pack<Types...> const& toCopy)
	{
		return pack<Types...>{toCopy, type_seq<Types...>{}};
	}*/

	template<typename... T>
	__forceinline constexpr void expand(T...) {}

	template<typename Expr, typename ...PackTypes>
	__forceinline auto unpack(Expr && func, pack<PackTypes...> const & argPack)
	{
		return argPack.unpackTo_(std::forward<Expr>(func), gen_seq<argPack.size>{});
	}


	template<class _Type, size_t _size, size_t... _i, class... _Args>
	__forceinline void arrinit(_Type(&arr)[_size], x::seq<_i...>&, _Args... args)
	{
		static_assert(_size == sizeof...(_Args), "Array size and number of initializing arguments must match.");
		
		x::va::expand((arr[_i] = args)...);
	}

#define arrinit(_arr, ...) x::va::arrinit(_arr, x::gen_seq<x::size(_arr)>{}, __VA_ARGS__)
}
#undef enable_if 
#undef decay 

#endif