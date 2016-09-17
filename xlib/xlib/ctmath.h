#ifndef CT_ALGORITHM_H
#define CT_ALGORITHM_H

#include "more_type_traits.h"

#pragma region CTMATH_MACROS
#define POW2(_val) ((_val)*(_val))
#define POW3(_val) ((_val)*(_val)*(_val))
#define MEAN(_val1, _val2) (((_val1) + (_val2))/2.0)

#define is_integral(_Type) std::is_integral<_Type>::value
#define is_unsigned(_Type) std::is_unsigned<_Type>::value
#pragma endregion
namespace x
{
	namespace ct
	{
		template<typename T>
		constexpr T pow(T val, unsigned p)
		{
			return p == 0 ? 1 : val * pow(val, p-1);
		}

		//////////////////////////////////////////////////////////////////////////////

		constexpr unsigned long long fact(unsigned value)
		{
			return value == 0 ? 1 : value*fact(value-1);
		}

		//////////////////////////////////////////////////////////////////////////////

		template<typename T, typename... R>
		constexpr auto sum(T first, R... rest)
		{
			return first + sum(rest...);
		}

		constexpr auto sum()
		{
			return 0;
		}

		//////////////////////////////////////////////////////////////////////////////

		template<typename T, typename... R>
		constexpr auto product(T first, R... rest)
		{
			return first * product(rest...);
		}

		constexpr auto product()
		{
			return 1;
		}

		//////////////////////////////////////////////////////////////////////////////

		template<unsigned rangePow, typename T>
		struct _CtSqrt
		{
			static constexpr T _sqrt(T x, T xlo, T xhi)
			{
				return (double)(POW2(MEAN(xlo, xhi)))/(double)(x) < 1.0-x::ct::pow(0.1,rangePow) ?
					_sqrt(x, MEAN(xlo, xhi), xhi) : (
					(double)(POW2(MEAN(xlo, xhi)))/(double)(x) > 1.0+x::ct::pow(0.1, rangePow) ?
						_sqrt(x, xlo, MEAN(xlo, xhi)) :
						MEAN(xlo, xhi));
			}
		};

		template<unsigned rangePow = 9, typename T>
		inline constexpr T sqrt(T value)
		{
			return _CtSqrt<rangePow,T>::_sqrt(value, 0, value);
		}

		//////////////////////////////////////////////////////////////////////////////

		template<unsigned rangePow, typename T>
		struct _CtCbrt
		{
			static constexpr T _cbrt(T x, T xlo, T xhi)
			{
				return (double)(POW3(MEAN(xlo, xhi)))/(double)(x) < 1.0-x::ct::pow(0.1, rangePow) ?
					_cbrt(x, MEAN(xlo, xhi), xhi) : (
					(double)(POW3(MEAN(xlo, xhi)))/(double)(x) > 1.0+x::ct::pow(0.1, rangePow) ?
						_cbrt(x, xlo, MEAN(xlo, xhi)) :
						MEAN(xlo, xhi));
			}
		};

		template<unsigned rangePow = 9, typename T>
		inline constexpr T cbrt(T value)
		{
			return _CtCbrt<rangePow, T>::_cbrt(value, 0, value);
		}

		//////////////////////////////////////////////////////////////////////////////

		template<typename T>
		inline constexpr T abs(T value)
		{
			return value < T(0) ? -value : value;
		}

		//////////////////////////////////////////////////////////////////////////////

		template<typename T>
		inline constexpr T sgn(T value)
		{
			return (value > T(0)) - (value < T(0));
		}

		template<typename T>
		inline constexpr T val(T _val)
		{
			return _val;
		}
	}
}

#undef POW2
#undef POW3
#undef MEAN

#endif
