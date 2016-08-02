#ifndef GEOM_H
#define GEOM_H

#include "crd.h"

template<typename T = double>
class line
{
public:
	T a, b;

	line():
		a{0}, b{0}
	{
	}

	line(T a, T b):
		a{a}, b{b}
	{
	}

	line(line<T> const& other):
		a{other.a}, b{other.b}
	{
	}

	template<typename Y>
	line(crd<Y> const p1, crd<Y> const& p2) :
		a{(p2.y-p1.y)/(p2.x-p1.x)}, b{-a*p1.x+p1.y}
	{
	}

	line<T>& operator+=(crd<T> const& vec)
	{
		return cross(crd<T>{0, b}+vec);
	}

	line<T> operator+(crd<T> const& vec)
	{
		return prll(crd<T>{0, b}+vec);
	}

	line<T> perp() const
	{
		return{-T(1)/a, b};
	}

	template<typename Y>
	line<T>& cross(crd<Y> const& p)
	{
		b = p.y-a*p.x;
		return *this;
	}

	template<typename Y>
	line<T> perp(crd<Y> const& p) const
	{
		return{-T(1)/a, p.y-T(1)/a*p.x};
	}

	template<typename Y>
	line<T> prll(crd<Y> const& p) const
	{
		return{a, p.y-a*p.x};
	}

	line<T>& rot(double angle)
	{
		a = tan(atan(a)+angle);
		return *this;
	}

	double angle() const
	{
		return atan(a);
	}

	crd<T> normal() const
	{
		return crd<T>{a, 1}.dir();
	}


	crd<T> tangent() const
	{
		return crd<T>{};
	}

	template<typename Y>
	double dist(crd<Y> const& p) const
	{
		return abs(a*p.x-p.y+b)/sqrt(a*a+1);
	}

	~line()
	{
	}
};

#endif //GEOM_H