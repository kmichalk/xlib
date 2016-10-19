#ifndef BITLITERAL_
#define BITLITERAL_H

#include "typearr.h"

using byte = unsigned char;

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

using _MinTypeArr = x::TypeArray<
	byte,
	unsigned short,
	unsigned int,
	unsigned long long>;

template<unsigned _nBits>
using _MinFitType = 
	_MinTypeArr::get<
		1 +
		(_nBits > 8) +
		(_nBits > 16) +
		(_nBits > 32)>;

template<char... _digits>
__forceinline constexpr typename _MinFitType<sizeof...(_digits)> operator "" _b()
{
	return conv2bin<_MinFitType<sizeof...(_digits)>, _digits...>::value;
}

#endif //BITLITERAL_H
