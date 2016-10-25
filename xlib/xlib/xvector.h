//#pragma once
#ifndef VECTOR_H
#define VECTOR_H

//#include "random_engine_.h"
#include <vector>
#include <list>
#include <iostream>
#include <string>
#include "more_type_traits.h"
#include "simple.h"
#include "autoref.h"
#include "fake.h"

#define DEBUG 0

#define enable_if typename std::enable_if_t
#define decay typename std::decay_t

namespace x
{

	template<typename> class _vector;

	//type expanding into n-dimensional vector
	template<typename T_, unsigned DIM>
	struct _vector_nd
	{
		using type = _vector<typename _vector_nd<T_, DIM - 1>::type>;
	};

	template<typename T_>
	struct _vector_nd<T_, 0>
	{
		using type = T_;
	};

	enum vector_opt
	{
		PTR_DELETE = 1,
		PTR_DEREF = 1,
		USEABLE = 1
	};

	template<typename _Type>
	class _vector
	{
	protected:
		/*class T_fake_
		{
		char fake_data_[sizeof(_Type)];
		public:
		~T_fake_() {}
		};*/

		using Shallow = std::conditional_t<
			std::is_class<_Type>::value && !x::is_trivially_copyable<_Type>::value,
			x::FakeType<_Type>, _Type>;

		/*using Realloc = std::conditional_t<
		std::is_class<_Type>::value && !x::is_trivially_copyable<_Type>::value,
		x::FakeType<_Type>, _Type>;*/

		template<typename Y>
		struct _CountDim
		{
			static constexpr size_t value = 0;
		};
		template<typename Y>
		struct _CountDim<_vector<Y>>
		{
			static constexpr size_t value = 1 + _CountDim<Y>::value;
		};

		template<typename Y>
		struct _AtomicType
		{
			using type = Y;
		};
		template<typename Y>
		struct _AtomicType<_vector<Y>>
		{
			using type = typename _AtomicType<Y>::type;
		};

		////////////////////////////////////////////////////////////////////////////

		static constexpr size_t DIM = _CountDim<_vector<_Type>>::value;
		static constexpr size_t ELEMENT_SIZE = sizeof(_Type);
		static constexpr size_t DEFAULT_ALLOC = 10;

		static size_t defaultAlloc_;
		static double allocMult_;

		////////////////////////////////////////////////////////////////////////////

		_Type* content_;
		size_t size_;
		size_t reserved_;

		////////////////////////////////////////////////////////////////////////////

		__forceinline _Type* alloc_(size_t newSize)
		{
			reserved_ = newSize;
			return new _Type[newSize];
		}

		void realloc_(size_t newSize)
		{
			//std::cout<<typeid(Realloc).name();
			_Type* newContent = alloc_(newSize);
			memcpy(newContent, content_, ELEMENT_SIZE*size_);
			delete[] reinterpret_cast<byte*>(content_);
			content_ = newContent;
		}

		template<bool _ptrDeref = false>
		void quicksort_(int r1, int r2)
		{
			int last = r1;
			for (int i = r1; i < r2; ++i) {
				if (compare_less_elem_<_ptrDeref>(content_[i], content_[r2])) {
					if (last != i) swap(i, last);
					++last;
				}
			}
			swap(last, r2);
			if (r1 < last - 1) quicksort_(r1, last - 1);
			if (last + 1 < r2) quicksort_(last + 1, r2);
		}

		template<bool useable, typename Y = _Type>
		enable_if<_vector<Y>::DIM >= 2,
			void> reserve_(std::initializer_list<size_t>& sizes,
				std::initializer_list<size_t>::iterator it)
		{
			if (*it > reserved_) {
				realloc_(*it);
				if (useable) size_ = *it;
			}
			if (++it != sizes.end()) {
				for (size_t i = 0; i<size_; ++i) {
					content_[i].reserve_<useable>(sizes, it);
				}
			}
		}
		template<bool useable, typename Y = _Type>
		enable_if<_vector<Y>::DIM == 1,
			void> reserve_(std::initializer_list<size_t>& sizes,
				std::initializer_list<size_t>::iterator it)
		{
			if (*it > reserved_) {
				realloc_(*it);
				if (useable) size_ = *it;
			}
		}

		template<typename Y = _Type>
		__forceinline enable_if<
			is_trivially_copyable<Y>::value,
			void> copy_(Y* dest, const Y* arr, size_t s) const
		{
			memcpy(dest, arr, ELEMENT_SIZE*s);
		}
		template<typename Y = _Type>
		__forceinline enable_if<
			!is_trivially_copyable<Y>::value,
			void> copy_(Y* dest, const Y* arr, size_t s) const
		{
			while (s--) dest[s] = arr[s];
		}

		////////////////////////////////////////////////////////////////////////////

		template<bool _ptrDeref = false, typename Y = _Type>
		__forceinline enable_if<std::is_pointer<Y>::value && _ptrDeref,
			bool> compare_elem_(const Y& el1, const Y& el2) const
		{
			return *el1 == *el2;
		}
		template<bool _ptrDeref = false, typename Y = _Type>
		__forceinline enable_if<!std::is_pointer<Y>::value || !_ptrDeref,
			bool> compare_elem_(const Y& el1, const Y& el2) const
		{
			return el1 == el2;
		}

		template<bool _ptrDelete = false, typename Y = _Type>
		__forceinline enable_if<std::is_pointer<Y>::value && _ptrDelete,
			void> destroy_elem_(const Y& el)
		{
			delete el;
		}
		template<bool _ptrDelete = false, typename Y = _Type>
		__forceinline enable_if<!std::is_pointer<Y>::value || !_ptrDelete,
			void> destroy_elem_(const Y& el)
		{
		}

		template<bool _ptrDeref = false, typename Y = _Type>
		__forceinline enable_if<std::is_pointer<Y>::value && _ptrDeref,
			void> assign_elem_(const Y& el1, const Y& el2)
		{
			return *el1 = *el2;
		}
		template<bool _ptrDeref = false, typename Y = _Type>
		__forceinline enable_if<!std::is_pointer<Y>::value || !_ptrDeref,
			void> assign_elem_(const Y& el1, const Y& el2)
		{
			return el1 = el2;
		}

