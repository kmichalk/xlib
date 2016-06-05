#ifndef RANGE_H
#define RANGE_H

namespace x
{

template<typename T1, typename T2>
struct two
{
	T1 a;
	T2 b;
	two(T1 const& a, T2 const& b):
		a{a}, b{b}
	{}
};

template<typename T>
class range: public two<T, T>
{
public:
	using two<T, T>::two;

	template<typename Y>
	inline bool contains(Y const& value) const
	{
		return (value>=a) & (value<=b);
	}
	inline void set(T const& a, T const& b)
	{
		this->a = a;
		this->b = b;
	}
};

}

#endif //RANGE_H
