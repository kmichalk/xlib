#ifndef REALCALL_H
#define REALCALL_H

#include "vamacros.h"
#include "autoref.h"

#define enable_if std::enable_if_t

#define realcall(...) CAT(_realcall_, NARGS(__VA_ARGS__))(__VA_ARGS__)
#define _realcall_2(_obj, _fn) (x::deref(_obj).std::remove_cv_t<std::remove_pointer_t<std::remove_reference_t<decltype(_obj)>>> :: _fn)
#define _realcall_1(_obj) (x::deref(_obj). std::remove_cv_t<std::remove_pointer_t<std::remove_reference_t<decltype(_obj)>>> :: operator())

#define direct_call(...) realcall(__VA_ARGS__)

#undef enable_if

#endif //REALCALL_H