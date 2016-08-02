#ifndef SIMPLE_H
#define SIMPLE_H

#include "more_type_traits.h"
#include "range.h"

#undef min
#undef max

#define enable_if std::enable_if_t

namespace x
{
	template<typename T1, typename T2>
	inline auto min(T1&& n1, T2&& n2)
	{
		return n1 < n2 ? n1 : n2;
	}

	template<typename T1, typename T2>
	inline auto max(T1&& n1, T2&& n2)
	{
		return n1 > n2 ? n1 : n2;
	}

	template<typename T1, typename T2 = int>
	inline auto cutl(T1&& val, T2&& limit = 0)
	{
		return val < limit ? limit : val;
	}

	template<typename T1, typename T2 = int>
	inline auto cutg(T1&& val, T2&& limit = 0)
	{
		return val > limit ? limit : val;
	}

	template<typename T1, typename T2, typename T3>
	inline auto cutr(T1&& val, T2&& a, T3&& b)
	{
		return val < a ? a : val > b ? b : val;
	}

	template <typename T>
	inline enable_if<std::is_signed<T>::value,
		int> sgn(T val)
	{
		return (T(0) < val) - (val < T(0));
	}

	template <typename T>
	inline enable_if<std::is_unsigned<T>::value,
		int> sgn(T val)
	{
		return bool(val);
	}

	template<typename T>
	inline enable_if<std::is_floating_point<T>::value,
		T> mant(T val)
	{
		return val-int(val);
	}

	template<typename T, typename = enable_if<true>>
	inline enable_if<std::is_integral<T>::value,
		double> mant(T val)
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
	
	template<typename T>
	inline enable_if<std::is_floating_point<T>::value,
		T> is_zero(T val)
	{
		return val > -0.00000001 && val < 0.00000001;
	}

	template<typename T, typename = enable_if<true>>
	inline enable_if<std::is_integral<T>::value,
		double> is_zero(T val)
	{
		return val==0;
	}

	template<typename T>
	inline enable_if<std::is_floating_point<T>::value,
		T> not_zero(T val)
	{
		return val < -0.00000001 || val > 0.00000001;
	}

	template<typename T, typename = enable_if<true>>
	inline enable_if<std::is_integral<T>::value,
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
	__forceinline auto pow2(T value)
	{
		return value*value;
	}

	template<typename T>
	__forceinline auto pow3(T value)
	{
		return value*value*value;
	}

	template<typename T>
	__forceinline bool is_nan(T value)
	{
		return value!=value;
	}

	template<typename T>
	__forceinline auto to_deg(T value)
	{
		return value/PI*180.0;
	}

	template<typename T>
	__forceinline auto to_rad(T value)
	{
		return value*PI/180.0;
	}
}

#undef enable_if
#endif //SIMPLE_H