		template<bool _ptrDeref = false, typename Y = _Type>
		__forceinline enable_if<std::is_pointer<Y>::value && _ptrDeref,
			bool> compare_less_elem_(const Y& el1, const Y& el2) const
		{
			return *el1 < *el2;
		}
		template<bool _ptrDeref = false, typename Y = _Type>
		__forceinline enable_if<!std::is_pointer<Y>::value || !_ptrDeref,
			bool> compare_less_elem_(const Y& el1, const Y& el2) const
		{
			return el1 < el2;
		}

		template<bool _ptrDeref = false, typename Y = _Type>
		__forceinline enable_if<std::is_pointer<Y>::value && _ptrDeref,
			typename std::remove_pointer_t<Y>&> elem_(Y& el)
		{
			return *el;
		}
		template<bool _ptrDeref = false, typename Y = _Type>
		__forceinline enable_if<!std::is_pointer<Y>::value || !_ptrDeref,
			Y&> elem_(Y& el)
		{
			return el;
		}

		////////////////////////////////////////////////////////////////////////////

		ADVANCED_MEMBER_TEST(has_less_op, typename R, operator<, const, typename A);

		////////////////////////////////////////////////////////////////////////////

	public:
		template<typename>
		friend class _vector;

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

		using type = _Type;

		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using value_type = _Type;
		using pointer = _Type*;
		using reference = _Type&;

		////////////////////////////////////////////////////////////////////////////

		struct iterator
		{
			_vector<_Type>& obj;
			size_t pos;
			iterator(_vector<_Type>& obj): obj{obj}, pos{0}
			{
			}
			iterator(_vector<_Type>& obj, size_t pos): obj{obj}, pos{pos}
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
				return obj.content_[pos];
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
			__forceinline operator bool()
			{
				return pos < obj.size_;
			}
			__forceinline auto operator->()
			{
				return x::pointer(obj.content_[pos]);
			}
			//TODO: Think about casts from iterator to _Type.
			/*__forceinline operator _Type const&() {
			return obj.content_[pos];
			}
			__forceinline operator _Type&() {
			return obj.content_[pos];
			}*/
		};

		////////////////////////////////////////////////////////////////////////////

		struct const_iterator
		{
			const _vector<_Type>& obj;
			size_t pos;
			const_iterator(const _vector<_Type>& obj): obj{obj}, pos{0}
			{
			}
			const_iterator(const _vector<_Type>& obj, size_t pos): obj{obj}, pos{pos}
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
				return obj.content_[pos];
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
			__forceinline operator bool()
			{
				return pos < obj.size_;
			}
			__forceinline auto operator->()
			{
				return x::pointer(obj.content_[pos]);
			}
			/*__forceinline operator _Type const&() {
			return obj.content_[pos];
			}
			__forceinline operator _Type&() {
			return obj.content_[pos];
			}*/
		};

		////////////////////////////////////////////////////////////////////////////

		struct loop_iterator
		{
			_vector<_Type>& obj;
			size_t pos;
			loop_iterator(_vector<_Type>& obj): obj{obj}, pos{0}
			{
			}
			loop_iterator(_vector<_Type>& obj, size_t pos): obj{obj}, pos{pos}
			{
				if (pos >= obj.size_)
					pos = pos % obj.size_;
			}
			loop_iterator(const loop_iterator& other): obj{other.obj}, pos{other.pos}
			{
			}
			__forceinline loop_iterator& operator++()
			{
				if (++pos >= obj.size_) pos = 0;
				return *this;
			}
			__forceinline loop_iterator& operator--()
			{
				if (--pos >= obj.size_) pos = obj.size_ - 1;
				return *this;
			}
			inline loop_iterator operator++(int)
			{
				loop_iterator copy{*this};
				if (++pos >= obj.size_) pos = 0;
				return copy;
			}
			inline loop_iterator operator--(int)
			{
				loop_iterator copy{*this};
				if (--pos >= obj.size_) pos = obj.size_ - 1;
				return copy;
			}
			__forceinline loop_iterator operator+(int i)
			{
				return loop_iterator(obj, pos + i);
			}
			__forceinline loop_iterator operator-(int i)
			{
				return loop_iterator(obj, pos - i);
			}
			__forceinline loop_iterator& operator+=(int i)
			{
				if ((pos += i) >= obj.size_) pos = obj.size_;
				return *this;
			}
			__forceinline loop_iterator& operator-=(int i)
			{
				if ((pos -= i) >= obj.size_) pos = obj.size_;
				return *this;
			}
			__forceinline loop_iterator operator+(size_t i)
			{
				return loop_iterator(obj, pos + i);
			}
			__forceinline loop_iterator operator-(size_t i)
			{
				return loop_iterator(obj, pos - i);
			}
			__forceinline loop_iterator& operator+=(size_t i)
			{
				pos += i; return *this;
			}
			__forceinline loop_iterator& operator-=(size_t i)
			{
				pos -= i; return *this;
			}
			__forceinline _Type& operator*()
			{
				return obj.content_[pos];
			}
			__forceinline bool operator==(const loop_iterator& other) const
			{
				return pos == other.pos;
			}
			__forceinline bool operator!=(const loop_iterator& other) const
			{
				return pos != other.pos;
			}
			__forceinline bool operator<(const loop_iterator& other) const
			{
				return pos < other.pos;
			}
			__forceinline bool operator>(const loop_iterator& other) const
			{
				return pos > other.pos;
			}
			__forceinline bool operator<=(const loop_iterator& other) const
			{
				return pos <= other.pos;
			}
			__forceinline bool operator>=(const loop_iterator& other) const
			{
				return pos >= other.pos;
			}
			__forceinline loop_iterator& operator=(const loop_iterator& other)
			{
				pos = other.pos;
				return *this;
			}
			__forceinline operator bool()
			{
				return true;
			}
			__forceinline auto operator->()
			{
				return x::pointer(obj.content_[pos]);
			}
			/*__forceinline operator _Type const&() {
			return obj.content_[pos];
			}
			__forceinline operator _Type&() {
			return obj.content_[pos];
			}*/
		};

		////////////////////////////////////////////////////////////////////////////

