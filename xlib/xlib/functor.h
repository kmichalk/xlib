#ifndef _X_FUNCTOR_H_
#define _X_FUNCTOR_H_

#include "more_type_traits.h"


namespace x
{
	/*template<class _Owner, class _Type>
	struct member
	{
		_Type(_Owner::*mptr);
		_Owner const& obj;

		member(_Owner const& obj, _Type(_Owner::*mptr)):
			obj{obj},
			mptr{mptr}
		{
		}

		__forceinline bool operator==(member<_Owner, _Type> const& other) const
		{
			return obj.*mptr == other.obj.*other.mptr;
		}

		__forceinline bool operator<(member<_Owner, _Type> const& other) const
		{
			return obj.*mptr < other.obj.*other.mptr;
		}

		__forceinline bool operator>(member<_Owner, _Type> const& other) const
		{
			return obj.*mptr > other.obj.*other.mptr;
		}

		__forceinline bool operator<=(member<_Owner, _Type> const& other) const
		{
			return obj.*mptr <= other.obj.*other.mptr;
		}

		__forceinline bool operator>=(member<_Owner, _Type> const& other) const
		{
			return obj.*mptr >= other.obj.*other.mptr;
		}

		__forceinline bool operator!=(member<_Owner, _Type> const& other) const
		{
			return obj.*mptr != other.obj.*other.mptr;
		}
		
	};*/


	template<class _Owner, class _Type>
	struct member
	{
		_Type(_Owner::*mptr);
		_Type value;

		member(_Type(_Owner::*mptr), _Type const& value):
			mptr{mptr},
			value{value}
		{
		}

		__forceinline bool operator==(_Owner const& obj) const
		{
			return obj.*mptr == value;
		}

		__forceinline bool operator<(_Owner const& obj) const
		{
			return obj.*mptr < value;
		}

		__forceinline bool operator>(_Owner const& obj) const
		{
			return obj.*mptr > value;
		}

		__forceinline bool operator<=(_Owner const& obj) const
		{
			return obj.*mptr <= value;
		}

		__forceinline bool operator>=(_Owner const& obj) const
		{
			return obj.*mptr >= value;
		}

		__forceinline bool operator!=(_Owner const& obj) const
		{
			return obj.*mptr != value;
		}

		__forceinline friend bool operator==(_Owner const& obj, member<_Owner, _Type> const& mbr) 
		{
			return obj.*mbr.mptr == mbr.value;
		}

		__forceinline friend bool operator<(_Owner const& obj, member<_Owner, _Type> const& mbr) 
		{
			return obj.*mbr.mptr < mbr.value;
		}

		__forceinline friend bool operator>(_Owner const& obj, member<_Owner, _Type> const& mbr) 
		{
			return obj.*mbr.mptr > mbr.value;
		}

		__forceinline friend bool operator<=(_Owner const& obj, member<_Owner, _Type> const& mbr) 
		{
			return obj.*mbr.mptr <= mbr.value;
		}

		__forceinline friend bool operator>=(_Owner const& obj, member<_Owner, _Type> const& mbr) 
		{
			return obj.*mbr.mptr >= mbr.value;
		}

		__forceinline friend bool operator!=(_Owner const& obj, member<_Owner, _Type> const& mbr) 
		{
			return obj.*mbr.mptr != mbr.value;
		}

	};


	namespace ft
	{
		template<class _Type>
		struct compare
		{
			_Type const& value;

			compare(_Type const& value):
				value{value}
			{
			}

			template<class _Arg>
			__forceinline bool operator()(_Arg const& elem)
			{
				return elem == value;
			}
		};

		template<class _Func>
		struct execute
		{
			using FuncData = x::naked<_Func>;
			FuncData* func;

			execute(FuncData& func):
				func{x::pointer(func)}
			{
			}

			template<class _Arg>
			__forceinline bool operator()(_Arg const& elem)
			{
				return (*func)(elem);
			}
		};

		template<class _Owner, class _Member>
		struct compare_member
		{
			_Member(_Owner::*member);
			_Member const& value;

			compare_member(_Member(_Owner::*member), _Member const& value):
				member{member},
				value{value}
			{
			}

			__forceinline bool operator()(_Owner const& elem)
			{
				return (elem.*member) == value;
			}
		};
	}	
}

#endif //_X_FUNCTOR_H_
