//#pragma once
#ifndef _X_VECTOR_H_
#define _X_VECTOR_H_

//#include "random_engine_.h"
#include <vector>
#include <list>
#include <iostream>
#include <string>
#include "more_type_traits.h"
#include "simple.h"
#include "autoref.h"
#include "result.h"
#include "xrnd.h"
#include "debug.h"
#include "fake.h"
#include "allocator.h"
#include "functor.h"

#define _DEBUG 1

#if _DEBUG == 1
#define _DEBUG_CTOR 0
#endif

#define enable_if std::enable_if_t
#define decay std::decay_t

//#define _DECLARE_FLAG_TYPE(_NAME) \
//class _##_NAME\
//{\
//	constexpr _##_NAME(){}\
//public:\
//	static constexpr _##_NAME _NAME = {};\
//}\
//constexpr _FLAG


namespace x
{
	template<class _Func>
	class func_wrap;

	template<class _Ret, class... _Arg>
	class func_wrap<_Ret(_Arg...)>
	{
		void* func;
	public:
		template<class _Func>
		func_wrap(_Func&& func):
			func{x::pointer(func)}
		{
		}

		__forceinline auto operator*()
		{
			return func;
		}
	};



	struct NoAlloc{};

	constexpr NoAlloc NO_ALLOC;

	struct Adopt{};

	constexpr Adopt ADOPT;


	template<class> class _vector;

	////////////////////////////////////////////////////////////////////////////

	//type expanding into n-dimensional vector
	template<class _Type, unsigned _dim>
	struct _vector_nd
	{
		using type = _vector<typename _vector_nd<_Type, _dim - 1>::type>;
	};

	template<class _Type>
	struct _vector_nd<_Type, 0>
	{
		using type = _Type;
	};

	template<class _Type, unsigned _dim>
	using _vector_nd_t = typename _vector_nd<_Type, _dim>::type;

	////////////////////////////////////////////////////////////////////////////


	template<class _Type>
	class _vector
	{
	public:
		template<class Y>
		struct _CountDim
		{
			static constexpr size_t value = 0;
		};
		template<class Y>
		struct _CountDim<_vector<Y>>
		{
			static constexpr size_t value = 1 + _CountDim<Y>::value;
		};

		template<class Y>
		struct _AtomicType
		{
			using type = Y;
		};
		template<class Y>
		struct _AtomicType<_vector<Y>>
		{
			using type = typename _AtomicType<Y>::type;
		};

		///////////////////////////////////////////////////////////////////////////////


		static constexpr bool IS_POINTER_ = std::is_pointer<_Type>::value;


		////////////////////////////////////////////////////////////////////////////

		static constexpr size_t DIM_ = _CountDim<_vector<_Type>>::value;
		static constexpr size_t ELEMENT_SIZE_ = sizeof(_Type);
		static constexpr size_t DEFAULT_ALLOC_ = 10;
		static constexpr size_t MIN_ALLOC_ = 1;

		static size_t defaultAlloc_;
		static double allocMult_;

		////////////////////////////////////////////////////////////////////////////

		size_t reserved_;
		size_t size_;
		_Type* data_;

		////////////////////////////////////////////////////////////////////////////


		//class moved_data
		//{
		//	_Type* data_;
		//	size_t size_;
		//	size_t reserved_;

		//public:
		//	moved_data(_vector<_Type> && vec):
		//		data_{vec.data_},
		//		size_{vec.size_},
		//		reserved_{vec.size_}
		//	{
		//		std::cout << "moved_data ctor\n";
		//		vec.data_ = nullptr;
		//	}

		//	/*__forceinline operator _vector<_Type>() const
		//	{
		//		return _vector<_Type>{ADOPT, data_, size_};
		//	}*/

		//	operator _vector<_Type>() const
		//	{
		//		std::cout << "cast moved_data\n";
		//		return *reinterpret_cast<const _vector<_Type>*>(this);
		//	}
		//};


		///////////////////////////////////////////////////////////////////////////////


		//__forceinline _Type* allocator<_Type>::allocate(size_t newSize)
		//{
		//	reserved_ = newSize;
		//	//return new _Type[newSize];
		//	return reinterpret_cast<_Type*>(new byte[newSize*ELEMENT_SIZE]);
		//}

		inline void reallocate_(size_t newSize)
		{
			allocator<_Type>::reallocate(data_, reserved_, newSize);
			reserved_ = newSize;
		}


		////////////////////////////////////////////////////////////////////////////

		ADVANCED_MEMBER_TEST(has_less_op, typename R, operator<, const, typename A);

		////////////////////////////////////////////////////////////////////////////
		
		
		/*template<_capture(_Type)>
		__forceinline enable_if<
			has_less_op<_Type, bool, const _Type&>::value ||
			has_less_op<_Type, bool, _Type&>::value ||
			!std::is_class<_Type>::value,*/

		void quicksort_(int r1, int r2)
		{
			int last = r1;
			for (int i = r1; i < r2; ++i) {
				if (data_[i] < data_[r2]) {
					if (last != i) swap(i, last);
					++last;
				}
			}
			this->swap(last, r2);
			if (r1 < last - 1) quicksort_(r1, last - 1);
			if (last + 1 < r2) quicksort_(last + 1, r2);
		}


		template<class _Func>
		void quicksort_(int r1, int r2, _Func&& comparator)
		{
			int last = r1;
			for (int i = r1; i < r2; ++i) {
				if (comparator(data_[i], data_[r2])) {
					if (last != i) swap(i, last);
					++last;
				}
			}
			this->swap(last, r2);
			if (r1 < last - 1) quicksort_(r1, last - 1);
			if (last + 1 < r2) quicksort_(last + 1, r2);
		}

		/*template<bool useable, class Y = _Type>
		enable_if<_vector<Y>::DIM_ >= 2,
			void> reserve_(std::initializer_list<size_t>& sizes,
				std::initializer_list<size_t>::iterator it)
		{
			if (*it > reserved_) {
				allocator<_Type>::reallocate(data_, reserved_, *it);
				if (useable) size_ = *it;
			}
			if (++it != sizes.end()) {
				for (size_t i = 0; i<size_; ++i) {
					data_[i].reserve_<useable>(sizes, it);
				}
			}
		}
		template<bool useable, class Y = _Type>
		enable_if<_vector<Y>::DIM_ == 1,
			void> reserve_(std::initializer_list<size_t>& sizes,
				std::initializer_list<size_t>::iterator it)
		{
			if (*it > reserved_) {
				allocator<_Type>::reallocate(data_, reserved_, *it);
				if (useable) size_ = *it;
			}
		}*/

		template<class Y = _Type>
		__forceinline enable_if<
			is_trivially_copyable<Y>::value,
			void> copy_(Y* dest, const Y* arr, size_t s) const
		{
			memcpy(dest, arr, ELEMENT_SIZE_*s);
		}
		template<class Y = _Type>
		__forceinline enable_if<
			!is_trivially_copyable<Y>::value,
			void> copy_(Y* dest, const Y* arr, size_t s) const
		{
			while (s--) dest[s] = arr[s];
		}


		////////////////////////////////////////////////////////////////////////////

		/////*template<bool _ptrDeref = false, class Y = _Type>
		////__forceinline enable_if<std::is_pointer<Y>::value && _ptrDeref,
		////	bool> compare_elem_(const Y& el1, const Y& el2) const
		////{
		////	return *el1 == *el2;
		////}
		////template<bool _ptrDeref = false, class Y = _Type>
		////__forceinline enable_if<!std::is_pointer<Y>::value || !_ptrDeref,
		////	bool> compare_elem_(const Y& el1, const Y& el2) const
		////{
		////	return el1 == el2;
		////}

		////template<bool _ptrDelete = false, class Y = _Type>
		////__forceinline enable_if<std::is_pointer<Y>::value && _ptrDelete,
		////	void> destroy_elem_(const Y& el)
		////{
		////	delete el;
		////}
		////template<bool _ptrDelete = false, class Y = _Type>
		////__forceinline enable_if<!std::is_pointer<Y>::value || !_ptrDelete,
		////	void> destroy_elem_(const Y& el)
		////{
		////}

		////template<bool _ptrDeref = false, class Y = _Type>
		////__forceinline enable_if<std::is_pointer<Y>::value && _ptrDeref,
		////	void> assign_elem_(const Y& el1, const Y& el2)
		////{
		////	return *el1 = *el2;
		////}
		////template<bool _ptrDeref = false, class Y = _Type>
		////__forceinline enable_if<!std::is_pointer<Y>::value || !_ptrDeref,
		////	void> assign_elem_(const Y& el1, const Y& el2)
		////{
		////	return el1 = el2;
		////}

		////template<bool _ptrDeref = false, class Y = _Type>
		////__forceinline enable_if<std::is_pointer<Y>::value && _ptrDeref,
		////	bool> compare_less_elem_(const Y& el1, const Y& el2) const
		////{
		////	return *el1 < *el2;
		////}
		////template<bool _ptrDeref = false, class Y = _Type>
		////__forceinline enable_if<!std::is_pointer<Y>::value || !_ptrDeref,
		////	bool> compare_less_elem_(const Y& el1, const Y& el2) const
		////{
		////	return el1 < el2;
		////}

		////template<bool _ptrDeref = false, class Y = _Type>
		////__forceinline enable_if<std::is_pointer<Y>::value && _ptrDeref,
		////	std::remove_pointer_t<Y>&> elem_(Y& el)
		////{
		////	return *el;
		////}
		////template<bool _ptrDeref = false, class Y = _Type>
		////__forceinline enable_if<!std::is_pointer<Y>::value || !_ptrDeref,
		////	Y&> elem_(Y& el)
		////{
		////	return el;
		////}*/


		////////////////////////////////////////////////////////////////////////////

	public:
		template<class>
		friend class _vector;

		////////////////////////////////////////////////////////////////////////////


		using type = _Type;

		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using value_type = _Type;
		using pointer = _Type*;
		using reference = _Type&;


		////////////////////////////////////////////////////////////////////////////


		/*class moved
		{
			template<class>	friend class _vector;

			_Type* data_;
			size_t size_;
			size_t reserved_;
		public:
			moved(_vector<_Type>&& vec):
				data_{vec.data_},
				size_{vec.size_},
				reserved_{vec.reserved_}
			{
				vec.data_ = nullptr;
			}
		};*/


		////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Const iterator class allowing const standard forward and reverse iteration through elements.
		/// </summary>
		/// <remarks>
		/// Each instance of iterator is connected with its 'owner' vector by const reference to that vector.
		/// It contains information about the position in vector. Allows to iterate through vector using
		/// C++ standard shortened for loop syntax. Elements accessed by this iterator can not be changed. 
		/// Basic arithmetic is allowed on iterators to allow changind
		/// or comparisons. The concept is to enable free access to the iterator's position and owner,
		/// so these fields are public.
		/// </remarks>
		struct const_iterator
		{
			using difference_type = ptrdiff_t; 
			using value_type = _Type; 
			using reference = _Type const&; 
			using pointer = _Type const*; 
			using iterator_category = std::bidirectional_iterator_tag;  

			/// <summary>
			/// Const reference to the vector being iterated.
			/// </summary>
			const _vector<_Type>& owner;

			/// <summary>
			/// Current position in vector.
			/// </summary>
			size_t pos;

			/// <summary>
			/// Constructs a const iterator over given vector.
			/// </summary>
			/// <param name="owner">the vector to be iterated</param>
			const_iterator(const _vector<_Type>& owner): 
				owner{owner}, 
				pos{0}
			{
			}

			/// <summary>
			/// Constructs a const iterator over given vector with specified starting position.
			/// </summary>
			/// <param name="owner">the vector to be iterated</param>
			/// <param name="pos">the starting position</param>
			const_iterator(const _vector<_Type>& owner, size_t pos): 
				owner{owner}, 
				pos{pos}
			{
			}

			/// <summary>
			/// The copy constructor.
			/// </summary>
			/// <param name="other">iterator to be copied</param>
			const_iterator(const const_iterator& other):
				owner{other.owner}, 
				pos{other.pos}
			{
			}

			/// <summary>
			/// The move constructor.
			/// </summary>
			/// <remarks>
			/// Does not differ from copy constructor.
			/// </remarks>
			/// <param name="other"></param>
			const_iterator(const_iterator&& other):
				owner{other.owner},
				pos{other.pos}
			{
			}

			/// <summary>
			/// Increments the iterator's position by 1.
			/// </summary>
			/// <returns>reference to the caller</returns>
			__forceinline const_iterator& operator++()
			{
				++pos;
				return *this;
			}

			/// <summary>
			/// Decrements the iterator's position by 1.
			/// </summary>
			/// <returns>reference to the caller</returns>
			__forceinline const_iterator& operator--()
			{
				--pos;
				return *this;
			}

			/// <summary>
			/// Performs postincrementation by 1 of iterator.
			/// </summary>
			/// <returns>new instance of iterator of position befor incrementation</returns>
			inline const_iterator operator++(int)
			{
				const_iterator copy{*this};
				++pos;
				return copy;
			}

			/// <summary>
			/// Performs postdecrementation by 1 of iterator.
			/// </summary>
			/// <returns>new instance of iterator of position befor decrementation</returns>
			inline const_iterator operator--(int)
			{
				const_iterator copy{*this};
				--pos;
				return copy;
			}

			/// <summary>
			/// Creates an iterator with a value added to position.
			/// </summary>
			/// <param name="i">value to be added</param>
			/// <returns>an iterator with added position</returns>
			__forceinline const_iterator operator+(int i)
			{
				return const_iterator(owner, pos + i);
			}

			/// <summary>
			/// Creates an iterator with a value subtracted from position.
			/// </summary>
			/// <param name="i">value to be subtracted</param>
			/// <returns>an iterator with subtracted position</returns>
			__forceinline const_iterator operator-(int i)
			{
				return const_iterator(owner, pos - i);
			}

			/// <summary>
			/// Adds a value to the iterators position.
			/// </summary>
			/// <param name="i">value to be added</param>
			/// <returns>reference to the caller</returns>
			__forceinline const_iterator& operator+=(int i)
			{
				pos += i;
				return *this;
			}

