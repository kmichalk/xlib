#ifndef TYPEARR_H
#define TYPEARR_H

#include "more_type_traits.h"

namespace x
{
	template<typename... _Types>
	struct TypeArray
	{
		constexpr TypeArray()
		{
		}

		template<typename _Get>
		static constexpr size_t num_of = x::find_type<_Get>::in<_Types...>::value;

		template<size_t _n>
		using get = x::select_t<_n, _Types...>;
	};
}
#endif //TYPEARR_H
