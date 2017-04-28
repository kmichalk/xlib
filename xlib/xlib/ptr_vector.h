#ifndef _X_PTR_VECTOR_H_
#define _X_PTR_VECTOR_H_

#include "xvector.h"

namespace x
{
	template<class _Type>
	class ptr_vector;

	template<class _Type>
	class ptr_vector<_Type*>
	{
		template<class _AllocType>
		using allocator_type = x::allocator<_AllocType>;

	public:
		/*template<class Y>
		struct _CountDim
		{
			static constexpr size_t value = 0;
		};
		template<class Y>
		struct _CountDim<ptr_vector<Y>>
		{
			static constexpr size_t value = 1 + _CountDim<Y>::value;
		};*/

		template<class Y>
		struct _AtomicType
		{
			using type = Y;
		};
		template<class Y>
		struct _AtomicType<ptr_vector<Y>>
		{
			using type = typename _AtomicType<Y>::type;
		};

		////////////////////////////////////////////////////////////////////////////

		static constexpr size_t DIM_ = 1;
		static constexpr size_t ELEMENT_SIZE_ = sizeof(_Type);
		static constexpr size_t DEFAULT_ALLOC_ = 10;
		static constexpr double DEFAULT_ALLOC_MULT_ = 2.0;
		static constexpr size_t MIN_ALLOC_ = 1;

		static size_t defaultAlloc_;
		static double allocMult_;

		////////////////////////////////////////////////////////////////////////////

		_Type* data_;
		size_t size_;
		size_t reserved_;

		////////////////////////////////////////////////////////////////////////////

		//__forceinline _Type* allocator_type<_Type*>::allocate(size_t newSize)
		//{
		//	reserved_ = newSize;
		//	//return new _Type[newSize];
		//	return reinterpret_cast<_Type*>(new byte[newSize*ELEMENT_SIZE]);
		//}

		inline void reallocate_(size_t newSize)
		{
			allocator_type<_Type*>::reallocate(data_, reserved_, newSize);
			reserved_ = newSize;
		}

		template<bool _ptrDeref = false>
		void quicksort_(int r1, int r2)
		{
			int last = r1;
			for (int i = r1; i < r2; ++i) {
				if (compare_less_data_[i], data_[r2]) {
					if (last != i) swap(i, last);
					++last;
				}
			}
			swap(last, r2);
			if (r1 < last - 1) quicksort_(r1, last - 1);
			if (last + 1 < r2) quicksort_(last + 1, r2);
		}

		/*template<bool useable, class Y = _Type>
		enable_if<ptr_vector<Y>::DIM_ >= 2,
		void> reserve_(std::initializer_list<size_t>& sizes,
		std::initializer_list<size_t>::iterator it)
		{
		if (*it > reserved_) {
		allocator_type<_Type*>::reallocate(data_, reserved_, *it);
		if (useable) size_ = *it;
		}
		if (++it != sizes.end()) {
		for (size_t i = 0; i<size_; ++i) {
		data_[i].reserve_<useable>(sizes, it);
		}
		}
		}
		template<bool useable, class Y = _Type>
		enable_if<ptr_vector<Y>::DIM_ == 1,
		void> reserve_(std::initializer_list<size_t>& sizes,
		std::initializer_list<size_t>::iterator it)
		{
		if (*it > reserved_) {
		allocator_type<_Type*>::reallocate(data_, reserved_, *it);
		if (useable) size_ = *it;
		}
		}*/


		ADVANCED_MEMBER_TEST(has_less_op, typename R, operator<, const, typename A);

		////////////////////////////////////////////////////////////////////////////

	public:
		template<class>
		friend class ptr_vector;

		////////////////////////////////////////////////////////////////////////////

		enum error
		{
			ERR_OUT_OF_SIZE,
			ERR_NOTHING_TO_REMOVE,
			ERR_INVALID_ALLOC_MULT,
			ERR_GET_EMPTY,
			ERR_NOT_FOUND,
			ERR_RESERVE_LESS,
			ERR_RESIZE_CONST
		};

		////////////////////////////////////////////////////////////////////////////

		using type = _Type*;

		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using value_type = _Type*;
		using pointer = _Type**;
		using reference = _Type*&;

		////////////////////////////////////////////////////////////////////////////

		struct iterator
		{
			ptr_vector<_Type*>& obj;
			size_t pos;
			iterator(ptr_vector<_Type*>& obj): obj{obj}, pos{0}
			{
			}
			iterator(ptr_vector<_Type*>& obj, size_t pos): obj{obj}, pos{pos}
			{
			}
			iterator(const iterator& other): obj{other.obj}, pos{other.pos}
			{
			}
			__forceinline iterator& operator++()
			{
				++pos;
				return *this;
			}
			__forceinline iterator& operator--()
			{
				--pos;
				return *this;
			}
			inline iterator operator++(int)
			{
				iterator copy{*this};
				++pos;
				return copy;
			}
			inline iterator operator--(int)
			{
				iterator copy{*this};
				--pos;
				return copy;
			}
			__forceinline iterator operator+(int i)
			{
				return iterator(obj, pos + i);
			}
			__forceinline iterator operator-(int i)
			{
				return iterator(obj, pos - i);
			}
			__forceinline iterator& operator+=(int i)
			{
				pos += i; return *this;
			}
			__forceinline iterator& operator-=(int i)
			{
				pos -= i; return *this;
			}
			__forceinline iterator operator+(size_t i)
			{
				return iterator(obj, pos + i);
			}
			__forceinline iterator operator-(size_t i)
			{
				return iterator(obj, pos - i);
			}
			__forceinline iterator& operator+=(size_t i)
			{
				pos += i; return *this;
			}
			__forceinline iterator& operator-=(size_t i)
			{
				pos -= i; return *this;
			}
			__forceinline _Type& operator*()
			{
				return obj.data_[pos];
			}
			__forceinline bool operator==(const iterator& other) const
			{
				return pos == other.pos;
			}
			__forceinline bool operator!=(const iterator& other) const
			{
				return pos != other.pos;
			}
			__forceinline bool operator<(const iterator& other) const
			{
				return pos < other.pos;
			}
			__forceinline bool operator>(const iterator& other) const
			{
				return pos > other.pos;
			}
			__forceinline bool operator<=(const iterator& other) const
			{
				return pos <= other.pos;
			}
			__forceinline bool operator>=(const iterator& other) const
			{
				return pos >= other.pos;
			}
			__forceinline iterator& operator=(const iterator& other)
			{
				pos = other.pos; return *this;
			}
			__forceinline operator bool() const
			{
				return pos < obj.size_;
			}
			__forceinline bool operator !() const
			{
				return pos >= obj.size_;
			}
			__forceinline auto operator->()
			{
				return x::pointer(obj.data_[pos]);
			}
			//TODO: Think about casts from iterator to _Type.
			/*__forceinline operator _Type const&() {
			return obj.data_[pos];
			}
			__forceinline operator _Type&() {
			return obj.data_[pos];
			}*/
		};

