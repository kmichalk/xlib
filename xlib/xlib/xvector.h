//#pragma once
#ifndef XARR_H
#define XARR_H
//#include "stdafx.h"

//#include "random_engine_.h"
#include <vector>
#include <list>
#include <iostream>
#include <string>
#include "more_type_traits.h"
#include "xrnd.h"
#include <map>

#define DEBUG 0

#define enable_if typename std::enable_if_t
#define decay typename std::decay_t

template <typename T>
__forceinline int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

namespace x {

template<typename> class vector_;

//type expanding into n-dimensional vector
template<typename T_, unsigned dim_>
struct vector_nd_
{
	using type = vector_<typename vector_nd_<T_, dim_ - 1>::type>;
};

template<typename T_>
struct vector_nd_<T_, 0>
{
	using type = T_;
};

namespace vector_opt {
enum fn_options
{
	PTR_DELETE = 1,
	PTR_DEREF = 1,
	USEABLE = 1
};
}

template<typename T>
class vector_
{
private:
	static bool force_single_shallow_delete__;

protected:
	class T_fake_
	{
		char fake_data_[sizeof(T)];
	public:
		~T_fake_() {}
	};

	using T_shallow_ = typename std::conditional<
		std::is_class<T>::value &&
		!std::is_trivially_copy_constructible<T>::value,
		T_fake_, T>::type;

	template<typename Y>
	struct count_dim_
	{
		static constexpr size_t value = 0;
	};
	template<typename Y>
	struct count_dim_<vector_<Y>>
	{
		static constexpr size_t value = 1 + count_dim_<Y>::value;
	};

	template<typename Y>
	struct atomic_type_
	{
		using type = Y;
	};
	template<typename Y>
	struct atomic_type_<vector_<Y>>
	{
		using type = typename atomic_type_<Y>::type;
	};

	T* content_;

	static size_t default_alloc_;
	static constexpr size_t T_size_ = sizeof(T);
	size_t size_;
	size_t reserved_;
	static double alloc_mult_;
	static constexpr size_t dim_ = count_dim_<vector_<T>>::value;

	__forceinline T* alloc_(size_t newSize)
	{
		reserved_ = newSize;
		return new T[newSize];
	}

	void realloc_(size_t newSize)
	{
		T* newContent = alloc_(newSize);
		memcpy(newContent, content_, T_size_*size_);
		delete[] reinterpret_cast<T_shallow_*>(content_);
		content_ = newContent;
	}

	template<bool ptr_deref = false>
	void quicksort_(int r1, int r2)
	{
		int last = r1;
		for (int i = r1; i < r2; ++i) {
			if (compare_less_elem_<ptr_deref>(content_[i], content_[r2])) {
				if (last != i) swap(i, last);
				++last;
			}
		}
		swap(last, r2);
		if (r1 < last - 1) quicksort_(r1, last - 1);
		if (last + 1 < r2) quicksort_(last + 1, r2);
	}

	template<bool useable, typename Y = T>
	enable_if<vector_<Y>::dim_>=2,
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
	template<bool useable, typename Y = T>
	enable_if<vector_<Y>::dim_==1,
		void> reserve_(std::initializer_list<size_t>& sizes,
			std::initializer_list<size_t>::iterator it)
	{
		if (*it > reserved_) {
			realloc_(*it);
			if (useable) size_ = *it;
		}
	}

	template<typename Y = T>
	__forceinline enable_if<
		std::is_trivially_copy_constructible<Y>::value,
		void> copy_(Y* dest, const Y* arr, size_t s) const
	{
		memcpy(dest, arr, T_size_*s);
	}
	template<typename Y = T>
	__forceinline enable_if<
		!std::is_trivially_copy_constructible<Y>::value,
		void> copy_(Y* dest, const Y* arr, size_t s) const
	{
		while (s--) dest[s] = arr[s];
	}

	template<bool ptr_deref = false, typename Y = T>
	__forceinline enable_if<std::is_pointer<Y>::value && ptr_deref,
		bool> compare_elem_(const Y& el1, const Y& el2) const
	{
		return *el1 == *el2;
	}
	template<bool ptr_deref = false, typename Y = T>
	__forceinline enable_if<!std::is_pointer<Y>::value || !ptr_deref,
		bool> compare_elem_(const Y& el1, const Y& el2) const
	{
		return el1 == el2;
	}

	template<bool ptr_delete = false, typename Y = T>
	__forceinline enable_if<std::is_pointer<Y>::value && ptr_delete,
		void> destroy_elem_(const Y& el)
	{
		delete el;
	}
	template<bool ptr_delete = false, typename Y = T>
	__forceinline enable_if<!std::is_pointer<Y>::value || !ptr_delete,
		void> destroy_elem_(const Y& el)
	{
	}

	template<bool ptr_deref = false, typename Y = T>
	__forceinline enable_if<std::is_pointer<Y>::value && ptr_deref,
		void> assign_elem_(const Y& el1, const Y& el2)
	{
		return *el1 = *el2;
	}
	template<bool ptr_deref = false, typename Y = T>
	__forceinline enable_if<!std::is_pointer<Y>::value || !ptr_deref,
		void> assign_elem_(const Y& el1, const Y& el2)
	{
		return el1 = el2;
	}