		struct const_loop_iterator
		{
			const _vector<_Type>& obj;
			size_t pos;
			const_loop_iterator(const _vector<_Type>& obj): obj{obj}, pos{0}
			{
			}
			const_loop_iterator(const _vector<_Type>& obj, size_t pos): obj{obj}, pos{pos}
			{
				if (pos >= obj.size_)
					pos = pos % obj.size_;
			}
			const_loop_iterator(const const_loop_iterator& other): obj{other.obj}, pos{other.pos}
			{
			}
			__forceinline const_loop_iterator& operator++()
			{
				if (++pos >= obj.size_) pos = 0;
				return *this;
			}
			__forceinline const_loop_iterator& operator--()
			{
				if (--pos >= obj.size_) pos = obj.size_ - 1;
				return *this;
			}
			inline const_loop_iterator operator++(int)
			{
				const_loop_iterator copy{*this};
				if (++pos >= obj.size_) pos = 0;
				return copy;
			}
			inline const_loop_iterator operator--(int)
			{
				const_loop_iterator copy{*this};
				if (--pos >= obj.size_) pos = obj.size_ - 1;
				return copy;
			}
			__forceinline const_loop_iterator operator+(int i)
			{
				return const_loop_iterator(obj, pos + i);
			}
			__forceinline const_loop_iterator operator-(int i)
			{
				return const_loop_iterator(obj, pos - i);
			}
			__forceinline const_loop_iterator& operator+=(int i)
			{
				if ((pos += i) >= obj.size_) pos = obj.size_;
				return *this;
			}
			__forceinline const_loop_iterator& operator-=(int i)
			{
				if ((pos -= i) >= obj.size_) pos = obj.size_;
				return *this;
			}
			__forceinline const_loop_iterator operator+(size_t i)
			{
				return const_loop_iterator(obj, pos + i);
			}
			__forceinline const_loop_iterator operator-(size_t i)
			{
				return const_loop_iterator(obj, pos - i);
			}
			__forceinline const_loop_iterator& operator+=(size_t i)
			{
				pos += i; return *this;
			}
			__forceinline const_loop_iterator& operator-=(size_t i)
			{
				pos -= i; return *this;
			}
			__forceinline const _Type& operator*() const
			{
				return obj.content_[pos];
			}
			__forceinline bool operator==(const const_loop_iterator& other) const
			{
				return pos == other.pos;
			}
			__forceinline bool operator!=(const const_loop_iterator& other) const
			{
				return pos != other.pos;
			}
			__forceinline bool operator<(const const_loop_iterator& other) const
			{
				return pos < other.pos;
			}
			__forceinline bool operator>(const const_loop_iterator& other) const
			{
				return pos > other.pos;
			}
			__forceinline bool operator<=(const const_loop_iterator& other) const
			{
				return pos <= other.pos;
			}
			__forceinline bool operator>=(const const_loop_iterator& other) const
			{
				return pos >= other.pos;
			}
			__forceinline const_loop_iterator& operator=(const const_loop_iterator& other)
			{
				pos = other.pos;
				return *this;
			}
			__forceinline operator bool()
			{
				return true;
			}
			__forceinline auto operator->()
			{
				return x::pointer(obj.content_[pos]);
			}
			/*__forceinline operator _Type const&() {
			return obj.content_[pos];
			}
			__forceinline operator _Type&() {
			return obj.content_[pos];
			}*/
		};

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
		__forceinline loop_iterator lbegin()
		{
			return loop_iterator(*this, 0);
		}
		__forceinline loop_iterator lend()
		{
			return loop_iterator(*this, size_);
		}
		__forceinline const_loop_iterator clbegin()
		{
			return const_loop_iterator(*this, 0);
		}
		__forceinline const_loop_iterator clend()
		{
			return const_loop_iterator(*this, size_);
		}


		////////////////////////////////////////////////////////////////////////////

		_vector():
			content_(alloc_(defaultAlloc_)), size_(0)
		{
			//std::cout << "def ctor\n";
			////std::cout<<typeid(Shallow).name();
		}

		_vector(size_t size, bool useable = false):
			content_(alloc_(size)), size_(useable ? size : 0)
		{
			//std::cout << "size ctor\n";
		}

		_vector(_Type* arr, size_t size, size_t extra = 0):
			content_(alloc_(size + extra)),
			size_(size)
		{
			copy_(content_, arr, size_);
		}

		template<size_t size, typename Y = _Type, typename = enable_if<!std::is_same<decay<Y>, char>::value>>
		_vector(const _Type(&arr)[size], size_t extra = 0):
			content_(alloc_(size + extra)),
			size_(size)
		{
			copy_(content_, static_cast<const _Type*>(arr), size_);
		}

		template<typename Y = _Type, typename = enable_if<std::is_same<decay<Y>, char>::value>>
		_vector(const _Type* arr):
			content_{alloc_(strlen(arr) + 1)},
			size_{reserved_}
		{
			copy_(content_, arr, reserved_);
		}

		_vector(std::initializer_list<_Type> arr):
			content_(alloc_(arr.size())),
			size_(arr.size())
		{
			//std::cout << "init_list ctor\n";
			int k = 0;
			for (auto i = arr.begin(); i != arr.end(); ++i, ++k) {
				content_[k] = *i;
			}
		}

		/*template<typename... Y,
		typename U = enable_if<every_is<_Type, Y...>::value>>
		vector_(Y... elements) :
		size_(sizeof...(elements)),
		reserved_(sizeof...(elements))
		{
		content_ = new _Type[size_]{elements...};
		}*/

		_vector(const _vector<_Type>& other):
			content_{alloc_(other.size_ ? other.size_ : defaultAlloc_)},
			size_{other.size_}
		{
			//std::cout << "copy ctor\n";
			if (size_) copy_(content_, other.content_, size_);
		}

		_vector(_vector<_Type>&& other):
			content_{other.content_},
			size_{other.size_}
		{
			other.content_ = nullptr;
			//std::cout << "move ctor\n";
		}

		explicit _vector(const std::vector<_Type>& vec):
			content_{alloc_(vec.size() ? int(vec.size()) : defaultAlloc_)},
			size_{int(vec.size())}
		{
			if (size_) copy_(content_, vec.data(), size_);
		}

		template<typename _Func, typename I>
		_vector(_Func&& fn_, I from, I to, I d = 1):
			content_(alloc_(abs((to - from) / d) + 1)),
			size_(abs((to - from) / d) + 1)
		{
			generate(fn_, from, d);
		}

		////////////////////////////////////////////////////////////////////////////

		//returns number representing nesting of vector
		//which is >1 if contained type is vector
		__forceinline constexpr size_t dim() const
		{
			return DIM;
		}

		//returns const pointer to the content
		__forceinline const _Type* content() const
		{
			return content_;
		}

		//returns current size
		__forceinline size_t size() const
		{
			return size_;
		}

