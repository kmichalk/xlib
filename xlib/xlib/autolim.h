#ifndef AUTOLIM_H
#define AUTOLIM_H

#include <limits>
#include <type_traits>

namespace x
{
	template<typename _Type>
	class Min
	{
		mutable _Type value_;
	public:
		inline constexpr Min():
			value_{std::numeric_limits<_Type>::max()}
		{
		}

		__forceinline bool operator()(const _Type val)
		{
			return (val < value_ ? (value_ = val, true) : false);
		}

		__forceinline _Type operator()() const
		{
			return value_;
		}
	};

	template<typename _Type>
	class Max
	{
		mutable _Type value_;
	public:
		inline constexpr Max():
			value_{std::numeric_limits<_Type>::lowest()}
		{
		}

		__forceinline bool operator()(const _Type val)
		{
			return (val > value_ ? (value_ = val, true) : false);
		}

		__forceinline _Type operator()() const
		{
			return value_;
		}
	};

	template<unsigned _num>
	class Counter
	{
		unsigned value_;
	public:
		inline constexpr Counter():
			value_{0}
		{
		}

		inline constexpr Counter(unsigned initVal) :
			value_{initVal}
		{
		}

		__forceinline bool operator++()
		{
			return ++value_ == _num;
		}

		__forceinline unsigned operator()() const
		{
			return value_;
		}
	};
}

#endif
