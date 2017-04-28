#ifndef AUTOLIM_H
#define AUTOLIM_H

#include <limits>
#include <type_traits>

namespace x
{
	/// <summary>
	/// Class to be used in various situations when minimum of some values 
	/// must be calculated. Automates the process of finding minimum
	/// by remembering the lowest value found.
	/// </summary>
	/// <typeparam name="_Type">Type of the value.</typeparam>
	template<typename _Type>
	class Min
	{
		mutable _Type value_;

	public:
		/// <summary>
		/// Default constructor.
		/// Sets value to the lowest possible.
		/// </summary>
		inline constexpr Min() noexcept :
			value_{std::numeric_limits<_Type>::max()}
		{
		}


		/// <summary>
		/// Compares with currently stored minimum value and assignes if it is lower.
		/// </summary>
		/// <param name="val">Value to be processed.</param>
		/// <returns>True if new minimum was found, otherwise false.</returns>
		__forceinline bool operator()(const _Type& val) noexcept
		{
			return (val < value_ ? (value_ = val, true) : false);
		}


		/// <summary>
		/// Gets current value.
		/// </summary>
		/// <returns>Value passed by copy.</returns>
		__forceinline _Type operator()() const noexcept
		{
			return value_;
		}
	};
	
	///////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// Class to be used in various situations when maximum of some values 
	/// must be calculated. Automates the process of finding maximum
	/// by remembering the highest value found.
	/// </summary>
	/// <typeparam name="_Type">Type of the value.</typeparam>
	template<typename _Type>
	class Max	
	{
		mutable _Type value_;

	public:
		/// <summary>
		/// Default constructor.
		/// Sets value to the highest possible.
		/// </summary>
		inline constexpr Max() noexcept:
			value_{std::numeric_limits<_Type>::lowest()}
		{
		}


		/// <summary>
		/// Compares with currently stored maximum value and assignes if it is higher.
		/// </summary>
		/// <param name="val">Value to be processed.</param>
		/// <returns>True if new maximum was found, otherwise false.</returns>
		__forceinline bool operator()(const _Type& val) noexcept
		{
			return (val > value_ ? (value_ = val, true) : false);
		}


		/// <summary>
		/// Gets current value.
		/// </summary>
		/// <returns>Value passed by copy.</returns>
		__forceinline _Type operator()() const noexcept
		{
			return value_;
		}
	};

	///////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// Simplifies iteration _num times.
	/// </summary>
	/// <param name="_num">Number to be counted to.</param>
	template<unsigned _num>
	class Counter
	{
		unsigned value_;

	public:
		/// <summary>
		/// Default constructor.
		/// Sets current value to 0.
		/// </summary>
		inline constexpr Counter() noexcept :
			value_{0}
		{
		}


		/// <summary>
		/// Increments counter and check whether specified _num was reached.
		/// </summary>
		/// <returns>True if counter reached _num, false otherwise.</returns>
		__forceinline bool operator++() noexcept
		{
			return ++value_ == _num;
		}


		/// <summary>
		/// Allows to use Counter as if it was simple unsigned value.
		/// </summary>
		/// <returns>Current counter value.</returns>
		__forceinline operator unsigned() const noexcept
		{
			return value_;
		}
	};
}

#endif
