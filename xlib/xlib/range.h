#ifndef _RANGE_H_
#define _RANGE_H_

#include <iostream>
#include "simple.h"

namespace x
{
	/// <summary>
	/// Class representing range containing \a a and \a b values representing 
	/// begin end end of the range as well as interval value.
	/// </summary>
	/// <typeparam name="_Type"> - type of values</typeparam>
	/// <remarks> 
	/// Purposes to use x::range can be iteration through set of values from a to b
	/// incrementing or decrementing by specified interval. It can also be used for 
	/// simplified checking whether some value lays between a and b.
	/// It asserts begin to be always lower than or equal to end.
	/// This class is iterator-friendly, so it can be used in shortened for loop syntax.
	/// </remarks>
	/// <example><code>bool in_range(x::vector<int> const& iv, x::range<int> const& r) {
	///		foreach(i, iv) { 
	///			if (!r.contains(*i)) return false;
	///		}
	///		return true;
	/// }</code></example>
	/// <example><code>x::vector<x::crd<double>> plotPoints;
	/// for (auto d : x::range<double>{-1,1,0.01}) {
	///		plotPoints.push_back({d, func(d)});
	/// }</code></example>
	template<typename _Type>
	class range
	{
		_Type a_;
		_Type b_;
		_Type interval_;

	public:
		/// <summary>
		/// Range iterator automates iteration through all values from begin value
		/// to end value inclusive with density specified by interval value.
		/// It does not modify x::range to which it is assigned so there is
		/// no need for x::range to have also const_iterator.
		/// </summary>
		struct iterator
		{
			/// <summary>
			/// Reference to x::range to which the iterator is assigned immutably.
			/// </summary>
			range<_Type> const& object;

			/// <summary>
			/// Current value of the iterator.
			/// </summary>
			_Type value;


			/// <summary>
			/// Basic constructor. Assigns iterator to a specific x::range object. 
			/// Initializes value to begin of \a object.
			/// </summary>
			/// <param name="object"> - object to be assigned to</param>
			iterator(
				range<_Type> const& object)
				:
				object{object},
				value{object.a_}
			{
			}

			/// <summary>
			/// Basic constructor. Assigns iterator to a specific x::range object. 
			/// Initializes value to specified.
			/// If the value exceeds specified range limits it is cut to fit the range.
			/// </summary>
			/// <param name="object"> - object to be assigned to</param>
			/// <param name="value"> - initial value</param>
			iterator(
				range<_Type> const& object,
				_Type value)
				:
				object{object},
				value{x::cutr(value, object.a_, object.b_)}
			{
			}


			/// <summary>
			/// Defaulted copy constructor. 
			/// Assigns created iterator to the same object.
			/// Initializes \a value to \a value of \a other.
			/// </summary>
			/// <param name="other"> - iterator to be copied</param>
			iterator(
				iterator const& other) = default;


			/// <summary>
			/// Defaulted move constructor.
			/// Assigns created iterator to the same object.
			/// Initializes \a value to moved \a value of \a other.
			/// </summary>
			/// <param name="other"></param>
			iterator(
				iterator&& other) = default;


			/// <summary>
			/// Adds \a object's \a interval to \a value. 
			/// </summary>
			/// <returns>reference to the caller</returns>
			__forceinline iterator& operator++()
			{
				value += object.interval_;
				return *this;
			}


			/// <summary>
			/// Subtracts \a object's \a interval to \a value. 
			/// </summary>
			/// <returns>reference to the caller</returns>
			__forceinline iterator& operator--()
			{
				value -= object.interval_;
				return *this;
			}


			/// <summary>
			/// Adds \a object's \a interval to \a value. 
			/// </summary>
			/// <returns>iterator with \a value from before modification</returns>
			inline iterator operator++(int)
			{
				iterator copy{*this};
				value += object.interval_;
				return copy;
			}


			/// <summary>
			/// Subtracts \a object's \a interval to \a value. 
			/// </summary>
			/// <returns>iterator with \a value from before modification</returns>
			inline iterator operator--(int)
			{
				iterator copy{*this};
				value -= object.interval_;
				return copy;
			}


			/// <summary>
			/// Adds \a addv to \a iterator's \a value.
			/// </summary>
			/// <returns>iterator with increased \a value</returns>
			__forceinline iterator operator+(_Type addv)
			{
				return iterator(object, value + addv);
			}


			/// <summary>
			/// Subtracts \a subv from \a iterator's \a value.
			/// </summary>
			/// <returns>iterator with decreased \a value</returns>
			__forceinline iterator operator-(_Type subv)
			{
				return iterator(object, value - subv);
			}


			/// <summary>
			/// Adds \a addv to \a iterator's \a value.
			/// </summary>
			/// <returns>reference to the caller with increased \a value</returns>
			__forceinline iterator& operator+=(_Type addv)
			{
				value += addv;
				return *this;
			}