		////////////////////////////////////////////////////////////////////////////

		struct const_iterator
		{
			const ptr_vector<_Type*>& obj;
			size_t pos;
			const_iterator(const ptr_vector<_Type*>& obj): obj{obj}, pos{0}
			{
			}
			const_iterator(const ptr_vector<_Type*>& obj, size_t pos): obj{obj}, pos{pos}
			{
			}
			const_iterator(const const_iterator& other): obj{other.obj}, pos{other.pos}
			{
			}
			__forceinline const_iterator& operator++()
			{
				++pos;
				return *this;
			}
			__forceinline const_iterator& operator--()
			{
				--pos;
				return *this;
			}
			inline const_iterator operator++(int)
			{
				const_iterator copy{*this};
				++pos;
				return copy;
			}
			inline const_iterator operator--(int)
			{
				const_iterator copy{*this};
				--pos;
				return copy;
			}
			__forceinline const_iterator operator+(int i)
			{
				return const_iterator(obj, pos + i);
			}
			__forceinline const_iterator operator-(int i)
			{
				return const_iterator(obj, pos - i);
			}
			__forceinline const_iterator& operator+=(int i)
			{
				pos += i;
				return *this;
			}
			__forceinline const_iterator& operator-=(int i)
			{
				pos -= i;
				return *this;
			}
			__forceinline const_iterator operator+(size_t i)
			{
				return const_iterator(obj, pos + i);
			}
			__forceinline const_iterator operator-(size_t i)
			{
				return const_iterator(obj, pos - i);
			}
			__forceinline const_iterator& operator+=(size_t i)
			{
				pos += i; return *this;
			}
			__forceinline const_iterator& operator-=(size_t i)
			{
				pos -= i; return *this;
			}
			__forceinline const _Type& operator*() const
			{
				return obj.data_[pos];
			}
			__forceinline bool operator==(const const_iterator& other) const
			{
				return pos == other.pos;
			}
			__forceinline bool operator!=(const const_iterator& other) const
			{
				return pos != other.pos;
			}
			__forceinline bool operator<(const const_iterator& other) const
			{
				return pos < other.pos;
			}
			__forceinline bool operator>(const const_iterator& other) const
			{
				return pos > other.pos;
			}
			__forceinline bool operator<=(const const_iterator& other) const
			{
				return pos <= other.pos;
			}
			__forceinline bool operator>=(const const_iterator& other) const
			{
				return pos >= other.pos;
			}
			__forceinline const_iterator& operator=(const const_iterator& other)
			{
				pos = other.pos; return *this;
			}
			__forceinline operator bool() const
			{
				return pos < obj.size_;
			}
			__forceinline bool operator !() const
			{
				return pos >= obj.size_;
			}
			__forceinline auto operator->()
			{
				return x::pointer(obj.data_[pos]);
			}
			/*__forceinline operator _Type const&() {
			return obj.data_[pos];
			}
			__forceinline operator _Type&() {
			return obj.data_[pos];
			}*/
		};


		///////////////////////////////////////////////////////////////////////////////
		

		__forceinline iterator begin()
		{
			return iterator(*this, 0);
		}

		__forceinline iterator end()
		{
			return iterator(*this, size_);
		}

		__forceinline const_iterator cbegin() const
		{
			return const_iterator(*this, 0);
		}

		__forceinline const_iterator cend() const
		{
			return const_iterator(*this, size_);
		}

		
		////////////////////////////////////////////////////////////////////////////


		ptr_vector():
			data_(allocator_type<_Type*>::raw_allocate(defaultAlloc_)),
			size_(0),
			reserved_{defaultAlloc_}
		{
		}

		ptr_vector(size_t size, bool useable = false):
			data_(allocator_type<_Type*>::raw_allocate(x::max(size, MIN_ALLOC_))),
			size_(useable ? size : 0),
			reserved_{x::max(size, MIN_ALLOC_)}
		{
		}

		ptr_vector(_Type* arr, size_t size, size_t extra = 0):
			data_(allocator_type<_Type*>::raw_allocate(size + extra)),
			size_(size),
			reserved_{size_ + extra}
		{
			allocator_type<_Type*>::raw_copy(data_, arr, size_);
		}

		template<size_t _size>
		ptr_vector(const _Type(&arr)[_size], size_t extra = 0):
			data_(allocator_type<_Type*>::raw_allocate(_size + extra)),
			size_(_size),
			reserved_{_size + extra}
		{
			allocator_type<_Type*>::raw_copy(data_, static_cast<const _Type*>(arr), _size);
		}

