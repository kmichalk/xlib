#ifndef _X_COMPARE_H_
#define _X_COMPARE_H_

#include "more_type_traits.h"
#include "va_handler.h"
#include "bitliteral.h"
#include "static_if.h"
#include "_void.h"


namespace x
{
	template<class _Type>
	class _Assigner
	{
		static constexpr size_t size = sizeof(_Type);
		using DivType = _MinFitType<size * 8>;
		static constexpr size_t nDiv = size / sizeof(DivType);
		static constexpr size_t divMod = size % sizeof(DivType);
		static constexpr size_t _offset32 = nDiv * 2;
		static constexpr size_t _offset16 = nDiv * 4 + (divMod / 2) - 1;
		static constexpr size_t _offset8 = nDiv * 8 + divMod - 1;


		template<int... _i>
		static void assign_(_Type* to, _Type const* from, x::seq<_i...>&)
		{
			x::va::expand((
				*(reinterpret_cast<DivType*>(to) + _i) =
					*(reinterpret_cast<DivType const*>(from) + _i))...);

			static_if(divMod >= 4) {
				*(reinterpret_cast<__int32*>(to) + _offset32) =
					*(reinterpret_cast<__int32 const*>(from) + _offset32);
			}
			static_if((divMod/2) % 2 == 1)	{
				*(reinterpret_cast<__int16*>(to) + _offset16) =
					*(reinterpret_cast<__int16 const*>(from) + _offset16);
			}
			static_if((divMod % 2) == 1)	{
				*(reinterpret_cast<__int8*>(to) + _offset8) =
					*(reinterpret_cast<__int8 const*>(from) + _offset8);
			}
		}

	public:
		template<class _Type>
		friend std::enable_if_t< !std::is_trivially_copy_assignable<_Type>::value,
			void> assign(_Type* to, _Type const* from);

		template<class _Type>
		friend std::enable_if_t<std::is_trivially_copy_assignable<_Type>::value,
			void> assign(_Type* to, _Type const* from);

		/*template<class _Type>
		friend std::enable_if_t< !std::is_fundamental<_Type>::value,
			void> assign(_Type& to, _Type const& from);

		template<class _Type>
		friend std::enable_if_t<std::is_fundamental<_Type>::value,
			void> assign(_Type& to, _Type const& from);*/
	};

	template<class _Type>
	__forceinline std::enable_if_t< !std::is_trivially_copy_assignable<_Type>::value,
		void> assign(_Type* to, _Type const* from)
	{
		_Assigner<_Type>::assign_(to, from, x::gen_seq<_Assigner<_Type>::nDiv>{});
	}

	template<class _Type>
	__forceinline std::enable_if_t<std::is_trivially_copy_assignable<_Type>::value,
		void> assign(_Type* to, _Type const* from)
	{
		*to = *from;
	}

	/*template<class _Type>
	__forceinline std::enable_if_t< !std::is_fundamental<_Type>::value,
		void> assign(_Type& to, _Type const& from)
	{
		std::cout << "assigner &\n";
		_Assigner<_Type>::assign_(x::pointer(to), x::pointer(from), x::gen_seq<_Assigner<_Type>::nDiv>{});
	}

	template<class _Type>
	__forceinline std::enable_if_t<std::is_fundamental<_Type>::value,
		void> assign(_Type& to, _Type const& from)
	{
		std::cout << "trivial &\n";
		x::deref(to) = x::deref(from);
	}*/
}

#endif //_X_COMPARE_H_