		//returns current number of elements for which memory is allocated
		__forceinline size_t capacity() const
		{
			return reserved_;
		}

		//returns current amount of allocated memory in bytes
		__forceinline size_t memory() const
		{
			return reserved_*ELEMENT_SIZE;
		}

		//returns current value of defaultly allocated memory
		__forceinline size_t default_alloc() const
		{
			return defaultAlloc_;
		}

		//Set size of array defaultly allocated by constructors
		//and operations such as erase
		__forceinline static void set_default_alloc(size_t defSize = DEFAULT_ALLOC)
		{
			defaultAlloc_ = defSize;
		}

		//returns current value of allocation multiplier
		__forceinline static double alloc_mult()
		{
			return allocMult_;
		}

		//sets allocation multiplier to "alm" if value is valid
		static void set_alloc_mult(double alm)
		{
			if (alm > 1.0) {
				allocMult_ = alm;
			}
			else {
				throw 0; // TODO: exception
			}
		}

		//false if is empty
		__forceinline operator bool() const
		{
			return size_;
		}

		__forceinline _vector<_Type>&& move()
		{
			return (_vector<_Type>&&)(*this);
		}

		/*template<typename Y = _Type>
		operator enable_if<std::is_same<decay<Y>, char>::value,
			std::string>() const
		{
			return std::string(content_);
		}*/

		//returns reference to nth element
		__forceinline _Type& operator[](unsigned n) const
		{
			return content_[n];
		}

		//returns copy of this vector concatenated with copy of "other"
		_vector<_Type> operator+(const _vector<_Type>& other) const
		{
			_vector<_Type> result(*this);
			return result.append(other);
		}

		template<bool _ptrDeref = false, typename Y = _Type>
		enable_if<std::is_trivial<Y>::value && !_ptrDeref,
			bool> identical(const _vector<_Type>& other) const
		{
			if (size_ == other.size_)
				return memcmp(content_, other.content_, size_) == 0;
			return false;
		}

		template<bool _ptrDeref = false, typename Y = _Type>
		enable_if<!std::is_trivial<Y>::value || _ptrDeref,
			bool> identical(const _vector<_Type>& other) const
		{
			if (size_ != other.size_)
				return false;
			for (size_t i = 0; i<size_; ++i) {
				if (!compare_elem_<_ptrDeref>(
					content_[i],
					other.content_[i])) {
					return false;
				}
			}
			return true;
		}

		__forceinline bool operator==(const _vector<_Type>& other) const
		{
			return identical<false>(other);
		}

		//returns reference to "n"th element
		_Type& at(size_t n) const
		{
			if (n < size_)
				return content_[n];
			throw ERR_OUT_OF_SIZE;
		}

		//returns reference to first element
		inline _Type& first() const
		{
			if (size_>0)
				return content_[0];
			throw(ERR_GET_EMPTY);
		}

		//returns reference to last element
		inline _Type& last() const
		{
			if (size_>0)
				return content_[size_ - 1];
			throw(ERR_GET_EMPTY);
		}

		//swaps content and parameters
		void swap(_vector<_Type>& other)
		{
			std::swap(size_, other.size_);
			std::swap(reserved_, other.reserved_);
			std::swap(content_, other.content_);
		}

		//swaps two elements
		__forceinline _vector<_Type>& swap(size_t pos1, size_t pos2)
		{
			_Type temp(content_[pos1]);
			content_[pos1] = content_[pos2];
			content_[pos2] = temp;
			return *this;
		}

		//returns vector_ containing non trivial copy of elements
		//in range <p1,p2> inclusive
		_vector<_Type> subset(size_t p1, size_t p2) const
		{
			if (size_ && (p2 >= p1)) {
				_vector<_Type> sub(p2 - p1 + 1, true);
				copy_(sub.content_, content_ + p1, sub.size_);
				return sub;
			}
			return _vector<_Type>();
		}

		//returns vector_ containing non trivial copy of elements
		//in range from p to the last element inclusive
		__forceinline _vector<_Type> subset(size_t p) const
		{
			if (size_) {
				_vector<_Type> sub(size_ - p, true);
				copy_(sub.content_, content_ + p, sub.size_);
				return sub;
			}
			return _vector<_Type>();
		}

		//removes every element beyond range <"p1","p2">
		template<bool _ptrDelete = false, typename Y = _Type>
		enable_if<std::is_pointer<Y>::value && _ptrDelete,
			_vector<_Type>&> cut(size_t p1, size_t p2)
		{
			if (p2 >= size_) p2 = size_ - 1;
			if ((int)p2 - (int)p1 > 0) {
				for (size_t i = 0; i < p1; ++i)
					delete content_[i];
				for (size_t i = p2 + 1; i < size_; ++i)
					delete content_[i];
				memcpy(content_, content_ + p1, ELEMENT_SIZE*(p2 - p1 + 1));
				size_ = p2 - p1 + 1;
			}
			else size_ = 0;

			return *this;
		}
		template<bool _ptrDelete = false, typename Y = _Type>
		enable_if<!std::is_pointer<Y>::value || !_ptrDelete,
			_vector<_Type>&> cut(size_t p1, size_t p2)
		{
			if (p2 >= size_) p2 = size_ - 1;
			if ((int)p2 - (int)p1 > 0) {
				memcpy(content_, content_ + p1, ELEMENT_SIZE*(p2 - p1 + 1));
				size_ = p2 - p1 + 1;
			}
			else size_ = 0;

			return *this;
		}

		template<bool _ptrDelete = false>
		_vector<_Type>& erase(size_t p1, size_t p2)
		{
			if (size_ && (p2 > p1)) {
				static_if(std::is_pointer<_Type>::value && _ptrDelete)
				{
					for (size_t i = p1; i <= p2; ++i)
						delete content_[i];
				}
				memcpy(content_ + p1, content_ + p2 + 1, (size_ - p2 - 1)*ELEMENT_SIZE);
				size_ -= (p2 - p1 + 1);
			}
			return *this;
		}

