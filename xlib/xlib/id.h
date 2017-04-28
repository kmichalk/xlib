#ifndef _ID_H_
#define _ID_H_

#define _HEADER_ONLY template<class = void>

namespace x
{
	template<class = void> 
	class _LastID
	{
		static long long unsigned lastID_;

	public:
		static inline long long unsigned next()
		{
			return ++lastID_;
		}
	};

	template<> long long unsigned _LastID<>::lastID_ = 0;
	template class _LastID<>;


	/// <summary>
	/// Generates unique id value for every instance for various purposes.
	/// </summary>
	class id
	{
		long long unsigned const value_;

	public:
		/// <summary>
		/// Default constructor.
		/// Initializes id with the next unused value.
		/// </summary>
		inline id():
			value_{_LastID<>::next()}
		{
		}


		/// <summary>
		/// Copy constructor.
		/// Initializes id with the next unused value.
		/// Does not copy id value of other.
		/// </summary>
		/// <param name="other"> - copied id object</param>
		inline id(id const& other):
			value_{_LastID<>::next()}
		{
		}

		
		/// <summary>
		/// Move constructor.
		/// Initializes id with value from other.
		/// </summary>
		/// <param name="other"> - moved id object.</param>
		inline id(id&& other):
			value_{other.value_}
		{
		}

		
		/// <summary>
		/// Does nothing since id of every object must be unique.
		/// </summary>
		/// <param name="other"> - assigned id object</param>
		/// <returns>reference to the caller</returns>
		__forceinline id& operator=(id const& other)
		{
			//const_cast<unsigned long long&>(value_) = _LastID<>::next();
			return *this;
		}

		
		/// <summary>
		/// Sets id to value of other.
		/// </summary>
		/// <param name="other"> - move-assigned id object</param>
		/// <returns>reference to the caller.</returns>
		__forceinline id& operator=(id&& other)
		{
			const_cast<unsigned long long&>(value_) = other.value_;
		}


		/// <summary>
		/// Allows to use id as if it was regular long long unigned value.
		/// </summary>
		/// <returns>id value.</returns>
		__forceinline operator long long unsigned() const
		{
			return value_;
		}


		/// <summary>
		/// Compares with other id
		/// </summary>
		/// <param name="other"> - id to be compared with</param>
		/// <returns>true if values are equal, otherwise false.</returns>
		__forceinline bool operator==(id const& other) const
		{
			return value_ == other.value_;
		}


		/// <summary>
		/// Compares with other id
		/// </summary>
		/// <param name="other"> - id to be compared with</param>
		/// <returns>true if values are not equal, otherwise false.</returns>
		__forceinline bool operator!=(id const& other) const
		{
			return value_ != other.value_;
		}

		__forceinline bool operator>=(id const& other) const
		{
			return value_ >= other.value_;
		}

		__forceinline bool operator<=(id const& other) const
		{
			return value_ <= other.value_;
		}

		__forceinline bool operator>(id const& other) const
		{
			return value_ > other.value_;
		}

		__forceinline bool operator<(id const& other) const
		{
			return value_ < other.value_;
		}
	};
}

#undef _HEADER_ONLY

#endif //_ID_H_