		ptr_vector(std::initializer_list<_Type*> list):
			data_(allocator_type<_Type*>::raw_allocate(list.size())),
			size_(list.size()),
			reserved_{list.size()}
		{
			allocator_type<_Type*>::raw_copy(data_, list.begin(), list.size());
		}

		ptr_vector(const ptr_vector<_Type*>& other):
			size_{other.size_ ? other.size_ : 0},
			reserved_{other.size_ ? other.size_ : defaultAlloc_},
			data_{allocator_type<_Type*>::raw_allocate(other.size_ ? other.size_ : defaultAlloc_)}
		{
			if (size_) allocator_type<_Type*>::raw_copy(data_, other.data_, size_);
		}

		ptr_vector(ptr_vector<_Type*>&& other):
			data_{other.data_},
			size_{other.size_},
			reserved_{other.reserved_}
		{
			other.data_ = nullptr;
		}


		explicit ptr_vector(const std::vector<_Type>& vec):
			size_{vec.size() ? vec.size() : 0},
			reserved_{size_ ? size_ : defaultAlloc_},
			data_{allocator_type<_Type*>::raw_allocate(reserved_)}
		{
			if (size_) allocator_type<_Type*>::raw_copy(data_, vec.data(), size_);
		}

		////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Returns number representing nesting of vector which is 1 for vector of pointers.
		/// </summary>
		/// <returns>The number of dimensions</returns>
		__forceinline static constexpr size_t dim() const
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
		__forceinline _Type* const* data() const
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
			data_ = allocator_type<_Type*>::allocate(reserved_);
			size_ = 0;
			return prev;
		}