	template<bool ptr_deref = false, typename Y = T>
	__forceinline enable_if<std::is_pointer<Y>::value && ptr_deref,
		bool> compare_less_elem_(const Y& el1, const Y& el2) const
	{
		return *el1 < *el2;
	}
	template<bool ptr_deref = false, typename Y = T>
	__forceinline enable_if<!std::is_pointer<Y>::value || !ptr_deref,
		bool> compare_less_elem_(const Y& el1, const Y& el2) const
	{
		return el1 < el2;
	}

	template<bool ptr_deref = false, typename Y = T>
	__forceinline enable_if<std::is_pointer<Y>::value && ptr_deref,
		typename std::remove_pointer_t<Y>&> elem_(Y& el)
	{
		return *el;
	}
	template<bool ptr_deref = false, typename Y = T>
	__forceinline enable_if<!std::is_pointer<Y>::value || !ptr_deref,
		Y&> elem_(Y& el)
	{
		return el;
	}

	ADVANCED_MEMBER_TEST(has_less_op, typename R, operator<, const, typename A);

public:

	template<typename>
	friend class vector_;

	enum error
	{
		ERR_OUT_OF_SIZE, ERR_NOTHING_toREMOVE,
		ERR_INVALID_ALLOC_MULT, ERR_GET_EMPTY,
		ERR_NOT_FOUND, ERR_RESERVE_LESS,
		ERR_RESIZE_CONST
	};

	typedef T type;

	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;

	struct iterator
	{
		vector_<T>& obj;
		size_t pos;
		iterator(vector_<T>& obj) : obj{obj}, pos{0} {}
		iterator(vector_<T>& obj, size_t pos) : obj{obj}, pos{pos} {}
		iterator(const iterator& other) : obj{other.obj}, pos{other.pos} {}
		__forceinline iterator& operator++() {
			++pos; return *this;
		}
		__forceinline iterator& operator--() {
			--pos; return *this;
		}
		__forceinline iterator operator+(size_t i) {
			return iterator(obj, pos+i);
		}
		__forceinline iterator operator-(size_t i) {
			return iterator(obj, pos-i);
		}
		__forceinline iterator& operator+=(size_t i) {
			pos += i; return *this;
		}
		__forceinline iterator& operator-=(size_t i) {
			pos -= i; return *this;
		}
		__forceinline T& operator*() {
			return obj.content_[pos];
		}
		__forceinline bool operator==(const iterator& other) const {
			return pos == other.pos;
		}
		__forceinline bool operator!=(const iterator& other) const {
			return pos != other.pos;
		}
		__forceinline bool operator<(const iterator& other) const {
			return pos < other.pos;
		}

		/*__forceinline bool operator<(const size_t& i) const {
			return pos < i;
		}*/

		__forceinline bool operator>(const iterator& other) const {
			return pos > other.pos;
		}
		__forceinline bool operator<=(const iterator& other) const {
			return pos <= other.pos;
		}
		__forceinline bool operator>=(const iterator& other) const {
			return pos >= other.pos;
		}
		__forceinline iterator& operator=(const iterator& other) {
			pos = other.pos; return *this;
		}
		__forceinline operator bool() {
			return pos < obj.size_;
		}
	};

	struct const_iterator
	{
		const vector_<T>& obj;
		size_t pos;
		const_iterator(const vector_<T>& obj) : obj{obj}, pos{0} {}
		const_iterator(const vector_<T>& obj, size_t pos) : obj{obj}, pos{pos} {}
		const_iterator(const const_iterator& other) : obj{other.obj}, pos{other.pos} {}
		__forceinline const_iterator& operator++() {
			++pos;
			return *this;
		}
		__forceinline const_iterator& operator--() {
			--pos;
			return *this;
		}
		__forceinline const_iterator operator+(size_t i) {
			return const_iterator(obj, pos+i);
		}
		__forceinline const_iterator operator-(size_t i) {
			return const_iterator(obj, pos-i);
		}
		__forceinline const_iterator& operator+=(size_t i) {
			pos += i;
			return *this;
		}
		__forceinline const_iterator& operator-=(size_t i) {
			pos -= i;
			return *this;
		}
		__forceinline const T& operator*() const {
			return obj.content_[pos];
		}
		__forceinline bool operator==(const const_iterator& other) const {
			return pos == other.pos;
		}
		__forceinline bool operator!=(const const_iterator& other) const {
			return pos != other.pos;
		}
		__forceinline bool operator<(const const_iterator& other) const {
			return pos < other.pos;
		}
		__forceinline bool operator>(const const_iterator& other) const {
			return pos > other.pos;
		}
		__forceinline bool operator<=(const const_iterator& other) const {
			return pos <= other.pos;
		}
		__forceinline bool operator>=(const const_iterator& other) const {
			return pos >= other.pos;
		}
		__forceinline const_iterator& operator=(const const_iterator& other) {
			pos = other.pos; return *this;
		}
		__forceinline operator bool() {
			return pos < obj.size_;
		}
	};

