#ifndef _RESULT_H_
#define _RESULT_H_

namespace x
{
	template<class _Type>
	struct result
	{
		bool const valid;
		_Type value;

		inline result():
			valid{false}
		{
		}

		inline result(_Type value) :
			valid{true},
			value{value}
		{
		}

		__forceinline operator bool() const
		{
			return valid;
		}

		__forceinline operator _Type() const
		{
			return value;
		}
	};
}

#endif //_RESULT_H_
