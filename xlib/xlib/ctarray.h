#ifndef CTARRAY_H
#define CTARRAY_H

#include "more_type_traits.h"

namespace x
{
	namespace ct
	{
		template<typename T, size_t _size>
		class array
		{

		public:
			const T arr[_size];
			static constexpr size_t size = _size;
			static constexpr gen_seq<_size> copySeq_ = gen_seq<_size>{};

			template<typename... Y, typename = enable_if<every_is<T,Y...>::value>>
			constexpr array(Y... values): 
				arr{values...} 
			{ 
			}
			constexpr array() = delete;
			constexpr array(array<T, _size> const& other) = default;
			constexpr array(array<T, _size> && other) = default;

			constexpr const T& operator[](size_t i) const
			{
				return arr[i];
			}
		};
	}
}


#endif
