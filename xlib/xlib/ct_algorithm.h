#ifndef CT_ALGORITHM_H
#define CT_ALGORITHM_H

#define POW2(_val) ((_val)*(_val))
#define POW3(_val) ((_val)*(_val)*(_val))
#define MEAN(_val1, _val2) (((_val1) + (_val2))/2.0)

namespace x
{
	namespace ct
	{
		/*template<int n, int p> 
		constexpr long long pow = n * pow<n, p-1>;

		template<int n>	
		constexpr long long pow<n, 0> = 1;
*/
		template<typename T>
		constexpr T pow(const T val,  const unsigned p)
		{
			return p == 0 ? 1 : val * pow(val, p-1);
		}

		//////////////////////////////////////////////////////////////////////////////

		template<int n>
		constexpr long long fact = n * fact<n-1>;

		template<>
		constexpr long long fact<0> = 1;

		//////////////////////////////////////////////////////////////////////////////

		template<int...>
		constexpr long long sum = 0;

		template<int n, int... rest>
		constexpr long long sum<n,rest...> = n + sum<rest...>;

		template<int n>
		constexpr long long sum<n> = n;

		//////////////////////////////////////////////////////////////////////////////

		template<int...>
		constexpr long long prod = 1;

		template<int n, int... rest>
		constexpr long long prod<n, rest...> = n * x::ct::prod<rest...>;

		template<int n>
		constexpr long long prod<n> = n;

		//////////////////////////////////////////////////////////////////////////////

		template<unsigned rangePow, typename T>
		struct _CtSqrt
		{
			static constexpr T _sqrt(const T x, const T xlo, const T xhi)
			{
				return (const double)(POW2(MEAN(xlo, xhi)))/(const double)(x) < 1.0-x::ct::pow(0.1,rangePow) ?
					_sqrt(x, MEAN(xlo, xhi), xhi) : (
					(const double)(POW2(MEAN(xlo, xhi)))/(const double)(x) > 1.0+x::ct::pow(0.1, rangePow) ?
						_sqrt(x, xlo, MEAN(xlo, xhi)) :
						MEAN(xlo, xhi));
			}
		};

		template<unsigned rangePow = 9, typename T>
		inline constexpr T sqrt(const T value)
		{
			return _CtSqrt<rangePow,T>::_sqrt(value, 0, value);
		}

		//////////////////////////////////////////////////////////////////////////////

		template<unsigned rangePow, typename T>
		struct _CtCbrt
		{
			static constexpr T _cbrt(const T x, const T xlo, const T xhi)
			{
				return (const double)(POW3(MEAN(xlo, xhi)))/(const double)(x) < 1.0-x::ct::pow(0.1, rangePow) ?
					_cbrt(x, MEAN(xlo, xhi), xhi) : (
					(const double)(POW3(MEAN(xlo, xhi)))/(const double)(x) > 1.0+x::ct::pow(0.1, rangePow) ?
						_cbrt(x, xlo, MEAN(xlo, xhi)) :
						MEAN(xlo, xhi));
			}
		};

		template<unsigned rangePow = 9, typename T>
		inline constexpr T cbrt(const T value)
		{
			return _CtCbrt<rangePow, T>::_cbrt(value, 0, value);
		}

		//////////////////////////////////////////////////////////////////////////////

		template<typename T>
		constexpr T abs(const T value)
		{
			return value < T(0) ? -value : value;
		}

		//////////////////////////////////////////////////////////////////////////////

		template<typename T>
		constexpr T sgn(const T value)
		{
			return (value > T(0)) - (value < T(0));
		}
	}
}

#undef POW2
#undef POW3
#undef MEAN

#endif
