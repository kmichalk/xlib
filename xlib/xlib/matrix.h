#ifndef _X_MATRIX_H_
#define _X_MATRIX_H_

#include "more_type_traits.h"
#include "allocator.h"

namespace x
{
	template<class _Type, class _First, class... _Rest>
	__forceinline void alloc(_Type*& dest, _First size, _Rest... rest)
	{
		dest = new multi_ptr_t<naked_t<_Type>, sizeof...(_Rest)>[size];
		for (size_t i = 0; i < size; ++i)
			alloc(dest[i], rest...);
	}

	template<class _Type, class _First>
	__forceinline void alloc(_Type*& dest, _First size)
	{
		dest = new _Type[size];
	}

	template<class _Type, class _First, class... _Rest>
	multi_ptr_t<_Type, sizeof...(_Rest) + 1> alloc(_First size, _Rest... rest)
	{
		multi_ptr_t<_Type, sizeof...(_Rest) + 1> ptr;
		alloc(ptr, size, rest...);
		return ptr;
	}



	template<class _Type>
	_Type**** old_alloc(size_t s1, size_t s2, size_t s3, size_t s4)
	{
		_Type**** ptr = new _Type***[s1];
		for (size_t i = 0; i < s1; ++i) {
			ptr[i] = new _Type**[s2];
			for (size_t j = 0; j < s2; ++j) {
				ptr[i][j] = new _Type*[s3];
				for (size_t k = 0; k < s3; ++k) {
					ptr[i][j][k] = new _Type[s4];
					delete[] ptr[i][j][k];
				}
				delete[] ptr[i][j];
			}
			delete[] ptr[i];
		}
		delete[] ptr;
		return ptr;
	}


	template<class _Type, class _First, class... _Rest, class _Func>
	void perform(_Type* array, _Func&& func, _First size, _Rest... rest)
	{
		for (size_t i = 0; i < size; ++i) {
			perform(array[i], func, rest...);
		}
	}

	template<class _Type, class _First, class _Func>
	__forceinline void perform(_Type* array, _Func&& func, _First size)
	{
		for (size_t i = 0; i < size; ++i) {
			func(array[i]);
		}
	}

	/*template<class _Type, unsigned _dim, class _Func>
	__forceinline void perform(_Type& array, _Func&& func)
	{
		func(array);
	}*/
	//template<class _Type, class _First, class... _Rest>
	//multi_ptr_t<_Type, sizeof...(_Arg)> alloc(_First size, _Rest... rest)
	//{
	//	static constexpr size_t DIM = sizeof...(_Rest)+1;
	//	//using array_type = multi_ptr_t<_Type, sizeof...(_A)>;
	//	multi_ptr_t<_Type, DIM> result = new multi_ptr_t<_Type, DIM - 1>[size];
	//	for (size_t i = 0;i<size, )
	//}


	template<class _Type>
	class matrix
	{
		using allocator_t = x::allocator<_Type>;

		size_t rows_;
		size_t cols_;
		_Type** data_;

	public:
		matrix() = delete;
		matrix(matrix<_Type> const& other):
			rows_{other.rows_},
			cols_{other.cols_},
			data_{alloc(rows_, cols_)}
		{
		}


	};
}

#endif //_X_MATRIX_H_