			/// <summary>
			/// Subtracts a value from the iterators position.
			/// </summary>
			/// <param name="i">value to be subtracted</param>
			/// <returns>reference to the caller</returns>
			__forceinline const_iterator& operator-=(int i)
			{
				pos -= i;
				return *this;
			}

			/// <summary>
			/// Creates an iterator with a value added to position.
			/// </summary>
			/// <param name="i">value to be added</param>
			/// <returns>an iterator with added position</returns>
			__forceinline const_iterator operator+(size_t i)
			{
				return const_iterator(owner, pos + i);
			}

			/// <summary>
			/// Creates an iterator with a value subtracted from position.
			/// </summary>
			/// <param name="i">value to be subtracted</param>
			/// <returns>an iterator with subtracted position</returns>
			__forceinline const_iterator operator-(size_t i)
			{
				return const_iterator(owner, pos - i);
			}

			/// <summary>
			/// Adds a value to the iterators position.
			/// </summary>
			/// <param name="i">value to be added</param>
			/// <returns>reference to the caller</returns>
			__forceinline const_iterator& operator+=(size_t i)
			{
				pos += i; return *this;
			}

			/// <summary>
			/// Subtracts a value from the iterators position.
			/// </summary>
			/// <param name="i">value to be subtracted</param>
			/// <returns>reference to the caller</returns>
			__forceinline const_iterator& operator-=(size_t i)
			{
				pos -= i; return *this;
			}

			/// <summary>
			/// Accesses the element on current iterator's position in vector.
			/// </summary>
			/// <returns>reference to the element at current position</returns>
			__forceinline const _Type& operator*() const
			{
				return owner.data_[pos];
			}

			/// <summary>
			/// Compares iterator position with other's.
			/// </summary>
			/// <param name="other">iterator to be compared</param>
			/// <returns>true if positions are equal, otherwise false</returns>
			__forceinline bool operator==(const const_iterator& other) const
			{
				return pos == other.pos;
			}

			/// <summary>
			/// Compares iterator position with other's.
			/// </summary>
			/// <param name="other">iterator to be compared</param>
			/// <returns>true if positions are not equal, otherwise false</returns>
			__forceinline bool operator!=(const const_iterator& other) const
			{
				return pos != other.pos;
			}

			/// <summary>
			/// Compares iterator position with other's.
			/// </summary>
			/// <param name="other">iterator to be compared</param>
			/// <returns>true if position is lower than other's, otherwise false</returns>
			__forceinline bool operator<(const const_iterator& other) const
			{
				return pos < other.pos;
			}

			/// <summary>
			/// Compares iterator position with other's.
			/// </summary>
			/// <param name="other">iterator to be compared</param>
			/// <returns>true if position is greater than other's, otherwise false</returns>
			__forceinline bool operator>(const const_iterator& other) const
			{
				return pos > other.pos;
			}

			/// <summary>
			/// Compares iterator position with other's.
			/// </summary>
			/// <param name="other">iterator to be compared</param>
			/// <returns>true if position is lower or equal than other's, otherwise false</returns>
			__forceinline bool operator<=(const const_iterator& other) const
			{
				return pos <= other.pos;
			}

			/// <summary>
			/// Compares iterator position with other's.
			/// </summary>
			/// <param name="other">iterator to be compared</param>
			/// <returns>true if position is greater or equal than other's, otherwise false</returns>
			__forceinline bool operator>=(const const_iterator& other) const
			{
				return pos >= other.pos;
			}

			/// <summary>
			/// Assigns parameters of iterator to other's.
			/// </summary>
			/// <param name="other">the assigned iterator</param>
			/// <returns>reference to the caller</returns>
			__forceinline const_iterator& operator=(const const_iterator& other)
			{
				pos = other.pos; return *this;
			}

			/// <summary>
			/// Checks whether the iterator's position is valid for the owner vector.
			/// </summary>
			/// <returns>true if the owner's size is not exceeded, otherwise false</returns>
			__forceinline operator bool() const
			{
				return pos < owner.size_;
			}

			/// <summary>
			/// Checks whether the iterator's position is not valid for the owner vector.
			/// </summary>
			/// <returns>true if the owner's size is exceeded, otherwise false</returns>
			__forceinline bool operator !() const
			{
				return pos >= owner.size_;
			}

			/// <summary>
			/// Allows to call a member function or access a member of an element on current position directly.
			/// </summary>
			/// <remarks>
			/// Enables behavior of the iterator as if it was a pointer to element contained in vector.
			/// Only a const function can be called on object pointed by iterator.
			/// </remarks>
			__forceinline auto operator->() const
			{
				return x::pointer(owner.data_[pos]);
			}
		};

		////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Iterator class allowing standard forward and reverse iteration through elements.
		/// </summary>
		/// <remarks>
		/// Each instance of iterator is connected with its 'owner' vector by reference to that vector.
		/// It contains information about the position in vector. Allows to iterate through vector using
		/// C++ standard shortened for loop syntax. Elements accessed by this iterator can be changed, for
		/// const iteration see const_iterator. Basic arithmetic is allowed on iterators to allow changind
		/// or comparisons. The concept is to enable free access to the iterator's position and owner,
		/// so these fields are public.
		/// </remarks>
		struct iterator
		{
			using difference_type = ptrdiff_t; 
			using value_type = _Type; 
			using reference = _Type &;
			using pointer = _Type *; 
			using iterator_category = std::bidirectional_iterator_tag; 

			/// <summary>
			/// Reference to the vector being iterated.
			/// </summary>
			_vector<_Type>& owner;

			/// <summary>
			/// Current position in vector.
			/// </summary>
			size_t pos;

			/// <summary>
			/// Constructs an iterator over given vector.
			/// </summary>
			/// <param name="owner">the vector to be iterated</param>
			iterator(_vector<_Type>& owner):
				owner{owner},
				pos{0}
			{
				//printf("vec\n");
			}

			/// <summary>
			/// Constructs an iterator over given vector with specified starting position.
			/// </summary>
			/// <param name="owner">the vector to be iterated</param>
			/// <param name="pos">the starting position</param>
			iterator(_vector<_Type>& owner, size_t pos):
				owner{owner},
				pos{pos}
			{
				//printf("vec, pos\n");
			}

			/// <summary>
			/// The copy constructor.
			/// </summary>
			/// <param name="other">iterator to be copied</param>
			iterator(const iterator& other):
				owner{other.owner},
				pos{other.pos}
			{
				//printf("copy\n");
			}

			/// <summary>
			/// The move constructor.
			/// </summary>
			/// <remarks>
			/// Does not differ from copy constructor.
			/// </remarks>
			/// <param name="other"></param>
			iterator(iterator&& other):
				owner{other.owner},
				pos{other.pos}
			{
				//printf("move\n");
			}

			/// <summary>
			/// Increments the iterator's position by 1.
			/// </summary>
			/// <returns>reference to the caller</returns>
			__forceinline iterator& operator++()
			{
				//printf("++\n");
				++pos;
				return *this;
			}

			/// <summary>
			/// Decrements the iterator's position by 1.
			/// </summary>
			/// <returns>reference to the caller</returns>
			__forceinline iterator& operator--()
			{
				--pos;
				return *this;
			}

			/// <summary>
			/// Performs postincrementation by 1 of iterator.
			/// </summary>
			/// <returns>new instance of iterator of position befor incrementation</returns>
			inline iterator operator++(int)
			{
				//printf("++\n");
				iterator copy{*this};
				++pos;
				return copy;
			}

			/// <summary>
			/// Performs postdecrementation by 1 of iterator.
			/// </summary>
			/// <returns>new instance of iterator of position befor decrementation</returns>
			inline iterator operator--(int)
			{
				iterator copy{*this};
				--pos;
				return copy;
			}

			/// <summary>
			/// Creates an iterator with a value added to position.
			/// </summary>
			/// <param name="i">value to be added</param>
			/// <returns>an iterator with added position</returns>
			__forceinline iterator operator+(int i)
			{
				return iterator(owner, pos + i);
			}

			/// <summary>
			/// Creates an iterator with a value subtracted from position.
			/// </summary>
			/// <param name="i">value to be subtracted</param>
			/// <returns>an iterator with subtracted position</returns>
			__forceinline iterator operator-(int i)
			{
				return iterator(owner, pos - i);
			}

			/// <summary>
			/// Adds a value to the iterators position.
			/// </summary>
			/// <param name="i">value to be added</param>
			/// <returns>reference to the caller</returns>
			__forceinline iterator& operator+=(int i)
			{
				pos += i;
				return *this;
			}

			/// <summary>
			/// Subtracts a value from the iterators position.
			/// </summary>
			/// <param name="i">value to be subtracted</param>
			/// <returns>reference to the caller</returns>
			__forceinline iterator& operator-=(int i)
			{
				pos -= i;
				return *this;
			}

			/// <summary>
			/// Creates an iterator with a value added to position.
			/// </summary>
			/// <param name="i">value to be added</param>
			/// <returns>an iterator with added position</returns>
			__forceinline iterator operator+(size_t i)
			{
				return iterator(owner, pos + i);
			}

			/// <summary>
			/// Creates an iterator with a value subtracted from position.
			/// </summary>
			/// <param name="i">value to be subtracted</param>
			/// <returns>an iterator with subtracted position</returns>
			__forceinline iterator operator-(size_t i)
			{
				return iterator(owner, pos - i);
			}

			/// <summary>
			/// Adds a value to the iterators position.
			/// </summary>
			/// <param name="i">value to be added</param>
			/// <returns>reference to the caller</returns>
			__forceinline iterator& operator+=(size_t i)
			{
				pos += i;
				return *this;
			}

			/// <summary>
			/// Subtracts a value from the iterators position.
			/// </summary>
			/// <param name="i">value to be subtracted</param>
			/// <returns>reference to the caller</returns>
			__forceinline iterator& operator-=(size_t i)
			{
				pos -= i;
				return *this;
			}

			/// <summary>
			/// Accesses the element on current iterator's position in vector.
			/// </summary>
			/// <returns>reference to the element at current position</returns>
			__forceinline _Type& operator*()
			{
				//printf("*\n");
				return owner.data_[pos];
			}

			/// <summary>
			/// Compares iterator position with other's.
			/// </summary>
			/// <param name="other">iterator to be compared</param>
			/// <returns>true if positions are equal, otherwise false</returns>
			__forceinline bool operator==(const iterator& other) const
			{
				return pos == other.pos;
			}

			/// <summary>
			/// Compares iterator position with other's.
			/// </summary>
			/// <param name="other">iterator to be compared</param>
			/// <returns>true if positions are not equal, otherwise false</returns>
			__forceinline bool operator!=(const iterator& other) const
			{
				//printf("!=\n");
				return pos != other.pos;
			}

			/// <summary>
			/// Compares iterator position with other's.
			/// </summary>
			/// <param name="other">iterator to be compared</param>
			/// <returns>true if position is lower than other's, otherwise false</returns>
			__forceinline bool operator<(const iterator& other) const
			{
				return pos < other.pos;
			}

			/// <summary>
			/// Compares iterator position with other's.
			/// </summary>
			/// <param name="other">iterator to be compared</param>
			/// <returns>true if position is greater than other's, otherwise false</returns>
			__forceinline bool operator>(const iterator& other) const
			{
				return pos > other.pos;
			}

			/// <summary>
			/// Compares iterator position with other's.
			/// </summary>
			/// <param name="other">iterator to be compared</param>
			/// <returns>true if position is lower or equal than other's, otherwise false</returns>
			__forceinline bool operator<=(const iterator& other) const
			{
				return pos <= other.pos;
			}

			/// <summary>
			/// Compares iterator position with other's.
			/// </summary>
			/// <param name="other">iterator to be compared</param>
			/// <returns>true if position is greater or equal than other's, otherwise false</returns>
			__forceinline bool operator>=(const iterator& other) const
			{
				return pos >= other.pos;
			}

			/// <summary>
			/// Assigns parameters of iterator to other's.
			/// </summary>
			/// <param name="other">the assigned iterator</param>
			/// <returns>reference to the caller</returns>
			__forceinline iterator& operator=(const iterator& other)
			{
				pos = other.pos; return *this;
			}

			/// <summary>
			/// Checks whether the iterator's position is valid for the owner vector.
			/// </summary>
			/// <returns>true if the owner's size is not exceeded, otherwise false</returns>
			__forceinline operator bool() const
			{
				return pos < owner.size_;
			}

			/// <summary>
			/// Checks whether the iterator's position is not valid for the owner vector.
			/// </summary>
			/// <returns>true if the owner's size is exceeded, otherwise false</returns>
			__forceinline bool operator !() const
			{
				return pos >= owner.size_;
			}

			/// <summary>
			/// Allows to call a member function or access a member of an element on current position directly.
			/// </summary>
			/// <remarks>
			/// Enables behavior of the iterator as if it was a pointer to element contained in vector.
			/// </remarks>
			__forceinline auto operator->()
			{
				return x::pointer(owner.data_[pos]);
			}

			/// <summary>
			/// Allows to cast iterator to const_iterator.
			/// </summary>
			__forceinline operator const_iterator& ()
			{
				return reinterpret_cast<const_iterator&>(*this);
			}

			/// <summary>
			/// Allows to cast iterator to const_iterator.
			/// </summary>
			__forceinline operator const_iterator const& () const
			{
				return reinterpret_cast<const_iterator const&>(*this);
			}
		};


		////////////////////////////////////////////////////////////////////////////
		
		/// <summary>
		/// Gets iterator pointing to the first element.
		/// </summary>
		/// <returns>iterator with position 0</returns>
		__forceinline iterator begin()
		{
			return iterator(*this, 0);
		}


		/// <summary>
		/// Gets iterator pointing to the element 1 place beyond the last.
		/// </summary>
		/// <returns>iterator with position equal to size</returns>
		__forceinline iterator end()
		{
			return iterator(*this, size_);
		}


