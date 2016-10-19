#ifndef _WRAP_H
#define _WRAP_H_

namespace x::wrap
{
	template<class _Type>
	class ptr
	{
		_Type* value_;

	public:
		template<class _Arg>
		inline ptr(_Arg& ref) :
			value_{static_cast<_Type*>(&ref)}
		{
		}

		template<class _Arg>
		inline ptr(_Arg* ptr) :
			value_{static_cast<_Type*>(ptr)}
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

		__forceinline _Type* operator->()
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