	struct loop_iterator
	{
		vector_<T>& obj;
		size_t pos;
		loop_iterator(vector_<T>& obj) : obj{obj}, pos{0} {}
		loop_iterator(vector_<T>& obj, size_t pos) : obj{obj}, pos{pos} {
			if (pos>=obj.size_)
				pos = pos % obj.size_;
		}
		loop_iterator(const loop_iterator& other) : obj{other.obj}, pos{other.pos} {
		}
		__forceinline loop_iterator& operator++() {
			if (++pos >= obj.size_) pos = 0;
			return *this;
		}
		__forceinline loop_iterator& operator--() {
			if (--pos >= obj.size_) pos = obj.size_-1;
			return *this;
		}
		__forceinline loop_iterator operator+(size_t i) {
			return loop_iterator(obj, pos+i);
		}
		__forceinline loop_iterator operator-(size_t i) {
			return loop_iterator(obj, pos-i);
		}
		__forceinline loop_iterator& operator+=(size_t i) {
			if ((pos += i)>=obj.size_) pos = obj.size_;
			return *this;
		}
		__forceinline loop_iterator& operator-=(size_t i) {
			if ((pos -= i)>=obj.size_) pos = obj.size_;
			return *this;
		}
		__forceinline T& operator*() {
			return obj.content_[pos];
		}
		__forceinline bool operator==(const loop_iterator& other) const {
			return pos == other.pos;
		}
		__forceinline bool operator!=(const loop_iterator& other) const {
			return pos != other.pos;
		}
		__forceinline bool operator<(const loop_iterator& other) const {
			return pos < other.pos;
		}
		__forceinline bool operator>(const loop_iterator& other) const {
			return pos > other.pos;
		}
		__forceinline bool operator<=(const loop_iterator& other) const {
			return pos <= other.pos;
		}
		__forceinline bool operator>=(const loop_iterator& other) const {
			return pos >= other.pos;
		}
		__forceinline loop_iterator& operator=(const loop_iterator& other) {
			pos = other.pos; 
			return *this;
		}
		__forceinline operator bool() {
			return true;
		}
	};

	struct const_loop_iterator
	{
		const vector_<T>& obj;
		size_t pos;
		const_loop_iterator(const vector_<T>& obj) : obj{obj}, pos{0} {}
		const_loop_iterator(const vector_<T>& obj, size_t pos) : obj{obj}, pos{pos} {
			if (pos>=obj.size_)
				pos = pos % obj.size_;
		}
		const_loop_iterator(const const_loop_iterator& other) : obj{other.obj}, pos{other.pos} {
		}
		__forceinline const_loop_iterator& operator++() {
			if (++pos >= obj.size_) pos = 0;
			return *this;
		}
		__forceinline const_loop_iterator& operator--() {
			if (--pos >= obj.size_) pos = obj.size_-1;
			return *this;
		}
		__forceinline const_loop_iterator operator+(size_t i) {
			return const_loop_iterator(obj, pos+i);
		}
		__forceinline const_loop_iterator operator-(size_t i) {
			return const_loop_iterator(obj, pos-i);
		}
		__forceinline const_loop_iterator& operator+=(size_t i) {
			if ((pos += i)>=obj.size_) pos = obj.size_;
			return *this;
		}
		__forceinline const_loop_iterator& operator-=(size_t i) {
			if ((pos -= i)>=obj.size_) pos = obj.size_;
			return *this;
		}
		__forceinline const T& operator*() const {
			return obj.content_[pos];
		}
		__forceinline bool operator==(const const_loop_iterator& other) const {
			return pos == other.pos;
		}
		__forceinline bool operator!=(const const_loop_iterator& other) const {
			return pos != other.pos;
		}
		__forceinline bool operator<(const const_loop_iterator& other) const {
			return pos < other.pos;
		}
		__forceinline bool operator>(const const_loop_iterator& other) const {
			return pos > other.pos;
		}
		__forceinline bool operator<=(const const_loop_iterator& other) const {
			return pos <= other.pos;
		}
		__forceinline bool operator>=(const const_loop_iterator& other) const {
			return pos >= other.pos;
		}
		__forceinline const_loop_iterator& operator=(const const_loop_iterator& other) {
			pos = other.pos;
			return *this;
		}
		__forceinline operator bool() {
			return true;
		}
	};

	iterator begin()
	{
		return iterator(*this, 0);
	}
	iterator end()
	{
		return iterator(*this, size_);
	}
	const_iterator cbegin() const
	{
		return const_iterator(*this, 0);
	}
	const_iterator cend() const
	{
		return const_iterator(*this, size_);
	}
	loop_iterator lbegin()
	{
		return loop_iterator(*this, 0);
	}
	loop_iterator lend()
	{
		return loop_iterator(*this, size_);
	}
	const_loop_iterator clbegin()
	{
		return const_loop_iterator(*this, 0);
	}
	const_loop_iterator clend()
	{
		return const_loop_iterator(*this, size_);
	}

	vector_() :
		content_(alloc_(default_alloc_)), size_(0)
	{
	}

	vector_(unsigned size, bool useable = false) :
		content_(alloc_(size)), size_(useable ? size : 0)
	{
	}

	vector_(T* arr, size_t size, size_t extra = 0) :
		content_(alloc_(size + extra)),
		size_(size)
	{
		copy_(content_, arr, size_);
	}
	template<size_t size>
	vector_(const T(&arr)[size], size_t extra = 0) :
		content_(alloc_(size + extra)),
		size_(size)
	{
		copy_(content_, static_cast<const T*>(arr), size_);
	}

	vector_(const T* arr, size_t size) :
		content_(alloc_(size)),
		size_(size)
	{
		copy_(content_, arr, size_);
	}

	vector_(std::initializer_list<T> arr) :
		content_(alloc_(arr.size())),
		size_(arr.size())
	{
		int k = 0;
		for (auto i = arr.begin(); i != arr.end(); ++i, ++k) {
			content_[k] = *i;
		}
	}

	/*template<typename... Y,
	typename U = enable_if<every_is<T, Y...>::value>>
	vector_(Y... elements) :
	size_(sizeof...(elements)),
	reserved_(sizeof...(elements))
	{
	content_ = new T[size_]{elements...};
	}*/

