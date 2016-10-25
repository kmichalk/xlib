#ifndef _RANGE_H_
#define _RANGE_H_

#include <iostream>
#include "simple.h"

namespace x
{
	template<typename _Type>
	class range
	{
		_Type a_;
		_Type b_;
		_Type interval_;

	public:
		struct iterator
		{
			range<_Type> const& object;
			size_t pos;

			
			iterator(
				range<_Type> const& object,
				size_t				pos	= 0)
				: 
				object	{object},
				pos		{pos}
			{
			}

			iterator(
				iterator const& other)
				:
				object	{other.object}, 
				pos		{other.pos}
			{
			}

			iterator(
				iterator&& other)
				:
				object	{other.object},
				pos		{other.pos}
			{
			}


			__forceinline iterator& operator++()
			{
				++pos;
				return *this;
			}

			__forceinline iterator& operator--()
			{
				--pos;
				return *this;
			}

			inline iterator operator++(int)
			{
				return iterator{object, pos++};
			}

			inline iterator operator--(int)
			{
				return iterator{object, pos--};
			}

			template<_concept<std::is_floating_point<_Type>::value>>
			__forceinline iterator operator+(_Type addv)
			{
				return iterator(object, pos + addv/object.interval_);
			}

			template<_concept<std::is_floating_point<_Type>::value>>
			__forceinline iterator operator-(_Type subv)
			{
				return iterator(object, pos - subv/object.interval_);
			}

			template<_concept<std::is_floating_point<_Type>::value>>
			__forceinline iterator& operator+=(_Type addv)
			{
				pos += addv/object.interval_; 
				return *this;
			}
			
			template<_concept<std::is_floating_point<_Type>::value>>
			__forceinline iterator& operator-=(_Type subv)
			{
				pos -= subv/object.interval_; 
				return *this;
			}
			
			__forceinline iterator operator+(size_t n) const
			{
				return iterator(object, pos + n);
			}
			
			__forceinline iterator operator-(size_t n) const
			{
				return iterator(object, pos - n);
			}
			
			__forceinline iterator& operator+=(size_t n)
			{
				pos += n;
				return *this;
			}
			
			__forceinline iterator& operator-=(size_t n)
			{
				pos -= n;
				return *this;
			}
			
			__forceinline _Type operator*() const
			{
				return object.a_ + object.interval_*pos;
			}
			
			__forceinline bool operator==(iterator const& other) const
			{
				return pos == other.pos;
			}
			
			__forceinline bool operator!=(iterator const& other) const
			{
				return pos != other.pos;
			}
			
			__forceinline bool operator<(iterator const& other) const
			{
				return pos < other.pos;
			}
			
			__forceinline bool operator>(iterator const& other) const
			{
				return pos > other.pos;
			}
			
			__forceinline bool operator<=(iterator const& other) const
			{
				return pos <= other.pos;
			}
			
			__forceinline bool operator>=(iterator const& other) const
			{
				return pos >= other.pos;
			}
			
			__forceinline iterator& operator=(iterator const& other)
			{
				pos = other.pos; 
				return *this;
			}
			
			__forceinline operator bool() const
			{
				return pos >= object.a_ && pos <= object.b_;
			}

			__forceinline operator _Type() const
			{
				return object.a_ + object.interval_*pos;;
			}
		};

		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using value_type = _Type;
		using pointer = _Type*;
		using reference = _Type&;

		range()
			:
			a_			{0},
			b_			{1},
			interval_	{1}
		{
		}

		range(
			_Type a, 
			_Type b,
			_Type interval = 1)
			:
			a_			{min(a,b)}, 
			b_			{max(a,b)},
			interval_	{interval}
		{
		}

		range(
			range<_Type> const& other)
			:
			a_			{other.a_}, 
			b_			{other.b_},
			interval_	{other.interval_}
		{
		}

		range(
			range<_Type>&& other)
			:
			a_			{std::move(other.a_)},
			b_			{std::move(other.b_)},
			interval_	{std::move(other.interval_)}
		{
		}

		range<_Type>& operator=(range<_Type> const& other)
		{
			a_			= other.a_;
			b_			= other.b_;
			interval_	= other.interval_;
		}

		range<_Type>& operator=(range<_Type> && other)
		{
			a_			= std::move(other.a_);
			b_			= std::move(other.b_);
			interval_	= std::move(other.interval_);
		}

		template<typename _Other>
		bool contains(_Other const& value) const
		{
			return (value >= a_) && (value <= b_);
		}

		bool contains(iterator const& it) const
		{
			return (it.value >= a_) && (it.value <= b_);
		}

		range<_Type> operator+(range<_Type> const& other) const
		{
			return range<_Type>{x::min(a_, other.a_), x::max(b_, other.b_)};
		}

		range<_Type>& operator+=(range<_Type> const& other)
		{
			a_ = min(a_, other.a_);
			b_ = max(b_, other.b_);
			return *this;
		}

		range<_Type>& set(_Type a, _Type b, _Type interval = 1)
		{
			a_			= min(a, b);
			b_			= max(a, b);
			interval_	= interval;
			return *this;
		}

		inline range<_Type>& setInterval(_Type value)
		{
			interval_ = value;
			return *this;
		}

		inline range<_Type>& divide(_Type value)
		{
			interval_ = (b_ - a_) / value;
			return *this;
		}

		_Type length() const
		{
			return b_ - a_;
		}

		inline constexpr _Type a() const
		{
			return a_;
		}

		inline constexpr _Type b() const
		{
			return b_;
		}

		inline iterator begin() const
		{
			return iterator{*this, 0};
		}

		inline iterator end() const
		{
			return iterator{*this, size_t((b_-a_)/interval_) + 1}; 
		}

		~range()
		{
		}

		friend std::ostream& operator<<(std::ostream& os, range<_Type> const& r)
		{
			return os<<'['<<r.a_<<','<<r.b_<<']';
		}
	};

	namespace ct
	{
		template<typename _Type, _Type a, _Type b>
		constexpr range<_Type> crange{a, b};
	}


	template<class... _Types>
	using MinPrecision = std::conditional_t <
		x::any_true<std::is_floating_point<_Types>::value...>::value,
		double,
		int>;

	template<class _A, class _B = _A, class _Intv = _A>
	__forceinline range<MinPrecision<_A, _B, _Intv>> _range(_A a, _B b, _Intv interval)
	{
		return{MinPrecision<_A, _B, _Intv>(a),MinPrecision<_A, _B, _Intv>(b),MinPrecision<_A, _B, _Intv>(interval)};
	}
}

#endif //_RANGE_H_
