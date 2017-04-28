#ifndef _X_BIGINT_H_
#define _X_BIGINT_H_

//#include "bitliteral.h"


namespace x
{
	//template<size_t _nBit>
	class int256
	{
		static constexpr __int64 bit1_ = __int64(1) << 63;

		__int64 i0_, i1_, i2_, i3_;

	public:
		/*_Int():
			data_{0};
		{
		}*/

		int256& operator+=(int256 const& other)
		{
			_asm {
				mov		eax, dword ptr[other.i3_]
				add		eax, dword ptr[i3_]
				mov		ecx, dword ptr[ebp - 8]
				adc		ecx, dword ptr[ebp-10h]
				mov		dword ptr[i3_], eax
				mov		dword ptr[ebp - 8], ecx
			}
		}
	};
}
#endif //_X_BIGINT_H_
