#ifndef LOOPS_H
#define LOOPS_H

#include "size.h"

#define loop(n) for (int __i__=0;__i__<n;++__i__)
#define foreach(i, cnt) for (size_t i = 0; i<x::size(cnt); ++i)
#define repeat while(true)

#endif