		/// <summary>
		/// Deallocates current memory and sets the given pointer as its data. 
		/// The pointer passed as argument should not be used later outside the caller vector.
		/// </summary>
		/// <param name="data">the new memory block to handle</param>
		/// <param name="size">the size of given memory block</param>
		/// <returns>reference to the caller</returns>
		ptr_vector<_Type*>& adopt(_Type* data, size_t size)
		{
			if (data_) allocator_type<_Type*>::deallocate(data_, size_);
			reserved_ = size;
			size_ = size;
			data_ = data;
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
		/// Equivalent of capacity() * sizeof(void*). Gets the amount of memory currently 
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
		inline static size_t default_alloc() const
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
		inline static void set_default_alloc(size_t defSize = DEFAULT_ALLOC_)
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
		inline static double alloc_mult()
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
		static void set_alloc_mult(double allocMult = DEFAULT_ALLOC_MULT_)
		{
			if (allocMult > 1.0) {
				allocMult_ = allocMult;
			}
		}

		
		/// <summary>
		/// Gets logical value of the vector's size, that means checks for non-emptiness of vector.
		/// </summary>
		/// <returns>true if the vector contains any accessible elements, otherwise false</returns>
		__forceinline operator bool() const
		{
			return size_;
		}

		/// <summary>
		/// Casts the vector to an lvalue reference. This function can find usage if one wants to
		/// increase readability of code while passing object that is known to be lvalue reference
		/// or to force adopting content by other vector, rather than copying it when passing the caller
		/// to the move constructor. When the vector's data is moved the caller object should no longer
		/// be used unless reserve is called and in result new memory is allocated.
		/// </summary>
		/// <returns>an lvalue reference to the caller</returns>
		__forceinline ptr_vector<_Type*>&& move()
		{
			return (ptr_vector<_Type*>&&)(*this);
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
		ptr_vector<_Type*> operator+(const ptr_vector<_Type*>& other) const
		{
			ptr_vector<_Type*> result(*this);
			return result.append(other);
		}


		
		bool identical(const ptr_vector<_Type*>& other) const
		{
			
		}

		/// <summary>
		/// Uses memcmp to compare the vector to given for equality of the date.
		/// It false without checking memory if the sizes are different.
		/// </summary>
		/// <param name="other">the vector to be compared to</param>
		/// <returns>true if the vectors' memory is equal, otherwise false</returns>
		bool operator==(const ptr_vector<_Type*>& other) const
		{
			if (size_ == other.size_)
				return memcmp(data_, other.data_, size_) == 0;
			return false;
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
		x::result<_Type*&> at(size_t pos) const
		{
			if (pos < size_)
				return data_[pos];
			return x::result<_Type*&>::INVALID;
		}

		/// <summary>
		/// Gets reference to the first element wrapped in x::result object.
		/// If the size of vector is 0, the function returns x::result of invalid state.
		/// </summary>
		/// <returns>the first element</returns>
		/// <see cref="at"/>
		/// <see cref="operator[]"/>
		/// <see cref="last"/>
		inline result<_Type*&> first() const
		{
			if (size_ > 0)
				return data_[0];
			return x::result<_Type*&>::INVALID;
		}

		/// <summary>
		/// Gets reference to the last element wrapped in x::result object.
		/// If the size of vector is 0, the function returns x::result of invalid state.
		/// </summary>
		/// <returns>the last element</returns>
		/// <see cref="at"/>
		/// <see cref="operator[]"/>
		/// <see cref="first"/>
		inline result<_Type*&> last() const
		{
			if (size_>0)
				return data_[size_ - 1];
			return x::result<_Type*&>::INVALID;
		}

		/// <summary>
		/// Swaps the data pointer and size parameters with other vector. This operation is fast and
		/// does not copy any allocated data.
		/// </summary>
		/// <param name="other">the vector to swapped with</param>
		void swap(ptr_vector<_Type*>& other)
		{
			allocator_type<ptr_vector<_Type*>>::swap(*this, other);
		}

		/// <summary>
		/// Swaps elements at given positions. This operation does not perform any deep copying,
		/// only raw byte form of the elements are copied. No constructor or assignment operator is called.
		/// </summary>
		/// <param name="pos1">position of the first element fo be swapped</param>
		/// <param name="pos2">position of the second element fo be swapped</param>
		/// <returns>reference to the caller</returns>
		ptr_vector<_Type*>& swap(size_t pos1, size_t pos2)
		{
			allocator_type<_Type*>::swap(data_[pos1], data_[pos2]);
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
		ptr_vector<_Type*> subset(size_t begin, size_t end) const
		{
			if (size_ && (end >= begin)) {
				ptr_vector<_Type*> sub(end - begin + 1, true);
				allocator_type<_Type*>::raw_copy(sub.data_, data_ + begin, sub.size_);
				return sub;
			}
			return ptr_vector<_Type*>();
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
		__forceinline ptr_vector<_Type*> subset(size_t begin) const
		{
			if (size_) {
				ptr_vector<_Type*> sub(size_ - begin, true);
				allocator_type<_Type*>::raw_copy(sub.data_, data_ + begin, sub.size_);
				return sub;
			}
			return ptr_vector<_Type*>();
		}

		//removes every element beyond range <"p1","p2">
		template<bool _ptrDelete = false, class Y = _Type>
		enable_if<std::is_pointer<Y>::value && _ptrDelete,
			ptr_vector<_Type*>&> cut(size_t p1, size_t p2)
		{
			if (p2 >= size_) p2 = size_ - 1;
			if ((int)p2 - (int)p1 > 0) {
				for (size_t i = 0; i < p1; ++i)
					delete data_[i];
				for (size_t i = p2 + 1; i < size_; ++i)
					delete data_[i];
				memcpy(data_, data_ + p1, ELEMENT_SIZE_*(p2 - p1 + 1));
				size_ = p2 - p1 + 1;
			}
			else size_ = 0;

			return *this;
		}


		template<bool _ptrDelete = false, class Y = _Type>
		enable_if<!std::is_pointer<Y>::value || !_ptrDelete,
			ptr_vector<_Type*>&> cut(size_t p1, size_t p2)
		{
			if (p2 >= size_) p2 = size_ - 1;
			if ((int)p2 - (int)p1 > 0) {
				memcpy(data_, data_ + p1, ELEMENT_SIZE_*(p2 - p1 + 1));
				size_ = p2 - p1 + 1;
			}
			else size_ = 0;

			return *this;
		}

		template<bool _ptrDelete = false>
		ptr_vector<_Type*>& erase(size_t p1, size_t p2)
		{
			if (size_ && (p2 > p1)) {
				/*static_if(std::is_pointer<_Type>::value && _ptrDelete)
				{
				for (size_t i = p1; i <= p2; ++i)
				delete data_[i];
				}*/
				memcpy(data_ + p1, data_ + p2 + 1, (size_ - p2 - 1)*ELEMENT_SIZE_);
				size_ -= (p2 - p1 + 1);
			}
			return *this;
		}

		////removes every element from position "p1" to "p2" inclusive
		//template<bool _ptrDelete = false, class Y = _Type>
		//enable_if<std::is_pointer<Y>::value && _ptrDelete,
		//	vector_<_Type>&> erase(size_t p1, size_t p2)
		//{
		//	if (size_ && (p2 > p1)) {
		//		for (size_t i = p1; i <= p2; ++i)
		//			delete data_[i];
		//		memcpy(data_+p1, data_+p2+1, (size_-p2-1)*T_size_);
		//		size_ -= (p2-p1+1);
		//	}
		//	return *this;
		//}
		//template<bool _ptrDelete = false, class Y = _Type>
		//enable_if<!std::is_pointer<Y>::value || !_ptrDelete,
		//	vector_<_Type>&> erase(size_t p1, size_t p2)
		//{
		//	if (size_ && (p2 > p1)) {
		//		memcpy(data_ + p1, data_ + p2 + 1, (size_ - p2 - 1)*T_size_);
		//		size_ -= (p2 - p1 + 1);
		//	}
		//	return *this;
		//}

		//removes "n"th element 
		ptr_vector<_Type*>& erase(size_t n)
		{
			if (n < size_) {
				allocator_type<_Type*>::destruct(data_[n]);
				//destroy_data_[n];
				memcpy(data_ + n, data_ + n + 1,
					(size_ - n - 1)*ELEMENT_SIZE_);
				--size_;
			}
			return *this;
		}

		//replaces each element with "elem" 
		ptr_vector<_Type*>& fill(const _Type& elem)
		{
			for (size_t i = 0; i < size_; ++i) {
				allocator_type<_Type*>::destruct(data_[i]);
				allocator_type<_Type*>::construct(data_ + i, elem);
			}
			return *this;
		}

		//resizes content to "size" and replaces each element with "elem" 
		ptr_vector<_Type*>& fill(const _Type& elem, size_t size)
		{
			allocator_type<_Type*>::deallocate(data_, size_);
			reserved_ = size;
			size_ = size;
			data_ = allocator_type<_Type*>::raw_allocate(reserved_);
			for (size_t i = 0; i < size_; ++i) {
				allocator_type<_Type*>::reconstruct(data_ + i, elem);
			}
			return *this;
		}

		//fills content with values of a function for specified arguments
		template<class _Func, class _Iter>
		enable_if<std::is_fundamental<_Iter>::value,
			ptr_vector<_Type*>&> generate(_Func&& fn_, _Iter from, _Iter to, _Iter d)
		{
			allocator_type<_Type*>::deallocate(data_, size_);
			reserved_ = abs((to - from) / d) + 1;
			size_ = reserved_;
			data_ = allocator_type<_Type*>::raw_allocate(reserved_);

			size_t i = 0;
			for (_Iter x = from; i<size_; x += d) {
				allocator_type<_Type*>::reconstruct(data_[i++], (_Type&&)fn_(x));
			}
			return *this;
		}

		//fills content with values of a function for specified arguments
		template<class _Func, class _Iter>
		enable_if<std::is_fundamental<_Iter>::value,
			ptr_vector<_Type*>&> generate(_Func&& fn_, _Iter from, _Iter d)
		{
			size_t i = 0;
			for (_Iter x = from; i<size_; x += d) {
				data_[i++] = fn_(x);
			}
			return *this;
		}

		//fills content with values <"from", "to"> with delta = "d"
		template<class Y = _Type>
		enable_if<std::is_fundamental<Y>::value,
			ptr_vector<Y>&> generate(Y from, Y to, Y d = 1)
		{
			allocator_type<_Type*>::deallocate(data_, size_);
			reserved_ = abs((to - from) / d) + 1;
			size_ = reserved_;
			data_ = allocator_type<_Type*>::raw_allocate(reserved_);

			size_t i = 0;
			for (Y x = from; i<size_; x += d) {
				data_[i++] = x;
			}
			return *this;
		}

		//returns vector_ containing "n" front elements
		__forceinline ptr_vector<_Type*> front(size_t n) const
		{
			return subset(0, n - 1);
		}

		//returns vector_ containing "n" tail elements
		__forceinline ptr_vector<_Type*> back(size_t n) const
		{
			return subset(size_ - n, size_ - 1);
		}

		//resizes array and does non trivial copy of "other"'s content
		ptr_vector<_Type*>& operator=(const ptr_vector<_Type*> & other)
		{
			if (other.size_) {
				if (other.size_ > reserved_) {
					if (data_)
						allocator_type<_Type*>::deallocate(data_, size_);
					reserved_ = other.size_;
					size_ = other.size_;
					data_ = allocator_type<_Type*>::raw_allocate(reserved_);
				}
				else {
					allocator_type<_Type*>::destruct(data_, size_);
					size_ = other.size_;
				}
				allocator_type<_Type*>::raw_copy(data_, other.data_, size_);
			}
			else {
				allocator_type<_Type*>::destruct(data_, size_);
				size_ = 0;
			}
			return *this;
		}

		ptr_vector<_Type*>& operator=(ptr_vector<_Type*> && other)
		{
			if (other.reserved_) {
				if (data_)
					allocator_type<_Type*>::deallocate(data_, size_);
				reserved_ = other.reserved_;
				size_ = other.size_;
				data_ = other.data_;
				other.data_ = nullptr;
			}
			else {
				allocator_type<_Type*>::destruct(data_, size_);
				size_ = 0;
			}
			return *this;
		}

		/*ptr_vector<_Type*>& operator=(moved const& other)
		{
		if (other.size_) {
		allocator_type<_Type*>::deallocate(data_, size_);
		data_ = other.data_;
		size_ = other.size_;
		reserved_ = other.reserved_;
		}
		else {
		allocator_type<_Type*>::destruct(data_, size_);
		size_ = 0;
		}
		return *this;
		}*/

		template<size_t size, class Y = _Type>
		ptr_vector<_Type*>& operator=(const Y(&arr)[size])
		{
			allocator_type<_Type*>::deallocate(data_, size_);
			data_ = allocator_type<_Type*>::raw_allocate(size);
			size_ = size;
			allocator_type<_Type*>::raw_copy(data_, static_cast<const _Type*>(arr), size_);
			return *this;
		}

		//adds new element on the end
		ptr_vector<_Type*>& push_back(const _Type& newElem)
		{
			if (size_ == reserved_)
				reallocate_(1 + reserved_ * allocMult_);

			allocator_type<_Type*>::construct(data_ + size_++, newElem);
			return *this;
		}

		//adds new element on the beginning
		__forceinline ptr_vector<_Type*>& push_front(_Type newElem)
		{
			return insert(newElem, 0);
		}

		template<class... _Arg>
		ptr_vector<_Type*>& construct_back(_Arg&&... args)
		{
			if (size_ == reserved_)
				reallocate_(1 + reserved_ * allocMult_);

			allocator_type<_Type*>::inline_construct(data_ + size_++, args...);
			return *this;
		}

		template<class... _Arg>
		__forceinline ptr_vector<_Type*>& construct_front(_Arg&&... args)
		{
			return construct(0, args...);
		}

		template<class... _Arg>
		ptr_vector<_Type*>& construct(size_t pos, _Arg&&... args)
		{
			if (pos > size_) pos = size_;

			if (size_ == reserved_) {
				reserved_ = 1 + reserved_*allocMult_;
				_Type* newData = allocator_type<_Type*>::raw_allocate(reserved_);
				memcpy(newData + pos + 1, data_ + pos, ELEMENT_SIZE_*(size_ - pos));
				allocator_type<_Type*>::inline_construct(newData + pos, args...);
				memcpy(newData, data_, ELEMENT_SIZE_*pos);
				delete[] reinterpret_cast<byte*>(data_);
				data_ = newData;
			}
			else {
				memcpy(data_ + pos + 1, data_ + pos, ELEMENT_SIZE_*(size_ - pos));
				allocator_type<_Type*>::inline_construct(data_ + pos, args...);
			}
			++size_;
			return *this;
		}

		//moves the part of content after "pos" one field forward
		//and pastes in new element on "pos"
		ptr_vector<_Type*>& insert(const _Type& newElem, size_t pos)
		{
			if (pos > size_) pos = size_;

			if (size_ == reserved_) {
				reserved_ = 1 + reserved_*allocMult_;
				_Type* newData = allocator_type<_Type*>::raw_allocate(reserved_);
				memcpy(newData + pos + 1, data_ + pos, ELEMENT_SIZE_*(size_ - pos));
				allocator_type<_Type*>::construct(newData + pos, newElem);
				memcpy(newData, data_, ELEMENT_SIZE_*pos);
				delete[] reinterpret_cast<byte*>(data_);
				data_ = newData;
			}
			else {
				memcpy(data_ + pos + 1, data_ + pos, ELEMENT_SIZE_*(size_ - pos));
				allocator_type<_Type*>::construct(data_ + pos, newElem);
			}
			++size_;
			return *this;
		}

		//moves the part of content after "pos" forward
		//and pastes in content of other array beginning on "pos"
		ptr_vector<_Type*>& insert(const ptr_vector<_Type*>& other, size_t pos)
		{
			if (pos > size_) pos = size_;

			if (reserved_ < size_ + other.size_) {
				reserved_ = size_ + other.size_;
				_Type* newData = allocator_type<_Type*>::raw_allocate(reserved_);
				memcpy(newData + pos + other.size_, data_ + pos, ELEMENT_SIZE_*(size_ - pos));
				allocator_type<_Type*>::raw_copy(newData + pos, other.data_, other.size_);
				memcpy(newData, data_, ELEMENT_SIZE_*pos);
				delete[] reinterpret_cast<byte*>(data_);
				data_ = newData;
			}
			else {
				memcpy(data_ + pos + other.size_, data_ + pos, ELEMENT_SIZE_*(size_ - pos));
				allocator_type<_Type*>::raw_copy(data_ + pos, other.data_, other.size_);
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
		allocator_type<_Type*>::destruct(data_[f]);
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
		allocator_type<_Type*>::destruct(data_[f]);
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
		int remove_(_Verifier&& verify, unsigned max)
		{
			int i = -1;
			int lastp = -1;
			int nrem = 0;
			int diff;

			while (++i < size_) {
				if (verify(data_[i])) {
					allocator_type<_Type*>::destruct(data_[i]);
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
		allocator_type<_Type*>::destruct(data_[i]);

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
		ptr_vector<_Type*>& remove(_Type const& val, unsigned max = 0)
		{
			remove_(x::ft::compare<_Type const>{val}, max);
			return *this;
		}


		template<class _Func>
		ptr_vector<_Type*>& remove_if(_Func&& fn_, unsigned max = 0)
		{
			remove_(x::ft::execute<x::naked<_Func>>{fn_}, max);
			return *this;
		}

		template<class _Member, _capture(_Type)>
		std::enable_if_t<std::is_class<_Type>::value,
			ptr_vector<_Type*>&> remove_by(_Member(_Type::*member), _Member const& value, unsigned max = 0)
		{
			remove_(x::ft::compare_member<_Type, _Member>{member, value}, max);
			return (ptr_vector<_Type*>&)(*this);
		}


		template<class _Func>
		ptr_vector<_Type*> copy_if(_Func&& fn_)
		{
			ptr_vector<_Type*> copied(size_);
			for (int i = 0; i < size_; ++i) {
				if (fn_(data_[i])) copied.push_back(data_[i]);
			}
			copied.shrink();
			return copied;
		}

		template<class _Func>
		ptr_vector<_Type*> extract_if(_Func&& fn_, unsigned max = 0)
		{
			ptr_vector<_Type*> extracted(size_);
			int i = -1, lastp = -1, nrem = 0;
			while (++i < size_) {
				if (fn_(data_[i])) {
					extracted.push_back(data_[i]);
					allocator_type<_Type*>::destruct(data_[i]);
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

		
		/// <summary>
		/// Erases specified element fast, swapping it with the current last element.
		/// The size of vector is decreased by 1 and value of the element that has been 
		/// thrown remains at position of 1 place beyond the last and can be accessed e.g. by 
		/// <code>vec[vec.size()]</code> but it is volatile and any vector transormation changing 
		/// its size, reallocation memory, etc. makes potential access to this element very unsafe
		/// or impossible. The order of contained elements is not preserved. If specified position
		/// is greater than or equal to size of vector, the function does nothing.
		/// </summary>
		/// <param name="which">position of element to be thrown</param>
		/// <returns>reference to the caller</returns>
		ptr_vector<_Type*>& throw_back(size_t which)
		{
			if (which < size_) {
				swap(which, --size_);
			}
			return *this;
		}


		/// <summary>
		/// Erases specified element fast by writing value of the last element.
		/// The size of vector is decreased by 1 and the value of removed element is returned.
		/// </summary>
		/// <param name="which">position of element to be removed</param>
		/// <returns>the value of removed element</returns>
		result<_Type*> dump(size_t which)
		{
			if (which < size_) {
				_Type* erased = data_[which];
				data_[which] = data_[--size_];
				return erased;
			}
			else
				return result<_Type*>::INVALID;

			return *this;
		}

		//removes first element
		__forceinline ptr_vector<_Type*>& pop_front()
		{
			if (size_) {
				erase(0);
			}
			return *this;
		}

		//removes last element
		ptr_vector<_Type*>& pop_back()
		{
			if (size_) {
				allocator_type<_Type*>::destruct(data_[--size_]);
			}
			return *this;
		}

		//uses quick sort on the whole scope
		template<class Y = _Type>
		__forceinline enable_if<
			has_less_op<Y, bool, const Y&>::value ||
			has_less_op<Y, bool, Y&>::value ||
			!std::is_class<Y>::value,
			ptr_vector<_Type*>&> sort()
		{
			quicksort_(0, size_ - 1);
			return *this;
		}

#ifdef _X_RND_H_	
		//set elements in random order
		//if "swap_each" is disabled, each element has
		//some probability of not being moved
		ptr_vector<_Type*>& shuffle()
		{
			for (int i = 0; i < size_ - 1; ++i) {
				swap(i, random<size_t>(i + 1, size_ - 1));
			}
			return *this;
		}

		
		_Type& get_random(size_t p1, size_t p2) const
		{
			return data_[random<size_t>(p1, p2)];
		}

		//return random element
		__forceinline _Type& get_random() const
		{
			return get_random(0, size_ - 1);
		}

		template<_capture(_Type)>
		enable_if<std::is_fundamental<_Type>::value,
			ptr_vector<_Type*>&> randomize(_Type r1, _Type r2)
		{
			for (int i = 0; i < size_; ++i) {
				data_[i] = random<_Type>(r1, r2);
			}
			return *this;
		}

		template<_capture(_Type)>
		enable_if<std::is_fundamental<_Type>::value,
			ptr_vector<_Type*>&> randomize(_Type r1, _Type r2, size_t size)
		{
			allocator_type<_Type*>::reallocate(data_, reserved_, size);
			reserved_ = size;
			size_ = reserved_;
			return randomize(r1, r2);
		}

		template<_capture(_Type)>
		enable_if<std::is_fundamental<_Type>::value,
			ptr_vector<_Type*>&> jitter(double r1, double r2)
		{
			double factor; _Type r = r2 - r1;
			for (int i = 0; i < size_; ++i) {
				factor = random<_Type>(-r, r);
				data_[i] *= (1 + (factor + r1*sgn(factor)));
			}
			return *this;
		}

		template<class Y = _Type>
		enable_if<std::is_fundamental<Y>::value,
			ptr_vector<_Type*>&> jitter(double r)
		{
			for (int i = 0; i < size_; ++i) {
				data_[i] *= (1 + random<_Type>(-r, r));
			}
			return *this;
		}

#endif

		//deallocate memory exceeding the actual size
		__forceinline ptr_vector<_Type*>& shrink()
		{
			reallocate_(size_);
			return *this;
		}

		//only more space reservation is accepted
		ptr_vector<_Type*>& reserve(size_t newSize)
		{
			if (data_) {
				if (newSize > reserved_)
					reallocate_(newSize);
			}
			else { 
				data_ = allocator_type<_Type*>::raw_allocate(newSize);
				reserved_ = newSize;
				size_ = 0;
			}
			return *this;
		}

		/*template<bool useable = false, class Y = _Type, class... N>
		enable_if<ptr_vector<Y>::DIM_ >= 2 &&
		sizeof...(N) <= ptr_vector<Y>::DIM_ &&
		all_true<std::is_convertible<N, size_t>::value...>::value,
		void> reserve(N... sizes)
		{
		std::initializer_list<size_t> sizes_il{size_t(sizes)...};
		reserve_<useable>(sizes_il, sizes_il.begin());
		}*/

		//inserts non trivial copy of "other"'s content on the end
		ptr_vector<_Type*>& append(const ptr_vector<_Type*>& other)
		{
			if (reserved_ < size_ + other.size_)
				reallocate_(size_ + other.size_);
			allocator_type<_Type*>::raw_copy(data_ + size_, other.data_, other.size_);
			size_ += other.size_;
			return *this;
		}

		////inserts new elements on the end
		//ptr_vector<_Type*>& append(std::initializer_list<_Type*> other)
		//{
		//	if (reserved_ < size_ + other.size())
		//		reallocate_(size_ + other.size());
		//	int k = size_;
		//	for (class std::initializer_list<_Type*>::iterator i = other.begin();
		//		i != other.end(); ++i, ++k) {
		//		data_[k] = _Type(*i);
		//	}
		//	size_ += other.size();
		//	return *this;
		//}

		//moves content forward and inserts non trivial copy
		//of "other"'s content on the beginning
		__forceinline ptr_vector<_Type*>& prepend(const ptr_vector<_Type*>& other)
		{
			insert(other, 0);
			return *this;
		}

		////moves content forward and
		////inserts new elements on the beginning
		//ptr_vector<_Type*>& prepend(std::initializer_list<_Type*> other)
		//{
		//	if (reserved_<size_ + other.size())
		//		reallocate_(size_ + other.size());
		//	memcpy(data_ + other.size(), data_, ELEMENT_SIZE_*size_);
		//	int k = 0;
		//	for (class std::initializer_list<_Type*>::iterator i = other.begin();
		//		i != other.end(); ++i, ++k) {
		//		data_[k] = _Type(*i);
		//	}
		//	size_ += other.size();
		//	return *this;
		//}

		//returns number of occurances of "elem"
		int count(const _Type* elem) const
		{
			int n = 0;
			for (size_t i = 0; i<size_; ++i) {
				if (data_[i] == elem) ++n;
			}
			return n;
		}

		//returns reference to element, value of which is equal to "elem"
		result<_Type*&> find(_Type const* elem)
		{
			for (size_t i = 0; i < size_; ++i)
				if (data_[i] == elem)
					return data_[i];
			return result<_Type*&>::INVALID;
		}


		result<size_t> pos_of(_Type const* elem) const
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
		result<_Type*&> find_by(_Func&& fn_, _Cmp const& val)
		{
		for (size_t i = 0; i < size_; ++i)
		if (fn_(data_[i]) == val) return data_[i];
		return result<_Type*&>::INVALID;
		}*/

		template<class _Func>
		result<_Type*&> find_by(_Func&& fn_)
		{
			for (size_t i = 0; i < size_; ++i)
				if (fn_(data_[i])) return data_[i];
			return result<_Type*&>::INVALID;
		}


		//returns reference to element, "member"'s value of which is equal to "val"
		template<class _Member, _capture(_Type)>
		std::enable_if_t<std::is_class<_Type>::value,
			iterator> find_by(_Member(_Type::*member), _Member const& val)
		{
			iterator it{begin()};
			for (; it; ++it)
				if (((*it)->*member) == val) return it;
			return it;
		}

		//replaces each occurance of "what" element with "val"
		ptr_vector<_Type*>& replace(const _Type*what, const _Type* val, unsigned nmax)
		{
			int nrep = 0;
			for (int i = 0; i < size_; ++i) {
				if (data_[i] == what) {
					allocator_type<_Type*>::reconstruct(data_ + i, val);
					if (++nrep == nmax) return *this;
				}
			}
			return *this;
		}


		//replaces each occurance of "what" element with "val"
		ptr_vector<_Type*>& replace(const _Type*what, const _Type* val)
		{
			for (int i = 0; i < size_; ++i) {
				if (data_[i] == what) {
					allocator_type<_Type*>::reconstruct(data_ + i, val);
				}
			}
			return *this;
		}

		//sets elements in reverse order
		ptr_vector<_Type*>& reverse()
		{
			for (int i = size_ / 2 - 1; i >= 0; --i) {
				swap(i, size_ - i - 1);
			}
			return *this;
		}

		//moves elements "n" fields forward / backward (if "n" < 0)
		//making them to appear on the beginning / end
		ptr_vector<_Type*>& shift(int n)
		{
			if (n %= (int)size_) {
				if (n < 0) n += size_;
				_Type* newData = allocator_type<_Type*>::raw_allocate(reserved_);
				memcpy(newData, data_ + size_ - n, ELEMENT_SIZE_*n);
				memcpy(newData + n, data_, ELEMENT_SIZE_*(size_ - n));
				delete[] reinterpret_cast<byte*>(data_);
				data_ = newData;
			}
			return *this;
		}

		_Type take(size_t n)
		{
			if (n < size_) {
				_Type elem = data_[n];
				erase(n);
				return elem;
			}
			throw ERR_OUT_OF_SIZE;
		}

		//true if is empty
		__forceinline bool empty() const
		{
			return !bool(size_);
		}

		//passes content to std::cout stream with "separator" after each
		void disp(
			char			separator = ' ',
			std::ostream&	os = std::cout,
			char			ending = '\n') const
		{
			for (size_t i = 0; i < size_; ++i)
				os << data_[i] << separator;
			os << ending;
		}

		void disp(
			size_t			p1,
			size_t			p2,
			char			separator = ' ',
			std::ostream&	os = std::cout,
			char			ending = '\n') const
		{
			if (p2 > size_ - 1) p2 = size_ - 1;
			for (size_t i = p1; i <= p2; ++i)
				os << data_[i] << separator;
			os << ending;
		}

		//passes content to output stream
		friend std::ostream& operator<<(std::ostream &os, const ptr_vector<_Type*> &vec)
		{
			vec.disp(' ', os, '\n');
			return os;
		}

		//pushes back new element from input stream
		friend std::istream &operator >> (std::istream &is, ptr_vector<_Type*> &arr)
		{
			if (arr.size_ == arr.reserved_) {
				arr.reallocate_(arr.reserved_, 1 + arr.reserved_ * arr.allocMult_);
			}
			is >> arr.data_[arr.size_++];
			return is;
		}

		//calls function on every element
		template<class _Func>
		ptr_vector<_Type*>& call(_Func&& fn_)
		{
			for (int i = 0; i<size_; ++i) {
				fn_(data_[i]);
			}
			return *this;
		}


		//calls any member function with given parameters on every element
		template<class... _Args, _capture(_Type), class _Ret>
		std::enable_if_t<std::is_class<_Type>::value,
			ptr_vector<_Type*>&>
			call(_Ret(_Type::*fn)(_Args...), _Args&&... args)
		{
			for (int i = 0; i<size_; ++i) {
				(data_[i]->*fn)(args...);
			}
			return *this;
		}

		//checks if condition is true for every element
		template<class _Func>
		bool true_for_all(_Func&& fn_)
		{
			for (int i = 0; i<size_; ++i)
				if (!fn_(data_[i])) return false;
			return true;
		}

		//forgets the content with no memory deallocation
		ptr_vector<_Type*>& clear()
		{
			allocator_type<_Type*>::destruct(data_, size_);
			size_ = 0;
			return *this;
		}

		ptr_vector<_Type*>& delete_each()
		{
			for (int i = 0; i < size_; ++i) {
				if (data_[i])
					delete data_[i];
			}
			return *this;
		}


		//clears all content and allocates new block of memory of default size		
		ptr_vector<_Type*>& erase()
		{
			if (size_) {
				allocator_type<_Type*>::deallocate(data_, size_);
				data_ = allocator_type<_Type*>::raw_allocate(defaultAlloc_);
				reserved_ = defaultAlloc_;
				size_ = 0;
			}
			return *this;
		}


		//returns std::vector with non trivial copy of content
		std::vector<_Type> to_std_vector()
		{
			return std::vector<_Type>(data_, data_ + size_ - 1);
		}

		std::list<_Type> to_std_list()
		{
			std::list<_Type> ret;
			int i = size_;
			while (i--)
				ret.push_front(data_[i]);
			return ret;
		}


		//If conversion is possible returns new instance of x::vector
		//with elements converted to specified _Type
		template<class _To>
		enable_if<std::is_convertible<_Type, _To>::value &&
			DIM_ == 1,
			ptr_vector<_To>> convert_to()
		{
			ptr_vector<_To> converted(size_);
			for (int i = 0; i < size_; ++i) {
				converted.data_[i] = _To(data_[i]);
			}
			converted.size_ = size_;
			return converted;
		}

		template<class _To>
		enable_if<
			std::is_convertible<typename _AtomicType<ptr_vector<_Type*>>::type, _To>::value &&
			DIM_ >= 2,
			typename _vector_nd<_To, DIM_>::type> convert_to()
		{
			class _vector_nd<_To, DIM_>::type converted(size_);
			for (int i = 0; i < size_; ++i) {
				converted.data_[i] = data_[i].convert_to<_To>();
			}
			converted.size_ = size_;
			return converted;
		}

		~ptr_vector()
		{
			if (data_) allocator_type<_Type*>::deallocate(data_, size_);
			//if (data_) delete[] data_;
		}

	};


	template<class _Type>
	size_t ptr_vector<_Type*>::defaultAlloc_ = ptr_vector<_Type*>::DEFAULT_ALLOC_;

	template<class _Type>
	double ptr_vector<_Type*>::allocMult_ = ptr_vector<_Type*>::DEFAULT_ALLOC_MULT_;


}

#endif //_X_PTR_VECTOR_H_

