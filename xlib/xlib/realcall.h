#ifndef REALCALL_H
#define REALCALL_H

#include "vamacros.h"

#define enable_if std::enable_if_t

template<typename T>
__forceinline enable_if<std::is_pointer<T>::value, std::remove_pointer_t<T>&> remove_ptr(T var)
{
	return *var;
}

template<typename T>
__forceinline enable_if<!std::is_pointer<T>::value, T&> remove_ptr(T& var)
{
	return var;
}

#define realcall(...) CAT(_realcall_, NARGS(__VA_ARGS__))(__VA_ARGS__)
#define _realcall_2(_obj, _fn) (remove_ptr(_obj).std::remove_cv_t<std::remove_pointer_t<std::remove_reference_t<decltype(_obj)>>> :: _fn)
#define _realcall_1(_obj) (remove_ptr(_obj). std::remove_cv_t<std::remove_pointer_t<std::remove_reference_t<decltype(_obj)>>> :: operator())

#endif //REALCALL_H