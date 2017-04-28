#ifndef _WRAP_H_
#define _WRAP_H_

#include "more_type_traits.h"

namespace x::wrap
{
	template<class _Type>
	class ptr
	{
		using PtrType = std::remove_pointer_t<_Type>*;
		PtrType value_;

	public:
		template<class _Arg, 
			_concept<!x::same_template<_Arg, ptr<_Type>>::value>>
		inline ptr(_Arg& ref) :
			value_{static_cast<PtrType>(&ref)}
		{
		}

		template<class _Arg>
		inline ptr(_Arg* ptr):
			value_{static_cast<PtrType>(ptr)}
		{
		}

		template<class _Other>
		inline ptr(ptr<_Other> const& other):
			value_{static_cast<PtrType>(other.value_)}
		{
		}

		template<class _Conv>
		__forceinline operator _Conv*()
		{
			return static_cast<_Conv*>(value_);
		}

		template<class _Conv>
		__forceinline operator _Conv&()
		{
			return static_cast<_Conv&>(*value_);
		}

		__forceinline _Type& operator*()
		{
			return *value_;
		}

		__forceinline PtrType operator->()
		{
			return value_;
		}
	};

	template<class _Type>
	class ref
	{
		_Type& value_;

	public:
		template<class _Arg>
		inline ref(_Arg& ref):
			value_{static_cast<_Type&>(ref)}
		{
		}

		template<class _Arg>
		inline ref(_Arg* ref):
			value_{static_cast<_Type&>(*ref)}
		{
		}

		template<class _Conv>
		__forceinline operator _Conv&()
		{
			return static_cast<_Conv&>(value_);
		}

		/*__forceinline _Type& operator*()
		{
			return value_;
		}*/

		__forceinline _Type* operator->()
		{
			return &value_;
		}
	};
}
#endif //_WRAP_H_