		/// <summary>
		/// Gets const_iterator pointing to the first element.
		/// </summary>
		/// <returns>iterator with position 0</returns>
		__forceinline const_iterator cbegin() const
		{
			return const_iterator(*this, 0);
		}


		/// <summary>
		/// Gets const_iterator pointing to the element 1 place beyond the last.
		/// </summary>
		/// <returns>iterator with position equal to size</returns>
		__forceinline const_iterator cend() const
		{
			return const_iterator(*this, size_);
		}

		

		////////////////////////////////////////////////////////////////////////////


	private:
		_vector(size_t capacity, size_t requestSize):
			reserved_{capacity ? capacity : defaultAlloc_},
			size_{requestSize <= capacity ? requestSize : capacity},
			data_{allocator<_Type>::raw_allocate(reserved_)}
		{
#if _DEBUG_CTOR == 1
			std::cout << "size ctor\n";
#endif
		}

	public:
		/// <summary>
		/// Constructs an empty vector.
		/// </summary>
		/// <remarks>
		/// The capacity of allocated memory is set to default allocation size which can be set using function
		/// set_default_alloc or viewed by calling default_alloc(). The size is set to 0, so no elements 
		/// are accessible.
		/// </remarks>
		_vector():
			reserved_{defaultAlloc_},
			size_(0),
			data_(allocator<_Type>::raw_allocate(reserved_))
		{
#if _DEBUG_CTOR == 1
			std::cout << "def ctor\n";
#endif
		}


		/// <summary>
		/// Constructs an empty vector allocating memory of given capacity.
		/// </summary>
		/// <remarks>
		/// If \a size argument was 0, memory of size 1 is automatically allocated. The size is set to 0, 
		/// so no elements are accessible.
		/// </remarks>
		/// <param name="capacity">the capacity of allocated memory</param>
		_vector(size_t capacity):
			reserved_{x::max(capacity, MIN_ALLOC_)},
			size_{0},
			data_{allocator<_Type>::raw_allocate(reserved_)}
		{
#if _DEBUG_CTOR == 1
			std::cout << "size ctor\n";
#endif
		}


		/// <summary>
		/// Constructs a vector copying elements from given array of known size. 
		/// </summary>
		/// <remarks>
		/// Memory of capacity equal to given \a size + \a extra is allocated. The \a extra amount of memory
		/// is 0 by default. This constructor assumes that the size of memory block is known and does not
		/// provide any security in case of invalid value or inaccessibility of the memory.
		/// If the \a arr pointer is nullptr, memory of size \a extra is allocated, or default size if it is 0. 
		/// </remarks>
		/// <param name="arr">array to copy from</param>
		/// <param name="size">the size of array</param>
		/// <param name="extra">additional memory to be allocated</param>
		_vector(_Type* arr, size_t size, size_t extra = 0):
			reserved_{arr ? (size + extra) : (extra ? extra : defaultAlloc_)},
			size_{arr ? size : 0},
			data_{allocator<_Type>::raw_allocate(reserved_)}
		{
#if _DEBUG_CTOR == 1
			std::cout << "dynamic arr ctor\n";
#endif
			if (size_)
				allocator<_Type>::raw_copy(data_, arr, size);
		}

		/*_vector(_Type* arr, size_t size, size_t extra = 0):
			data_(allocator<_Type>::raw_allocate(size + extra)),
			size_(size),
			reserved_{size_+extra}
		{
			allocator<_Type>::raw_copy(data_, arr, size_);
		}*/

		/// <summary>
		/// Constructs a vector copying elements from given static array of known size. 
		/// </summary>
		/// <remarks>
		/// This operation is safer than constructing a vector from dynamic array. 
		/// Memory of capacity equal to \a _size is allocated.
		/// </remarks>
		/// <param name="_size">deduced size of static array</param>
		/// <param name="arr">array to copy from</param>
		template<size_t _size>
		_vector(const _Type(&arr)[_size]):
			reserved_{_size},
			size_{_size},
			data_{allocator<_Type>::raw_allocate(_size)}
		{
#if _DEBUG_CTOR == 1
			std::cout << "static arr ctor\n";
#endif
			allocator<_Type>::raw_copy(data_, static_cast<const _Type*>(arr), _size);
		}

		//template<class Y = _Type, class = enable_if<std::is_same<decay<Y>, char>::value>>
		//_vector(const _Type* arr):
		//	data_{allocator<_Type>::allocate(strlen(arr) + 1)},
		//	size_{reserved_}
		//{
		//	copy_(data_, arr, reserved_);
		//}

		/// <summary>
		/// Constructs a vector initializing explicitly given elements from initializer_list.
		/// </summary>
		/// <remarks>
		/// All elements fro the list are accessible after the constructor. If the list was empty, memory
		/// of default size is allocated.
		/// </remarks>
		/// <param name="list"></param>
		_vector(std::initializer_list<_Type> list):
			_vector(list.size(), list.size())
		{
#if _DEBUG_CTOR == 1
			std::cout << "init_list\n";
#endif
			if (size_)
				allocator<_Type>::raw_copy(data_, list.begin(), size_);
			////std::cout << "init_list ctor\n";
			/*int k = 0;
			for (auto i = list.begin(); i != list.end(); ++i, ++k) {
				allocator<_Type>::construct(data_ + k, *i);
			}*/
		}

		/*template<class... Y,
		class U = enable_if<every_is<_Type, Y...>::value>>
		vector_(Y... elements) :
		size_(sizeof...(elements)),
		reserved_(sizeof...(elements))
		{
		data_ = new _Type[size_]{elements...};
		}*/

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <remarks>
		/// Copies data of the argument vector to the constructed one.
		/// If the argument vector is empty, memory of default capacity is allocated.
		/// </remarks>
		/// <param name="other">vector to be copied from</param>
		_vector(const _vector<_Type>& other):
			reserved_{other.size_ ? other.size_ : defaultAlloc_},
			size_{other.size_ ? other.size_ : 0},
			data_{allocator<_Type>::raw_allocate(reserved_)}
		{
#if _DEBUG_CTOR == 1
			std::cout << "copy ctor\n";
#endif
			if (size_) 
				allocator<_Type>::raw_copy(data_, other.data_, size_);
		}


		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <remarks>
		/// Fields are initialized to the other vector's values. The argument vector is made
		/// permanently unusable. This constructor does not copy any element, just sets the handled data
		/// and parameters. It will be called if <code>move()</code> method is called on vector passed
		/// as argument or a rvalue reference was passed to the constructor. Also due to what standard says
		/// this constructor will be used when vector is returned from function so one should not bother
		/// about unnecessary copying of returned data because it will not happen.
		/// </remarks>
		/// <param name="other">the vector to be moved</param>
		_vector(_vector<_Type>&& other):
			data_{other.data_},
			size_{other.size_},
			reserved_{other.reserved_}
		{
			other.data_ = nullptr;
#if _DEBUG_CTOR == 1
			std::cout << "move ctor\n";
#endif
		}

		/*_vector(moved const& movedData):
			data_{movedData.data_},
			size_{movedData.size_},
			reserved_{movedData.reserved_}
		{
		}
*/
		/// <summary>
		/// Constructs a vector copying data from an instance of std::vector.
		/// </summary>
		/// <remarks>
		/// Constructed vector will contain non-trivial copy of all elements from the other.
		/// If the argument vector is empty, memory of default size is allocated.
		/// </remarks>
		/// <param name="vec">the std::vector to copy data from</param>
		explicit _vector(const std::vector<_Type>& vec):
			_vector(vec.size(), vec.size())
		{
#if _DEBUG_CTOR == 1
			std::cout << "std::vec ctor\n";
#endif
			if (size_) 
				allocator<_Type>::raw_copy(data_, vec.data(), size_);
		}


		/// <summary>
		/// Constructs a vector without allocating any memory.
		/// </summary>
		/// <remarks>
		/// This is special purpose constructor and can be used e.g. to instantiate a vector before adopting
		/// some external array. Any operation except adopt should not be performed on such created vector,
		/// however the vector can be safely destructed.
		/// </remarks>
		/// <param name="flag">NoAlloc object, generally accessed by <code>x::NO_ALLOC</code></param>
		explicit _vector(NoAlloc flag):
			reserved_{0},
			size_{0},
			data_{nullptr}
		{
#if _DEBUG_CTOR == 1
			std::cout << "noalloc ctor\n";
#endif
		}


		/// <summary>
		/// Constructs a vector adopting existing array of known size.
		/// </summary>
		/// <remarks>
		/// Sets the vector's handled data to existing array instead of allocating memory and copying from
		/// that array. 
		/// </remarks>
		/// <param name="flag">Adopt object, generally accessed by <code>x::ADOPT</code></param>
		/// <param name="data">the array to be adopted</param>
		/// <param name="size">the size of array</param>
		explicit _vector(Adopt flag, _Type* data, size_t size):
			reserved_{size},
			size_{size},
			data_{data}
		{
#if _DEBUG_CTOR == 1
			std::cout<< "adopt ctor\n";
#endif
		}


		_vector(_vector<_Type> const& other, size_t begin, size_t end):
			reserved_{(end >= begin && end < other.size_) ? end - begin + 1 : defaultAlloc_},
			size_{reserved_ ? reserved_ : 0},
			data_{allocator<_Type>::raw_allocate(reserved_)}
		{
			std::cout << "range ctor\n";
#if _DEBUG_CTOR == 1
#endif
			if (size_ > 0)
				allocator<_Type>::raw_copy(data_, other.data_ + begin, size_);
		}


		_vector(const_iterator const& begin, const_iterator const& end):
			reserved_{(end.pos >= begin.pos && end.pos < begin.owner.size_) ? end.pos - begin.pos + 1 : defaultAlloc_},
			size_{reserved_ ? reserved_ : 0},
			data_{allocator<_Type>::raw_allocate(reserved_)}
		{
			if (size_ > 0)
				allocator<_Type>::raw_copy(data_, begin.owner.data_ + begin.pos, size_);
		}

		////////////////////////////////////////////////////////////////////////////


		/// <summary>
		/// Returns number representing nesting of vector which is grater by 1 if the type parameter is
		/// also a vector. E.g. <code> vector&ltvector&ltvector&ltint&gt&gt&gt </code> will be of dimension 3.
		/// </summary>
		/// <returns>The number of dimensions</returns>
		__forceinline static constexpr size_t dim()
		{
			return DIM_;
		}


		/// <summary>
		/// Returns pointer to currently allocated and handled data. It should not be used
		/// to modify the vector's data in any way, only for iteration or informational purposes.
		/// One must be aware that the pointer may be invalid after some modifications of the vector
		/// due to memory reallocation.
		/// </summary>
		/// <returns>the pointer to allocated data</returns>
		__forceinline _Type const* data() const
		{
			return data_;
		}


		/// <summary>
		/// Gets pointer to so far and handled data and allocates new memory of the previous size.
		/// The resulting pointer can be safely used for any modifications and will no longer be
		/// used by the caller vector.
		/// </summary>
		/// <returns>the pointer to previous data</returns>
		_Type* extract_data()
		{
			_Type* prev = data_;
			reserved_ = defaultAlloc_;
			data_ = allocator<_Type>::allocate(reserved_);
			size_ = 0;
			return prev;
		}


		/// <summary>
		/// Forgets the currently handled data setting it to nullptr and making the vector unusable.
		/// </summary>
		/// <returns>
		/// This function can be used when a vector was used to handle adopted data. At the end of life of 
		/// vector the data can be discarded and then the destructor will not try to deallocate the data, 
		/// which would cause a runtime error if it was deallocated externally. 
		/// </returns>
		void discard_data()
		{
			data_ = nullptr;
			size_ = 0;
			reserved_ = 0;
		}


		/// <summary>
		/// Deallocates current memory and sets the given pointer as its data. 
		/// The pointer passed as argument should not be used later outside the caller vector.
		/// </summary>
		/// <param name="data">the new memory block to handle</param>
		/// <param name="size">the size of given memory block</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& adopt(_Type* data, size_t size)
		{
			if (data_) allocator<_Type>::deallocate(data_, size_);
			reserved_ = size;
			size_ = size;
			data_ = data;
			return *this;
		}


		/// <summary>
		/// Gets current size of the vector, that means amount of accessible elements.
		/// </summary>
		/// <returns>the current vector size</returns>
		__forceinline size_t size() const
		{
			return size_;
		}


		/// <summary>
		/// Equivalent to calling size() - 1. Gets the last valid and accessible position of the
		/// vector that can be safely used as argument to operator[].
		/// </summary>
		/// <returns>the last accessible position</returns>
		__forceinline size_t lastpos() const
		{
			return size_ - 1;
		}


		/// <summary>
		/// Returns the current total number of elements for which memory is allocated.
		/// </summary>
		/// <returns>the current memory capacity</returns>
		__forceinline size_t capacity() const
		{
			return reserved_;
		}


		/// <summary>
		/// Equivalent of capacity() * sizeof(_Type). Gets the amount of memory currently 
		/// allocated by the vector in bytes.
		/// </summary>
		/// <returns>the byte size of allocated memory</returns>
		__forceinline size_t memory() const
		{
			return reserved_*ELEMENT_SIZE_;
		}


		/// <summary>
		/// Gets the current capacity of memory that will be defaultly allocated e.g.
		/// by constructors and some functions that clear, reduce the size of vector 
		/// or reallocate memory in every case when the new desired memory capacity is not specified.
		/// This value is static and so is the same for all vectors of the same.
		/// </summary>
		/// <returns>the current default vector capacity</returns>
		/// <see cref="set_default_alloc"/>
		__forceinline static size_t default_alloc()
		{
			return defaultAlloc_;
		}


		/// <summary>
		/// Sets the current capacity of memory that will be defaultly allocated e.g.
		/// by constructors and some functions that clear, reduce the size of vector 
		/// or reallocate memory in every case when the new desired memory capacity is not specified.
		/// This value is static and so is the same for all vectors of the same.
		/// </summary>
		/// <param name="defSize">the new default vector capacity, DEFAULT: global default capacity = 10</param>
		/// <see cref="default_alloc"/>
		__forceinline static void set_default_alloc(size_t defSize = DEFAULT_ALLOC_)
		{
			defaultAlloc_ = defSize;
		}


