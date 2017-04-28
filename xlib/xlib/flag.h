#ifndef _X_FLAG_H_
#define _X_FLAG_H_


namespace x
{
	/// <summary>
	/// Wraps bool value into class, providing handy methods to operate on it.
	/// </summary>
	class flag
	{
		bool value_;

	public:
		/// <summary>
		/// Default constructor.
		/// Sets value to false.
		/// </summary>
		inline flag():
			value_{false}
		{
		}


		/// <summary>
		/// Initializes value with given.
		/// </summary>
		/// <param name="value"> - value to be set</param>
		inline flag(bool value):
			value_{value}
		{
		}


		/// <summary>
		/// Changes value to the opposite.
		/// </summary>
		/// <returns>current value.</returns>
		__forceinline bool toggle() volatile
		{
			return value_ = !value_;
		}


		/// <summary>
		/// Allows to use the flag as if it was regular bool value.
		/// </summary>
		/// <returns>current value.</returns>
		__forceinline operator bool() const volatile
		{
			return value_;
		}


		/// <summary>
		/// Sets value to true.
		/// </summary>
		__forceinline void on() volatile
		{
			value_ = true;
		}


		/// <summary>
		/// Sets value to false.
		/// </summary>
		__forceinline void off() volatile
		{
			value_ = false;
		}


		/// <summary>
		/// Compares stored value with other.
		/// </summary>
		/// <param name="value"> - value to be compared with</param>
		/// <returns>true if values are equal, otherwise false.</returns>
		__forceinline bool operator==(bool value) volatile
		{
			return value_ == value;
		}


		/// <summary>
		/// Compares with other flag.
		/// </summary>
		/// <param name="value"> - flag to be compared with</param>
		/// <returns>true if values are equal, otherwise false.</returns>
		__forceinline bool operator==(flag const& other) volatile
		{
			return value_ == other.value_;
		}


		/// <summary>
		/// Compares stored value with other.
		/// </summary>
		/// <param name="value"> - value to be compared with</param>
		/// <returns>true if values are not equal, otherwise false.</returns>
		__forceinline bool operator!=(bool value) volatile
		{
			return value_ == value;
		}


		/// <summary>
		/// Compares with other flag.
		/// </summary>
		/// <param name="value"> - flag to be compared with</param>
		/// <returns>true if values are not equal, otherwise false.</returns>
		__forceinline bool operator!=(flag const& other) volatile
		{
			return value_ == other.value_;
		}
	};



	template<class _Friend>
	class readonly_flag
	{
		friend _Friend;

		bool value_;

		__forceinline bool toggle() volatile
		{
			return value_ = !value_;
		}

		__forceinline void on() volatile
		{
			value_ = true;
		}

		__forceinline void off() volatile
		{
			value_ = false;
		}

	public:
		inline readonly_flag():
			value_{true}
		{
		}

		inline readonly_flag(bool value) :
			value_{value}
		{
		}		

		__forceinline operator bool() const volatile
		{
			return value_;
		}

		__forceinline bool operator==(bool value) volatile
		{
			return value_ == value;
		}

		__forceinline bool operator==(readonly_flag<_Friend> const& other) volatile
		{
			return value_ == other.value_;
		}

		__forceinline bool operator!=(bool value) volatile
		{
			return value_ == value;
		}

		__forceinline bool operator!=(readonly_flag<_Friend> const& other) volatile
		{
			return value_ == other.value_;
		}
	};
}

#endif //_X_FLAG_H_
