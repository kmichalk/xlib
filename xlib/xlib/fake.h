#ifndef FAKE_H
#define FAKE_H

#include "more_type_traits.h"
#include "byte.h"

namespace x
{
	template<typename _Type>
	class FakeType
	{
		static constexpr size_t size_ = sizeof(_Type);

		byte fakeData_[size_];

	public:
		inline FakeType() noexcept
		{
		}

		FakeType(FakeType<_Type> const&) = delete;

		FakeType(FakeType<_Type>&&) = delete;

		__forceinline FakeType<_Type>& operator=(FakeType<_Type> const& other) = default;
		/*{
			memcpy(fakeData_, other.fakeData_, size_);
		}*/

		~FakeType()
		{
		}

		template<typename _To>
		inline operator _To*() const noexcept
		{
			return reinterpret_cast<_To*>(fakeData_);
		}
	};

	/*template<typename _Type>
	using FakeType = byte[sizeof(_Type)];*/

	/*template<typename _Type>
	__forceinline _Type* fake() noexcept
	{
		return reinterpret_cast<_Type*>(new FakeType<_Type>);
	}

	template<typename _Type>
	__forceinline _Type* fake(size_t size) noexcept
	{
		return reinterpret_cast<_Type*>(new FakeType<_Type>[size]);
	}*/


	///////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// Class recursively expanding to specified size.
	/// </summary>
	/// <typeparam name="_size"> - desired size of generated type</typeparam>
	/// <typeparam name="_Fake"> - type, size of which _size parameter will be multiplied
	/// by default: byte</typeparam>
	template<size_t _size, class _Fake = byte>
	class fake
	{
		std::conditional_t<_size == sizeof(_Fake), _Fake, fake<_size - sizeof(_Fake), _Fake>> data_;
	};
}

#endif //FAKE_H
