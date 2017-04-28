#ifndef BITLITERAL_H
#define BITLITERAL_H

#include "typearr.h"
#include "byte.h"

namespace x
{
	template<typename _Type, char... _rest>
	struct conv2bin;

	template<typename _Type, char _first, char... _rest>
	struct conv2bin<_Type, _first, _rest...>
	{
		static_assert(_first == '0' || _first == '1',
			"Not valid binary number.");

		_Type static constexpr value =
			(_first - '0') * (1 << sizeof...(_rest)) +
			conv2bin<_Type, _rest...>::value;
	};

	template<typename _Type, char _first>
	struct conv2bin<_Type, _first>
	{
		static_assert(_first == '0' || _first == '1',
			"Not valid binary number.");

		_Type static constexpr value =
			(_first - '0');
	};

	using _IntTypes = x::TypeArray<
		byte,
		__int16,
		__int32,
		__int64>;

	template<unsigned _nBits>
	using _MinFitType =
		_IntTypes::get<
		1 +
		(_nBits > 8) +
		(_nBits > 16) +
		(_nBits > 32)>;

	template<unsigned _nBits>
	using _MaxLessType =
		_IntTypes::get<
		(_nBits <= 8) +
		(_nBits <= 16) +
		(_nBits <= 32) +
		(_nBits <= 64)>;

	template<char... _digits>
	__forceinline constexpr typename _MinFitType<sizeof...(_digits)> operator "" _b()
	{
		return conv2bin<_MinFitType<sizeof...(_digits)>, _digits...>::value;
	}

	template<size_t _bitNum, class _Type = unsigned __int32>
	constexpr _Type BIT = _Type(1) << _bitNum;

	template<class _Type>
	constexpr _Type FIRST_BIT = _Type(1) << (sizeof(_Type) * 8 - 1);

	template<class _Type>
	constexpr _Type LAST_BIT = _Type(1);


	template<class _Type>
	static constexpr size_t BIT_SIZE = sizeof(_Type) * 8;
	
}

#define bitsizeof(_Type) x::BIT_SIZE<_Type>


#endif //BITLITERAL_H
