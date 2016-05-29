#ifndef SIZE_H
#define SIZE_H

#include "more_type_traits.h"

#define enable_if std::enable_if_t

namespace x {
ADVANCED_MEMBER_TEST(_has_size, typename R, size, const)

template<typename T, size_t size_>
__forceinline size_t size(T(&arr)[size_])
{
	return size_;
}

template<typename T>
__forceinline enable_if<_has_size<T, size_t>::value,
	size_t> size(T const& container)
{
	return container.size();
}

#undef enable_if

}

#endif //SIZE_H