		/// <summary>
		/// Gets current value of allocation multiplier, a factor by which the current capacity will be 
		/// multiplied if a memory reallocation is needed.
		/// This value is static and so is the same for all vectors of the same.
		/// </summary>
		/// <returns>the current reallocation capacity multiplier</returns>
		/// <see cref="set_alloc_mult"/>
		__forceinline static double alloc_mult()
		{
			return allocMult_;
		}


		/// <summary>
		/// Gets current value of allocation multiplier, a factor by which the current capacity will be
		/// multiplied if a memory reallocation is needed.
		/// This value is static and so is the same for all vectors of the same.
		/// </summary>
		/// <param name="allocMult">the new reallocation capacity multiplier, 
		///		DEFAULT: default reallocation multiplier = 2.0</param>
		/// <see cref="alloc_mult"/>
		static void set_alloc_mult(double alm)
		{
			if (alm > 1.0) {
				allocMult_ = alm;
			}
			else {
				throw 0; // TODO: exception
			}
		}


		/// <summary>
		/// Checks whether the vector contains any accessible element.
		/// </summary>
		/// <returns>true if the vector contains any accessible elements, otherwise false</returns>
		__forceinline operator bool() const
		{
			return data_ != nullptr && size_ > 0;
		}


		///// <summary>
		///// Introduces behavior as if vector of pointers to base class objects was a vector of 
		///// pointers to derived class objects and enables such cast.
		///// </summary>
		///// <remarks>
		///// This cast operator can be safely used only in class hierarchies with non-virtual inheritance.
		///// It returns reference to the caller object with the data cast to array of pointers to desired class.
		///// Every operation done on the cast reference will be actually performed on the original vector.
		///// </remarks>
		//template<class _Other, _capture(_Type), 
		//	_concept<std::is_pointer<_Type>::value && 
		//		std::is_base_of<x::naked<_Other>, x::naked<_Type>>::value &&
		//		!std::>>
		//__forceinline operator _vector<_Other*>& () noexcept
		//{
		//	return reinterpret_cast<_vector<_Other*>&>(*this);
		//}


		//template<class _Other, _capture(_Type), 
		//	_concept<std::is_pointer<_Type>::value && 
		//		std::is_base_of<x::naked<_Other>, x::naked<_Type>>::value>>
		//__forceinline operator _vector<_Other*> const& () const noexcept
		//{
		//	return reinterpret_cast<_vector<_Other*> const&>(*this);
		//}


		/// <summary>
		/// Casts the vector to an lvalue reference. This function can find usage if one wants to
		/// increase readability of code while passing object that is known to be lvalue reference
		/// or to force adopting content by other vector, rather than copying it when passing the caller
		/// to the move constructor. When the vector's data is moved the caller object should no longer
		/// be used unless reserve is called and in result new memory is allocated.
		/// </summary>
		/// <returns>an lvalue reference to the caller</returns>
		__forceinline _vector<_Type>&& move()
		{
			return (_vector<_Type>&&)(*this);
		}

		/// <summary>
		/// Gets reference to the element at specified position.
		/// This function does not validate the position, only tries to access an element of the data array.
		/// For safe access to an element use at().
		/// </summary>
		/// <param name="pos">poition of the element to get</param>
		/// <returns>the element at given position</returns>
		/// <see cref="at"/>
		__forceinline _Type& operator[](size_t pos) const
		{
			return data_[pos];
		}

		/// <summary>
		/// Creates a copy of the caller vector and appends the right-hand operand, whose elements
		/// are copied to the resulting vector.
		/// </summary>
		/// <param name="other"></param>
		/// <returns>copied vector with appended copied right-hand opperand vector</returns>
		/// <see cref="operator+="/>
		/// <see cref="append"/>
		_vector<_Type> operator+(const _vector<_Type>& other) const
		{
			_vector<_Type> result(*this);
			return result.append(other);
		}


		/// <summary>
		/// Forces the use of memcmp to compare the vector to given for equality of the date.
		/// </summary>
		/// <remarks>
		/// It returns false without checking memory if the sizes are different.
		/// </remarks>
		/// <param name="other">the vector to be compared to</param>
		/// <returns>true if the vectors' memory is equal, otherwise false</returns>
		bool memcmp(const _vector<_Type>& other) const
		{
			if (size_ == other.size_)
				return ::memcmp(data_, other.data_, size_*ELEMENT_SIZE_) == 0;
			return false;
		}


		///////////////////////////////////////////////////////////////////////////////
		
	private:
		template<_capture(_Type)>
		inline std::enable_if_t<std::is_pod<_Type>::value,
			bool> equal_to_(const _vector<_Type>& other) const
		{
			return this->memcmp(other);
		}

		template<_capture(_Type)>
		std::enable_if_t<!std::is_pod<_Type>::value,
			bool> equal_to_(const _vector<_Type>& other) const
		{
			if (size_ != other.size_)
				return false;

			for (size_t i = 0; i<size_; ++i) {
				if (!(data_[i] == other.data_[i]))
					return false;
			}
			return true;
		}
		
	public:
		/// <summary>
		/// Uses operator== to compare each element. If the size of given vector is different than
		/// the size of the caller, this function automatically returns false.
		/// For trivial types this function does the same as memcmp.
		/// </summary>
		/// <param name="other">the vector to be compared to</param>
		/// <returns>true if each element is equal, otherwise false</returns>
		__forceinline bool equal_to(_vector<_Type> const& other) const
		{
			return equal_to_(other);
		}

		/// <summary>
		/// Uses operator== to compare each element. If the size of given vector is different than
		/// the size of the caller, this function automatically returns false.
		/// </summary>
		/// <typeparam name="_Func">deduced type of the comparator</typeparam>
		/// <param name="other">the vector to be compared to</param>
		/// <param name="comparator">object or function comparing two elements</param>
		/// <returns>true if each element is equal, otherwise false</returns>
		template<class _Func>
		bool equal_to(_vector<_Type> const& other, _Func&& comparator) const
		{
			if (size_ != other.size_)
				return false;
			
			for (size_t i = 0; i<size_; ++i) {
				if (!comparator(data_[i], other.data_[i]))
					return false;
			}
			return true;
		}

		///////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Compares the vector to another for equality of all elements.
		/// </summary>
		/// <remarks>
		/// Equivalent to <code>equal_to(other)</code>. If the size of given vector is different than
		/// the size of the caller, this function automatically returns false.
		/// </remarks>
		/// <param name="other">the vector to be compared to</param>
		/// <returns>true if the vectors' memory is equal, otherwise false</returns>
		/// <see cref="equal_to"/>
		__forceinline bool operator==(_vector<_Type> const& other) const
		{
			return equal_to(other);
		}


		/// <summary>
		/// Compares the vector to another for non-equality of any elements. 
		/// </summary>
		/// <remarks>
		/// Equivalent to <code>! equal_to(other)</code>. If the size of given vector is different than
		/// the size of the caller, this function automatically returns false.
		/// </remarks>
		/// <param name="other">the vector to be compared to</param>
		/// <returns>true if the vectors' memory is not equal, otherwise false</returns>
		__forceinline bool operator!=(_vector<_Type> const& other) const
		{
			return !equal_to(other);
		}


		/// <summary>
		/// Gets reference to the element at specified position wrapped in x::result object.
		/// If the position is greater than the maximum accessible, the function returns
		/// x::result of invalid state.
		/// </summary>
		/// <param name="pos">poition of the element to get</param>
		/// <returns>the element at given position</returns>
		/// <see cref="operator[]"/>
		/// <see cref="last"/>
		/// <see cref="first"/>
		result<_Type&> at(size_t n) const
		{
			if (n < size_)
				return data_[n];
			return result<_Type&>::INVALID;
		}


		/// <summary>
		/// Gets reference to the first element wrapped in x::result object.
		/// If the size of vector is 0, the function returns x::result of invalid state.
		/// </summary>
		/// <returns>the first element</returns>
		/// <see cref="at"/>
		/// <see cref="operator[]"/>
		/// <see cref="last"/>
		inline result<_Type&> first() const
		{
			if (size_>0)
				return data_[0];
			return result<_Type&>::INVALID;
		}


		/// <summary>
		/// Gets reference to the last element wrapped in x::result object.
		/// If the size of vector is 0, the function returns x::result of invalid state.
		/// </summary>
		/// <returns>the last element</returns>
		/// <see cref="at"/>
		/// <see cref="operator[]"/>
		/// <see cref="first"/>
		inline result<_Type&> last() const
		{
			if (size_ > 0)
				return data_[size_ - 1];
			return x::result<_Type&>::INVALID;
		}


		/// <summary>
		/// Swaps the data pointer and size parameters with other vector. This operation is fast and
		/// does not copy any allocated data.
		/// </summary>
		/// <param name="other">the vector to swapped with</param>
		inline void swap(_vector<_Type>& other)
		{
			allocator<_vector<_Type>>::swap(*this, other);
		}


		/// <summary>
		/// Swaps elements at given positions. This operation does not perform any deep copying,
		/// only raw byte form of the elements are copied. No constructor or assignment operator is called.
		/// </summary>
		/// <param name="pos1">position of the first element fo be swapped</param>
		/// <param name="pos2">position of the second element fo be swapped</param>
		/// <returns>reference to the caller</returns>//swaps two elements
		__forceinline _vector<_Type>& swap(size_t pos1, size_t pos2)
		{
			allocator<_Type>::swap(data_[pos1], data_[pos2]);
			return *this;
		}


		/// <summary>
		/// Constructs a new vector containing copied elements of the caller vector beginning from 
		/// the first specified position and ending at the second position inclusively.
		/// If the vector is empty or \a begin is greater than \a end the resulting vector will be empty.
		/// </summary>
		/// <param name="begin">beginning of range</param>
		/// <param name="end">end of range</param>
		/// <returns>new vector with elements from given range</returns>
		_vector<_Type> subset(size_t begin, size_t end) const
		{
			if (size_ && (end >= begin)) {
				_vector<_Type> sub(end - begin + 1, true);
				allocator<_Type>::raw_copy(sub.data_, data_ + begin, sub.size_);
				return sub;
			}
			return _vector<_Type>();
		}


		/// <summary>
		/// Constructs a new vector containing copied elements of the caller vector beginning from 
		/// the specified position and ending at the last element.
		/// If the vector is empty or \a begin is greater than the last position 
		/// the resulting vector will be empty
		/// </summary>
		/// <param name="begin">beginning of range</param>
		/// <returns>new vector with elements from given range</returns>
		/// <see cref="subset"/>
		__forceinline _vector<_Type> subset(size_t begin) const
		{
			if (size_) {
				_vector<_Type> sub(size_ - begin, true);
				allocator<_Type>::raw_copy(sub.data_, data_ + begin, sub.size_);
				return sub;
			}
			return _vector<_Type>();
		}


		/// <summary>
		/// Removes every element beyond given range. If \a newBegin is greater or equal to \a newEnd, 
		/// the vector is cleared. If \a newEnd is greater or equal to the size of vector, 
		/// it is trimmed to the last accessible position.
		/// </summary>
		/// <param name="newBegin">position of te first element in the resulting vector</param>
		/// <param name="newEnd">position of te last element in the resulting vector</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& cut(size_t newBegin, size_t newEnd)
		{
			if (newEnd >= size_) newEnd = size_ - 1;
			if ((int)newEnd - (int)newBegin > 0) {
				allocator<_Type>::destruct(data_, newBegin);
				allocator<_Type>::destruct(data_ + newEnd + 1, size_ - newEnd - 1);
				memcpy(data_, data_ + newBegin, ELEMENT_SIZE_*(newEnd - newBegin + 1));
				size_ = newEnd - newBegin + 1;
			}
			else size_ = 0;

			return *this;
		}


		/// <summary>
		/// Removes every element in given range inclusively. 
		/// If \a p2 is greater than \a p1, the function does nothing. If \a p2 is greater or equal to 
		/// the size of vector, it is trimmed to the last accessible position.
		/// </summary>
		/// <param name="p1">position of te first element to be removed</param>
		/// <param name="p2">position of te last element to be removed</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& erase(size_t p1, size_t p2)
		{
			if (size_ && (p2 > p1)) {
				memcpy(data_ + p1, data_ + p2 + 1, (size_ - p2 - 1)*ELEMENT_SIZE_);
				size_ -= (p2 - p1 + 1);
			}
			return *this;
		}


		/// <summary>
		/// Removes one specified element. If the element's position is greater than the last accessible
		/// position, the function does nothing.
		/// </summary>
		/// <param name="pos">position of the element to be removed</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& erase(size_t pos)
		{
			if (pos < size_) {
				allocator<_Type>::destruct(data_[pos]);
				//destroy_data_[n];
				memcpy(data_ + pos, data_ + pos + 1,
					(size_ - pos - 1)*ELEMENT_SIZE_);
				--size_;
			}
			return *this;
		}


		/// <summary>
		/// Uses already allocated memory to fill it with copies of the same element.
		/// The size of vector is not changed.
		/// </summary>
		/// <param name="elem">an element to be filled with</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& fill(const _Type& elem)
		{
			for (size_t i = 0; i < size_; ++i) {
				allocator<_Type>::reconstruct(data_ + i, elem);
			}
			return *this;
		}

		
		/// <summary>
		/// Fills the vector with copies of the same element. Allocates new memory 
		/// of given size if needed or uses the existing memory if it is enough.
		/// The size of vector is set to given.
		/// </summary>
		/// <param name="elem">an element to be filled with</param>
		/// <param name="size">new size of the vector</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& fill(const _Type& elem, size_t size)
		{
			clear_assert_size_(size);

			for (size_t i = 0; i < size_; ++i) {
				allocator<_Type>::construct(data_ + i, elem);
			}
			return *this;
		}

