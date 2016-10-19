#ifndef BITMANAGER_H
#define BITMANAGER_H

#include "bitliteral.h"
#include "auto_cast.h"

namespace x
{
	template<typename _MemType>
	class _bitmanager
	{
		byte static constexpr MASK[8] = {
			00000001_b,
			00000010_b,
			00000100_b,
			00001000_b,
			00010000_b,
			00100000_b,
			01000000_b,
			10000000_b
		};

		class bit
		{
			_MemType* memory_;
			size_t bitNum_;

			friend struct _bitmanager;

			__forceinline bit(_MemType* memory, size_t bitNum = 0):
				memory_{memory},
				bitNum_{bitNum}
			{
			}

			__forceinline bit& setBitNum_(size_t bitNum)
			{
				bitNum_ = bitNum;
				return *this;
			}

		public:
			__forceinline void operator=(bool value)
			{
				(value ?
					reinterpret_cast<byte*>(memory_)[bitNum_/8] |= _bitmanager::MASK[bitNum_%8] :
					reinterpret_cast<byte*>(memory_)[bitNum_/8] &= ~_bitmanager::MASK[bitNum_%8]);
			}

			__forceinline operator bool() const
			{
				return reinterpret_cast<byte*>(memory_)[bitNum_/8] & _bitmanager::MASK[bitNum_%8];
			}

			~bit()
			{
				delete memory_;
			}
		};

		bit bitHandler_;

	public:
		using MemoryType = _MemType;

		inline _bitmanager():
			bitHandler_{new _MemType{0}}
		{
		}

		inline _bitmanager(_MemType initValue) :
			bitHandler_{new _MemType{initValue}}
		{
		}

		_bitmanager(_bitmanager const& other):
			bitHandler_{new _MemType{*other.bitHandler_.memory_}}
		{
		}

		_bitmanager(_bitmanager&& other):
			bitHandler_{other.bitHandler_.memory_}
		{
		}

		/*inline _bitmanager(_MemType* memory):
			bitHandler_{x::force_cast<byte*>(memory)}
		{
		}*/

		__forceinline void setBit(size_t bitNum, bool value = true)
		{
			bitHandler_.setBitNum_(bitNum) = value;
		}

		__forceinline bit& operator[](size_t bitNum)
		{
			return bitHandler_.setBitNum_(bitNum);
		}

		__forceinline _MemType operator | (_bitmanager<_MemType> const& other) const
		{
			return *bitHandler_.memory_ | *other.bitHandler_.memory_;
		}

		__forceinline _MemType operator & (_bitmanager<_MemType> const& other) const
		{
			return *bitHandler_.memory_ & *other.bitHandler_.memory_;
		}

		__forceinline _MemType operator ^ (_bitmanager<_MemType> const& other) const
		{
			return *bitHandler_.memory_ ^ *other.bitHandler_.memory_;
		}

		__forceinline _MemType operator |= (_bitmanager<_MemType> const& other)
		{
			return *bitHandler_.memory_ |= *other.bitHandler_.memory_;
		}

		__forceinline _MemType operator &= (_bitmanager<_MemType> const& other)
		{
			return *bitHandler_.memory_ &= *other.bitHandler_.memory_;
		}

		__forceinline _MemType operator ^= (_bitmanager<_MemType> const& other)
		{
			return *bitHandler_.memory_ ^= *other.bitHandler_.memory_;
		}

		__forceinline _MemType operator ~ () const
		{
			return ~ *bitHandler_.memory_;
		}

		~_bitmanager() = default;
	};

	template<unsigned _nBits>
	using bitmanager = _bitmanager<_MinFitType<_nBits>>;
}
#endif //BITMANAGER_H
