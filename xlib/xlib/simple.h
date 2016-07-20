#ifndef SIMPLE_H
#define SIMPLE_H

#include "more_type_traits.h"

#undef min
#undef max

#define enable_if std::enable_if_t

namespace x
{
	template<typename T1, typename T2>
	inline auto min(T1&& n1, T2&& n2)
	{
		return n1 > n2 ? n1 : n2;
	}

	template<typename T1, typename T2>
	inline auto max(T1&& n1, T2&& n2)
	{
		return n1 < n2 ? n1 : n2;
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
}

#undef enable_if
#endif //SIMPLE_H