	private:
		void clear_assert_size_(size_t newSize) 
		{
			if (reserved_ < newSize) {
				if (data_)
					allocator<_Type>::deallocate(data_, size_);
				reserved_ = newSize;
				data_ = allocator<_Type>::raw_allocate(reserved_);
			}
			else {
				allocator<_Type>::destruct(data_, size_);
			}
			size_ = newSize;
		}


	public:
		/// <summary>
		/// Fills the vector in specified rage with copies of the same element. 
		/// Allocates new memory if the \a end position exceeds the size of vector.
		/// If both \a begin and \a end positions exceed the size of vector the behaviour is undefined.
		/// If \a begin is greater than or equal to \a end, the function does nothing.
		/// The size of vector is set to given.
		/// </summary>
		/// <param name="elem">an element to be filled with</param>
		/// <param name="begin">the beginning of fill range</param>
		/// <param name="end">the end of fill range</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& fill(const _Type& elem, size_t begin, size_t end)
		{
			if (begin >= end) return *this;

			size_t fillSize = end - begin + 1;
			if (end < size_) {
				allocator<_Type>::destruct(data_ + begin, fillSize);
				for (size_t i = begin; i < end; ++i) {
					allocator<_Type>::construct(data_ + i, elem);
				}
			}
			else if (begin < size_) {
				allocator<_Type>::destruct(data_ + begin, size_-begin);
			}


			if (end < size_)
			for (size_t i = 0; i < size_; ++i) {
				allocator<_Type>::construct(data_ + i, elem);
			}
			return *this;
		}

		MEMBER_TEST(has_addeq_op_, operator+=);
		MEMBER_TEST(has_add_op_, operator+);

		/// <summary>
		/// Generates the vector's content basing on a given function calculated over a range of
		/// iterable values. The function will be executed on every value from \a begin to
		/// value \a end with delta \a d and the results of execution will become the vector's data.
		/// If there is more memory needed it will be allocated.
		/// If the \a _Iter type has no += operator, this function template will fail to compile.
		/// </summary>
		/// <typeparam name="_Func">deduced type of function, lambda of functor object</typeparam>
		/// <typeparam name="_Iter">type used for iteration over range</typeparam>
		/// <param name="func">function to be executed over range</param>
		/// <param name="begin">the beginning of generation range</param>
		/// <param name="end">the end of generation range</param>
		/// <param name="d">delta in iteration</param>
		/// <returns>reference to the caller</returns>
		template<class _Func, class _Iter>
		_vector<_Type>& generate(_Func&& func, _Iter begin, _Iter end, _Iter d)
		{
			allocator<_Type>::deallocate(data_, size_);
			reserved_ = abs((end - begin) / d) + 1;
			size_ = reserved_;
			data_ = allocator<_Type>::raw_allocate(reserved_);

			size_t i = 0;
			for (_Iter x = begin; i<size_; x += d) {
				allocator<_Type>::reconstruct(data_ + (i++), (_Type&&)func(x));
			}

			return *this;
		}


		/// <summary>
		/// Generates the vector's content basing on a given function calculated over a range of
		/// iterable values. The function will be executed on every value from \a begin with delta \a d 
		/// and the results of execution will become the vector's data of the current size, so
		/// the first element becomes <code>begin</code> and the last becomes <code>begin + d*size()</code>.
		/// No more memory is allocated. If a numeric limit of the \a _Iter type is exceeded,
		/// the behaviour is undefined. If the \a _Iter type has no += operator, this function template 
		/// will fail to compile.
		/// </summary>
		/// <typeparam name="_Func">deduced type of function, lambda of functor object</typeparam>
		/// <typeparam name="_Iter">type used for iteration over range</typeparam>
		/// <param name="func">function to be executed over range</param>
		/// <param name="begin">the beginning of generation range</param>
		/// <param name="d">delta in iteration</param>
		/// <returns>reference to the caller</returns>
		template<class _Func, class _Iter>
		_vector<_Type>& generate(_Func&& func, _Iter begin, _Iter d)
		{
			size_t i = 0;
			for (_Iter x = begin; i<size_; x += d) {
				data_[i++] = func(x);
			}
			return *this;
		}

		//fills content with values <"from", "to"> with delta = "d"
		template<class Y = _Type>
		enable_if<std::is_fundamental<Y>::value,
			_vector<Y>&> generate(Y from, Y to, Y d = 1)
		{
			allocator<_Type>::deallocate(data_, size_);
			reserved_ = abs((to - from) / d) + 1;
			size_ = reserved_;
			data_ = allocator<_Type>::raw_allocate(reserved_);

			size_t i = 0;
			for (Y x = from; i<size_; x += d) {
				data_[i++] = x;
			}
			return *this;
		}

	
		/// <summary>
		/// Equivalent to subset(0, n-1). Creates a vector containing \a n first elements of the caller
		/// vector. If the vector is empty, empty vector is returned. If \a n is greater than
		/// the size of the vector, a full copy is returned.
		/// </summary>
		/// <param name="n">number of elements to copy from the beginning of vector</param>
		/// <returns>vector containing \a n first elements</returns>
		__forceinline _vector<_Type> front(size_t n) const
		{
			return subset(0, n - 1);
		}

		
		/// <summary>
		/// Equivalent to subset(size() - n). Creates a vector containing \a n last elements of the caller
		/// vector. If the vector is empty, empty vector is returned. If \a n is greater than
		/// the size of the vector, a full copy is returned.
		/// </summary>
		/// <param name="n">number of elements to copy from the end of vector</param>
		/// <returns>vector containing \a n last elements</returns>
		__forceinline _vector<_Type> back(size_t n) const
		{
			return subset(size_ - n);
		}

		
		/// <summary>
		/// The assignment operator. Copies data of right-hand side vector to the left-hand side.
		/// </summary>
		/// <remarks>
		/// If the right-hand side vector is empty, the vector is cleared, no memory is copied nor deallocated.
		/// </remarks>
		/// <param name="other">the vector to be copied</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& operator=(const _vector<_Type> & other)
		{
			if (other.size_) {
				clear_assert_size_(other.size_);
				allocator<_Type>::raw_copy(data_, other.data_, size_);
				return *this;
			}
			else {
				return clear();
			}
		}


		/// <summary>
		/// The move assignment operator. Moves data of right-hand side vector to the left-hand side.
		/// </summary>
		/// <remarks>
		/// Deallocates current memory and sets handled data and parameters of vector to the moved one's.
		/// The moved vector is made permanently unusable.
		/// </remarks>
		/// <param name="other"></param>
		/// <returns></returns>
		_vector<_Type>& operator=(_vector<_Type> && other)
		{
			if (other.reserved_) {
				if (data_)
					allocator<_Type>::deallocate(data_, size_);
				reserved_ = other.reserved_;
				size_ = other.size_;
				data_ = other.data_;
				other.data_ = nullptr;
			}
			else {
				allocator<_Type>::destruct(data_, size_);
				size_ = 0;
			}
			return *this;
		}


		_vector<_Type>& operator=(_Type const& value)
		{
			allocator<_Type>::deallocate(data_, size_);
			size_ = 1;
			reserved_ = 1;
			data_ = allocator<_Type>::allocate(1);
			allocator<_Type>::construct(data_, value);
			return *this;
		}

		
		/// <summary>
		/// Appends a copy of given object at the end of vector.
		/// </summary>
		/// <param name="newElem">element to be added at the end</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& push_back(_Type const& elem)
		{
			if (size_ == reserved_)
				reallocate_(1 + reserved_ * allocMult_);

			allocator<_Type>::construct(data_ + size_++, elem);
			return *this;
		}

		/// <summary>
		/// Appends a copy of given object at the beginning of vector.
		/// </summary>
		/// <param name="newElem">element to be added at the beginning</param>
		/// <returns>reference to the caller</returns>
		__forceinline _vector<_Type>& push_front(_Type const& newElem)
		{
			return insert(newElem, 0);
		}

		template<class... _Arg>
		_vector<_Type>& construct_back(_Arg&&... args)
		{
			if (size_ == reserved_)
				reallocate_(1 + reserved_ * allocMult_);

			allocator<_Type>::inline_construct(data_ + size_++, args...);
			return *this;
		}

		template<class... _Arg>
		__forceinline _vector<_Type>& construct_front(_Arg&&... args)
		{
			return construct(0, args...);
		}

		template<class... _Arg>
		_vector<_Type>& construct(size_t pos, _Arg&&... args)
		{
			if (pos > size_) pos = size_;

			if (size_ == reserved_) {
				reserved_ = 1 + reserved_*allocMult_;
				_Type* newData = allocator<_Type>::raw_allocate(reserved_);
				memcpy(newData + pos + 1, data_ + pos, ELEMENT_SIZE_*(size_ - pos));
				allocator<_Type>::inline_construct(newData + pos, args...);
				memcpy(newData, data_, ELEMENT_SIZE_*pos);
				delete[] reinterpret_cast<byte*>(data_);
				data_ = newData;
			}
			else {
				memcpy(data_ + pos + 1, data_ + pos, ELEMENT_SIZE_*(size_ - pos));
				allocator<_Type>::inline_construct(data_ + pos, args...);
			}
			++size_;
			return *this;
		}

		/// <summary>
		/// Inserts copy of given element at the position \a pos, moving all following elements 
		/// one place forward. If the specified position is greater than size of vector, the 
		/// element is appended at the end of vector so the behavior of function becomas equal 
		/// to push_back. The size of vector is incremented by 1.
		/// </summary>
		/// <param name="newElem">the element to be inserted to vector</param>
		/// <param name="pos">the position to insert new element</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& insert(const _Type& newElem, size_t pos)
		{
			if (pos > size_)
				return push_back(newElem);

			if (size_ == reserved_) {
				reserved_ = 1 + reserved_*allocMult_;
				_Type* newData = allocator<_Type>::raw_allocate(reserved_);
				memcpy(newData + pos + 1, data_ + pos, ELEMENT_SIZE_*(size_ - pos));
				allocator<_Type>::construct(newData + pos, newElem);
				memcpy(newData, data_, ELEMENT_SIZE_*pos);
				delete[] reinterpret_cast<byte*>(data_);
				data_ = newData;
			}
			else {
				memcpy(data_ + pos + 1, data_ + pos, ELEMENT_SIZE_*(size_ - pos));
				allocator<_Type>::construct(data_ + pos, newElem);
			}
			++size_;
			return *this;
		}

		/// <summary>
		/// Inserts copy of given vector at the position \a pos, moving all following elements 
		/// forward number of places equal to \a other's size. If the specified position is greater 
		/// than size of vector, the \a other vector is appended at the end of vector so the behavior 
		/// of function becomas equal to append. The size of vector is incremented by the size of \a other.
		/// </summary>
		/// <param name="other">the vector to be inserted</param>
		/// <param name="pos">the position to insert vector's content</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& insert(const _vector<_Type>& other, size_t pos)
		{
			if (pos > size_)
				return append(other);

			if (reserved_ < size_ + other.size_) {
				reserved_ = size_ + other.size_;
				_Type* newData = allocator<_Type>::raw_allocate(reserved_);
				memcpy(newData + pos + other.size_, data_ + pos, ELEMENT_SIZE_*(size_ - pos));
				allocator<_Type>::raw_copy(newData + pos, other.data_, other.size_);
				memcpy(newData, data_, ELEMENT_SIZE_*pos);
				delete[] reinterpret_cast<byte*>(data_);
				data_ = newData;
			}
			else {
				memcpy(data_ + pos + other.size_, data_ + pos, ELEMENT_SIZE_*(size_ - pos));
				allocator<_Type>::raw_copy(data_ + pos, other.data_, other.size_);
			}
			size_ += other.size_;
			return *this;
		}

	private:
		/*template<class _Verifier>
		int remove_(unsigned max)
		{
			size_t lastback = size_;
			_DEBUG_VAR_DISP(size_);
			_DEBUG_VAR_DISP(lastback);
			size_t nrem = 0;
			_DEBUG_VAR_DISP(nrem);
			for (size_t f = 0; f < lastback; ++f) {
				_DEBUG_VAR_DISP(f);
				if (_Verifier::verify(data_[f])) {
					std::cout << "verified\n";
					++nrem;
					_DEBUG_VAR_DISP(nrem);
					allocator<_Type>::destruct(data_[f]);
					while (--lastback > f) {
						_DEBUG_VAR_DISP(lastback);
						if (!_Verifier::verify(data_[lastback])) {
							std::cout << "verified not\n";
							x::assign(data_ + f, data_ + lastback);							
							break;
						}	
						++nrem;
						_DEBUG_VAR_DISP(nrem);
					}
				}
			}
			
			size_ -= nrem;
			return nrem;
		}*/

		/*template<class _Verifier>
		int remove_(unsigned max)
		{
			size_t lastback = size_;
			size_t nrem = 0;
			for (size_t f = 0; f < lastback; ++f) {
				if (_Verifier::verify(data_[f])) {
					++nrem;
					allocator<_Type>::destruct(data_[f]);
					while (--lastback > f) {
						if (!_Verifier::verify(data_[lastback])) {
							x::assign(data_ + f, data_ + lastback);
							break;
						}
						++nrem;
					}
				}
			}

			size_ -= nrem;
			return nrem;
		}*/

		template<class _Verifier>
		int remove_(_Verifier&& verify, size_t max)
		{
			int i = -1;
			int lastp = -1;
			int nrem = 0;
			int diff;

			while (++i < size_) {
				if (verify(data_[i])) {
					allocator<_Type>::destruct(data_[i]);
					diff = i - nrem;
					if (diff > lastp) {
						memcpy(data_ + lastp,
							data_ + lastp + nrem,
							ELEMENT_SIZE_*(diff - lastp));
						lastp = diff;
					}
					if (++nrem == max) break;
				}
			}
			if (size_ - lastp - nrem) {
				memcpy(data_ + lastp,
					data_ + lastp + nrem,
					ELEMENT_SIZE_*(size_ - lastp - nrem));
			}
			size_ -= nrem;
			return nrem;
		}

