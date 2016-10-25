#ifndef _FIXED_STR_H_
#define _FIXED_STR_H_

#include "more_type_traits.h"


namespace x
{
	template<size_t _size>
	class str
	{
		char data_[_size + 1];

		__forceinline void make_end_()
		{
			data_[_size] = '\0';
		}

	public:
		__forceinline str():
			data_{}
		{
		}

		__forceinline str(char const* data)
		{
			memcpy(data_, data, _size);
			make_end_();
		}

		__forceinline str(char const(&data)[_size]):
			data_{data}
		{
			make_end_();
		}

		__forceinline str(str<_size> const& other):
			data_{other.data_}
		{
			make_end_();
		}

		__forceinline str(str<_size>&& other):
			data_{std::move(other.data_)}
		{
			make_end_();
		}

		__forceinline str<_size>& operator=(str<_size> const& other)
		{
			memcpy(data_, other.data_, _size);
			return *this;
		}

		__forceinline str<_size>& operator=(str<_size>&& other)
		{
			memmove(data_, other.data_, _size);
		}

		template<size_t _otherSize>
		__forceinline bool operator==(str<_otherSize> const& other)
		{
			static_if(_size == _otherSize)
			{
				return !strcoll(data_, other.data_);
			}
			return false;
		}

		__forceinline operator char*()
		{
			return data_;
		}

		__forceinline char& operator[](size_t pos)
		{
			return data_[pos];
		}

		template<size_t _pos, _concept<_pos <= _size - 1>>
		__forceinline char& get()
		{
			return data_[_pos];
		}
	};
}

#endif //_FIXED_STR_H_
