#ifndef FAKE_H
#define FAKE_H

namespace x
{
	using byte = unsigned char;

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

		inline void operator=(FakeType<_Type> const& other)
		{
			memcpy(fakeData_, other.fakeData_, size_);
		}

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

	template<typename _Type>
	__forceinline _Type* fake() noexcept
	{
		return reinterpret_cast<_Type*>(new FakeType<_Type>);
	}

	template<typename _Type>
	__forceinline _Type* fake(size_t size) noexcept
	{
		return reinterpret_cast<_Type*>(new FakeType<_Type>[size]);
	}
}

#endif //FAKE_H