		/*template<class _Verifier>
		int remove_(unsigned max)
		{
			int i = -1, lastp = -1, nrem = 0;
			while (++i < size_) {
				if (_Verifier::verify(data_[i])) {
					allocator<_Type>::destruct(data_[i]);

					if (i > lastp + nrem) {
						memcpy(data_ + lastp,
							data_ + lastp + nrem,
							ELEMENT_SIZE_*(i - lastp - nrem));
						lastp = i - nrem;
					}
					if (++nrem == max) break;
				}
			}
			if (size_ - lastp - nrem) {
				memcpy(data_ + lastp,
					data_ + lastp + nrem,
					ELEMENT_SIZE_*(size_ - lastp - nrem));
			}
			size_ -= nrem;
			return nrem;
		}*/

		

	public:
		/// <summary>
		/// Removes every occurence of the given element. The amount of removed elements can be limited
		/// by specifying \a max. The maximum number of elements to be removed set to 0 is equivalent
		/// to no limit, which is default.
		/// </summary>
		/// <param name="val">element to be removed from the vector</param>
		/// <param name="max">maximum number of occurences to be removed, DEFAULT: 0 (no limit)</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& remove(_Type const& val, size_t max = 0)
		{
			remove_(x::ft::compare<_Type const>{val}, max);
			return *this;
		}


		/// <summary>
		/// Removes every occurence of the element for which the specified function returns true.
		/// The amount of removed elements can be limited by specifying \a max. 
		/// The maximum number of elements to be removed set to 0 is equivalent to no limit, which is default.
		/// </summary>
		/// <typeparam name="_Func">deduced type of function</typeparam>
		/// <param name="func">function deciding about removal of elements</param>
		/// <param name="max">maximum number of occurences to be removed, DEFAULT: 0 (no limit)</param>
		template<class _Func>
		_vector<_Type>& remove_if(_Func&& func, size_t max = 0)
		{
			remove_(x::ft::execute<x::naked<_Func>>{func}, max);
			return *this;
		}


		/// <summary>
		/// Removes every occurence of the element for which the specified member is equal to given value.
		/// </summary>
		/// <remarks>
		/// The amount of removed elements can be limited by specifying \a max. 
		/// The maximum number of elements to be removed set to 0 is equivalent to no limit, which is default.
		/// </remarks>
		/// <param name="member">member of each element to be checked</param>
		/// <param name="value">value of the member denoting removal</param>
		/// <param name="max">maximum number of occurences to be removed, DEFAULT: 0 (no limit)</param>
		template<class _Member, _capture(_Type)>
		std::enable_if_t<std::is_class<_Type>::value,
			_vector<_Type>&> remove_by(_Member(_Type::*member), _Member const& value, size_t max = 0)
		{
			remove_(x::ft::compare_member<_Type,_Member>{member, value}, max);
			return (_vector<_Type>&)(*this);
		}

		/// <summary>
		/// Copies every occurence of the element for which the specified function returns true into a new
		/// instance of vector.
		/// </summary>
		/// <remarks>
		/// The amount of copied elements can be limited by specifying \a max. 
		/// The maximum number of elements to be copied set to 0 is equivalent to no limit, which is default.
		/// </remarks>
		/// <typeparam name="_Func">deduced type of function</typeparam>
		/// <param name="func">function deciding about copying of elements</param>
		/// <param name="max">maximum number of occurences to be copied, DEFAULT: 0 (no limit)</param>
		/// <returns>vector containing copied elements</returns>
		template<class _Func>
		_vector<_Type> copy_if(_Func&& func, size_t max = 0)
		{
			_vector<_Type> copied(size_);
			size_t ncop = 0;
			for (int i = 0; i < size_; ++i) {
				if (func(data_[i])) {
					copied.push_back(data_[i]);
					if (++nrem == max) break;
				}
			}
			copied.shrink();
			return copied;
		}


		/// <summary>
		/// Copies every occurence of the element for which the specified function returns true into a new
		/// instance of vector and removes them from the caller vector.
		/// </summary>
		/// <remarks>
		/// The amount of extracted elements can be limited by specifying \a max. 
		/// The maximum number of elements to be extracted set to 0 is equivalent to no limit, which is default.
		/// </remarks>
		/// <typeparam name="_Func">deduced type of function</typeparam>
		/// <param name="func">function deciding about extraction of elements</param>
		/// <param name="max">maximum number of occurences to be extracted, DEFAULT: 0 (no limit)</param>
		/// <returns>vector containing extracted elements</returns>
		template<class _Func>
		_vector<_Type> extract_if(_Func&& func, size_t max = 0)
		{
			_vector<_Type> extracted(size_);
			int i = -1, lastp = -1, nrem = 0;
			while (++i < size_) {
				if (func(data_[i])) {
					extracted.push_back(data_[i]);
					allocator<_Type>::destruct(data_[i]);
					if (i > lastp + nrem) {
						memcpy(data_ + lastp,
							data_ + lastp + nrem,
							ELEMENT_SIZE_*(i - lastp - nrem));
						lastp = i - nrem;
					}
					if (++nrem == max) break;
				}
			}
			if (size_ - lastp - nrem) {
				memcpy(data_ + lastp,
					data_ + lastp + nrem,
					ELEMENT_SIZE_*(size_ - lastp - nrem));
			}
			size_ -= nrem;
			//extracted.shrink();
			return extracted;
		}


		/*/// <summary>
		/// Erases specified element fast, swapping it with the current last element.
		/// The size of vector is decreased by 1 and value of the element that has been 
		/// thrown remains at position of 1 place beyond the last and can be accessed e.g. by 
		/// <code>vec[vec.size()]</code> but it is volatile and any vector transormation changing 
		/// its size, reallocation memory, etc. makes potential access to this element very unsafe
		/// or impossible. The order of contained elements is not preserved. If specified position
		/// is greater than or equal to size of vector, the function does nothing.
		/// </summary>
		/// <param name="which">position of element to be thrown</param>
		/// <returns>reference to the throwed element</returns>*/
		
		/// <summary>
		/// Erases specified element fast, swapping it with the current last element.
		/// The size of vector is decreased by 1 and value of the element that has been 
		/// thrown is not destructed and remains at position of 1 place beyond the last and can be 
		/// accessed e.g. by the reference returned by function or <code>vec[vec.size()]</code> but 
		/// it is volatile and any vector transormation changing its size, reallocation memory, etc. 
		/// makes potential access to this element very unsafe or impossible. 
		/// The order of contained elements is not preserved. If specified position
		/// is greater than or equal to size of vector, the function returns result of invalid state.
		/// </summary>
		/// <param name="which">position of element to be thrown</param>
		/// <returns>reference to the throwed element</returns>
		result<_Type&> throw_back(size_t which)
		{
			if (which < size_) {
				swap(which, --size_);
				return data_[size_];
			}
			return result<_Type&>::INVALID;
		}


		/*/// <summary>
		/// Erases specified element fast by writing value of the last element.
		/// The size of vector is decreased by 1 and the value of removed element is returned.
		/// If the given position is not accessible, the result of invalid state is returned.
		/// </summary>
		/// <param name="which">position of element to be removed</param>
		/// <returns>the value of removed element</returns>
		result<_Type> dump(size_t which)
		{
			if (which < size_) {
				_Type erased = data_[which];
				x::assign(data_ + which, data_ + (--size_));
				return erased;
			}
			else
				return result<_Type>::INVALID;
		}*/

		/// <summary>
		/// Erases specified element fast by writing value of the last element.
		/// The size of vector is decreased by 1.
		/// If the given position is not accessible, this function does nothing.
		/// </summary>
		/// <param name="which">position of element to be removed</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& dump(size_t which)
		{
			if (which < size_) {
				allocator<_Type>::destruct(data_[which]);
				x::assign(data_ + which, data_ + (--size_));
			}
			return *this;
		}

		
		/// <summary>
		/// Removes the first element from vector. If the vector is empty, this function does nothing.
		/// </summary>
		/// <returns>reference to the caller</returns>
		__forceinline _vector<_Type>& pop_front()
		{
			if (size_) {
				allocator<_Type>::destruct(data_[0]);
				erase(0);
			}
			return *this;
		}


		/// <summary>
		/// Removes the last element from vector. If the vector is empty, this function does nothing.
		/// </summary>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& pop_back()
		{
			if (size_) {
				allocator<_Type>::destruct(data_[--size_]);
			}
			return *this;
		}

		
		/// <summary>
		/// Uses quick sort on the whole vector. The elements are sorted in order from the lowest
		/// to the highest by using operator &lt on pairs of them.
		/// This function is fast but does not provide any security, so such issues as stack overflow
		/// can occur if the data is too large.
		/// </summary>
		/// <returns>reference to the caller</returns>
		inline _vector<_Type>& sort()
		{
			quicksort_(0, (int)size_ - 1);
			return *this;
		}


		/// <summary>
		/// Uses quick sort on the specified range. The elements are sorted in order from the lowest
		/// to the highest by using operator &lt on pairs of them.
		/// If \a begin position &gt= \a end, this function does nothing.
		/// If \a end &gt= size of vector, the size of vector - 1 is used as the end of sort range.
		/// This function is fast but does not provide any security, so such issues as stack overflow
		/// can occur if the data is too large.
		/// </summary>
		/// <param name="begin">the beginning of sort range</param>
		/// <param name="end">the end of sort range</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& sort(size_t begin, size_t end)
		{
			if (begin >= end) return *this;
			if (end >= size_) (int&)end = (int)size_ - 1;
			quicksort_((int)begin, (int)end);
			return *this;
		}


		template<class _Func>
		inline _vector<_Type>& sort(_Func&& comparator)
		{
			quicksort_(0, (int)size_ - 1, comparator);
			return *this;
		}


		template<class _Func>
		inline _vector<_Type>& sort(size_t begin, size_t end, _Func&& comparator)
		{
			if (begin >= end) return;
			if (end >= size_) (int&)end = (int)size_ - 1;
			quicksort_((int)begin, (int)end, comparator);
			return *this;
		}

#ifdef _X_RND_H_	
		/// <summary>
		/// Sets elements currently contained in vector in random order. 
		/// If \a swapEach is enabled, every element will be placed somewhere else, otherwise
		/// there is some probability for each element of not being moved.
		/// </summary>
		/// <param name="swapEach">flag denoting that every element will change place, DEFAULT: disabled</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& shuffle(bool swapEach = false)
		{
			for (int i = 0; i < size_ - 1; ++i) {
				swap(i, random<size_t>(i + swapEach, size_ - 1));
			}
			return *this;
		}


		/// <summary>
		/// Gets randomly selected element from specified position range.
		/// </summary>
		/// <param name="begin">the beginning of range</param>
		/// <param name="end">the end of range</param>
		/// <returns>reference to randomly picked element</returns>
		_Type& get_random(size_t begin, size_t end) const
		{
			return data_[random<size_t>(begin, end)];
		}


		/// <summary>
		/// Gets randomly selected element from the whole vector.
		/// Equivalent to <code> get_random(0, size() - 1) </code>.
		/// </summary>
		/// <returns>reference to randomly picked element</returns>
		__forceinline _Type& get_random() const
		{
			return get_random(0, size_ - 1);
		}

		
		/// <summary>
		/// Fills vector with randomly generated elements from specified range.
		/// If there is no implementation of x::random specialization on the type contained in vector,
		/// the function will fail to compile. By default, all fundamental types are possible to
		/// be randomized.
		/// </summary>
		/// <param name="r1">the beginning of randomization range</param>
		/// <param name="r2">the end of randomization range</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& randomize(_Type r1, _Type r2)
		{
			for (int i = 0; i < size_; ++i) {
				allocator<_Type>::reconstruct(data_+i, random<_Type>(r1, r2));
			}
			return *this;
		}


		/// <summary>
		/// Reallocates the memory to given \a size and fills vector with randomly generated elements 
		/// from specified range. If there is no implementation of x::random specialization on the 
		/// type contained in vector, the function will fail to compile. 
		/// By default, all fundamental types are possible to be randomized.
		/// </summary>
		/// <param name="r1">the beginning of randomization range</param>
		/// <param name="r2">the end of randomization range</param>
		/// <param name="size">new size of the vector</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& randomize(_Type r1, _Type r2, size_t size)
		{
			clear_assert_size_(size);
			return randomize(r1, r2);
		}


		/// <summary>
		/// Jitters each element from the current value by randomized factor from range.
		/// </summary>
		/// <remarks>
		/// Multiplies every element by a randomized factor in range from 1.0 +\a r1 to 1.0 +\a r2.
		/// If \a r1 &gt \a r2, this function does nothing.
		/// </remarks>
		/// <param name="r1">minimum of the jitter range</param>
		/// <param name="r2">maximum of the jitter range</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& jitter(double r1, double r2)
		{
			if (r1 > r2) return *this;

			for (int i = 0; i < size_; ++i) {
				data_[i] *= (1 + random<_Type>(r1, r2));
			}
			return *this;
		}
		/*_vector<_Type>& jitter(double r1, double r2)
		{
			double factor; _Type r = r2 - r1;
			for (int i = 0; i < size_; ++i) {
				factor = random<_Type>(-r, r);
				data_[i] *= (1 + (factor + r1*sgn(factor)));
			}
			return *this;
		}*/

		/// <summary>
		/// Jitters each element from the current value by randomized factor from range.
		/// </summary>
		/// <remarks>
		/// Multiplies every element by a randomized factor in range from 1.0 -\a r to 1.0 +\a r.
		/// If \a r1 &gt \a r2, this function does nothing.
		/// Equivalent to <code>jitter(-r, r)</code>.
		/// </remarks>
		/// </summary>
		/// <param name="r">radius of the jitter range</param>
		/// <returns>reference to the caller</returns>
		inline _vector<_Type>& jitter(double r)
		{
			return jitter(-r, r);
		}

#endif

