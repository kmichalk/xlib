#ifndef CRD_H
#define CRD_H

#include "more_type_traits.h"
#include "simple.h"
#include <ostream>

#define conditional typename std::conditional_t
#define is_real_type(type) std::is_floating_point<type>::value
#define is_signed(type) !std::is_unsigned<type>::value

template<typename _Type = double>
class crd: public x::triv_cpy
{
	template<typename _Other>
	using is_real_type_ = std::is_floating_point<_Other>;

	template<typename _Other>
	using is_integral_ = std::is_integral<_Other>;

	template<typename _Other>
	using new_type_ =
		typename std::conditional_t<
		is_real_type_<_Type>::value ||
		(is_integral_<_Type>::value && is_integral_<_Other>::value),
		_Type, _Other>;

	template<typename _Other>
	using new_type =
		conditional<is_real_type(_Type),
		_Type, conditional<is_real_type(_Other),
			_Other, conditional<is_signed(_Type),
				_Type, conditional<is_signed(_Other),
					_Other, _Type>
				>
			>
		>;


	static_assert(std::is_fundamental<_Type>::value,
		"coord can only operate on fundamental, arithmetic types.");
public:
	_Type x, y;

	static const crd<_Type> ZERO;

	constexpr crd() :
		x{0}, y{0}
	{
	}

	constexpr crd(_Type x, _Type y) :
		x{x}, y{y}
	{
	}

	constexpr crd(const crd<_Type>& other) :
		x{other.x}, y{other.y}
	{
	}

	static crd<_Type> versor(_Type angle)
	{
		return{cos(angle), sin(angle)};
	}

	_Type len() const
	{
		return sqrt(x*x+y*y);
	}

	_Type len2() const
	{
		return x*x+y*y;
	}

	_Type dist(const crd<_Type>& other) const
	{
		return (other-(*this)).len();
	}

	void operator=(const crd<_Type>& other)
	{
		x = other.x;
		y = other.y;
	}
	template<typename _Other>
	crd<new_type_<_Other>> operator+(const crd<_Other>& other) const
	{
		return crd<new_type_<_Other>>(x+other.x, y+other.y);
	}
	template<typename _Other>
	crd<new_type_<_Other>> operator-(const crd<_Other>& other) const
	{
		return crd<new_type_<_Other>>(x-other.x, y-other.y);
	}
	template<typename _Other>
	crd<new_type_<_Other>> operator*(_Other value) const
	{
		return crd<new_type_<_Other>>(x*value, y*value);
	}
	template<typename _Other>
	new_type_<_Other> operator*(const crd<_Other>& other) const
	{
		return x*other.x+y*other.y;
	}
	template<typename _Other>
	crd<new_type_<_Other>> operator/(_Other value) const
	{
		return crd<new_type_<_Other>>(x/value, y/value);
	}
	template<typename _Other>
	void operator+=(const crd<_Other>& other)
	{
		x += other.x;
		y += other.y;
	}
	template<typename _Other>
	void operator-=(const crd<_Other>& other)
	{
		x -= other.x;
		y -= other.y;
	}
	template<typename _Other>
	void operator*=(_Other value)
	{
		x *= value;
		y *= value;
	}
	template<typename _Other>
	void operator/=(_Other value)
	{
		x /= value;
		y /= value;
	}
	crd<_Type> operator-() const
	{
		return crd<_Type>(-x, -y);
	}
	template<typename _Other>
	bool operator==(crd<_Other> const& other) const
	{
		return x==other.x && y==other.y;
	}
	crd<_Type> dir() const
	{
		_Type l = len();
		return crd<_Type>(x/l, y/l);
	}
	crd<_Type> normal() const
	{
		_Type l = len();
		return crd<_Type>(-y/l, x/l);
	}
	crd<_Type>& normalize()
	{
		operator/=(len());
		return *this;
	}
	crd<_Type>& setLen(_Type l)
	{
		operator/=(len()/l);
		return *this;
	}
	_Type angle() const
	{
		_Type ang = atan2(y, x);
		return ang+(ang < 0)*2*PI;
	}

	crd<_Type>& rot90p()
	{
		_Type temp = x;
		x = -y;
		y = temp;
		return *this;
	}

	crd<_Type>& rot90n()
	{
		_Type temp = -x;
		x = y;
		y = temp;
		return *this;
	}

	crd<_Type>& reverse()
	{
		x = -x;
		y = -y;
		return *this;
	}

	crd<_Type> inverse() const
	{
		return (*this)/len2();
	}

	_Type crossVal(crd<_Type> const& other) const
	{
		return x*other.y - y*other.x;
	}

	crd<_Type> crossVal(_Type z) const
	{
		return{y*z,-x*z};
	}

	inline void zero()
	{
		x = 0; 
		y = 0;
	}

	friend std::ostream& operator<<(std::ostream& os, crd<_Type> const& c)
	{
		return os<<'('<<c.x<<','<<c.y<<')';
	}
};

template<typename _Type1, typename _Type2>
inline auto crossVal(crd<_Type1> const& c1, crd<_Type2> const& c2)
{
	return c1.x*c2.y - c1.y*c2.x;
}

template<typename _Type>
inline crd<_Type> multCrd(crd<_Type> const& c1, crd<_Type> const& c2)
{
	return {c1.x*c2.x, c1.y*c2.y};
}

template<typename _Type1, typename _Type2>
inline auto dot(crd<_Type1> const& c1, crd<_Type2> const& c2)
{
	return c1.x*c2.x + c1.y*c2.y;
}

template<typename _Type>
__forceinline auto hypot(crd<_Type> const& c)
{
	return hypot(c.x, c.y);
}

template<typename _Type>
crd<_Type> project(crd<_Type> const& v, crd<_Type> const& on)
{
	_Type dotParam = (v*on)/on.len2();
	return crd<_Type>{dotParam*on.x, dotParam*on.y};
}

template<typename _Type>
__forceinline crd<_Type> abs(crd<_Type> const& c)
{
	return{abs(c.x),abs(c.y)};
}

template<typename _Type1, typename _Type2>
inline auto triangleField(crd<_Type1> const& c1, crd<_Type2> const& c2)
{
	return abs(crossVal(c1, c2)) / 2.0;
}

template<typename _Type1, typename _Type2>
inline auto mean(crd<_Type1> const& c1, crd<_Type2> const& c2)
{
	return (c2-c1)/2.0 + c1;
}

namespace x
{
	template<class _Type>
	inline crd<_Type> crdmin(crd<_Type> const& c1, crd<_Type> const& c2) noexcept
	{
		return{x::min(c1.x,c2.x), x::min(c1.y,c2.y)};
	}

	template<class _Type>
	inline crd<_Type> crdmax(crd<_Type> const& c1, crd<_Type> const& c2) noexcept
	{
		return{x::max(c1.x,c2.x), x::max(c1.y,c2.y)};
	}
}

template<typename _Type>
const crd<_Type> crd<_Type>::ZERO = {0,0};

#undef conditional
#undef is_real_type
#undef is_signed

#endif