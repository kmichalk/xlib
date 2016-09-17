#ifndef RANGE_H
#define RANGE_H

#include <iostream>
#include "simple.h"

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
			return a==other.a && b==other.b;
		}
	};

	template<typename T>
	class range
	{
		T a_, b_;

	public:
		range()
		{
		}

		inline constexpr range(T a, T b):
			a_{min(a,b)}, b_{max(a,b)}
		{
		}

		range(range<T> const& other):
			a_{other.a_}, b_{other.b_}
		{
		}

		range<T>& operator=(range<T> const& other)
		{
			a_ = other.a_;
			b_ = other.b_;
		}

		template<typename Y>
		bool contains(Y const& value) const
		{
			return (value>=a_) && (value<=b_);
		}

		range<T> operator+(range<T> const& other) const
		{
			return range<T>{x::min(a_, other.a_), x::max(b_, other.b_)};
		}

		range<T>& operator+=(range<T> const& other)
		{
			a_ = min(a_, other.a_);
			b_ = max(b_, other.b_);
			return *this;
		}

		void set(T a, T b)
		{
			a_ = min(a, b);
			b_ = max(a, b);
		}

		T length() const
		{
			return b_ - a_;
		}

		inline constexpr T a() const
		{
			return a_;
		}

		inline constexpr T b() const
		{
			return b_;
		}

		~range()
		{
		}

		friend std::ostream& operator<<(std::ostream& os, range<T> const& r)
		{
			return os<<'['<<r.a_<<','<<r.b_<<']';
		}
	};

	namespace ct
	{
		template<typename T, T a, T b>
		constexpr range<T> crange{a, b};
	}

}

#endif //RANGE_H