		/// <summary>
		/// Deallocates currently unused memory beyond the minimum required to store accessible elements.
		/// </summary>
		/// <remarks>
		/// Its usage is to manually control the amount of memory used by vector and prevent from 
		/// keeping unused memory e.g. when the vector is known to change size rarely or never,
		/// since functions changing the size or capacity of vector allocate new memory if needed
		/// but do not deallocate excessive memory.
		/// All elements currently contained in vector are preserved and the size does not change.
		/// </remarks>
		/// <returns>reference to the caller</returns>
		inline _vector<_Type>& shrink()
		{
			reallocate_(size_);
			return *this;
		}

		
		/// <summary>
		/// Allocates desired amount of memory if the current capacity of vector is not sufficient.
		/// </summary>
		/// <remarks>
		/// If \a capacity is lower than capacity of currently allocated memory, this function does nothing.
		/// Its usage is to manually control amount of memory handled by the vector for 
		/// optimization purposes, e.g. if there are some operations to be performed on vector
		/// known to increase vector's size. Similar functionality can be achieved by passing 
		/// desired size of vector to constructor but it does not allow to do it on the flow.
		/// </remarks>
		/*/// <example>
		/// <code>x::vector&ltBase*&gt vec;
		/// vec.reserve(200);
		/// for (int i=0; i&lt200; ++i) {
		///		vec.push_back(new Derived{x::random&ltint&gt(0, i)});
		/// }</code>
		/// </example>*/
		/// <param name="capacity">the desired capacity of vector</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& reserve(size_t capacity)
		{
			if (capacity > reserved_)
				reallocate_(capacity);
			
			return *this;
		}


		/// <summary>
		/// Forces the size to be set to given value.
		/// </summary>
		/// <remarks>
		/// If given size is greater than current capacity it is set to the value of capacity.
		/// </remarks>
		/// <param name="size">the value of vector's size</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& force_size(size_t size)
		{
			if (size <= reserved_)
				size_ = size;
			else
				size_ = reserved_;
			return *this;
		}


		/// <summary>
		/// Forces the current maximum value of size, that is equal to capacity.
		/// </summary>
		/// <returns>reference to the caller</returns>
		inline _vector<_Type>& force_size()
		{
			size_ = reserved_;
			return *this;
		}

		/// <summary>
		/// Allocates additional amount of memory.
		/// </summary>
		/// <remarks>
		/// Equivalent to <code>reserve(reserved() + \a capacity) </code>. Asserts that the memory 
		/// will be added, contrary to reserve which will do nothing if the specified capacity does
		/// not extend the current vector's capacity.
		/// </remarks>
		/// <see cref="reserve"/>
		/// <param name="capacity"></param>
		/// <returns></returns>
		inline _vector<_Type>& extend(size_t capacity)
		{
			reserve(reserved_ + capacity);
			return *this;
		}

		/*template<bool useable = false, class Y = _Type, class... N>
		enable_if<_vector<Y>::DIM_ >= 2 &&
			sizeof...(N) <= _vector<Y>::DIM_ &&
			all_true<std::is_convertible<N, size_t>::value...>::value,
			void> reserve(N... sizes)
		{
			std::initializer_list<size_t> sizes_il{size_t(sizes)...};
			reserve_<useable>(sizes_il, sizes_il.begin());
		}*/

		/// <summary>
		/// Copies content of \a other vector and appends it at the end.
		/// </summary>
		/// <param name="other">vector to be appended</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& append(const _vector<_Type>& other)
		{
			if (reserved_ < size_ + other.size_)
				reallocate_(size_ + other.size_);
			allocator<_Type>::raw_copy(data_ + size_, other.data_, other.size_);
			size_ += other.size_;
			return *this;
		}

		////inserts new elements on the end
		//_vector<_Type>& append(std::initializer_list<_Type> other)
		//{
		//	if (reserved_ < size_ + other.size())
		//		reallocate_(size_ + other.size());
		//	int k = size_;
		//	for (class std::initializer_list<_Type>::iterator i = other.begin();
		//		i != other.end(); ++i, ++k) {
		//		data_[k] = _Type(*i);
		//	}
		//	size_ += other.size();
		//	return *this;
		//}

		/// <summary>
		/// Copies content of \a other vector and pepends it at the beginning.
		/// </summary>
		/// <param name="other">vector to be prepended</param>
		/// <returns>reference to the caller</returns>
		__forceinline _vector<_Type>& prepend(const _vector<_Type>& other)
		{
			insert(other, 0);
			return *this;
		}

		//moves content forward and
		//inserts new elements on the beginning
		/*_vector<_Type>& prepend(std::initializer_list<_Type> other)
		{
			if (reserved_<size_ + other.size())
				reallocate_(size_ + other.size());
			memcpy(data_ + other.size(), data_, ELEMENT_SIZE_*size_);
			int k = 0;
			for (class std::initializer_list<_Type>::iterator i = other.begin();
				i != other.end(); ++i, ++k) {
				data_[k] = _Type(*i);
			}
			size_ += other.size();
			return *this;
		}*/

		/// <summary>
		/// Counts number of occurences of given element in vector.
		/// </summary>
		/// <remarks>
		/// If the contained type has no overloaded operator ==, this function will fail to compile.
		/// </remarks>
		/// <param name="elem">value to be counted</param>
		/// <returns>number of occurences of counted value</returns>
		size_t count(const _Type& elem) const
		{
			size_t n = 0;
			for (size_t i = 0; i < size_; ++i) 
				if (data_[i] == elem) ++n;
	
			return n;
		}


		/// <summary>
		/// Counts number of elements fulfilling given \a condition.
		/// </summary>
		/// <remarks>
		/// The \a condition must be a delegate that takes contained type as argument and returns bool or
		/// type convertible to bool, otherwise this function will fail to compile.
		/// </remarks>
		/// <typeparam name="_Func">deducet type of function</typeparam>
		/// <param name="condition">the condition to be checked over elements</param>
		/// <returns>number of elements fulfilling condition</returns>
		template<class _Func>
		size_t count_if(_Func&& condition) const
		{
			size_t n = 0;
			for (size_t i = 0; i < size_; ++i)
				if (condition(data_[i])) ++n;

			return n;
		}

		/// <summary>
		/// Finds first occurence of given element.
		/// </summary>
		/// <remarks>
		/// The resulting reference is wrapped in result object and if the element was not found,
		/// result of invalid state is returned.
		/// If the contained type has no overloaded operator ==, this function will fail to compile.
		/// </remarks>
		/// <param name="elem">element to be found</param>
		/// <returns>reference to found element</returns>
		result<_Type&> find(_Type const& elem) const
		{
			for (size_t i = 0; i < size_; ++i)
				if (data_[i] == elem)
					return data_[i];
			return result<_Type&>::INVALID;
		}


		/// <summary>
		/// Finds first occurence of given element and gets its position in vector.
		/// </summary>
		/// <remarks>
		/// The resulting position is wrapped in result object and if the element was not found,
		/// result of invalid state is returned.
		/// If the contained type has no overloaded operator ==, this function will fail to compile.
		/// </remarks>
		/// <param name="elem">element to be found</param>
		/// <returns>position of found element</returns>
		result<size_t> pos_of(_Type const& elem) const
		{
			for (size_t i = 0; i < size_; ++i)
				if (data_[i] == elem)
					return i;
			return result<size_t>::INVALID;
		}


		/*enable_if<std::is_convertible<Y, U>::value,
			bool> contain(U const& elem) const
		{
			for (size_t i = 0; i < size_; ++i)
				if (compare_(Udata_[i], elem)) return true;
			return false;
		}*/

		//returns reference to element, for which "fn"'s return value is equal to "val"
		/*template<class _Cmp, class _Func>
		result<_Type&> find_by(_Func&& func, _Cmp const& val)
		{
			for (size_t i = 0; i < size_; ++i)
				if (func(data_[i]) == val) return data_[i];
			return result<_Type&>::INVALID;
		}*/

		/// <summary>
		/// Searches the vector for element for which specified condition is fulfilled.
		/// </summary>
		/// <remarks>
		/// The condition can be any delegate which result is or can be converted to bool.
		/// If the vector is empty or no matching element was found, this function
		/// returns result of invalid state.
		/// </remarks>
		/// <typeparam name="_Func">deduced type of condition function</typeparam>
		/// <param name="condition">the condition to be checked</param>
		/// <returns>element fulfilling the condition</returns>
		template<class _Func>
		result<_Type&> find_by(_Func&& condition) const
		{
			for (size_t i = 0; i < size_; ++i)
				if (condition(data_[i])) return data_[i];
			return result<_Type&>::INVALID;
		}


		/// <summary>
		/// Searches the vector for element for which specified member has some value.
		/// </summary>
		/// <remarks>
		/// If the vector is empty or no matching element was found, this function
		/// returns result of invalid state.
		/// </remarks>
		/// <typeparam name="_Member">the type of checked member</typeparam>
		/// <param name="member">member field to be checked</param>
		/// <param name="value">the searched member value</param>
		/// <returns>element fulfilling the condition</returns>
		template<class _Member, _capture(_Type)>
		std::enable_if_t<std::is_class<_Type>::value,
			result<_Type&>> find_by(_Member(_Type::*member), _Member const& value)
		{
			for (int i = 0; i<size_; ++i) {
				if (data_[i].*member == value)
					return data_[i];
			}
			return result<_Type&>::INVALID;
		}


		/// <summary>
		/// Searches the vector for element for which specified member function returns some value.
		/// </summary>
		/// <remarks>
		/// If the vector is empty or no matching element was found, this function
		/// returns result of invalid state.
		/// </remarks>
		/// <typeparam name="_Ret">the return type of checked member function</typeparam>
		/// <param name="member">member field to be checked</param>
		/// <param name="value">the searched member value</param>
		/// <returns>element fulfilling the condition</returns>
		template<class _Ret, _capture(_Type)>
		std::enable_if_t<std::is_class<_Type>::value,
			result<_Type&>> find_by(_Ret(_Type::*member)(), _Ret const& value)
		{
			for (int i = 0; i<size_; ++i) {
				if ((data_[i].*member)() == value)
					return data_[i];
			}
			return result<_Type&>::INVALID;
		}

		
		/// <summary>
		/// Replaces each occurence of specified value with other value.
		/// </summary>
		/// <remarks>
		/// Maximum of replacements can be specified by setting \a max. Maximum set to 0 means 
		/// there is no limit of replacements, which is default.
		/// </remarks>
		/// <param name="what">element to be replaced</param>
		/// <param name="value">element to be placed instead of found elements</param>
		/// <param name="max">maximum number of replacements, DEFAULT: 0 (unlimited replacements)</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& replace(const _Type& what, const _Type& value, unsigned max = 0)
		{
			int nrep = 0;
			for (int i = 0; i < size_; ++i) {
				if (data_[i] == what) {
					allocator<_Type>::reconstruct(data_ + i, value);
					if (++nrep == max) return *this;
				}
			}
			return *this;
			//return nrep;
		}

		
		/// <summary>
		/// Sets elements of vector in the reverse order.
		/// </summary>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& reverse()
		{
			for (int i = size_/2 - 1; i >= 0; --i) {
				swap(i, size_ - i - 1);
			}
			return *this;
		}

		
		/// <summary>
		/// Moves elments by \a n places, making elements that go beyond the vector appear on
		/// the beginning.
		/// </summary>
		/// <remarks>
		/// If \a n is positive, elements will be moved towards higher position, if negative:
		///	towards 0. \a n can be any number, if it exceeds the size of vector, the vector will
		/// be shifted by the remainder from division of \a n by size.
		/// </remarks>
		/// <param name="n">number of places to shift the vector</param>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& shift(int n)
		{
			if (n %= (int)size_) {
				if (n < 0) n += size_;
				_Type* newData = allocator<_Type>::raw_allocate(reserved_);
				memcpy(newData, data_ + size_ - n, ELEMENT_SIZE_*n);
				memcpy(newData + n, data_, ELEMENT_SIZE_*(size_ - n));
				allocator<_Type>::raw_deallocate(data_, size_);
				data_ = newData;
			}
			return *this;
		}


		/// <summary>
		/// Erases element of given position from vector and returns its value. 
		/// </summary>
		/// <remarks>
		/// If the position &gt= size of vector, result of invalid state will be returned.
		/// </remarks>
		/// <param name="pos">position of the element to be taken</param>
		/// <returns>the taken element</returns>
		result<_Type> take(size_t pos)
		{
			if (pos < size_) {
				_Type elem = data_[pos];
				erase(pos);
				return elem;
			}
			return result<_Type>::INVALID;
		}

		
		/// <summary>
		/// Checks whether vector is empty.
		/// </summary>
		/// <remarks>
		/// Vector is considered empty if it does not contain any accessible elements nonetheless 
		/// it can have allocated, handled memory.
		/// </remarks>
		/// <returns>true if vector is empty, otherwise false</returns>
		__forceinline bool empty() const
		{
			return !bool(size_);
		}


		/// <summary>
		/// Searches the vector for occurences of repeated elements and removes the repetitions.
		/// </summary>
		/// <remarks>
		/// After calling this function the vector will contain element of every value that occured
		/// beforehand once. All repetitions are removed, the objects are destructed properly. 
		/// The order of elements can change significantly.
		/// </remarks>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& unify()
		{
			for (int i = 0; i < size_; ++i) {
				for (int k = i + 1; k < size_; ) {
					if (data_[i] == data_[k])
						dump(k);
					else
						++k;
				}
				//if (i % (size_ / 10) == 0) std::cout<< '.';
			}
			return *this;
		}


		//passes content to std::cout stream with "separator" after each
		void disp(
			char			separator	= ' ',
			std::ostream&	os			= std::cout,
			char			ending		= '\n') const
		{
			for (size_t i = 0; i < size_; ++i)
				os << data_[i] << separator;
			os << ending;
		}

		void disp(
			size_t			p1, 
			size_t			p2,
			char			separator	= ' ',
			std::ostream&	os			= std::cout,
			char			ending		= '\n') const
		{
			if (p2 > size_ - 1) p2 = size_ - 1;
			for (size_t i = p1; i <= p2; ++i)
				os << data_[i] << separator;
			os << ending;
		}

