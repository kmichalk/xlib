#ifndef AUTOLIM_H
#define AUTOLIM_H

#include <limits>

namespace x
{
	template<typename T>
	class Min
	{
		T value_;
	public:
		inline Min():
			value_{std::numeric_limits<T>::max()}
		{
		}

		__forceinline void check(T val)
		{
			if (val < value_) value_ = val;
		}

		__forceinline T operator()() const
		{
			return value_;
		}
	};

	template<typename T>
	class Max
	{
		T value_;
	public:
		inline Max():
			value_{std::numeric_limits<T>::lowest()}
		{
		}

		__forceinline void check(T val)
		{
			if (val > value_) value_ = val;
		}

		__forceinline T operator()() const
		{
			return value_;
		}
	};
}

#endif