	vector_(const vector_<T>& other) :
		content_{alloc_(other.size_ ? other.size_ : default_alloc_)},
		size_{other.size_}
	{
		if (size_) copy_(content_, other.content_, size_);
	}

	vector_(const std::vector<T>& vec) :
		content_{alloc_(vec.size() ? int(vec.size()) : default_alloc_)},
		size_{int(vec.size())}
	{
		if (size_) copy_(content_, vec.data(), size_);
	}

	template<typename Y = T>
	vector_(enable_if<std::is_same<decay<Y>, char>::value, const std::string&> str) :
		content_(alloc_(str.size()+1)),
		size_(str.size()+1)
	{
		if (size_) copy_(content_, str.data(), size_);
	}

	/*template<typename I>
	vector_(enable_if<std::is_fundamental<I>::value, 
		T(*)(I)> fn, I from, I to, I d = 1) :
		content_(alloc_(abs((to - from) / d) + 1)),
		size_(abs((to - from) / d) + 1)
	{
		generate(fn, from, d);
	}*/
	template<typename F, typename I>
	vector_(F&& fn, I from, I to, I d = 1) :
		content_(alloc_(abs((to - from) / d) + 1)),
		size_(abs((to - from) / d) + 1)
	{
		generate(fn, from, d);
	}

	__forceinline constexpr size_t dim() const
	{
		return dim_;
	}

	//returns const pointer to the content
	__forceinline const T* content() const
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
		return reserved_*T_size_;
	}

	//returns current value of defaultly allocated memory
	__forceinline size_t default_alloc() const
	{
		return default_alloc_;
	}

	__forceinline static void set_default_alloc(size_t defSize)
	{
		default_alloc_ = defSize;
	}

	//returns current value of allocation multiplier
	__forceinline double alloc_mult() const
	{
		return alloc_mult_;
	}

	//sets allocation multiplier to "alm" if value is valid
	bool set_alloc_mult(double alm)
	{
		if (alm > 1.0) {
			alloc_mult_ = alm;
			return true;
		}
		return false;
	}

	//false if is empty
	__forceinline operator bool() const
	{
		return size_;
	}

	template<typename Y=T>
	operator enable_if<std::is_same<decay<Y>,char>::value,
		std::string>() const
	{
		return std::string(content_);
	}

	//returns reference to "n"th element
	__forceinline T& operator[](unsigned n) const
	{
		return content_[n];
	}

	//returns copy of this vector concatenated with copy of "other"
	vector_<T> operator+(const vector_<T>& other) const
	{
		vector_<T> result(*this);
		return result.append(other);
	}

	template<bool ptr_deref = false>
	bool identical(const vector_<T>& other)
	{
		if (size_ != other.size_)
			return false;
		for (size_t i = 0; i<size_; ++i) {
			if (!compare_elem_<ptr_deref>(
				content_[i],
				other.content_[i])) {
				return false;
			}
		}
		return true;
	}

	__forceinline bool operator==(const vector_<T>& other)
	{
		return identical<false>(other);
	}

	//returns reference to "n"th element
	T& at(size_t n) const
	{
		if (n < size_)
			return content_[n];
		throw ERR_OUT_OF_SIZE;
	}

	//returns reference to first element
	inline T& first() const
	{
		if (size_>0)
			return content_[0];
		throw(ERR_GET_EMPTY);
	}

	//returns reference to last element
	inline T& last() const
	{
		if (size_>0)
			return content_[size_-1];
		throw(ERR_GET_EMPTY);
	}

	//swaps content and parameters
	void swap(vector_<T>& other)
	{
		std::swap(size_, other.size_);
		std::swap(reserved_, other.reserved_);
		std::swap(content_, other.content_);
		std::swap(alloc_mult_, other.alloc_mult_);
	}

	//swaps two elements
	__forceinline vector_<T>& swap(size_t pos1, size_t pos2)
	{
		T temp(content_[pos1]);
		content_[pos1] = content_[pos2];
		content_[pos2] = temp;
		return *this;
	}

	//returns vector_ containing non trivial copy of elements
	//in range <"p1","p2"> inclusive
	vector_<T> subset(size_t p1, size_t p2) const
	{
		if (size_ && (p2 > p1)) {
			vector_<T> sub(p2-p1+1, true);
			copy_(sub.content_, content_+p1, sub.size_);
			return sub;
		}
		return vector_<T>();
	}

	//removes every element beyond range <"p1","p2">
	template<bool ptr_delete = false, typename Y = T>
	enable_if<std::is_pointer<Y>::value && ptr_delete,
		vector_<T>&> cut(size_t p1, size_t p2)
	{
		if (size_ && (p2 > p1)) {
			for (size_t i = 0; i < p1; ++i)
				delete content_[i];
			for (size_t i = p2 + 1; i < size_; ++i)
				delete content_[i];
			memcpy(content_, content_+p1, T_size_*(p2-p1+1));
			size_ = p2-p1+1;
		}
		return *this;
	}
	template<bool ptr_delete = false, typename Y = T>
	enable_if<!std::is_pointer<Y>::value || !ptr_delete,
		vector_<T>&> cut(size_t p1, size_t p2)
	{
		if (size_ && (p2 > p1)) {
			memcpy(content_, content_ + p1, T_size_*(p2 - p1 + 1));
			size_ = p2 - p1 + 1;
		}
		return *this;
	}