		////removes every element from position "p1" to "p2" inclusive
		//template<bool _ptrDelete = false, typename Y = _Type>
		//enable_if<std::is_pointer<Y>::value && _ptrDelete,
		//	vector_<_Type>&> erase(size_t p1, size_t p2)
		//{
		//	if (size_ && (p2 > p1)) {
		//		for (size_t i = p1; i <= p2; ++i)
		//			delete content_[i];
		//		memcpy(content_+p1, content_+p2+1, (size_-p2-1)*T_size_);
		//		size_ -= (p2-p1+1);
		//	}
		//	return *this;
		//}
		//template<bool _ptrDelete = false, typename Y = _Type>
		//enable_if<!std::is_pointer<Y>::value || !_ptrDelete,
		//	vector_<_Type>&> erase(size_t p1, size_t p2)
		//{
		//	if (size_ && (p2 > p1)) {
		//		memcpy(content_ + p1, content_ + p2 + 1, (size_ - p2 - 1)*T_size_);
		//		size_ -= (p2 - p1 + 1);
		//	}
		//	return *this;
		//}

		//removes "n"th element 
		template<bool _ptrDelete = false>
		_vector<_Type>& erase(size_t n)
		{
			if (n < size_) {
				destroy_elem_<_ptrDelete>(content_[n]);
				memcpy(content_ + n, content_ + n + 1,
					(size_ - n - 1)*ELEMENT_SIZE);
				--size_;
			}
			return *this;
		}

		//replaces each element with "elem" 
		_vector<_Type>& fill(const _Type& elem)
		{
			for (size_t i = 0; i < size_; ++i) {
				content_[i] = _Type(elem);
			}
			return *this;
		}

		//resizes content to "size" and replaces each element with "elem" 
		_vector<_Type>& fill(const _Type& elem, size_t size)
		{
			delete[] content_;
			content_ = alloc_(size);
			size_ = size;
			for (size_t i = 0; i < size_; ++i) {
				content_[i] = _Type(elem);
			}
			return *this;
		}

		//fills content with values of a function for specified arguments
		template<typename _Func, typename I>
		enable_if<std::is_fundamental<I>::value,
			_vector<_Type>&> generate(_Func&& fn_, I from, I to, I d)
		{
			delete[] content_;
			content_ = alloc_(abs((to - from) / d) + 1);
			size_ = abs((to - from) / d) + 1;
			size_t i = 0;
			for (I x = from; i<size_; x += d) {
				content_[i++] = fn_(x);
			}
			return *this;
		}

		//fills content with values of a function for specified arguments
		template<typename _Func, typename I>
		enable_if<std::is_fundamental<I>::value,
			_vector<_Type>&> generate(_Func&& fn_, I from, I d)
		{
			size_t i = 0;
			for (I x = from; i<size_; x += d) {
				content_[i++] = fn_(x);
			}
			return *this;
		}

		//fills content with values <"from", "to"> with delta = "d"
		template<typename Y = _Type>
		enable_if<std::is_fundamental<Y>::value,
			_vector<Y>&> generate(Y from, Y to, Y d = 1)
		{
			delete[] content_;
			content_ = alloc_(abs((to - from) / d) + 1);
			size_ = abs((to - from) / d) + 1;
			size_t i = 0;
			for (Y x = from; i<size_; x += d) {
				content_[i++] = x;
			}
			return *this;
		}

		//returns vector_ containing "n" front elements
		__forceinline _vector<_Type> front(size_t n) const
		{
			return subset(0, n - 1);
		}

		//returns vector_ containing "n" tail elements
		__forceinline _vector<_Type> back(size_t n) const
		{
			return subset(size_ - n, size_ - 1);
		}

		//resizes array and does non trivial copy of "other"'s content
		_vector<_Type>& operator=(const _vector<_Type> & other)
		{
			if (other.size_) {
				delete[] content_;
				content_ = alloc_(other.size_);
				size_ = other.size_;
				copy_(content_, other.content_, size_);
			}
			return *this;
		}

		_vector<_Type>& operator=(_vector<_Type> && other)
		{
			if (other.size_) {
				delete[] content_;
				content_ = other.content_;
				size_ = other.size_;
				other.content_ = nullptr;
			}
			return *this;
		}

		template<size_t size, typename Y = _Type>
		_vector<_Type>& operator=(const Y(&arr)[size])
		{
			delete[] content_;
			content_ = alloc_(size);
			size_ = size;
			copy_(content_, static_cast<const _Type*>(arr), size_);
			return *this;
		}

		_vector<_Type>& force_trivial_copy(const _vector<_Type> &other)
		{
			if (other.size_) {
				delete[] content_;
				content_ = alloc_(other.size_);
				size_ = other.size_;
				memcpy(content_, other.content_, ELEMENT_SIZE*size_);
			}
			return *this;
		}

		//adds new element on the end
		_vector<_Type>& push_back(const _Type& newElem)
		{
			if (size_ == reserved_)
				realloc_(1 + reserved_ * allocMult_);
			content_[size_++] = newElem;
			return *this;
		}

		//adds new element on the beginning
		__forceinline _vector<_Type>& push_front(_Type newElem)
		{
			insert(newElem, 0);
			return *this;
		}

		//moves the part of content after "pos" one field forward
		//and pastes in new element on "pos"
		_vector<_Type>& insert(const _Type& newElem, size_t pos)
		{
			if (size_ == reserved_) {
				_Type* newContent = alloc_(1 + reserved_*allocMult_);
				memcpy(newContent + pos + 1, content_ + pos, ELEMENT_SIZE*(size_ - pos));
				newContent[pos] = _Type(newElem);
				memcpy(newContent, content_, ELEMENT_SIZE*pos);
				delete[] reinterpret_cast<Shallow*>(content_);
				content_ = newContent;
			}
			else {
				memcpy(content_ + pos + 1, content_ + pos, ELEMENT_SIZE*(size_ - pos));
				content_[pos] = _Type(newElem);
			}
			++size_;
			return *this;
		}

		//moves the part of content after "pos" forward
		//and pastes in content of other array beginning on "pos"
		_vector<_Type>& insert(const _vector<_Type>& other, size_t pos)
		{
			if (reserved_<size_ + other.size_) {
				_Type* newContent = alloc_(size_ + other.size_);
				memcpy(newContent + pos + other.size_, content_ + pos, ELEMENT_SIZE*(size_ - pos));
				copy_(newContent + pos, other.content_, other.size_);
				memcpy(newContent, content_, ELEMENT_SIZE*pos);
				delete[] reinterpret_cast<Shallow*>(content_);
				content_ = newContent;
			}
			else {
				memcpy(content_ + pos + other.size_, content_ + pos, ELEMENT_SIZE*(size_ - pos));
				copy_(content_ + pos, other.content_, other.size_);
			}
			size_ += other.size_;
			return *this;
		}

