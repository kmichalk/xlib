#pragma once

#include "more_type_traits.h"
#include <cmath>

#define conditional typename std::conditional_t
#define is_real_type(type) std::is_floating_point<type>::value
#define is_signed(type) !std::is_unsigned<type>::value
#define PI 3.14159265359

template<typename T = double>
class coord
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

	static const coord<T> ZERO;

	coord() :
		x{0}, y{0}
	{}
	coord(T x, T y) :
		x{x}, y{y}
	{}
	coord(const coord<T>& other) :
		x{other.x}, y{other.y}
	{}

	T len() const
	{
		return sqrt(x*x+y*y);
	}

	T dist(const coord<T>& other) const
	{
		return (other-(*this)).len();
	}

	void operator=(const coord<T>& other)
	{
		x = other.x;
		y = other.y;
	}
	template<typename Y>
	coord<new_type_<Y>> operator+(const coord<Y>& other) const
	{
		return coord<new_type_<Y>>(x+other.x, y+other.y);
	}
	template<typename Y>
	coord<new_type_<Y>> operator-(const coord<Y>& other) const
	{
		return coord<new_type_<Y>>(x-other.x, y-other.y);
	}
	template<typename Y>
	coord<new_type_<Y>> operator*(Y value) const
	{
		return coord<new_type_<Y>>(x*value, y*value);
	}
	template<typename Y>
	new_type_<Y> operator*(const coord<Y>& other) const
	{
		return x*other.x+y*other.y;
	}
	template<typename Y>
	coord<new_type_<Y>> operator/(Y value) const
	{
		return coord<new_type_<Y>>(x/value, y/value);
	}
	template<typename Y>
	void operator+=(const coord<Y>& other)
	{
		x += other.x;
		y += other.y;
	}
	template<typename Y>
	void operator-=(const coord<Y>& other)
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
	coord<T> operator-() const
	{
		return coord<T>(-x, -y);
	}
	coord<T> dir() const
	{
		T l = len();
		return coord<T>(x/l, y/l);
	}
	coord<T> normal() const
	{
		T l = len();
		return coord<T>(-y/l, x/l);
	}
	T angle() const
	{
		T ang = atan2(y, x);
		if (ang < 0) return ang+(ang < 0)*2*PI;

	}
	inline void zero()
	{
		x = 0; 
		y = 0;
	}

};

template<typename T>
const coord<T> coord<T>::ZERO = {0,0};

#undef conditional
#undef is_real_type
#undef is_signed