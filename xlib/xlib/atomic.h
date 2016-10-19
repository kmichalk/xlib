#ifndef _ATOMIC_H_
#define _ATOMIC_H_

namespace x
{
	template<typename _Type>
	class atomic
	{
		_Type value_;
		volatile bool lock_;

		__forceinline void wait_() const volatile
		{
			while (lock_) {}
		}

	public:
		atomic():
			value_	{},
			lock_	{false}
		{
		}
		
		atomic(_Type value):
			value_	{value},
			lock_	{false}
		{
		}

		atomic(atomic<_Type> const& other):
			value_{other.value()},
			lock_{false}
		{
		}

		atomic(atomic<_Type>&& other):
			value_{other.value_},
			lock_{false}
		{
		}

		atomic<_Type>& operator=(atomic<_Type> const& other) volatile
		{

		}

		_Type value() volatile const
		{
			wait_();
			return value_;
		}

		inline void lock() volatile
		{
			lock_ = true;
		}

		inline void unlock() volatile
		{
			lock_ = false;
		}
	};
}
#endif //_ATOMIC_H_