	//removes every element from position "p1" to "p2" inclusive
	template<bool ptr_delete = false, typename Y = T>
	enable_if<std::is_pointer<Y>::value && ptr_delete,
		vector_<T>&> erase(size_t p1, size_t p2)
	{
		if (size_ && (p2 > p1)) {
			for (size_t i = p1; i <= p2; ++i)
				delete content_[i];
			memcpy(content_+p1, content_+p2+1, (size_-p2-1)*T_size_);
			size_ -= (p2-p1+1);
		}
		return *this;
	}
	template<bool ptr_delete = false, typename Y = T>
	enable_if<!std::is_pointer<Y>::value || !ptr_delete,
		vector_<T>&> erase(size_t p1, size_t p2)
	{
		memcpy(content_ + p1, content_ + p2 + 1, (size_ - p2 - 1)*T_size_);
		size_ -= (p2 - p1 + 1);
		return *this;
	}

	//removes "n"th element 
	template<bool ptr_delete = false>
	vector_<T>& erase(size_t n)
	{
		if (n < size_) {
			destroy_elem_<ptr_delete>(content_[n]);
			memcpy(content_ + n, content_ + n + 1,
				(size_ - n - 1)*T_size_);
			--size_;
		}
		return *this;
	}

	//replaces each element with "elem" 
	vector_<T>& fill(const T& elem)
	{
		for (size_t i = 0; i < size_; ++i) {
			content_[i] = T(elem);
		}
		return *this;
	}

	//resizes content to "size" and replaces each element with "elem" 
	vector_<T>& fill(const T& elem, size_t size)
	{
		delete[] content_;
		content_ = alloc_(size);
		size_ = size;
		for (size_t i = 0; i < size_; ++i) {
			content_[i] = T(elem);
		}
		return *this;
	}

	//fills content with values of a function for specified arguments
	template<typename F, typename I>
	enable_if<std::is_fundamental<I>::value,
		vector_<T>&> generate(F&& fn, I from, I to, I d)
	{
		delete[] content_;
		content_ = alloc_(abs((to - from) / d) + 1);
		size_ = abs((to - from) / d) + 1;
		size_t i = 0;
		for (I x = from; i<size_; x += d) {
			content_[i++] = fn(x);
		}
		return *this;
	}

	//fills content with values of a function for specified arguments
	template<typename F, typename I>
	enable_if<std::is_fundamental<I>::value,
		vector_<T>&> generate(F&& fn, I from, I d)
	{
		size_t i = 0;
		for (I x = from; i<size_; x += d) {
			content_[i++] = fn(x);
		}
		return *this;
	}

	//fills content with values <"from", "to"> with delta = "d"
	template<typename Y = T>
	enable_if<std::is_fundamental<Y>::value,
		vector_<Y>&> generate(Y from, Y to, Y d = 1)
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
	__forceinline vector_<T> front(size_t n) const
	{
		return subset(0, n-1);
	}

	//returns vector_ containing "n" tail elements
	__forceinline vector_<T> back(size_t n) const
	{
		return subset(size_-n, size_-1);
	}

	//resizes array and does non trivial copy of "other"'s content
	vector_<T>& operator=(const vector_<T> &other)
	{
		if (other.size_) {
			delete[] content_;
			content_ = alloc_(other.size_);
			size_ = other.size_;
			copy_(content_, other.content_, size_);
		}
		return *this;
	}
	
	template<size_t size, typename Y=T>
	vector_<T>& operator=(const Y(&arr)[size])
	{
		delete[] content_;
		content_ = alloc_(size);
		size_ = size;
		copy_(content_, static_cast<const T*>(arr), size_);
		return *this;
	}

	vector_<T>& force_trivial_copy(const vector_<T> &other)
	{
		if (other.size_) {
			delete[] content_;
			content_ = alloc_(other.size_);
			size_ = other.size_;
			memcpy(content_, other.content_, T_size_*size_);
		}
		return *this;
	}

	//adds new element on the end
	vector_<T>& push_back(const T& newElem)
	{
		if (size_ == reserved_)
			realloc_(reserved_ * alloc_mult_);
		content_[size_++] = newElem;
		return *this;
	}

	//adds new element on the beginning
	__forceinline vector_<T>& push_front(T newElem)
	{
		insert(newElem, 0);
		return *this;
	}

	//moves the part of content after "pos" one field forward
	//and pastes in new element on "pos"
	vector_<T>& insert(const T& newElem, size_t pos)
	{
		if (size_==reserved_) {
			T* newContent = alloc_(reserved_*alloc_mult_);
			memcpy(newContent+pos+1, content_+pos, T_size_*(size_-pos));
			newContent[pos] = T(newElem);
			memcpy(newContent, content_, T_size_*pos);
			delete[] reinterpret_cast<T_shallow_*>(content_);
			content_ = newContent;
		}
		else {
			memcpy(content_+pos+1, content_+pos, T_size_*(size_-pos));
			content_[pos] = T(newElem);
		}
		++size_;
		return *this;
	}

	//moves the part of content after "pos" forward
	//and pastes in content of other array beginning on "pos"
	vector_<T>& insert(const vector_<T>& other, size_t pos)
	{
		if (reserved_<size_ + other.size_) {
			T* newContent = alloc_(size_+other.size_);
			memcpy(newContent + pos + other.size_, content_ + pos, T_size_*(size_ - pos));
			copy_(newContent + pos, other.content_, other.size_);
			memcpy(newContent, content_, T_size_*pos);
			delete[] reinterpret_cast<T_shallow_*>(content_);
			content_ = newContent;
		}
		else {
			memcpy(content_ + pos + other.size_, content_ + pos, T_size_*(size_ - pos));
			copy_(content_ + pos, other.content_, other.size_);
		}
		size_ += other.size_;
		return *this;
	}

