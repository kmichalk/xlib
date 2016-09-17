#ifndef CRD_H
#define CRD_H

#include "more_type_traits.h"
#include "simple.h"
#include <ostream>

#define conditional typename std::conditional_t
#define is_real_type(type) std::is_floating_point<type>::value
#define is_signed(type) !std::is_unsigned<type>::value

template<typename T = double>
class crd: public x::triv_cpy
{
	template<typename Y>
	using is_real_type_ = std::is_floating_point<Y>;

	template<typename Y>
	using is_integral_ = std::is_integral<Y>;

	template<typename Y>
	using new_type_ =
		typename std::conditional_t<
		is_real_type_<T>::value ||
		(is_integral_<T>::value && is_integral_<Y>::value),
		T, Y>;

	template<typename Y>
	using new_type =
		conditional<is_real_type(T),
		T, conditional<is_real_type(Y),
			Y, conditional<is_signed(T),
				T, conditional<is_signed(Y),
					Y, T>
				>
			>
		>;


	static_assert(std::is_fundamental<T>::value,
		"coord can only operate on fundamental, arithmetic types.");
public:
	T x, y;

	static const crd<T> ZERO;

	constexpr crd() :
		x{0}, y{0}
	{
	}

	constexpr crd(T x, T y) :
		x{x}, y{y}
	{
	}

	constexpr crd(const crd<T>& other) :
		x{other.x}, y{other.y}
	{
	}

	static crd<T> versor(T angle)
	{
		return{cos(angle), sin(angle)};
	}

	T len() const
	{
		return sqrt(x*x+y*y);
	}

	T len2() const
	{
		return x*x+y*y;
	}

	T dist(const crd<T>& other) const
	{
		return (other-(*this)).len();
	}

	void operator=(const crd<T>& other)
	{
		x = other.x;
		y = other.y;
	}
	template<typename Y>
	crd<new_type_<Y>> operator+(const crd<Y>& other) const
	{
		return crd<new_type_<Y>>(x+other.x, y+other.y);
	}
	template<typename Y>
	crd<new_type_<Y>> operator-(const crd<Y>& other) const
	{
		return crd<new_type_<Y>>(x-other.x, y-other.y);
	}
	template<typename Y>
	crd<new_type_<Y>> operator*(Y value) const
	{
		return crd<new_type_<Y>>(x*value, y*value);
	}
	template<typename Y>
	new_type_<Y> operator*(const crd<Y>& other) const
	{
		return x*other.x+y*other.y;
	}
	template<typename Y>
	crd<new_type_<Y>> operator/(Y value) const
	{
		return crd<new_type_<Y>>(x/value, y/value);
	}
	template<typename Y>
	void operator+=(const crd<Y>& other)
	{
		x += other.x;
		y += other.y;
	}
	template<typename Y>
	void operator-=(const crd<Y>& other)
	{
		x -= other.x;
		y -= other.y;
	}
	template<typename Y>
	void operator*=(Y value)
	{
		x *= value;
		y *= value;
	}
	template<typename Y>
	void operator/=(Y value)
	{
		x /= value;
		y /= value;
	}
	crd<T> operator-() const
	{
		return crd<T>(-x, -y);
	}
	template<typename Y>
	bool operator==(crd<Y> const& other) const
	{
		return x==other.x && y==other.y;
	}
	crd<T> dir() const
	{
		T l = len();
		return crd<T>(x/l, y/l);
	}
	crd<T> normal() const
	{
		T l = len();
		return crd<T>(-y/l, x/l);
	}
	crd<T>& normalize()
	{
		operator/=(len());
		return *this;
	}
	crd<T>& setLen(T l)
	{
		operator/=(len()/l);
		return *this;
	}
	T angle() const
	{
		T ang = atan2(y, x);
		return ang+(ang < 0)*2*PI;
	}

	crd<T>& rot90p()
	{
		T temp = x;
		x = -y;
		y = temp;
		return *this;
	}

	crd<T>& rot90n()
	{
		T temp = -x;
		x = y;
		y = temp;
		return *this;
	}

	crd<T>& reverse()
	{
		x = -x;
		y = -y;
		return *this;
	}

	crd<T> inverse() const
	{
		return (*this)/len2();
	}

	T crossVal(crd<T> const& other) const
	{
		return x*other.y - y*other.x;
	}

	crd<T> crossVal(T z) const
	{
		return{y*z,-x*z};
	}

	inline void zero()
	{
		x = 0; 
		y = 0;
	}

	friend std::ostream& operator<<(std::ostream& os, crd<T> const& c)
	{
		return os<<'('<<c.x<<','<<c.y<<')';
	}
};

template<typename T, typename Y>
inline auto crossVal(crd<T> const& c1, crd<Y> const& c2)
{
	return c1.x*c2.y - c1.y*c2.x;
}

template<typename T, typename Y>
inline auto multCrd(crd<T> const& c1, crd<Y> const& c2)
{
	return crd<decltype(T*Y)>{c1.x*c2.x, c1.y*c2.y};
}

template<typename T, typename Y>
inline auto dot(crd<T> const& c1, crd<Y> const& c2)
{
	return c1.x*c2.x + c1.y*c2.y;
}

template<typename T>
__forceinline auto hypot(crd<T> const& c)
{
	return hypot(c.x, c.y);
}

template<typename T>
crd<T> project(crd<T> const& v, crd<T> const& on)
{
	T dotParam = (v*on)/on.len2();
	return crd<T>{dotParam*on.x, dotParam*on.y};
}

template<typename T>
__forceinline crd<T> abs(crd<T> const& c)
{
	return{abs(c.x),abs(c.y)};
}

template<typename T, typename Y>
inline auto triangleField(crd<T> const& c1, crd<Y> const& c2)
{
	return abs(crossVal(c1, c2)) / 2.0;
}

template<typename T, typename Y>
inline auto mean(crd<T> const& c1, crd<Y> const& c2)
{
	return (c2-c1)/2.0 + c1;
}

template<typename T>
const crd<T> crd<T>::ZERO = {0,0};

#undef conditional
#undef is_real_type
#undef is_signed

#endif