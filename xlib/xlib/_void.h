#ifndef _VOID_H
#define _VOID_H

/// <summary>
/// Empty template used to explicitly wrap void type into struct or simulate various types of void.
/// </summary>
/// <typeparam name="_Type">Type generating _void struct, by default: void.</typeparam>
template<typename _Type = void>
struct _void {};


//#define _return return _void<>()

#endif