	//removes up to "n" specified elements "toRem" if such occur.
	//if "n"=0 removes all
	//returns number of removed elements
	template<bool ptr_delete = false, bool ptr_deref = false, typename U, typename Y=T>
	enable_if<std::is_convertible<Y,U>::value,
		int> remove(U const& val, unsigned max = 0)
	{
		int i = -1, lastp = -1, nrem = 0;
		while (++i < size_) {
			if (compare_elem_<ptr_deref>((U)content_[i], val)) {
				destroy_elem_<ptr_delete>(content_[i]);
				if (i > lastp + nrem) {
					memcpy(content_ + lastp,
						content_ + lastp + nrem,
						T_size_*(i - lastp - nrem));
					lastp = i - nrem;
				}
				if (++nrem == max) break;
			}
		}
		if (size_ - lastp - nrem) {
			memcpy(content_ + lastp,
				content_ + lastp + nrem,
				T_size_*(size_ - lastp - nrem));
		}
		size_ -= nrem;
		return nrem;
	}

	template<bool ptr_delete = false, typename F>
	int remove_if(F&& fn, unsigned max = 0)
	{
		int i = -1, lastp = -1, nrem = 0;
		while (++i < size_) {
			if (fn(content_[i])) {
				destroy_elem_<ptr_delete>(content_[i]);
				if (i > lastp + nrem) {
					memcpy(content_ + lastp,
						content_ + lastp + nrem,
						T_size_*(i - lastp - nrem));
					lastp = i - nrem;
				}
				if (++nrem == max) break;
			}
		}
		if (size_ - lastp - nrem) {
			memcpy(content_ + lastp,
				content_ + lastp + nrem,
				T_size_*(size_ - lastp - nrem));
		}
		size_ -= nrem;
		return nrem;
	}

	template<bool ptr_deref = false, typename F>
	vector_<T> copy_if(F&& fn)
	{
		vector_<T> copied(size_);
		for (int i = 0; i < size_; ++i) {
			if (fn(content_[i])) copied.push_back(elem_<ptr_deref>(content_[i]));
		}
		copied.shrink();
		return copied;
	}

	template<bool ptr_delete = false, bool ptr_deref = false, typename F>
	vector_<T> extract_if(F&& fn, unsigned max = 0)
	{
		vector_<T> extracted(size_);
		int i = -1, lastp = -1, nrem = 0;
		while (++i < size_) {
			if (fn(content_[i])) {
				extracted.push_back(elem_<ptr_deref>(content_[i]));
				destroy_elem_<ptr_delete>(content_[i]);
				if (i > lastp + nrem) {
					memcpy(content_ + lastp,
						content_ + lastp + nrem,
						T_size_*(i - lastp - nrem));
					lastp = i - nrem;
				}
				if (++nrem == max) break;
			}
		}
		if (size_ - lastp - nrem) {
			memcpy(content_ + lastp,
				content_ + lastp + nrem,
				T_size_*(size_ - lastp - nrem));
		}
		size_ -= nrem;
		extracted.shrink();
		return extracted;
	}

	//removes first element
	template<bool ptr_delete = false>
	__forceinline vector_<T>& pop_front()
	{
		if (size_) {
			destroy_elem_<ptr_delete>(content_[0]);
			erase(0);
		}
		return *this;
	}

	//removes last element
	template<bool ptr_delete = false>
	vector_<T>& pop_back()
	{
		if (size_) {
			destroy_elem_<ptr_delete>(content_[size_ - 1]);
			--size_;
		}
		return *this;
	}

	//uses quick sort on the whole scope
	template<typename Y = T>
	__forceinline enable_if<
		has_less_op<Y, bool, const Y&>::value ||
		has_less_op<Y, bool, Y&>::value ||
		!std::is_class<Y>::value,
		vector_<T>&> sort()
	{
		quicksort_(0, size_-1);
		return *this;
	}

#ifdef XRND_H	
	//set elements in random order
	//if "swap_each" is disabled, each element has
	//some probability of not being moved
	vector_<T>& shuffle(bool swap_each = true)
	{
		for (int i = 0; i<size_-2; ++i) {
			swap(i, random<size_t>(i+swap_each, size_-1));
		}
		return *this;
	}

	//return random element from specified range <"p1","p2"> inclusive
	T& get_random(size_t p1, size_t p2) const
	{
		return content_[random<size_t>(p1, p2)];
	}

	//return random element
	__forceinline T& get_random() const
	{
		return get_random(0, size_-1);
	}

	template<typename Y = T>
	enable_if<std::is_fundamental<Y>::value,
		vector_<Y>&> randomize(Y r1, Y r2)
	{
		for (int i = 0; i < size_; ++i) {
			content_[i] = random<Y>(r1, r2);
		}
		return *this;
	}

	template<typename Y = T>
	enable_if<std::is_fundamental<Y>::value,
		vector_<T>&> randomize(double r1, double r2)
	{
		double factor; T r = r2-r1;
		for (int i = 0; i < size_; ++i) {
			factor = random<T>(-r, r);
			content_[i] *= (1+(factor + r1*sgn(factor)));
		}
		return *this;
	}

