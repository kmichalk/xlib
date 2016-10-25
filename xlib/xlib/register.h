#ifndef _REGISTER_H_
#define _REGISTER_H_

template<class _Type>
class register_value
{
public:
	__forceinline register_value(_Type value)
	{
		_asm {
			mov eax, dword ptr[value]
		}
	}

	register_value() = delete;
	register_value(register_value<_Type> const&) = delete;
	register_value(register_value<_Type> &&) = delete;

	__forceinline void operator+=(_Type value)
	{
		_asm {
			add eax, dword ptr[value]
		}
	}

	__forceinline void operator-=(_Type value)
	{
		_asm {
			sub eax, dword ptr[value]
		}
	}

	__forceinline operator _Type()
	{
		_Type value;
		_asm {
			mov dword ptr[value], eax
		}
		return value;
	}

	__forceinline ~register_value()
	{
	}

	friend __forceinline void operator<<(_Type& var, register_value<_Type> const& regVal)
	{
		_asm {
			mov dword ptr[var], eax
		}
	}
};

#define _REG_SET(_register, _var) _asm{ mov _register, dword ptr[_var] }
#define _REG_ADD(_register, _value) _asm{ add _register, dword ptr[_value] }
#define _REG_SUB(_register, _value) _asm{ sub _register, dword ptr[_value] }
#define _REG_INC(_register) _asm{ inc _register }
#define _REG_DEC(_register) _asm{ dec _register }
#define _REG_MUL(_register, _value) _asm{ imul _register, _register, dword ptr[_value] }
//#define _REG_DIV(_register, _value) /*_asm { mov edx, _register }*/ _asm{ mov edx, dword ptr[_value] } _asm{ idiv eax }
#define _REG_GET(_register, _var) _asm{ mov dword ptr[_var], _register }

#endif //_REGISTER_H_