		//removes up to "n" specified elements "toRem" if such occur.
		//if "n"=0 removes all
		//returns number of removed elements
		template<bool _ptrDelete = false, bool _ptrDeref = false, typename U, typename Y = _Type>
		enable_if<std::is_convertible<Y, U>::value,
			int> remove(U const& val, unsigned max = 0)
		{
			int i = -1, lastp = -1, nrem = 0;
			while (++i < size_) {
				if (compare_elem_<_ptrDeref>((U)content_[i], val)) {
					destroy_elem_<_ptrDelete>(content_[i]);
					if (i > lastp + nrem) {
						memcpy(content_ + lastp,
							content_ + lastp + nrem,
							ELEMENT_SIZE*(i - lastp - nrem));
						lastp = i - nrem;
					}
					if (++nrem == max) break;
				}
			}
			if (size_ - lastp - nrem) {
				memcpy(content_ + lastp,
					content_ + lastp + nrem,
					ELEMENT_SIZE*(size_ - lastp - nrem));
			}
			size_ -= nrem;
			return nrem;
		}

		template<bool _ptrDelete = false, typename _Func>
		int remove_if(_Func&& fn_, unsigned max = 0)
		{
			int i = -1, lastp = -1, nrem = 0;
			while (++i < size_) {
				if (fn_(content_[i])) {
					destroy_elem_<_ptrDelete>(content_[i]);
					if (i > lastp + nrem) {
						memcpy(content_ + lastp,
							content_ + lastp + nrem,
							ELEMENT_SIZE*(i - lastp - nrem));
						lastp = i - nrem;
					}
					if (++nrem == max) break;
				}
			}
			if (size_ - lastp - nrem) {
				memcpy(content_ + lastp,
					content_ + lastp + nrem,
					ELEMENT_SIZE*(size_ - lastp - nrem));
			}
			size_ -= nrem;
			return nrem;
		}

		template<bool _ptrDeref = false, typename _Func>
		_vector<_Type> copy_if(_Func&& fn_)
		{
			_vector<_Type> copied(size_);
			for (int i = 0; i < size_; ++i) {
				if (fn_(content_[i])) copied.push_back(elem_<_ptrDeref>(content_[i]));
			}
			copied.shrink();
			return copied;
		}

		template<bool _ptrDelete = false, bool _ptrDeref = false, typename _Func>
		_vector<_Type> extract_if(_Func&& fn_, unsigned max = 0)
		{
			_vector<_Type> extracted(size_);
			int i = -1, lastp = -1, nrem = 0;
			while (++i < size_) {
				if (fn_(content_[i])) {
					extracted.push_back(elem_<_ptrDeref>(content_[i]));
					destroy_elem_<_ptrDelete>(content_[i]);
					if (i > lastp + nrem) {
						memcpy(content_ + lastp,
							content_ + lastp + nrem,
							ELEMENT_SIZE*(i - lastp - nrem));
						lastp = i - nrem;
					}
					if (++nrem == max) break;
				}
			}
			if (size_ - lastp - nrem) {
				memcpy(content_ + lastp,
					content_ + lastp + nrem,
					ELEMENT_SIZE*(size_ - lastp - nrem));
			}
			size_ -= nrem;
			extracted.shrink();
			return extracted;
		}

		//Erases specified element fast, swapping it with the current last element. 
		//The order of contained elements is not preserved.
		template<bool _ptrDelete = false>
		_vector<_Type>& throw_back(size_t which)
		{
			swap(which, --size_);
			destroy_elem_<_ptrDelete>(content_[size_]);
			return *this;
		}

		//removes first element
		template<bool _ptrDelete = false>
		__forceinline _vector<_Type>& pop_front()
		{
			if (size_) {
				destroy_elem_<_ptrDelete>(content_[0]);
				erase(0);
			}
			return *this;
		}

		//removes last element
		template<bool _ptrDelete = false>
		_vector<_Type>& pop_back()
		{
			if (size_) {
				destroy_elem_<_ptrDelete>(content_[--size_]);
			}
			return *this;
		}

		//uses quick sort on the whole scope
		template<typename Y = _Type>
		__forceinline enable_if<
			has_less_op<Y, bool, const Y&>::value ||
			has_less_op<Y, bool, Y&>::value ||
			!std::is_class<Y>::value,
			_vector<_Type>&> sort()
		{
			quicksort_(0, size_ - 1);
			return *this;
		}

#ifdef XRND_H	
		//set elements in random order
		//if "swap_each" is disabled, each element has
		//some probability of not being moved
		_vector<_Type>& shuffle(bool swap_each = true)
		{
			for (int i = 0; i<size_ - 2; ++i) {
				swap(i, random<size_t>(i + swap_each, size_ - 1));
			}
			return *this;
		}

		//return random element from specified range <"p1","p2"> inclusive
		_Type& get_random(size_t p1, size_t p2) const
		{
			return content_[random<size_t>(p1, p2)];
		}

		//return random element
		__forceinline _Type& get_random() const
		{
			return get_random(0, size_ - 1);
		}

		template<typename Y = _Type>
		enable_if<std::is_fundamental<Y>::value,
			_vector<Y>&> randomize(Y r1, Y r2)
		{
			for (int i = 0; i < size_; ++i) {
				content_[i] = random<Y>(r1, r2);
			}
			return *this;
		}

		template<typename Y = _Type>
		enable_if<std::is_fundamental<Y>::value,
			_vector<_Type>&> randomize(double r1, double r2)
		{
			double factor; _Type r = r2 - r1;
			for (int i = 0; i < size_; ++i) {
				factor = random<_Type>(-r, r);
				content_[i] *= (1 + (factor + r1*sgn(factor)));
			}
			return *this;
		}

		template<typename Y = _Type>
		enable_if<std::is_fundamental<Y>::value,
			_vector<_Type>&> randomize(double r)
		{
			for (int i = 0; i < size_; ++i) {
				content_[i] *= (1 + random<_Type>(-r, r));
			}
			return *this;
		}

#endif

		//deallocate memory exceeding the actual size
		__forceinline void shrink()
		{
			realloc_(size_);
		}

		//only more space reservation is accepted
		template<typename Y = _Type>
		enable_if<_vector<Y>::DIM == 1,
			void> reserve(size_t newSize)
		{
			if (newSize > reserved_) {
				realloc_(newSize);
				return;
			}
			throw(ERR_RESERVE_LESS);
		}

