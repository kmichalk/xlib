#ifndef LOOPS_H
#define LOOPS_H

#include "size.h"

#define repeat(_n) for (int __i__=0;__i__<_n;++__i__)
#define foreach(_i, _cnt) for (size_t _i = 0; _i < x::size(_cnt); ++_i)
#define loop while(true)
#define until(_test) while(! (_test))
#define booliter(_iterName) for (int _iterName=0;_iterName<2;++_iterName)
#define iter(_iterName, _from, _to) for (auto _iterName{_from}; _iterName <= _to; ++_iterName) 
#endif