	template<typename Y = T>
	enable_if<std::is_fundamental<Y>::value,
		vector_<T>&> randomize(double r)
	{
		for (int i = 0; i < size_; ++i) {
			content_[i] *= (1 + random<T>(-r, r));
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
	template<typename Y = T>
	enable_if<vector_<Y>::dim_==1,
		void> reserve(size_t newSize)
	{
		if (newSize > reserved_) {
			realloc_(newSize);
			return;
		}
		throw(ERR_RESERVE_LESS);
	}

	template<bool useable = false, typename Y = T, typename... N>
	enable_if<vector_<Y>::dim_>=2 &&
		sizeof...(N)<=vector_<Y>::dim_ &&
		all_true<std::is_convertible<N, size_t>::value...>::value,
		void> reserve(N... sizes)
	{
		std::initializer_list<size_t> sizes_il{size_t(sizes)...};
		reserve_<useable>(sizes_il, sizes_il.begin());
	}

	//inserts non trivial copy
	//of "other"'s content on the end
	vector_<T>& append(const vector_<T>& other)
	{
		if (reserved_<size_ + other.size_)
			realloc_(size_ + other.size_);
		copy_(content_+size_, other.content_, other.size_);
		size_ += other.size_;
		return *this;
	}

	//inserts new elements on the end
	vector_<T>& append(std::initializer_list<T> other)
	{
		if (reserved_<size_ + other.size())
			realloc_(size_ + other.size());
		int k = size_;
		for (typename std::initializer_list<T>::iterator i = other.begin();
		i != other.end(); ++i, ++k) {
			content_[k] = T(*i);
		}
		size_ += other.size();
		return *this;
	}

	//moves content forward and inserts non trivial copy
	//of "other"'s content on the beginning
	__forceinline vector_<T>& prepend(const vector_<T>& other)
	{
		insert(other, 0);
		return *this;
	}

	//moves content forward and
	//inserts new elements on the beginning
	vector_<T>& prepend(std::initializer_list<T> other)
	{
		if (reserved_<size_ + other.size())
			realloc_(size_ + other.size());
		memcpy(content_ + other.size(), content_, T_size_*size_);
		int k = 0;
		for (typename std::initializer_list<T>::iterator i = other.begin();
		i != other.end(); ++i, ++k) {
			content_[k] = T(*i);
		}
		size_ += other.size();
		return *this;
	}

	//returns number of occurances of "elem"
	template<bool ptr_deref = false>
	int count(const T& elem) const
	{
		int n = 0;
		for (size_t i = 0; i<size_; ++i) {
			if (compare_elem_<ptr_deref>(content_[i], elem)) ++n;
		}
		return n;
	}

	//returns reference to element, value of which is equal to "elem"
	template<bool ptr_deref = false, typename U, typename Y=T>
	enable_if<std::is_convertible<Y,U>::value, 
		T&> find(U const& elem)
	{
		for (size_t i = 0; i < size_; ++i)
			if (compare_elem_<ptr_deref>((U)content_[i], elem)) 
				return content_[i];
		throw(ERR_NOT_FOUND);
	}

	template<bool ptr_deref = false, typename U, typename Y=T>
	enable_if<std::is_convertible<Y,U>::value,
		bool> contain(U const& elem)
	{
		for (size_t i = 0; i < size_; ++i)
			if (compare_elem_<ptr_deref>((U)content_[i], elem)) return true;
		return false;
	}

	//returns reference to element, for which "fn"'s return value is equal to "val"
	template<typename cT, typename F>
	T& find_by(F&& fn, cT val)
	{
		for (size_t i = 0; i < size_; ++i)
			if (fn(content_[i]) == val) return content_[i];
		throw(ERR_NOT_FOUND);
	}

	//returns reference to element, "member"'s value of which is equal to "val"
	template<typename cT, typename Y = T>
	enable_if<std::is_class<Y>::value, 
		Y&>	find_by(cT Y::*member, cT&& val)
	{
		for (size_t i = 0; i < size_; ++i)
			if (content_[i].*member == val) return content_[i];
		throw ERR_NOT_FOUND;
	}

	//replaces each occurance of "what" element with "val"
	template<bool ptr_deref = false>
	int replace(const T& what, const T& val, unsigned nmax = 0)
	{
		int nrep = 0;
		for (int i = 0; i < size_; ++i) {
			if (compare_elem_<ptr_deref>(content_[i], what)) {
				assign_elem_<ptr_deref>(content_[i], val);
				if (++nrep==nmax) return nrep;
			}
		}
		return nrep;
	}

	//sets elements in reverse order
	vector_<T>& reverse()
	{
		for (int i = size_ / 2 - 1; i >= 0; --i) {
			swap(i, size_ - i - 1);
		}
		return *this;
	}

	//moves elements "n" fields forward / backward (if "n" < 0)
	//making them to appear on the beginning / end
	vector_<T>& shift(int n)
	{
		if (n %= int(size_)) {
			if (n < 0) n += size_;
			T* newContent = alloc_(size_);
			memcpy(newContent, content_ + size_ - n, T_size_*n);
			memcpy(newContent + n, content_, T_size_*(size_-n));
			delete[] reinterpret_cast<T_shallow_*>(content_);
			content_ = newContent;
		}
		return *this;
	}

	T take(size_t n)
	{
		if (n<size_) {
			T elem = content_[n];
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
		std::ostream&> operator<<(std::ostream &os, const vector_<Y> &arr);

	template<typename Y>
	friend enable_if< !std::is_same<decay<Y>, char>::value,
		std::ostream&> operator<<(std::ostream &os, const vector_<Y> &arr);

	//pushes back new element from input stream
	friend std::istream &operator>>(std::istream &is, vector_<T> &arr)
	{
		if (arr.size_ == arr.reserved_)
			arr.realloc_(arr.reserved_ * arr.alloc_mult_);
		is >> arr.content_[arr.size_++];
		return is;
	}

	//calls function on every element
	template<typename F>
	vector_<T>& call(F&& fn)
	{
		for (int i = 0; i<size_; ++i) {
			fn(content_[i]);
		}
		return *this;
	}

	//calls function on every element for which condition is true
	template<typename C, typename F>
	vector_<T>& call_if(C&& cond, F&& fn)
	{
		for (int i = 0; i<size_; ++i) {
			if (cond(content_[i])) fn(content_[i]);
		}
		return *this;
	}

	//calls any member function with given parameters on every element
	template<typename... argT, typename Y = T, typename R = void>
	enable_if<std::is_class<Y>::value, vector_<T>&>
		call(R(Y::*f)(argT...), argT... args)
	{
		for (int i = 0; i<size_; ++i) {
			(content_[i].*f)(args...);
		}
		return *this;
	}

	//checks if condition is true for every element
	template<typename F>
	bool true_for_all(F&& fn)
	{
		for (int i = 0; i<size_; ++i)
			if (!fn(content_[i])) return false;
		return true;
	}

	//forgets the content with no memory deallocation
	template<bool ptr_delete = false, typename Y = T>
	enable_if<std::is_pointer<Y>::value && ptr_delete,
		vector_<T>&> clear()
	{
		if (size_) {
			for (int i = 0; i < size_; ++i)
				delete content_[i];
			size_ = 0;
		}
		return *this;
	}
	template<bool ptr_delete = false, typename Y = T>
	enable_if<!std::is_pointer<Y>::value || !ptr_delete,
		vector_<T>&> clear()
	{
		size_ = 0;
		return *this;
	}

	//clears all content and allocates new block of memory of default size
	template<bool ptr_delete = false, typename Y = T>
	enable_if<std::is_pointer<Y>::value && ptr_delete,
		vector_<T>&> erase()
	{
		if (size_) {
			for (int i = 0; i < size_; ++i)
				delete content_[i];
			delete[] content_;
			content_ = alloc_(default_alloc_);
			size_ = 0;
		}
		return *this;
	}
	template<bool ptr_delete = false, typename Y = T>
	enable_if<!std::is_pointer<Y>::value || !ptr_delete,
		vector_<T>&> erase()
	{
		if (size_) {
			delete[] content_;
			content_ = alloc_(default_alloc_);
			size_ = 0;
		}
		return *this;
	}

	//returns std::vector with non trivial copy of content
	std::vector<T> tostd_vector()
	{
		return std::vector<T>(content_, content_ + size_ - 1);
	}

	std::list<T> tostd_list()
	{
		std::list<T> ret;
		int i = size_;
		while (i--)
			ret.push_front(content_[i]);
		return ret;
	}

	//explicit operator std::vector<T>()
	//{
	//	return std::vector<T>(content_, content_ + size_ - 1);
	//}

	template<typename Y>
	enable_if<std::is_convertible<T, Y>::value &&
		dim_==1,
		vector_<Y>> convert_to()
	{
		vector_<Y> converted(size_);
		for (int i = 0; i < size_; ++i) {
			converted.content_[i] = Y(content_[i]);
		}
		converted.size_ = size_;
		return converted;
	}
	template<typename Y>
	enable_if<std::is_convertible<typename atomic_type_<vector_<T>>::type, Y>::value &&
		dim_ >= 2,
		typename vector_nd_<Y, dim_>::type> convert_to()
	{
		typename vector_nd_<Y, dim_>::type converted(size_);
		for (int i = 0; i < size_; ++i) {
			converted.content_[i] = content_[i].convert_to<Y>();
		}
		converted.size_ = size_;
		return converted;
	}

	/*template<bool ptr_deref = false>
	void save_data(std::fstream& file)
	{
	xsrl xfile(file);
	for (int i = 0; i<size_; ++i) {
	xfile<<elem_<ptr_deref>(content_[i]);
	}
	}

	void load_data(std::fstream& file)
	{
	xsrl xfile(file);
	file.
	}*/

	~vector_()
	{
		if (force_single_shallow_delete__) {
			force_single_shallow_delete__ = false;
		}
		else {
			delete[] content_;
		}
	}

};

template<typename T>
size_t vector_<T>::default_alloc_ = 10;

template<typename T>
double vector_<T>::alloc_mult_ = 2.0;

template<typename T>
bool vector_<T>::force_single_shallow_delete__ = false;


template<typename T, unsigned dim_ = 1>
using vector = typename std::conditional<dim_==1, vector_<T>,
	typename vector_nd_<T, dim_>::type>::type;

using string = vector_<char>;


template<typename Y>
enable_if<std::is_same<decay<Y>, char>::value,
	std::ostream&> operator<<(std::ostream &os, const vector_<Y> &arr)
{
	return os << arr.content_;
}

template<typename Y>
enable_if< !std::is_same<decay<Y>, char>::value,
	std::ostream&> operator<<(std::ostream &os, const vector_<Y> &arr)
{
	arr.disp(' ', os, '\n');
	return os;
}

} //end namespace x

#undef enable_if
#undef decay
#undef DEBUG

#endif //XARR_H