		template<bool useable = false, typename Y = _Type, typename... N>
		enable_if<_vector<Y>::DIM >= 2 &&
			sizeof...(N) <= _vector<Y>::DIM &&
			all_true<std::is_convertible<N, size_t>::value...>::value,
			void> reserve(N... sizes)
		{
			std::initializer_list<size_t> sizes_il{size_t(sizes)...};
			reserve_<useable>(sizes_il, sizes_il.begin());
		}

		/*template<typename... Y>
		enable_if<all_true<std::is_convertible<Y, _Type>::value...>::value,
		vector_<_Type>&> append(Y&&... elements)
		{
		if (reserved_<size_ + sizeof...(Y))
		realloc_(size_ + sizeof...(Y));
		select_t<1, Y...> elemArr[sizeof...(Y)]{std::forward<Y>(elements)...};
		copy_(content_+size_, elemArr, sizeof...(Y));
		size_ += sizeof...(Y);
		return *this;
		}

		template<typename... Y>
		enable_if<all_true<std::is_convertible<Y, _Type>::value...>::value,
		vector_<_Type>&> append2(Y&&... elements)
		{
		if (reserved_<size_ + sizeof...(Y))
		realloc_(size_ + sizeof...(Y));
		select_t<1, Y...> elemArr[sizeof...(Y)]{std::forward<Y>(elements)...};
		copy_(content_+size_, elemArr, sizeof...(Y));
		size_ += sizeof...(Y);
		return *this;
		}
		*/
		//inserts non trivial copy
		//of "other"'s content on the end
		_vector<_Type>& append(const _vector<_Type>& other)
		{
			if (reserved_<size_ + other.size_)
				realloc_(size_ + other.size_);
			copy_(content_ + size_, other.content_, other.size_);
			size_ += other.size_;
			return *this;
		}

		//inserts new elements on the end
		_vector<_Type>& append(std::initializer_list<_Type> other)
		{
			if (reserved_<size_ + other.size())
				realloc_(size_ + other.size());
			int k = size_;
			for (typename std::initializer_list<_Type>::iterator i = other.begin();
				i != other.end(); ++i, ++k) {
				content_[k] = _Type(*i);
			}
			size_ += other.size();
			return *this;
		}

		//moves content forward and inserts non trivial copy
		//of "other"'s content on the beginning
		__forceinline _vector<_Type>& prepend(const _vector<_Type>& other)
		{
			insert(other, 0);
			return *this;
		}

		//moves content forward and
		//inserts new elements on the beginning
		_vector<_Type>& prepend(std::initializer_list<_Type> other)
		{
			if (reserved_<size_ + other.size())
				realloc_(size_ + other.size());
			memcpy(content_ + other.size(), content_, ELEMENT_SIZE*size_);
			int k = 0;
			for (typename std::initializer_list<_Type>::iterator i = other.begin();
				i != other.end(); ++i, ++k) {
				content_[k] = _Type(*i);
			}
			size_ += other.size();
			return *this;
		}

		//returns number of occurances of "elem"
		template<bool _ptrDeref = false>
		int count(const _Type& elem) const
		{
			int n = 0;
			for (size_t i = 0; i<size_; ++i) {
				if (compare_elem_<_ptrDeref>(content_[i], elem)) ++n;
			}
			return n;
		}

		//returns reference to element, value of which is equal to "elem"
		template<bool refpos = true, bool _ptrDeref = false, typename U, typename Y = _Type>
		enable_if<std::is_convertible<Y, U>::value,
			_Type&> find(U const& elem)
		{
			for (size_t i = 0; i < size_; ++i)
				if (compare_elem_<_ptrDeref>((U)content_[i], elem))
					return content_[i];
			throw(ERR_NOT_FOUND);
		}

		template<bool refpos = true, bool _ptrDeref = false, typename U, typename Y = _Type>
		enable_if<std::is_convertible<Y, U>::value,
			size_t> pos_of(U const& elem) const
		{
			for (size_t i = 0; i < size_; ++i)
				if (compare_elem_<_ptrDeref>((U)content_[i], elem))
					return i;
			throw(ERR_NOT_FOUND);
		}

		template<bool _ptrDeref = false, typename U, typename Y = _Type>
		enable_if<std::is_convertible<Y, U>::value,
			bool> contain(U const& elem) const
		{
			for (size_t i = 0; i < size_; ++i)
				if (compare_elem_<_ptrDeref>((U)content_[i], elem)) return true;
			return false;
		}

		//returns reference to element, for which "fn"'s return value is equal to "val"
		template<typename cT, typename _Func>
		_Type& find_by(_Func&& fn_, cT val)
		{
			for (size_t i = 0; i < size_; ++i)
				if (fn_(content_[i]) == val) return content_[i];
			throw(ERR_NOT_FOUND);
		}

		//returns reference to element, "member"'s value of which is equal to "val"
		template<typename cT, typename Y = _Type>
		enable_if<std::is_class<Y>::value,
			Y&>	find_by(cT Y::*member, cT&& val)
		{
			for (size_t i = 0; i < size_; ++i)
				if (content_[i].*member == val) return content_[i];
			throw ERR_NOT_FOUND;
		}

		//replaces each occurance of "what" element with "val"
		template<bool _ptrDeref = false>
		int replace(const _Type& what, const _Type& val, unsigned nmax = 0)
		{
			int nrep = 0;
			for (int i = 0; i < size_; ++i) {
				if (compare_elem_<_ptrDeref>(content_[i], what)) {
					assign_elem_<_ptrDeref>(content_[i], val);
					if (++nrep == nmax) return nrep;
				}
			}
			return nrep;
		}

		//sets elements in reverse order
		_vector<_Type>& reverse()
		{
			for (int i = size_ / 2 - 1; i >= 0; --i) {
				swap(i, size_ - i - 1);
			}
			return *this;
		}

		//moves elements "n" fields forward / backward (if "n" < 0)
		//making them to appear on the beginning / end
		_vector<_Type>& shift(int n)
		{
			if (n %= int(size_)) {
				if (n < 0) n += size_;
				_Type* newContent = alloc_(size_);
				memcpy(newContent, content_ + size_ - n, ELEMENT_SIZE*n);
				memcpy(newContent + n, content_, ELEMENT_SIZE*(size_ - n));
				delete[] reinterpret_cast<Shallow*>(content_);
				content_ = newContent;
			}
			return *this;
		}

