#ifndef SIMPLE_H
#define SIMPLE_H

#include <cmath>

#undef min
#undef max

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
inline auto cutl(T1&& n, T2&& limit = 0)
{
	return n < limit ? limit : n;
}
template<typename T1, typename T2 = int>
inline auto cutle(T1&& n, T2&& limit = 0)
{
	return n <= limit ? limit : n;
}
template<typename T1, typename T2 = int>
inline auto cutg(T1&& n, T2&& limit = 0)
{
	return n > limit ? limit : n;
}
template<typename T1, typename T2 = int>
inline auto cutge(T1&& n, T2&& limit = 0)
{
	return n >= limit ? limit : n;
}

}
#endif //SIMPLE_H
