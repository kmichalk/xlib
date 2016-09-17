#ifndef _VOID_H
#define _VOID_H

template<typename T = void>
struct _void {};
#define _return return _void<>()

#endif
