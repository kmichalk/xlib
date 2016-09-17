#ifndef AUTOLIM_H
#define AUTOLIM_H

#include <limits>
#include <type_traits>

namespace x
{
	template<typename T>
	class Min
	{
		mutable T value_;
	public:
		inline constexpr Min():
			value_{std::numeric_limits<T>::max()}
		{
		}

		__forceinline bool operator()(const T val)
		{
			return (val < value_ ? (value_ = val, true) : false);
		}

		__forceinline T operator()() const
		{
			return value_;
		}
	};

	template<typename T>
	class Max
	{
		mutable T value_;
	public:
		inline constexpr Max():
			value_{std::numeric_limits<T>::lowest()}
		{
		}

		__forceinline bool operator()(const T val)
		{
			return (val > value_ ? (value_ = val, true) : false);
		}

		__forceinline T operator()() const
		{
			return value_;
		}
	};

	template<unsigned N>
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
			return ++value_ == N;
		}

		__forceinline unsigned operator()() const
		{
			return value_;
		}
	};
}

#endif
