#ifndef DISP_H
#define DISP_H

#include <iostream>

template<class _Any = void>
class _Disp
{
public:
	static _Disp<_Any> performer;

	template<class _Arg>
	inline _Disp<_Any>& operator<<(_Arg const& arg)
	{
		std::cout << arg;
		return *this;
	}
};

template<class _Arg>
inline _Disp<>& operator , (_Disp<>& os, _Arg const& var)
{
	return os << " " << var;
}

template<class _Any>
_Disp<_Any> _Disp<_Any>::performer = {};

template<class _Any = void>
class _DispLn
{
public:
	static _DispLn<_Any> performer;

	template<class _Arg>
	inline _DispLn& operator<<(_Arg const& arg)
	{
		std::cout << arg << std::endl;
		return *this;
	}
};

template<class _Any>
_DispLn<_Any> _DispLn<_Any>::performer = {};


#define disp _Disp<>::performer << 
#define displn _DispLn<>::performer <<

#endif
