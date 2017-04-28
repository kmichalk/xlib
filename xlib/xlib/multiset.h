//#ifndef _X_MULTISET_H_
//#define _X_MULTISET_H_
//
//
//#include <iostream>
//#include "more_type_traits.h"
//#include "simple.h"
//#include "autoref.h"
//#include "result.h"
//#include "allocator.h"
//
//#define DEBUG 0
//
//#define enable_if typename std::enable_if_t
//#define decay typename std::decay_t
//
//namespace x
//{
//
//	template<typename> class _multiset;
//
//	////////////////////////////////////////////////////////////////////////////
//
//	//type expanding into n-dimensional multiset
//	template<typename T_, unsigned DIM_>
//	struct _multiset_nd
//	{
//		using type = _multiset<typename _multiset_nd<T_, DIM_ - 1>::type>;
//	};
//
//	template<typename T_>
//	struct _multiset_nd<T_, 0>
//	{
//		using type = T_;
//	};
//
//	////////////////////////////////////////////////////////////////////////////
//
//	template<typename _Type>
//	class _multiset
//	{
//	public:
//		template<typename Y>
//		struct _CountDim
//		{
//			static constexpr size_t value = 0;
//		};
//		template<typename Y>
//		struct _CountDim<_multiset<Y>>
//		{
//			static constexpr size_t value = 1 + _CountDim<Y>::value;
//		};
//
//		template<typename Y>
//		struct _AtomicType
//		{
//			using type = Y;
//		};
//		template<typename Y>
//		struct _AtomicType<_multiset<Y>>
//		{
//			using type = typename _AtomicType<Y>::type;
//		};
//
//		////////////////////////////////////////////////////////////////////////////
//
//		static constexpr size_t DIM_ = _CountDim<_multiset<_Type>>::value;
//		static constexpr size_t ELEMENT_SIZE_ = sizeof(_Type);
//		static constexpr size_t DEFAULT_ALLOC_ = 10;
//
//		static size_t defaultAlloc_;
//		static double allocMult_;
//
//		////////////////////////////////////////////////////////////////////////////
//
//		_Type* data_;
//		size_t size_;
//		size_t reserved_;
//
//		////////////////////////////////////////////////////////////////////////////
//
//		//__forceinline _Type* allocator<_Type>::allocate(size_t newSize)
//		//{
//		//	reserved_ = newSize;
//		//	//return new _Type[newSize];
//		//	return reinterpret_cast<_Type*>(new byte[newSize*ELEMENT_SIZE]);
//		//}
//
//		inline void reallocate_(size_t newSize)
//		{
//			allocator<_Type>::reallocate(data_, reserved_, newSize);
//			reserved_ = newSize;
//		}
//
//		template<bool _ptrDeref = false>
//		void quicksort_(int r1, int r2)
//		{
//			int last = r1;
//			for (int i = r1; i < r2; ++i) {
//				if (compare_less_data_[i], data_[r2]) {
//					if (last != i) swap(i, last);
//					++last;
//				}
//			}
//			swap(last, r2);
//			if (r1 < last - 1) quicksort_(r1, last - 1);
//			if (last + 1 < r2) quicksort_(last + 1, r2);
//		}
//
//
//		ADVANCED_MEMBER_TEST(has_less_op, typename R, operator<, const, typename A);
//
//		////////////////////////////////////////////////////////////////////////////
//
//	public:
//		template<typename>
//		friend class _multiset;
//
//		////////////////////////////////////////////////////////////////////////////
//
//		enum error
//		{
//			ERR_OUT_OF_SIZE,
//			ERR_NOTHING_TO_REMOVE,
//			ERR_INVALID_ALLOC_MULT,
//			ERR_GET_EMPTY,
//			ERR_NOT_FOUND,
//			ERR_RESERVE_LESS,
//			ERR_RESIZE_CONST
//		};
//
//		////////////////////////////////////////////////////////////////////////////
//
//		using type = _Type;
//
//		using size_type = size_t;
//		using difference_type = ptrdiff_t;
//		using value_type = _Type;
//		using pointer = _Type*;
//		using reference = _Type&;
//
//
//		////////////////////////////////////////////////////////////////////////////
//
//
//		/*class moved
//		{
//		template<class>	friend class _multiset;
//
//		_Type* data_;
//		size_t size_;
//		size_t reserved_;
//		public:
//		moved(_multiset<_Type>&& mset):
//		data_{mset.data_},
//		size_{mset.size_},
//		reserved_{mset.reserved_}
//		{
//		mset.data_ = nullptr;
//		}
//		};*/
//
//		////////////////////////////////////////////////////////////////////////////
//
//		struct iterator
//		{
//			_multiset<_Type>& obj;
//			size_t pos;
//			iterator(_multiset<_Type>& obj): obj{obj}, pos{0}
//			{
//			}
//			iterator(_multiset<_Type>& obj, size_t pos): obj{obj}, pos{pos}
//			{
//			}
//			iterator(const iterator& other): obj{other.obj}, pos{other.pos}
//			{
//			}
//			__forceinline iterator& operator++()
//			{
//				++pos;
//				return *this;
//			}
//			__forceinline iterator& operator--()
//			{
//				--pos;
//				return *this;
//			}
//			inline iterator operator++(int)
//			{
//				iterator copy{*this};
//				++pos;
//				return copy;
//			}
//			inline iterator operator--(int)
//			{
//				iterator copy{*this};
//				--pos;
//				return copy;
//			}
//			__forceinline iterator operator+(int i)
//			{
//				return iterator(obj, pos + i);
//			}
//			__forceinline iterator operator-(int i)
//			{
//				return iterator(obj, pos - i);
//			}
//			__forceinline iterator& operator+=(int i)
//			{
//				pos += i; return *this;
//			}
//			__forceinline iterator& operator-=(int i)
//			{
//				pos -= i; return *this;
//			}
//			__forceinline iterator operator+(size_t i)
//			{
//				return iterator(obj, pos + i);
//			}
//			__forceinline iterator operator-(size_t i)
//			{
//				return iterator(obj, pos - i);
//			}
//			__forceinline iterator& operator+=(size_t i)
//			{
//				pos += i; return *this;
//			}
//			__forceinline iterator& operator-=(size_t i)
//			{
//				pos -= i; return *this;
//			}
//			__forceinline _Type& operator*()
//			{
//				return obj.data_[pos];
//			}
//			__forceinline bool operator==(const iterator& other) const
//			{
//				return pos == other.pos;
//			}
//			__forceinline bool operator!=(const iterator& other) const
//			{
//				return pos != other.pos;
//			}
//			__forceinline bool operator<(const iterator& other) const
//			{
//				return pos < other.pos;
//			}
//			__forceinline bool operator>(const iterator& other) const
//			{
//				return pos > other.pos;
//			}
//			__forceinline bool operator<=(const iterator& other) const
//			{
//				return pos <= other.pos;
//			}
//			__forceinline bool operator>=(const iterator& other) const
//			{
//				return pos >= other.pos;
//			}
//			__forceinline iterator& operator=(const iterator& other)
//			{
//				pos = other.pos; return *this;
//			}
//			__forceinline operator bool()
//			{
//				return pos < obj.size_;
//			}
//			__forceinline auto operator->()
//			{
//				return x::pointer(obj.data_[pos]);
//			}
//			//TODO: Think about casts from iterator to _Type.
//			/*__forceinline operator _Type const&() {
//			return obj.data_[pos];
//			}
//			__forceinline operator _Type&() {
//			return obj.data_[pos];
//			}*/
//		};
//
//		////////////////////////////////////////////////////////////////////////////
//
//		struct const_iterator
//		{
//			const _multiset<_Type>& obj;
//			size_t pos;
//			const_iterator(const _multiset<_Type>& obj): obj{obj}, pos{0}
//			{
//			}
//			const_iterator(const _multiset<_Type>& obj, size_t pos): obj{obj}, pos{pos}
//			{
//			}
//			const_iterator(const const_iterator& other): obj{other.obj}, pos{other.pos}
//			{
//			}
//			__forceinline const_iterator& operator++()
//			{
//				++pos;
//				return *this;
//			}
//			__forceinline const_iterator& operator--()
//			{
//				--pos;
//				return *this;
//			}
//			inline const_iterator operator++(int)
//			{
//				const_iterator copy{*this};
//				++pos;
//				return copy;
//			}
//			inline const_iterator operator--(int)
//			{
//				const_iterator copy{*this};
//				--pos;
//				return copy;
//			}
//			__forceinline const_iterator operator+(int i)
//			{
//				return const_iterator(obj, pos + i);
//			}
//			__forceinline const_iterator operator-(int i)
//			{
//				return const_iterator(obj, pos - i);
//			}
//			__forceinline const_iterator& operator+=(int i)
//			{
//				pos += i;
//				return *this;
//			}
//			__forceinline const_iterator& operator-=(int i)
//			{
//				pos -= i;
//				return *this;
//			}
//			__forceinline const_iterator operator+(size_t i)
//			{
//				return const_iterator(obj, pos + i);
//			}
//			__forceinline const_iterator operator-(size_t i)
//			{
//				return const_iterator(obj, pos - i);
//			}
//			__forceinline const_iterator& operator+=(size_t i)
//			{
//				pos += i; return *this;
//			}
//			__forceinline const_iterator& operator-=(size_t i)
//			{
//				pos -= i; return *this;
//			}
//			__forceinline const _Type& operator*() const
//			{
//				return obj.data_[pos];
//			}
//			__forceinline bool operator==(const const_iterator& other) const
//			{
//				return pos == other.pos;
//			}
//			__forceinline bool operator!=(const const_iterator& other) const
//			{
//				return pos != other.pos;
//			}
//			__forceinline bool operator<(const const_iterator& other) const
//			{
//				return pos < other.pos;
//			}
//			__forceinline bool operator>(const const_iterator& other) const
//			{
//				return pos > other.pos;
//			}
//			__forceinline bool operator<=(const const_iterator& other) const
//			{
//				return pos <= other.pos;
//			}
//			__forceinline bool operator>=(const const_iterator& other) const
//			{
//				return pos >= other.pos;
//			}
//			__forceinline const_iterator& operator=(const const_iterator& other)
//			{
//				pos = other.pos; return *this;
//			}
//			__forceinline operator bool()
//			{
//				return pos < obj.size_;
//			}
//			__forceinline auto operator->()
//			{
//				return x::pointer(obj.data_[pos]);
//			}
//			/*__forceinline operator _Type const&() {
//			return obj.data_[pos];
//			}
//			__forceinline operator _Type&() {
//			return obj.data_[pos];
//			}*/
//		};
//
//		////////////////////////////////////////////////////////////////////////////
//
//
//		__forceinline iterator begin()
//		{
//			return iterator(*this, 0);
//		}
//		__forceinline iterator end()
//		{
//			return iterator(*this, size_);
//		}
//		__forceinline const_iterator cbegin() const
//		{
//			return const_iterator(*this, 0);
//		}
//		__forceinline const_iterator cend() const
//		{
//			return const_iterator(*this, size_);
//		}
//		
//		////////////////////////////////////////////////////////////////////////////
//
//		_multiset():
//			data_(allocator<_Type>::raw_allocate(defaultAlloc_)),
//			size_(0),
//			reserved_{defaultAlloc_}
//		{
//			//std::cout << "def ctor\n";
//		}
//
//		_multiset(size_t size, bool useable = false):
//			data_(allocator<_Type>::raw_allocate(size)),
//			size_(useable ? size : 0),
//			reserved_{size}
//		{
//			//std::cout << "size ctor\n";
//		}
//
//		_multiset(_Type* arr, size_t size, size_t extra = 0):
//			data_(allocator<_Type>::raw_allocate(size + extra)),
//			size_(size),
//			reserved_{size_ + extra}
//		{
//			allocator<_Type>::raw_copy(data_, arr, size_);
//		}
//
//		template<size_t _size>
//		_multiset(const _Type(&arr)[_size], size_t extra = 0):
//			data_(allocator<_Type>::raw_allocate(_size + extra)),
//			size_(_size),
//			reserved_{_size + extra}
//		{
//			allocator<_Type>::raw_copy(data_, static_cast<const _Type*>(arr), _size);
//		}
//
//		//template<typename Y = _Type, typename = enable_if<std::is_same<decay<Y>, char>::value>>
//		//_multiset(const _Type* arr):
//		//	data_{allocator<_Type>::allocate(strlen(arr) + 1)},
//		//	size_{reserved_}
//		//{
//		//	copy_(data_, arr, reserved_);
//		//}
//
//		_multiset(std::initializer_list<_Type> arr):
//			data_(allocator<_Type>::raw_allocate(arr.size())),
//			size_(arr.size()),
//			reserved_{arr.size()}
//		{
//			//std::cout << "init_list ctor\n";
//			int k = 0;
//			for (auto i = arr.begin(); i != arr.end(); ++i, ++k) {
//				data_[k] = *i;
//			}
//		}
//
//		/*template<typename... Y,
//		typename U = enable_if<every_is<_Type, Y...>::value>>
//		multiset_(Y... elements) :
//		size_(sizeof...(elements)),
//		reserved_(sizeof...(elements))
//		{
//		data_ = new _Type[size_]{elements...};
//		}*/
//
//		_multiset(const _multiset<_Type>& other):
//			size_{other.size_ ? other.size_ : 0},
//			reserved_{other.size_ ? other.size_ : defaultAlloc_},
//			data_{allocator<_Type>::raw_allocate(other.size_ ? other.size_ : defaultAlloc_)}
//		{
//			//std::cout << "copy ctor\n";
//			if (size_) allocator<_Type>::raw_copy(data_, other.data_, size_);
//		}
//
//		_multiset(_multiset<_Type>&& other):
//			data_{other.data_},
//			size_{other.size_},
//			reserved_{other.reserved_}
//		{
//			other.data_ = nullptr;
//			//std::cout << "move ctor\n";
//		}
//
//		/*_multiset(moved const& movedData):
//		data_{movedData.data_},
//		size_{movedData.size_},
//		reserved_{movedData.reserved_}
//		{
//		}
//		*/
//		explicit _multiset(const std::multiset<_Type>& mset):
//			size_{mset.size() ? mset.size() : 0},
//			reserved_{size_ ? size_ : defaultAlloc_},
//			data_{allocator<_Type>::raw_allocate(reserved_)}
//		{
//			if (size_) allocator<_Type>::raw_copy(data_, mset.data(), size_);
//		}
//
//		/*template<typename _Func, typename I>
//		_multiset(_Func&& fn_, I from, I to, I d = 1):
//		reserved_{abs((to - from) / d) + 1},
//		size_{abs((to - from) / d) + 1},
//		data_{allocator<_Type>::allocate(abs((to - from) / d) + 1)}
//		{
//		generate(fn_, from, d);
//		}*/
//
//		////////////////////////////////////////////////////////////////////////////
//
//		//returns number representing nesting of multiset
//		//which is >1 if contained type is multiset
//		__forceinline constexpr size_t dim() const
//		{
//			return DIM_;
//		}
//
//		//returns const pointer to the content
//		__forceinline _Type const* data() const
//		{
//			return data_;
//		}
//
//		_Type* extract_data()
//		{
//			_Type* prev = data_;
//			reserved_ = defaultAlloc_;
//			data_ = allocator<_Type>::allocate(reserved_);
//			size_ = 0;
//			return prev;
//		}
//
//		_multiset<_Type>& adopt(_Type* data, size_t size)
//		{
//			if (data_) allocator<_Type>::deallocate(data_, size_);
//			reserved_ = size;
//			size_ = size;
//			data_ = data;
//		}
//
//		//returns current size
//		__forceinline size_t size() const
//		{
//			return size_;
//		}
//
//		__forceinline size_t lastpos() const
//		{
//			return size_ - 1;
//		}
//
//		//returns current number of elements for which memory is allocated
//		__forceinline size_t capacity() const
//		{
//			return reserved_;
//		}
//
//		//returns current amount of allocated memory in bytes
//		__forceinline size_t memory() const
//		{
//			return reserved_*ELEMENT_SIZE_;
//		}
//
//		//returns current value of defaultly allocated memory
//		__forceinline size_t default_alloc() const
//		{
//			return defaultAlloc_;
//		}
//
//		//Set size of array defaultly allocated by constructors
//		//and operations such as erase
//		__forceinline static void set_default_alloc(size_t defSize = DEFAULT_ALLOC_)
//		{
//			defaultAlloc_ = defSize;
//		}
//
//		//returns current value of allocation multiplier
//		__forceinline static double alloc_mult()
//		{
//			return allocMult_;
//		}
//
//		//sets allocation multiplier to "alm" if value is valid
//		static void set_alloc_mult(double alm)
//		{
//			if (alm > 1.0) {
//				allocMult_ = alm;
//			}
//			else {
//				throw 0; // TODO: exception
//			}
//		}
//
//		//false if is empty
//		__forceinline operator bool() const
//		{
//			return size_;
//		}
//
//		__forceinline _multiset<_Type>&& move()
//		{
//			return (_multiset<_Type>&&)(*this);
//		}
//
//		//returns reference to nth element
//		__forceinline _Type& operator[](size_t pos) const
//		{
//			return data_[pos];
//		}
//
//		//returns copy of this multiset concatenated with copy of "other"
//		_multiset<_Type> operator+(const _multiset<_Type>& other) const
//		{
//			_multiset<_Type> result(*this);
//			return result.append(other);
//		}
//
//		template<_capture(_Type)>
//		enable_if<std::is_trivial<_Type>::value,
//			bool> identical(const _multiset<_Type>& other) const
//		{
//			if (size_ == other.size_)
//				return memcmp(data_, other.data_, size_) == 0;
//			return false;
//		}
//
//		template<_capture(_Type)>
//		enable_if<!std::is_trivial<_Type>::value,
//			bool> identical(const _multiset<_Type>& other) const
//		{
//			if (size_ != other.size_)
//				return false;
//			for (size_t i = 0; i<size_; ++i) {
//				if (!(data_[i] == other.data_[i])) {
//					return false;
//				}
//			}
//			return true;
//		}
//
//		__forceinline bool operator==(const _multiset<_Type>& other) const
//		{
//			return identical<false>(other);
//		}
//
//		//returns reference to "n"th element
//		x::result<_Type&> at(size_t n) const
//		{
//			if (n < size_)
//				return data_[n];
//			return x::result<_Type&>::INVALID;
//		}
//
//		//returns reference to first element
//		inline result<_Type&> first() const
//		{
//			if (size_>0)
//				return data_[0];
//			return x::result<_Type&>::INVALID;
//		}
//
//		//returns reference to last element
//		inline result<_Type&> last() const
//		{
//			if (size_>0)
//				return data_[size_ - 1];
//			return x::result<_Type&>::INVALID;
//		}
//
//		//swaps content and parameters
//		inline void swap(_multiset<_Type>& other)
//		{
//			allocator<_multiset<_Type>>::swap(*this, other);
//			/*std::swap(size_, other.size_);
//			std::swap(reserved_, other.reserved_);
//			std::swap(data_, other.data_);*/
//		}
//
//		//swaps two elements
//		__forceinline _multiset<_Type>& swap(size_t pos1, size_t pos2)
//		{
//			allocator<_Type>::swap(data_[pos1], data_[pos2]);
//			/*_Type temp(data_[pos1]);
//			data_[pos1] = data_[pos2];
//			data_[pos2] = temp;*/
//			return *this;
//		}
//
//		//returns multiset_ containing non trivial copy of elements
//		//in range <p1,p2> inclusive
//		_multiset<_Type> subset(size_t p1, size_t p2) const
//		{
//			if (size_ && (p2 >= p1)) {
//				_multiset<_Type> sub(p2 - p1 + 1, true);
//				allocator<_Type>::raw_copy(sub.data_, data_ + p1, sub.size_);
//				return sub;
//			}
//			return _multiset<_Type>();
//		}
//
//		//returns multiset_ containing non trivial copy of elements
//		//in range from p to the last element inclusive
//		__forceinline _multiset<_Type> subset(size_t p) const
//		{
//			if (size_) {
//				_multiset<_Type> sub(size_ - p, true);
//				allocator<_Type>::raw_copy(sub.data_, data_ + p, sub.size_);
//				return sub;
//			}
//			return _multiset<_Type>();
//		}
//
//		//removes every element beyond range <"p1","p2">
//		template<bool _ptrDelete = false, typename Y = _Type>
//		enable_if<std::is_pointer<Y>::value && _ptrDelete,
//			_multiset<_Type>&> cut(size_t p1, size_t p2)
//		{
//			if (p2 >= size_) p2 = size_ - 1;
//			if ((int)p2 - (int)p1 > 0) {
//				for (size_t i = 0; i < p1; ++i)
//					delete data_[i];
//				for (size_t i = p2 + 1; i < size_; ++i)
//					delete data_[i];
//				memcpy(data_, data_ + p1, ELEMENT_SIZE_*(p2 - p1 + 1));
//				size_ = p2 - p1 + 1;
//			}
//			else size_ = 0;
//
//			return *this;
//		}
//		template<bool _ptrDelete = false, typename Y = _Type>
//		enable_if<!std::is_pointer<Y>::value || !_ptrDelete,
//			_multiset<_Type>&> cut(size_t p1, size_t p2)
//		{
//			if (p2 >= size_) p2 = size_ - 1;
//			if ((int)p2 - (int)p1 > 0) {
//				memcpy(data_, data_ + p1, ELEMENT_SIZE_*(p2 - p1 + 1));
//				size_ = p2 - p1 + 1;
//			}
//			else size_ = 0;
//
//			return *this;
//		}
//
//		template<bool _ptrDelete = false>
//		_multiset<_Type>& erase(size_t p1, size_t p2)
//		{
//			if (size_ && (p2 > p1)) {
//				/*static_if(std::is_pointer<_Type>::value && _ptrDelete)
//				{
//				for (size_t i = p1; i <= p2; ++i)
//				delete data_[i];
//				}*/
//				memcpy(data_ + p1, data_ + p2 + 1, (size_ - p2 - 1)*ELEMENT_SIZE_);
//				size_ -= (p2 - p1 + 1);
//			}
//			return *this;
//		}
//
//		////removes every element from position "p1" to "p2" inclusive
//		//template<bool _ptrDelete = false, typename Y = _Type>
//		//enable_if<std::is_pointer<Y>::value && _ptrDelete,
//		//	multiset_<_Type>&> erase(size_t p1, size_t p2)
//		//{
//		//	if (size_ && (p2 > p1)) {
//		//		for (size_t i = p1; i <= p2; ++i)
//		//			delete data_[i];
//		//		memcpy(data_+p1, data_+p2+1, (size_-p2-1)*T_size_);
//		//		size_ -= (p2-p1+1);
//		//	}
//		//	return *this;
//		//}
//		//template<bool _ptrDelete = false, typename Y = _Type>
//		//enable_if<!std::is_pointer<Y>::value || !_ptrDelete,
//		//	multiset_<_Type>&> erase(size_t p1, size_t p2)
//		//{
//		//	if (size_ && (p2 > p1)) {
//		//		memcpy(data_ + p1, data_ + p2 + 1, (size_ - p2 - 1)*T_size_);
//		//		size_ -= (p2 - p1 + 1);
//		//	}
//		//	return *this;
//		//}
//
//		//removes "n"th element 
//		_multiset<_Type>& erase(size_t n)
//		{
//			if (n < size_) {
//				allocator<_Type>::destruct(data_[n]);
//				//destroy_data_[n];
//				memcpy(data_ + n, data_ + n + 1,
//					(size_ - n - 1)*ELEMENT_SIZE_);
//				--size_;
//			}
//			return *this;
//		}
//
//		//replaces each element with "elem" 
//		_multiset<_Type>& fill(const _Type& elem)
//		{
//			for (size_t i = 0; i < size_; ++i) {
//				allocator<_Type>::destruct(data_[i]);
//				allocator<_Type>::construct(data_ + i, elem);
//			}
//			return *this;
//		}
//
//		//resizes content to "size" and replaces each element with "elem" 
//		_multiset<_Type>& fill(const _Type& elem, size_t size)
//		{
//			allocator<_Type>::deallocate(data_, size_);
//			reserved_ = size;
//			size_ = size;
//			data_ = allocator<_Type>::raw_allocate(reserved_);
//			for (size_t i = 0; i < size_; ++i) {
//				allocator<_Type>::reconstruct(data_ + i, elem);
//			}
//			return *this;
//		}
//
//		//fills content with values of a function for specified arguments
//		template<typename _Func, typename _Iter>
//		enable_if<std::is_fundamental<_Iter>::value,
//			_multiset<_Type>&> generate(_Func&& fn_, _Iter from, _Iter to, _Iter d)
//		{
//			allocator<_Type>::deallocate(data_, size_);
//			reserved_ = abs((to - from) / d) + 1;
//			size_ = reserved_;
//			data_ = allocator<_Type>::raw_allocate(reserved_);
//
//			size_t i = 0;
//			for (_Iter x = from; i<size_; x += d) {
//				allocator<_Type>::reconstruct(data_[i++], (_Type&&)fn_(x));
//			}
//			return *this;
//		}
//
//		//fills content with values of a function for specified arguments
//		template<typename _Func, typename _Iter>
//		enable_if<std::is_fundamental<_Iter>::value,
//			_multiset<_Type>&> generate(_Func&& fn_, _Iter from, _Iter d)
//		{
//			size_t i = 0;
//			for (_Iter x = from; i<size_; x += d) {
//				data_[i++] = fn_(x);
//			}
//			return *this;
//		}
//
//		//fills content with values <"from", "to"> with delta = "d"
//		template<typename Y = _Type>
//		enable_if<std::is_fundamental<Y>::value,
//			_multiset<Y>&> generate(Y from, Y to, Y d = 1)
//		{
//			allocator<_Type>::deallocate(data_, size_);
//			reserved_ = abs((to - from) / d) + 1;
//			size_ = reserved_;
//			data_ = allocator<_Type>::raw_allocate(reserved_);
//
//			size_t i = 0;
//			for (Y x = from; i<size_; x += d) {
//				data_[i++] = x;
//			}
//			return *this;
//		}
//
//		//returns multiset_ containing "n" front elements
//		__forceinline _multiset<_Type> front(size_t n) const
//		{
//			return subset(0, n - 1);
//		}
//
//		//returns multiset_ containing "n" tail elements
//		__forceinline _multiset<_Type> back(size_t n) const
//		{
//			return subset(size_ - n, size_ - 1);
//		}
//
//		//resizes array and does non trivial copy of "other"'s content
//		_multiset<_Type>& operator=(const _multiset<_Type> & other)
//		{
//			if (other.size_) {
//				if (other.size_ > reserved_) {
//					if (data_)
//						allocator<_Type>::deallocate(data_, size_);
//					reserved_ = other.size_;
//					size_ = other.size_;
//					data_ = allocator<_Type>::raw_allocate(reserved_);
//				}
//				else {
//					allocator<_Type>::destruct(data_, size_);
//					size_ = other.size_;
//				}
//				allocator<_Type>::raw_copy(data_, other.data_, size_);
//			}
//			else {
//				allocator<_Type>::destruct(data_, size_);
//				size_ = 0;
//			}
//			return *this;
//		}
//
//		_multiset<_Type>& operator=(_multiset<_Type> && other)
//		{
//			if (other.reserved_) {
//				if (data_)
//					allocator<_Type>::deallocate(data_, size_);
//				reserved_ = other.reserved_;
//				size_ = other.size_;
//				data_ = other.data_;
//				other.data_ = nullptr;
//			}
//			else {
//				allocator<_Type>::destruct(data_, size_);
//				size_ = 0;
//			}
//			return *this;
//		}
//
//		/*_multiset<_Type>& operator=(moved const& other)
//		{
//		if (other.size_) {
//		allocator<_Type>::deallocate(data_, size_);
//		data_ = other.data_;
//		size_ = other.size_;
//		reserved_ = other.reserved_;
//		}
//		else {
//		allocator<_Type>::destruct(data_, size_);
//		size_ = 0;
//		}
//		return *this;
//		}*/
//
//		template<size_t size, typename Y = _Type>
//		_multiset<_Type>& operator=(const Y(&arr)[size])
//		{
//			allocator<_Type>::deallocate(data_, size_);
//			data_ = allocator<_Type>::raw_allocate(size);
//			size_ = size;
//			allocator<_Type>::raw_copy(data_, static_cast<const _Type*>(arr), size_);
//			return *this;
//		}
//
//		//adds new element on the end
//		_multiset<_Type>& push_back(const _Type& newElem)
//		{
//			if (size_ == reserved_)
//				reallocate_(1 + reserved_ * allocMult_);
//
//			allocator<_Type>::construct(data_ + size_++, newElem);
//			return *this;
//		}
//
//		//adds new element on the beginning
//		__forceinline _multiset<_Type>& push_front(_Type newElem)
//		{
//			insert(newElem, 0);
//			return *this;
//		}
//
//		//moves the part of content after "pos" one field forward
//		//and pastes in new element on "pos"
//		_multiset<_Type>& insert(const _Type& newElem, size_t pos)
//		{
//			if (pos > size_) pos = size_;
//
//			if (size_ == reserved_) {
//				reserved_ = 1 + reserved_*allocMult_;
//				_Type* newData = allocator<_Type>::raw_allocate(reserved_);
//				memcpy(newData + pos + 1, data_ + pos, ELEMENT_SIZE_*(size_ - pos));
//				allocator<_Type>::construct(newData + pos, newElem);
//				memcpy(newData, data_, ELEMENT_SIZE_*pos);
//				delete[] reinterpret_cast<byte*>(data_);
//				data_ = newData;
//			}
//			else {
//				memcpy(data_ + pos + 1, data_ + pos, ELEMENT_SIZE_*(size_ - pos));
//				allocator<_Type>::construct(data_ + pos, newElem);
//			}
//			++size_;
//			return *this;
//		}
//
//		//moves the part of content after "pos" forward
//		//and pastes in content of other array beginning on "pos"
//		_multiset<_Type>& insert(const _multiset<_Type>& other, size_t pos)
//		{
//			if (pos > size_) pos = size_;
//
//			if (reserved_ < size_ + other.size_) {
//				reserved_ = size_ + other.size_;
//				_Type* newData = allocator<_Type>::raw_allocate(reserved_);
//				memcpy(newData + pos + other.size_, data_ + pos, ELEMENT_SIZE_*(size_ - pos));
//				allocator<_Type>::raw_copy(newData + pos, other.data_, other.size_);
//				memcpy(newData, data_, ELEMENT_SIZE_*pos);
//				delete[] reinterpret_cast<byte*>(data_);
//				data_ = newData;
//			}
//			else {
//				memcpy(data_ + pos + other.size_, data_ + pos, ELEMENT_SIZE_*(size_ - pos));
//				allocator<_Type>::raw_copy(data_ + pos, other.data_, other.size_);
//			}
//			size_ += other.size_;
//			return *this;
//		}
//
//	private:
//		template<class _Verifier>
//		int remove_(unsigned max)
//		{
//			int i = -1, lastp = -1, nrem = 0;
//			while (++i < size_) {
//				if (_Verifier::verify(data_[i])) {
//					allocator<_Type>::destruct(data_[i]);
//					if (i > lastp + nrem) {
//						memcpy(data_ + lastp,
//							data_ + lastp + nrem,
//							ELEMENT_SIZE_*(i - lastp - nrem));
//						lastp = i - nrem;
//					}
//					if (++nrem == max) break;
//				}
//			}
//			if (size_ - lastp - nrem) {
//				memcpy(data_ + lastp,
//					data_ + lastp + nrem,
//					ELEMENT_SIZE_*(size_ - lastp - nrem));
//			}
//			size_ -= nrem;
//			return nrem;
//		}
//
//		struct VerifierCache
//		{
//			static void const* value;
//		};
//
//		template<class _Cmp>
//		struct ComparatorVerifier
//		{
//			static __forceinline bool verify(_Type const& elem)
//			{
//				return elem == *reinterpret_cast<_Cmp*>(VerifierCache::value);
//			}
//		};
//
//		template<class _Func>
//		struct FuncVerifier
//		{
//			static inline bool verify(_Type const& elem)
//			{
//				return (*reinterpret_cast<_Func*>(VerifierCache::value))(elem);
//			}
//		};
//
//		template<class _Member>
//		struct MemberVerifier
//		{
//			struct Data
//			{
//				_Member(_Type::*member);
//				_Member const& value;
//			};
//
//			static inline bool verify(_Type const& elem)
//			{
//				return (elem.*reinterpret_cast<Data const*>(VerifierCache::value)->member) ==
//					reinterpret_cast<Data const*>(VerifierCache::value)->value;
//			}
//		};
//
//	public:
//		template<typename _Cmp>
//		_multiset<_Type>& remove(_Cmp const& val, unsigned max = 0)
//		{
//			VerifierCache::value = &val;
//			remove_<ComparatorVerifier<_Cmp const>>(max);
//			return *this;
//		}
//
//
//		template<typename _Func>
//		_multiset<_Type>& remove_if(_Func&& fn_, unsigned max = 0)
//		{
//			VerifierCache::value = x::pointer(fn_);
//			remove_<FuncVerifier<x::naked<_Func> const>>(max);
//			return *this;
//		}
//
//		template<typename _Member, _capture(_Type)>
//		std::enable_if_t<std::is_class<_Type>::value,
//			_multiset<_Type>&> remove_by(_Member(_Type::*member), _Member const& value, unsigned max = 0)
//		{
//			MemberVerifier<_Member>::Data verifierData{member, value};
//			VerifierCache::value = &verifierData;
//			remove_<MemberVerifier<_Member>>(max);
//			return *this;
//		}
//
//
//		template<typename _Func>
//		_multiset<_Type> copy_if(_Func&& fn_)
//		{
//			_multiset<_Type> copied(size_);
//			for (int i = 0; i < size_; ++i) {
//				if (fn_(data_[i])) copied.push_back(data_[i]);
//			}
//			copied.shrink();
//			return copied;
//		}
//
//		template<typename _Func>
//		_multiset<_Type> extract_if(_Func&& fn_, unsigned max = 0)
//		{
//			_multiset<_Type> extracted(size_);
//			int i = -1, lastp = -1, nrem = 0;
//			while (++i < size_) {
//				if (fn_(data_[i])) {
//					extracted.push_back(data_[i]);
//					allocator<_Type>::destruct(data_[i]);
//					if (i > lastp + nrem) {
//						memcpy(data_ + lastp,
//							data_ + lastp + nrem,
//							ELEMENT_SIZE_*(i - lastp - nrem));
//						lastp = i - nrem;
//					}
//					if (++nrem == max) break;
//				}
//			}
//			if (size_ - lastp - nrem) {
//				memcpy(data_ + lastp,
//					data_ + lastp + nrem,
//					ELEMENT_SIZE_*(size_ - lastp - nrem));
//			}
//			size_ -= nrem;
//			//extracted.shrink();
//			return extracted;
//		}
//
//		//Erases specified element fast, swapping it with the current last element. 
//		//The order of contained elements is not preserved.
//
//		_multiset<_Type>& throw_back(size_t which)
//		{
//			if (which < size_) {
//				allocator<_Type>::destruct(data_[which]);
//				swap(which, --size_);
//			}
//			//x::assign(data_ + which, data_ + (--size_));
//			return *this;
//		}
//
//		_multiset<_Type>& dump(size_t which)
//		{
//			if (which < size_) {
//				allocator<_Type>::destruct(data_[which]);
//				x::assign(data_ + which, data_ + (--size_));
//			}
//			return *this;
//		}
//
//		//removes first element
//		__forceinline _multiset<_Type>& pop_front()
//		{
//			if (size_) {
//				allocator<_Type>::destruct(data_[0]);
//				erase(0);
//			}
//			return *this;
//		}
//
//		//removes last element
//		_multiset<_Type>& pop_back()
//		{
//			if (size_) {
//				allocator<_Type>::destruct_data_[--size_];
//			}
//			return *this;
//		}
//
//		//uses quick sort on the whole scope
//		template<typename Y = _Type>
//		__forceinline enable_if<
//			has_less_op<Y, bool, const Y&>::value ||
//			has_less_op<Y, bool, Y&>::value ||
//			!std::is_class<Y>::value,
//			_multiset<_Type>&> sort()
//		{
//			quicksort_(0, size_ - 1);
//			return *this;
//		}
//
//#ifdef _X_RND_H_	
//		//set elements in random order
//		//if "swap_each" is disabled, each element has
//		//some probability of not being moved
//		_multiset<_Type>& shuffle(bool swap_each = true)
//		{
//			for (int i = 0; i<size_ - 2; ++i) {
//				swap(i, random<size_t>(i + swap_each, size_ - 1));
//			}
//			return *this;
//		}
//
//		//return random element from specified range <"p1","p2"> inclusive
//		_Type& get_random(size_t p1, size_t p2) const
//		{
//			return data_[random<size_t>(p1, p2)];
//		}
//
//		//return random element
//		__forceinline _Type& get_random() const
//		{
//			return get_random(0, size_ - 1);
//		}
//
//		template<typename Y = _Type>
//		enable_if<std::is_fundamental<Y>::value,
//			_multiset<Y>&> randomize(Y r1, Y r2)
//		{
//			for (int i = 0; i < size_; ++i) {
//				data_[i] = random<Y>(r1, r2);
//			}
//			return *this;
//		}
//
//		template<typename Y = _Type>
//		enable_if<std::is_fundamental<Y>::value,
//			_multiset<_Type>&> randomize(double r1, double r2)
//		{
//			double factor; _Type r = r2 - r1;
//			for (int i = 0; i < size_; ++i) {
//				factor = random<_Type>(-r, r);
//				data_[i] *= (1 + (factor + r1*sgn(factor)));
//			}
//			return *this;
//		}
//
//		template<typename Y = _Type>
//		enable_if<std::is_fundamental<Y>::value,
//			_multiset<_Type>&> randomize(double r)
//		{
//			for (int i = 0; i < size_; ++i) {
//				data_[i] *= (1 + random<_Type>(-r, r));
//			}
//			return *this;
//		}
//
//#endif
//
//		//deallocate memory exceeding the actual size
//		__forceinline void shrink()
//		{
//			reallocate_(size_);
//		}
//
//		//only more space reservation is accepted
//		_multiset<_Type>& reserve(size_t newSize)
//		{
//			if (newSize > reserved_)
//				reallocate_(newSize);
//
//			return *this;
//		}
//
//		/*template<bool useable = false, typename Y = _Type, typename... N>
//		enable_if<_multiset<Y>::DIM_ >= 2 &&
//		sizeof...(N) <= _multiset<Y>::DIM_ &&
//		all_true<std::is_convertible<N, size_t>::value...>::value,
//		void> reserve(N... sizes)
//		{
//		std::initializer_list<size_t> sizes_il{size_t(sizes)...};
//		reserve_<useable>(sizes_il, sizes_il.begin());
//		}*/
//
//		//inserts non trivial copy of "other"'s content on the end
//		_multiset<_Type>& append(const _multiset<_Type>& other)
//		{
//			if (reserved_ < size_ + other.size_)
//				reallocate_(size_ + other.size_);
//			allocator<_Type>::raw_copy(data_ + size_, other.data_, other.size_);
//			size_ += other.size_;
//			return *this;
//		}
//
//		//inserts new elements on the end
//		_multiset<_Type>& append(std::initializer_list<_Type> other)
//		{
//			if (reserved_ < size_ + other.size())
//				reallocate_(size_ + other.size());
//			int k = size_;
//			for (typename std::initializer_list<_Type>::iterator i = other.begin();
//				i != other.end(); ++i, ++k) {
//				data_[k] = _Type(*i);
//			}
//			size_ += other.size();
//			return *this;
//		}
//
//		//moves content forward and inserts non trivial copy
//		//of "other"'s content on the beginning
//		__forceinline _multiset<_Type>& prepend(const _multiset<_Type>& other)
//		{
//			insert(other, 0);
//			return *this;
//		}
//
//		//moves content forward and
//		//inserts new elements on the beginning
//		_multiset<_Type>& prepend(std::initializer_list<_Type> other)
//		{
//			if (reserved_<size_ + other.size())
//				reallocate_(size_ + other.size());
//			memcpy(data_ + other.size(), data_, ELEMENT_SIZE_*size_);
//			int k = 0;
//			for (typename std::initializer_list<_Type>::iterator i = other.begin();
//				i != other.end(); ++i, ++k) {
//				data_[k] = _Type(*i);
//			}
//			size_ += other.size();
//			return *this;
//		}
//
//		//returns number of occurances of "elem"
//		int count(const _Type& elem) const
//		{
//			int n = 0;
//			for (size_t i = 0; i<size_; ++i) {
//				if (data_[i] == elem) ++n;
//			}
//			return n;
//		}
//
//		//returns reference to element, value of which is equal to "elem"
//		result<_Type&> find(_Type const& elem)
//		{
//			for (size_t i = 0; i < size_; ++i)
//				if (data_[i] == elem)
//					return data_[i];
//			return result<_Type&>::INVALID;
//		}
//
//
//		size_t pos_of(_Type const& elem) const
//		{
//			for (size_t i = 0; i < size_; ++i)
//				if (compare_(Udata_[i], elem))
//					return i;
//			throw(ERR_NOT_FOUND);
//		}
//
//
//		/*enable_if<std::is_convertible<Y, U>::value,
//		bool> contain(U const& elem) const
//		{
//		for (size_t i = 0; i < size_; ++i)
//		if (compare_(Udata_[i], elem)) return true;
//		return false;
//		}*/
//
//		//returns reference to element, for which "fn"'s return value is equal to "val"
//		/*template<typename _Cmp, typename _Func>
//		result<_Type&> find_by(_Func&& fn_, _Cmp const& val)
//		{
//		for (size_t i = 0; i < size_; ++i)
//		if (fn_(data_[i]) == val) return data_[i];
//		return result<_Type&>::INVALID;
//		}*/
//
//		template<typename _Func>
//		result<_Type&> find_by(_Func&& fn_)
//		{
//			for (size_t i = 0; i < size_; ++i)
//				if (fn_(data_[i])) return data_[i];
//			return result<_Type&>::INVALID;
//		}
//
//
//		//returns reference to element, "member"'s value of which is equal to "val"
//		template<class _Member, _capture(_Type)>
//		std::enable_if_t<std::is_class<_Type>::value,
//			iterator> find_by(_Member(_Type::*member), _Member const& val)
//		{
//			iterator it{begin()};
//			for (; it; ++it)
//				if (((*it).*member) == val) return it;
//			return it;
//		}
//
//		//replaces each occurance of "what" element with "val"
//		int replace(const _Type& what, const _Type& val, unsigned nmax = 0)
//		{
//			int nrep = 0;
//			for (int i = 0; i < size_; ++i) {
//				if (data_[i] == what) {
//					allocator<_Type>::reconstruct(data_ + i, val);
//					if (++nrep == nmax) return nrep;
//				}
//			}
//			return nrep;
//		}
//
//		//sets elements in reverse order
//		_multiset<_Type>& reverse()
//		{
//			for (int i = size_ / 2 - 1; i >= 0; --i) {
//				swap(i, size_ - i - 1);
//			}
//			return *this;
//		}
//
//		//moves elements "n" fields forward / backward (if "n" < 0)
//		//making them to appear on the beginning / end
//		_multiset<_Type>& shift(int n)
//		{
//			if (n %= (int)size_) {
//				if (n < 0) n += size_;
//				_Type* newData = allocator<_Type>::raw_allocate(reserved_);
//				memcpy(newData, data_ + size_ - n, ELEMENT_SIZE_*n);
//				memcpy(newData + n, data_, ELEMENT_SIZE_*(size_ - n));
//				delete[] reinterpret_cast<byte*>(data_);
//				data_ = newData;
//			}
//			return *this;
//		}
//
//		_Type take(size_t n)
//		{
//			if (n < size_) {
//				_Type elem = data_[n];
//				erase(n);
//				return elem;
//			}
//			throw ERR_OUT_OF_SIZE;
//		}
//
//		//true if is empty
//		__forceinline bool empty() const
//		{
//			return !bool(size_);
//		}
//
//		//passes content to std::cout stream with "separator" after each
//		void disp(
//			char			separator = ' ',
//			std::ostream&	os = std::cout,
//			char			ending = '\n') const
//		{
//			for (size_t i = 0; i < size_; ++i)
//				os << data_[i] << separator;
//			os << ending;
//		}
//
//		void disp(
//			size_t			p1,
//			size_t			p2,
//			char			separator = ' ',
//			std::ostream&	os = std::cout,
//			char			ending = '\n') const
//		{
//			if (p2 > size_ - 1) p2 = size_ - 1;
//			for (size_t i = p1; i <= p2; ++i)
//				os << data_[i] << separator;
//			os << ending;
//		}
//
//		//passes content to output stream
//		friend std::ostream& operator<<(std::ostream &os, const _multiset<_Type> &mset)
//		{
//			mset.disp(' ', os, '\n');
//			return os;
//		}
//
//		//pushes back new element from input stream
//		friend std::istream &operator >> (std::istream &is, _multiset<_Type> &arr)
//		{
//			if (arr.size_ == arr.reserved_) {
//				arr.reallocate_(arr.reserved_, 1 + arr.reserved_ * arr.allocMult_);
//			}
//			is >> arr.data_[arr.size_++];
//			return is;
//		}
//
//		//calls function on every element
//		template<typename _Func>
//		_multiset<_Type>& call(_Func&& fn_)
//		{
//			for (int i = 0; i<size_; ++i) {
//				fn_(data_[i]);
//			}
//			return *this;
//		}
//
//		//calls function on every element for which condition is true
//		template<typename _Cond, typename _Func>
//		_multiset<_Type>& call_if(_Func&& fn, _Cond&& cond)
//		{
//			for (int i = 0; i<size_; ++i) {
//				if (cond(data_[i])) fn(data_[i]);
//			}
//			return *this;
//		}
//
//		//calls any member function with given parameters on every element
//		template<typename... _Args, _capture(_Type), typename _Ret>
//		std::enable_if_t<std::is_class<_Type>::value,
//			_multiset<_Type>&>
//			call(_Ret(_Type::*fn)(_Args...), _Args&&... args)
//		{
//			for (int i = 0; i<size_; ++i) {
//				(data_[i].*fn)(args...);
//			}
//			return *this;
//		}
//
//		//checks if condition is true for every element
//		template<typename _Func>
//		bool true_for_all(_Func&& fn_)
//		{
//			for (int i = 0; i<size_; ++i)
//				if (!fn_(data_[i])) return false;
//			return true;
//		}
//
//		//forgets the content with no memory deallocation
//
//		template<x::multiset_opt = x::multiset_opt::PTR_DELETE>
//		_multiset<_Type>& clear()
//		{
//			allocator<_Type>::destruct(data_, size_);
//			size_ = 0;
//			return *this;
//		}
//
//		//clears all content and allocates new block of memory of default size		
//		_multiset<_Type>& erase()
//		{
//			if (size_) {
//				allocator<_Type>::deallocate(data_, size_);
//				data_ = allocator<_Type>::raw_allocate(defaultAlloc_);
//				reserved_ = defaultAlloc_;
//				size_ = 0;
//			}
//			return *this;
//		}
//
//
//		//returns std::multiset with non trivial copy of content
//		std::multiset<_Type> to_std_multiset()
//		{
//			return std::multiset<_Type>(data_, data_ + size_ - 1);
//		}
//
//		std::list<_Type> to_std_list()
//		{
//			std::list<_Type> ret;
//			int i = size_;
//			while (i--)
//				ret.push_front(data_[i]);
//			return ret;
//		}
//
//
//		//If conversion is possible returns new instance of x::multiset
//		//with elements converted to specified _Type
//		template<typename _To>
//		enable_if<std::is_convertible<_Type, _To>::value &&
//			DIM_ == 1,
//			_multiset<_To>> convert_to()
//		{
//			_multiset<_To> converted(size_);
//			for (int i = 0; i < size_; ++i) {
//				converted.data_[i] = _To(data_[i]);
//			}
//			converted.size_ = size_;
//			return converted;
//		}
//
//		template<typename _To>
//		enable_if<std::is_convertible<typename _AtomicType<_multiset<_Type>>::type, _To>::value &&
//			DIM_ >= 2,
//			typename _multiset_nd<_To, DIM_>::type> convert_to()
//		{
//			typename _multiset_nd<_To, DIM_>::type converted(size_);
//			for (int i = 0; i < size_; ++i) {
//				converted.data_[i] = data_[i].convert_to<_To>();
//			}
//			converted.size_ = size_;
//			return converted;
//		}
//
//		~_multiset()
//		{
//			if (data_) allocator<_Type>::deallocate(data_, size_);
//			//if (data_) delete[] data_;
//		}
//
//	};
//
//
//
//	template<typename _Type>
//	size_t _multiset<_Type>::defaultAlloc_ = 10;
//
//	template<typename _Type>
//	double _multiset<_Type>::allocMult_ = 2.0;
//
//
//	template<typename _Type, unsigned _dim = 1>
//	using multiset = typename std::conditional<_dim == 1, _multiset<_Type>,
//		typename _multiset_nd<_Type, _dim>::type>::type;
//
//	template<class _Type>
//	void const* _multiset<_Type>::VerifierCache::value = nullptr;
//
//
//} //namespace x
//
//#undef enable_if
//#undef decay
//#undef DEBUG
//
//#endif //_X_MULTISET_H_