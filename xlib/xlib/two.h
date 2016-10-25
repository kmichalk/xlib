#ifndef _TWO_H_
#define _TWO_H_

namespace x
{
	template<typename T1, typename T2 = T1>
	struct two
	{
		T1 a;
		T2 b;

		two()
		{
		}

		constexpr two(T1 const& a, T2 const& b):
			a{a}, b{b}
		{
		}

		two(two const& other):
			a{other.a},
			b{other.b}
		{
		}

		two& operator=(two const& other)
		{
			a = other.a;
			b = other.b;
		}

		bool operator==(two const& other) const
		{
			return a == other.a && b == other.b;
		}
	};
}
#endif //_TWO_H_
