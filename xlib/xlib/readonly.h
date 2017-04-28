#ifndef _X_READONLY_H_
#define _X_READONLY_H_


namespace x
{
	/// <summary>
	/// Wrapper class containing data of type _Type preventing free access 
	/// to this data from any class besides specified as _Owner.
	/// </summary>
	template<class _Type, class _Owner>
	class readonly
	{
		friend _Owner;

		_Type value;


		/// <summary>
		/// Assigns contained value.
		/// </summary>
		/// <param name="value">Value to be set.</param>
		/// <returns>Reference to the caller object.</returns>
		readonly<_Type, _Owner>& inline operator=(_Type const& value)
		{
			this->value = value;
			return *this;
		}


		/// <summary>
		/// Compares contained value.
		/// </summary>
		/// <param name="value">Value to be compared with.</param>
		/// <returns>True if values are equal, otherwise false.</returns>
		bool inline operator==(_Type const& value) const
		{
			return this->value == value;
		}


		/// <summary>
		/// Compares contained value.
		/// </summary>
		/// <param name="value">Value to be compared with.</param>
		/// <returns>True if values are not equal, otherwise false.</returns>
		bool inline operator!=(_Type const& value) const
		{
			return this->value != value;
		}

	public:
		/// <summary>
		/// Default constructor calling default constructor of contained data.
		/// </summary>
		readonly():
			value{}
		{
		}


		/// <summary>
		/// Constructor initializing data to specified value.
		/// </summary>
		/// <param name="value">Value to be set.</param>
		readonly(_Type const& value):
			value{value}
		{
		}


		/// <summary>
		/// Implicit conversion operator allowing non-owner scopes to access the data by copy.
		/// </summary>
		/// <returns></returns>
		__forceinline operator _Type() const
		{
			return value;
		}
	};
}

#endif //_X_READONLY_H_
