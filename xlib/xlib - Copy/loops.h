#ifndef LOOPS_H
#define LOOPS_H

#include "size.h"

#define repeat(n) for (int __i__=0;__i__<n;++__i__)
#define foreach(i, cnt) for (size_t i = 0, __size=x::size(cnt); i<__size; ++i)
#define loop while(true)
#define until(test) while(! (test))

#endif