			/// <summary>
			/// Subtracts \a subv from \a iterator's \a value.
			/// </summary>
			/// <returns>reference to the caller with decreased \a value</returns>
			__forceinline iterator& operator-=(_Type subv)
			{
				value -= subv;
				return *this;
			}


			/// <summary>
			/// Adds \a object's \a interval multiplied by \a n to \a iterator's \a value.
			/// </summary>
			/// <returns>iterator with increased \a value</returns>
			__forceinline iterator operator+(size_t n) const
			{
				return iterator(object, value + n*object.interval_);
			}


			/// <summary>
			/// Subtracts \a object's \a interval multiplied by \a n from \a iterator's \a value.
			/// </summary>
			/// <returns>iterator with decreased \a value</returns>
			__forceinline iterator operator-(size_t n) const
			{
				return iterator(object, value - n*object.interval_);
			}


			/// <summary>
			/// Adds \a object's \a interval multiplied by \a n to \a iterator's \a value.
			/// </summary>
			/// <returns>reference to the caller with increased \a value</returns>
			__forceinline iterator& operator+=(size_t n)
			{
				value += n*object.interval_;
				return *this;
			}


			/// <summary>
			/// Subtracts \a object's \a interval multiplied by \a n from \a iterator's \a value.
			/// </summary>
			/// <returns>reference to the caller with decreased \a value</returns>
			__forceinline iterator& operator-=(size_t n)
			{
				value -= n*object.interval_;
				return *this;
			}


			/// <summary>
			/// Overloaded dereference operator to stand by unified syntax for getting value 
			/// pointed by an iterator. It does not dereference any pointer so it is safe.
			/// </summary>
			/// <returns>current value of the iterator</returns>
			__forceinline _Type operator*() const
			{
				return value;
			}


			/// <summary>
			/// Compares two iterators in respect of \a value.
			/// It does not compare assigned ranges.
			/// </summary>
			/// <returns>true if values are equal, otherwise false</returns>
			__forceinline bool operator==(iterator const& other) const
			{
				return value == other.value;
			}


			/// <summary>
			/// Compares two iterators in respect of \a value.
			/// It does not compare assigned ranges.
			/// </summary>
			/// <returns>true if values are not equal, otherwise false</returns>
			__forceinline bool operator!=(iterator const& other) const
			{
				return value != other.value;
			}


			/// <summary>
			/// Compares two iterators in respect of \a value.
			/// It does not compare assigned ranges.
			/// </summary>
			/// <returns>true if caller's \a value is lower than \a other's, otherwise false</returns>
			__forceinline bool operator<(iterator const& other) const
			{
				return value < other.value;
			}


			/// <summary>
			/// Compares two iterators in respect of \a value.
			/// It does not compare assigned ranges.
			/// </summary>
			/// <returns>true if caller's \a value is greater than \a other's, otherwise false</returns>
			__forceinline bool operator>(iterator const& other) const
			{
				return value > other.value;
			}


			/// <summary>
			/// Compares two iterators in respect of \a value.
			/// It does not compare assigned ranges.
			/// </summary>
			/// <returns>true if caller's \a value is lower than or equal to \a other's, otherwise false</returns>
			__forceinline bool operator<=(iterator const& other) const
			{
				return value <= other.value;
			}


			/// <summary>
			/// Compares two iterators in respect of \a value.
			/// It does not compare assigned ranges.
			/// </summary>
			/// <returns>true if caller's \a value is greater than or equal to \a other's, otherwise false</returns>
			__forceinline bool operator>=(iterator const& other) const
			{
				return value >= other.value;
			}


			/// <summary>
			/// Assignes \a value of \a other iterator
			/// It does not change the assigned \a object.
			/// </summary>
			/// <returns>reference to the caller</returns>
			__forceinline iterator& operator=(iterator const& other)
			{
				value = other.value;
				return *this;
			}


			/// <summary>
			/// Checks whether \a assigned range includes \a value.
			/// </summary>
			/// <returns>true if \a assigned range includes \a value, otherwise false</returns>
			__forceinline operator bool() const
			{
				return value >= object.a_ && value <= object.b_;
			}


			/// <summary>
			/// Allows to use iterator as if it was regular value of specified \a _Type
			/// </summary>
			/// <
			/// <returns></returns>
			__forceinline operator _Type() const
			{
				return value;
			}
		};

		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using value_type = _Type;
		using pointer = _Type*;
		using reference = _Type&;

		 constexpr range()
			:
			a_			{0},
			b_			{1},
			interval_	{1}
		{
		}

		constexpr range(
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
			return iterator{*this, a_};
		}

		inline iterator end() const
		{
			return iterator{*this, b_ + interval_}; 
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
