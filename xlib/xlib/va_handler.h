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

		template<typename Func, size_t... Is>
		__forceinline auto unpackTo_(Func&& func, seq<Is...>&) const
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

		template<typename Func, typename... PackTypes>
		friend auto unpack(Func&&, pack<PackTypes...> const&);
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

	template<typename Func, typename ...PackTypes>
	__forceinline auto unpack(Func && func, pack<PackTypes...> const & argPack)
	{
		return argPack.unpackTo_(std::forward<Func>(func), gen_seq<argPack.size>{});
	}

	template<typename _ArrType, size_t _arrSize, typename _InitFirst, typename... _InitRest>
	__forceinline void arrinit(_ArrType(&arr)[_arrSize], _InitFirst&& firstVal, _InitRest&&... restVal)
	{
		static_assert(_arrSize == sizeof...(_InitRest)+1, "Array size and number of initializing arguments don't match.");

		arr[0] = firstVal;
		arrinit((_ArrType(&)[_arrSize-1])(*((_ArrType*)arr + 1)), restVal...);
	}

	template<typename _ArrType, size_t _arrSize, typename _InitFirst>
	__forceinline void arrinit(_ArrType(&arr)[_arrSize], _InitFirst&& firstVal)
	{
		static_assert(_arrSize == 1, "Array size and number of initializing arguments don't match.");

		arr[0] = firstVal;
	}
}
#undef enable_if 
#undef decay 

#endif