		/// <summary>
		/// Passes vector's elements to ostream iteratively.
		/// </summary>
		/// <remarks>
		/// Each element is by default separated with space. New line character is printed after
		/// passing all elements. If the contained type cannot be passed to ostream with operator&lt&lt,
		/// this function will fail to compile.
		/// </remarks>
		/// <param name="os">the ostream to print elements</param>
		/// <param name="vec">the vector to be printed</param>
		/// <returns>reference to the left-hand side ostream</returns>
		friend std::ostream& operator<<(std::ostream &os, const _vector<_Type> &vec)
		{
			vec.disp(' ', os, '\n');
			return os;
		}

		/// <summary>
		/// Reads one input from the input stream and pushes it back as new element.
		/// </summary>
		/// <remarks>
		/// If the contained type has no overloaded operator>> or is not fundamental type, this function
		/// will fail to compile.
		/// </remarks>
		/// <param name="is">the input stream</param>
		/// <param name="vec">the vector to add element from input</param>
		/// <returns>reference to the caller</returns>
		friend std::istream &operator >> (std::istream &is, _vector<_Type> &vec)
		{
			if (vec.size_ == vec.reserved_) {
				vec.reallocate_(1 + vec.reserved_ * vec.allocMult_);
			}
			is >> vec.data_[vec.size_++];
			return is;
		}


		/// <summary>
		/// Sets every element iteratively asking for input from specified istream.
		/// </summary>
		/// <remarks>
		/// The size of vector does not change, number of elements equal to current size is read.
		/// If the contained type has no overloaded operator>> or is not fundamental type, this function
		/// will fail to compile. The purpose of this function is e.g. to conveniently initialize 
		/// vector of strings or any other text dependent types, to load a number of strings from 
		/// input at once, etc.
		/// </remarks>
		/// <param name="is">the input stream</param>
		/// <returns>reference to the caller</returns>
		/// <see cref="read_until"/>
		_vector<_Type>& read_fully(std::istream& is)
		{
			for (int i = 0; i<size_; ++i) 
				is >> data_[i];
			
			return *this;
		}


		/// <summary>
		/// Reads a number of elements iteratively asking for input from specified istream.
		/// </summary>
		/// <remarks>
		/// The size of vector is set to \a newSize and this amount of elements are read.
		/// If the contained type has no overloaded operator>> or is not fundamental type, this function
		/// will fail to compile. The purpose of this function is e.g. to conveniently initialize 
		/// vector of strings or any other text dependent types, to load a number of strings from 
		/// input at once, etc.
		/// </remarks>
		/// <param name="is">the input stream</param>
		/// <param name="newSize">the desired number of elements to be read</param>
		/// <returns>reference to the caller</returns>
		/// <see cref="read_until"/>
		_vector<_Type>& read_fully(std::istream& is, size_t newSize)
		{
			clear_assert_size_(newSize);
			return read_fully(is);
		}


		/// <summary>
		/// Reads elements iteratively asking for input from specified istream until the read element
		/// is equal to given \a end value.
		/// </summary>
		/// <remarks>
		/// The size of vector is set to the amount of elements read.
		/// If the contained type has no overloaded operator>> or is not fundamental type, this function
		/// will fail to compile.
		/// </remarks>
		/// <param name="is">the input stream</param>
		/// <param name="end">a value that terminates reading to vector</param>
		/// <returns>reference to the caller</returns>
		/// <see cref="read_fully"/>
		_vector<_Type>& read_until(std::istream& is, _Type const& end)
		{
			_Type elem;

			if (is.eof())
				is >> elem;
			while (! (elem == end) && !is.eof()) {
				push_back(elem);
				is >> elem;
			}
			return *this;
		}


		////template<class _Func>
		//explicit _vector<_Type>& read_until(std::istream& is, x::Fn<bool(_Type const&)> condition)
		//{
		//	_Type elem;
		//	is >> elem;F
		//	while (!condition(elem)) {
		//		push_back(elem);
		//		is >> elem;
		//	}
		//	return *this;
		//}
		template<class _Func>
		std::enable_if_t<x::is_callable<_Func>::value, 
			_vector<_Type>&> read_until(std::istream& is, _Func&& condition)
		{
			_Type elem;

			if (is.eof())
				is >> elem;
			while (!(*condition)(elem) && ! is.eof()) {
				push_back(elem);
				is >> elem;
			}
			return *this;
		}
		
		/// <summary>
		/// Iterates through vector and calls a function on every element.
		/// </summary>
		/// <remarks>
		/// The function to be called can be any delegate that takes object of contained type,
		/// reference or type convertible to contained type as argument.
		/// The resulting type of function can be any, but the results are not preserved anywhere
		/// and are not checked so it is suggested to use this function with delegates returning void.
		/// See function compute for receiving results from multiple function calls on elements.
		/// </remarks>
		/// <typeparam name="_Func">deduced type of function</typeparam>
		/// <param name="func">function to be called on elements</param>
		/// <returns>reference to the caller</returns>
		template<class _Func>
		_vector<_Type>& call(_Func&& func)
		{
			for (int i = 0; i<size_; ++i) {
				func(data_[i]);
			}
			return *this;
		}


		/// <summary>
		/// Iterates through vector and calls a member function with the same pack of arguments
		/// on every element.
		/// </summary>
		/// <remarks>
		/// The resulting type of function can be any, but the results are not preserved anywhere
		/// and are not checked so it is suggested to use this function with members returning void.
		/// See function compute for receiving results from multiple function calls on elements.
		/// This function is only enabled if the contained type is class.
		/// </remarks>
		/// <typeparam name="_Ret">return type of member function</typeparam>
		/// <typeparam name="_Args">types of arguments of member function</typeparam>
		/// <param name="member">the member function to be called</param>
		/// <param name="args">pack of arguments the member to be called with</param>
		/// <returns>reference to the caller</returns>
		template<class _Ret, class... _Args, _capture(_Type)>
		std::enable_if_t<std::is_class<_Type>::value, 
			_vector<_Type>&>
			call(_Ret(_Type::*member)(_Args...), _Args&&... args)
		{
			for (int i = 0; i<size_; ++i) {
				(data_[i].*member)((std::remove_reference_t<_Args>&&)args...);
			}
			return *this;
		}



		/// <summary>
		/// Computes a function over data contained in vector.
		/// </summary>
		/// <remarks>
		/// The \a func must be a delegate that takes contained type or reference to it as an agrument,
		/// otherwise this function will fail to compile. The \a func is iteratively called on every element.
		/// The computed results are put into a new instance of vector which is returned.
		/// </remarks>
		/// <typeparam name="_Func">deduced type of function</typeparam>
		/// <param name="func">the function to be computed</param>
		/// <returns>vector containing results of function computation</returns>
		template<class _Func>
		_vector<std::result_of_t<_Func(_Type)>> compute(_Func&& func) const
		{
			using Res = std::result_of_t<_Func(_Type)>;
			_vector<Res> result(size_);
			for (int i = 0; i < size_; ++i) {
				allocator<Res>::construct(result.data_ + i, func(data_[i]));
			}
			result.size_ = size_;
			return result;
		}


		/// <summary>
		/// Computes a function over data contained in vector.
		/// </summary>
		/// <remarks>
		/// The \a func must be a delegate that takes contained type or reference to it as an agrument,
		/// otherwise this function will fail to compile. The \a func is iteratively called on every element.
		/// The computed results are put into a specified instance of vector reference to which is returned.
		/// The concept behind this function is to provide vector with already allocated enough memory
		/// in order to optimize compoutation. If the \a result vector is too small, required memory will
		/// be allocated anyway.
		/// </remarks>
		/// <typeparam name="_Func">deduced type of function</typeparam>
		/// <param name="func">the function to be computed</param>
		/// <param name="result">the vector to containd computed data<param>
		/// <returns>reference to vector containing results of function computation</returns>
		template<class _Func>
		_vector<std::result_of_t<_Func(_Type)>>& compute(_Func&& func, _vector<std::result_of_t<_Func(_Type)>>& result) const
		{	
			using Res = std::result_of_t<_Func(_Type)>;
			result.clear_assert_size_(size_);
			for (int i = 0; i < size_; ++i) {
				allocator<Res>::construct(result.data_ + i, func(data_[i]));
			}
			return result;
		}


		
		/// <summary>
		/// Checks whether a condition is fulfilled for every element.
		/// </summary>
		/// Tests every element by execting a function on it. This function assumes that the ergument
		/// function returns a value that is bool or is convertible to bool.
		/// If the vector is empty, this function always returns true. 
		/// <param name="condition">the condition function to be checked</param>
		/// <returns>true if condition is fulfilled for every element, otherwise false</returns>
		template<class _Func>
		bool true_for_all(_Func&& condition) const
		{
			for (int i = 0; i < size_; ++i)
				if (!condition(data_[i])) 
					return false;
			return true;
		}

		
		/// <summary>
		/// Clears all content with no memory deallocation.
		/// </summary>
		/// <remarks>
		/// This function just forgets the content stored in vector, making it empty, but the memory
		/// already allocated remains for future usage. If the contained type is class, the elements
		/// re destructed properly by a call to destructor. It is useful when one wants to use the vector
		/// further and add new elements after clearing. For explicit control of memory deallocation,
		/// see no-argument fucntion erase().
		/// </remarks>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& clear()
		{
			allocator<_Type>::destruct(data_, size_);
			size_ = 0;
			return *this;
		}


		/// <summary>
		/// Clears all content and allocates new block of memory of default size. 
		/// </summary>
		/// <remarks>
		/// This function just destructs the content stored in vector, making it empty, 
		/// deallocates the current memory and allocates new block of default size. If the contained 
		/// type is class, the elements are destructed properly by a call to destructor. 
		/// It is useful when one wants to use the vector further but the number of elements will
		/// be reduced. For clearing vector with no memory deallocation, see clear().
		/// </remarks>
		/// <returns>reference to the caller</returns>
		_vector<_Type>& erase()
		{
			if (size_) {
				allocator<_Type>::deallocate(data_, size_);
				data_ = allocator<_Type>::raw_allocate(defaultAlloc_);
				reserved_ = defaultAlloc_;
				size_ = 0;
			}
			return *this;
		}

	
		/// <summary>
		/// Converts the vector to std::vector object.
		/// </summary>
		/// <returns>std::vector object containing copy of elments from the caller vector</returns>
		std::vector<_Type> to_std_vector()
		{
			return std::vector<_Type>(data_, data_ + size_);
		}


		/// <summary>
		/// Converts the vector to std::list object.
		/// </summary>
		/// <returns>std::list object containing copy of elments from the caller vector</returns>
		std::list<_Type> to_std_list()
		{
			return std::list<_Type>(data_, data_ + size_);
		}

	private:
		template<class _To>
		enable_if<std::is_convertible<_Type, _To>::value &&
			DIM_ == 1,
			_vector<_To>> convert_to_() const
		{
			_vector<_To> converted(size_);
			for (int i = 0; i < size_; ++i) {
				allocator<_To>::construct(converted.data_ + i, _To(data_[i]));
			}
			converted.size_ = size_;
			return converted;
		}

		template<class _To>
		enable_if<
			std::is_convertible<typename _AtomicType<_vector<_Type>>::type, _To>::value &&
			DIM_ >= 2,
			_vector_nd_t<_To, DIM_>> convert_to_() const
		{
			_vector_nd_t<_To, DIM_> converted(size_);
			for (int i = 0; i < size_; ++i) {
				allocator<_vector_nd_t<_To, DIM_ - 1>>::construct(
					converted.data_ + i, data_[i].convert_to<_To>().move());
			}
			converted.size_ = size_;
			return converted;
		}

	public:
		/// <summary>
		/// Converts every element to type specified as template argument and puts them into new instance
		/// of vector containing these converted elements.
		/// </summary>
		/// <remarks>
		/// If contained type of caller vector is also vector of some type (if value of dim() &gt= 2), 
		/// this function will convert every vector element calling this function. The result will be 
		/// then a vector of vectors of type contained in contained vector. This behavior will proceed 
		/// recursively until the vector elements contain any type different than another nested vector template.
		/// The template argument type \a _To must have constructor that takes the mostly nested type as argument
		/// or the contained type has to be convertible to \a _To type, otherwise this function will fail to compile. 
		/// The caller vector is not modified.
		/// </remarks>
		/// <typeparam name="_To">the type to convert elements to</typeparam>
		/// <returns>new instance of vector of contained elements</returns>
		/// <see cref="dim"/>
		template<class _To>
		__forceinline _vector_nd_t<_To, DIM_> convert_to() const
		{
			return convert_to_<_To>();
		}


		/// <summary>
		/// If the contained type is pointer calls delete on each element.
		/// </summary>
		/// <remarks>
		/// This function is only enabled when contained type is pointer. It treates the elements as non-array
		/// pointers. Generally it is not safe to call this function and issues caused by previous external
		/// memory deallocation can occur. It does not clear the vector, that means the size of accessible 
		/// data remains unchanged. If the vector was intended to be cleared also, <code>clear()</code> must be called.
		/// </remarks>
		/// <returns>reference to the caller</returns>
		template<_capture(_Type)>
		std::enable_if_t<std::is_pointer<_Type>::value,
			_vector<_Type>&> delete_each()
		{
			for (int i = 0; i < size_; ++i)
				delete data_[i];
			return *this;
		}

		/// <summary>
		/// Destructs the object.
		/// </summary> 
		/// Destructs all elements and deallocates the memory. Should not be called explicitly.<remarks>
		/// </remarks>
		~_vector()
		{
			if (data_) allocator<_Type>::deallocate(data_, size_);
			data_ = nullptr;
		}

	};



	template<class _Type>
	size_t _vector<_Type>::defaultAlloc_ = 10;

	template<class _Type>
	double _vector<_Type>::allocMult_ = 2.0;


	template<class _Type, unsigned _dim = 1>
	using vector = std::conditional_t<_dim == 1, _vector<_Type>, _vector_nd_t<_Type, _dim>>;


} //namespace x

#undef enable_if
#undef decay
#undef DEBUG


#include "loops.h"


#endif //_X_VECTOR_H_