		_Type take(size_t n)
		{
			if (n<size_) {
				_Type elem = content_[n];
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
		void disp(const char separator = ' ',
			std::ostream& os = std::cout,
			const char ending = '\n') const
		{
			for (int i = 0; i<size_; ++i) {
				os << content_[i] << separator;
			}
			os << ending;
		}

		void disp(size_t p1, size_t p2,
			const char separator = ' ',
			std::ostream& os = std::cout,
			const char ending = '\n') const
		{
			if (p2 > size_ - 1)p2 = size_ - 1;
			for (int i = p1; i <= p2; ++i) {
				os << content_[i] << separator;
			}
			os << ending;
		}

		//passes content to output stream
		template<typename Y>
		friend enable_if<std::is_same<decay<Y>, char>::value,
			std::ostream&> operator<<(std::ostream &os, const _vector<Y> &arr);

		template<typename Y>
		friend enable_if< !std::is_same<decay<Y>, char>::value,
			std::ostream&> operator<<(std::ostream &os, const _vector<Y> &arr);

		//pushes back new element from input stream
		friend std::istream &operator >> (std::istream &is, _vector<_Type> &arr)
		{
			if (arr.size_ == arr.reserved_)
				arr.realloc_(1 + arr.reserved_ * arr.allocMult_);
			is >> arr.content_[arr.size_++];
			return is;
		}

		//calls function on every element
		template<typename _Func>
		_vector<_Type>& call(_Func&& fn_)
		{
			for (int i = 0; i<size_; ++i) {
				fn_(content_[i]);
			}
			return *this;
		}

		//calls function on every element for which condition is true
		template<typename C, typename _Func>
		_vector<_Type>& call_if(C&& cond, _Func&& fn_)
		{
			for (int i = 0; i<size_; ++i) {
				if (cond(content_[i])) fn_(content_[i]);
			}
			return *this;
		}

		//calls any member function with given parameters on every element
		template<typename... argT, typename Y = _Type, typename R = void>
		enable_if<std::is_class<Y>::value, _vector<_Type>&>
			call(R(Y::*f)(argT...), argT... args)
		{
			for (int i = 0; i<size_; ++i) {
				(content_[i].*f)(args...);
			}
			return *this;
		}

		//checks if condition is true for every element
		template<typename _Func>
		bool true_for_all(_Func&& fn_)
		{
			for (int i = 0; i<size_; ++i)
				if (!fn_(content_[i])) return false;
			return true;
		}

		//forgets the content with no memory deallocation
		template<bool _ptrDelete = false, typename Y = _Type>
		enable_if<std::is_pointer<Y>::value && _ptrDelete,
			_vector<_Type>&> clear()
		{
			if (size_) {
				for (int i = 0; i < size_; ++i)
					delete content_[i];
				size_ = 0;
			}
			return *this;
		}
		template<bool _ptrDelete = false, typename Y = _Type>
		enable_if<!std::is_pointer<Y>::value || !_ptrDelete,
			_vector<_Type>&> clear()
		{
			size_ = 0;
			return *this;
		}

		//clears all content and allocates new block of memory of default size
		template<bool _ptrDelete = false, typename Y = _Type>
		enable_if<std::is_pointer<Y>::value && _ptrDelete,
			_vector<_Type>&> erase()
		{
			if (size_) {
				for (int i = 0; i < size_; ++i)
					delete content_[i];
				delete[] content_;
				content_ = alloc_(defaultAlloc_);
				size_ = 0;
			}
			return *this;
		}

		template<bool _ptrDelete = false, typename Y = _Type>
		enable_if<!std::is_pointer<Y>::value || !_ptrDelete,
			_vector<_Type>&> erase()
		{
			if (size_) {
				delete[] content_;
				content_ = alloc_(defaultAlloc_);
				size_ = 0;
			}
			return *this;
		}

		//returns std::vector with non trivial copy of content
		std::vector<_Type> to_std_vector()
		{
			return std::vector<_Type>(content_, content_ + size_ - 1);
		}

		std::list<_Type> to_std_list()
		{
			std::list<_Type> ret;
			int i = size_;
			while (i--)
				ret.push_front(content_[i]);
			return ret;
		}


		//If conversion is possible returns new instance of x::vector
		//with elements converted to specified _Type
		template<typename _To>
		enable_if<std::is_convertible<_Type, _To>::value &&
			DIM == 1,
			_vector<_To>> convert_to()
		{
			_vector<_To> converted(size_);
			for (int i = 0; i < size_; ++i) {
				converted.content_[i] = _To(content_[i]);
			}
			converted.size_ = size_;
			return converted;
		}

		template<typename _To>
		enable_if<std::is_convertible<typename _AtomicType<_vector<_Type>>::type, _To>::value &&
			DIM >= 2,
			typename _vector_nd<_To, DIM>::type> convert_to()
		{
			typename _vector_nd<_To, DIM>::type converted(size_);
			for (int i = 0; i < size_; ++i) {
				converted.content_[i] = content_[i].convert_to<_To>();
			}
			converted.size_ = size_;
			return converted;
		}

		/*template<bool _ptrDeref = false>
		void save_data(std::fstream& file)
		{
		xsrl xfile(file);
		for (int i = 0; i<size_; ++i) {
		xfile<<elem_<_ptrDeref>(content_[i]);
		}
		}

		void load_data(std::fstream& file)
		{
		xsrl xfile(file);
		file.
		}*/

		~_vector()
		{
			if (content_) delete[] content_;
		}

	};



	template<typename _Type>
	size_t _vector<_Type>::defaultAlloc_ = 10;

	template<typename _Type>
	double _vector<_Type>::allocMult_ = 2.0;


	template<typename _Type, unsigned _dim = 1>
	using vector = typename std::conditional<_dim == 1, _vector<_Type>,
		typename _vector_nd<_Type, _dim>::type>::type;

	/*using string = _vector<char>;
	using wstring = _vector<wchar_t>;*/

	template<typename Y>
	enable_if<std::is_same<decay<Y>, char>::value,
		std::ostream&> operator<<(std::ostream &os, const _vector<Y> &arr)
	{
		return os << arr.content_;
	}

	template<typename Y>
	enable_if< !std::is_same<decay<Y>, char>::value,
		std::ostream&> operator<<(std::ostream &os, const _vector<Y> &arr)
	{
		arr.disp(' ', os, '\n');
		return os;
	}

} //namespace x

#undef enable_if
#undef decay
#undef DEBUG

#endif //XARR_H