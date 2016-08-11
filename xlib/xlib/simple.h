#ifndef SIMPLE_H
#define SIMPLE_H

#include "more_type_traits.h"
#include "range.h"
#include "ct_algorithm.h"
#include <cmath>

#define PI 3.141592653589793

#undef min
#undef max

#define enable_if std::enable_if_t

namespace x
{
	template<typename T1, typename T2>
	inline auto min(T1 n1, T2 n2) noexcept
	{
		return n1 < n2 ? n1 : n2;
	}

	template<typename T1, typename T2>
	inline auto max(T1 n1, T2 n2) noexcept
	{
		return n1 > n2 ? n1 : n2;
	}

	template<typename T1, typename T2 = int>
	inline auto cutl(T1 val, T2 limit = 0) noexcept
	{
		return val < limit ? limit : val;
	}

	template<typename T1, typename T2 = int>
	inline auto cutg(T1 val, T2 limit = 0) noexcept
	{
		return val > limit ? limit : val;
	}

	template<typename T1, typename T2, typename T3>
	inline auto cutr(T1 val, T2 a, T3 b) noexcept
	{
		return val < a ? a : val > b ? b : val;
	}

	template <typename T>
	inline constexpr int sgn(T val) noexcept
	{
		return (T(0) < val) - (val < T(0));
	}

	template<typename T>
	inline constexpr enable_if<std::is_floating_point<T>::value,
		T> mant(T val) noexcept
	{
		return val-int(val);
	}

	template<typename T, typename = enable_if<true>>
	inline constexpr enable_if<std::is_integral<T>::value,
		double> mant(T val) noexcept
	{
		return 0;
	}

	template<typename R = double, typename... T>
	std::conditional_t<every_is<int, T...>::value, int, double> va_sum(T&&... args)
	{
		double result = 0;
		expand((result += args)...);
		return result;
	}

	template<typename R = double, typename... T>
	std::conditional_t<every_is<int, T...>::value, int, double> va_mult(T... args)
	{
		double result = 1;
		expand((result *= args)...);
		return result;
	}
	
	/*Function checking if floating point number is near to 0 with specified error.*/
	template<unsigned rangePow = 6, typename T>
	inline constexpr enable_if<std::is_floating_point<T>::value,
		T> is_zero(T val)
	{
		return val > -x::ct::pow(0.1,rangePow) && val < x::ct::pow(0.1, rangePow);
	}

	/*Function checking if floating point number is near to 0 with specified error.*/
	template<typename T>
	inline constexpr enable_if<std::is_integral<T>::value,
		double> is_zero(T val)
	{
		return val==0;
	}

	/*Function checking if floating point number is NOT near to 0 with specified error.*/
	template<unsigned rangePow = 6, typename T>
	inline constexpr enable_if<std::is_floating_point<T>::value,
		T> not_zero(T val)
	{
		return val < -x::ct::pow(0.1, rangePow) || val > x::ct::pow(0.1, rangePow);
	}

	/*Function checking if floating point number is NOT near to 0 with specified error.*/
	template<typename T, typename = enable_if<true>>
	inline constexpr enable_if<std::is_integral<T>::value,
		double> not_zero(T val)
	{
		return val;
	}

	template<typename T>
	two<T> solve_eq(T a, T b, T c)
	{
		auto delta = sqrt(b*b-4*a*c);
		return{(-b-delta)/(2*a),(-b+delta)/(2*a)};
	}

	/*Normalize angle value in radians to fit range [0,360).*/
	template<typename T>
	T norm_rf(T val)
	{
		val = fmod(val, 2*PI);
		if (val < 0) val += 2*PI;
		return val;
	}

	/*Normalize angle value in radians to fit range [-180,180).*/
	template<typename T>
	T norm_rh(T val)
	{
		val = fmod(val + PI, 2*PI);
		if (val < 0) val += 2*PI;
		return val - PI;
	}

	/*Normalize angle value in degrees to fit range [0,360).*/
	template<typename T>
	T norm_df(T val)
	{
		val = fmod(x, 360.0);
		if (val < 0) val += 360.0;
		return val;
	}

	/*Normalize angle value in degrees to fit range [-180,180).*/
	template<typename T>
	T norm_dh(T val)
	{
		val = fmod(val + 180.0, 360.0);
		if (val < 0) val += 360.0;
		return val - 180.0;
	}
	/*enum SolveMode{ NORMAL, ENABLE_1, THROW};

	template<typename T, SolveMode mode = NORMAL>
	enable_if<mode==NORMAL, 
		two<T>> solve_eq(T a, T b, T c)
	{
		auto delta = sqrt(b*b-4*a*c);
		return{(-b-delta)/(2*a),(-b+delta)/(2*a)};
	}

	template<typename T, SolveMode mode = NORMAL>
	enable_if<mode==ENABLE_1,
		two<T>> solve_eq(T a, T b, T c)
	{
		auto delta = sqrt(b*b-4*a*c);
		return{
			(-b-delta)/(2*a),
			delta ? (-b+delta)/(2*a) : nan()};
	}

	template<typename T, SolveMode mode = NORMAL>
	enable_if<mode==THROW,
		two<T>> solve_eq2(T a, T b, T c)
	{
		auto delta = b*b-4*a*c;
		if (delta<0) throw 0;
		delta = sqrt(delta);
		return{(-b-delta)/(2*a),(-b+delta)/(2*a)};
	}*/

	template<typename T>
	__forceinline constexpr auto pow2(T value) noexcept
	{
		return value*value;
	}

	template<typename T>
	__forceinline constexpr auto pow3(T value) noexcept
	{
		return value*value*value;
	}

	template<typename T>
	__forceinline constexpr bool is_nan(T value) noexcept
	{
		return value!=value;
	}

	template<typename T>
	__forceinline constexpr auto to_deg(T value) noexcept
	{
		return value/PI*180.0;
	}

	template<typename T>
	__forceinline constexpr auto to_rad(T value) noexcept
	{
		return value*PI/180.0;
	}
}

#undef enable_if
#endif //SIMPLE_H
