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

	class id
	{
		long long unsigned const value_;

	public:
		inline id():
			value_{_LastID<>::next()}
		{
		}

		inline id(id const&):
			value_{_LastID<>::next()}
		{
		}

		
		inline id(id&&):
			value_{_LastID<>::next()}
		{
		}

		
		__forceinline id& operator=(id const&)
		{
			//value_ = other.value;
		}

		
		__forceinline id& operator=(id&&)
		{
		}


		__forceinline operator long long unsigned() const
		{
			return value_;
		}


		__forceinline bool operator==(id const& other) const
		{
			return value_ == other.value_;
		}


		__forceinline bool operator!=(id const& other) const
		{
			return value_ != other.value_;
		}


		inline ~id() = default;
	};
}

#undef _HEADER_ONLY

#endif //_ID_H_
