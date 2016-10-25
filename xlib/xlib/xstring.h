#ifndef _X_STRING_H_
#define _X_STRING_H_

#include <vector>
#include <list>
#include <iostream>
#include <string>
#include "more_type_traits.h"
#include "simple.h"
#include "range.h"
#include "autoref.h"
#include "auto_cast.h"
#include "xvector.h"


#define enable_if typename std::enable_if_t
#define decay typename std::decay_t

namespace x
{
	class string
	{
	protected:
		static constexpr size_t ELEMENT_SIZE = 1;
		static constexpr size_t DEFAULT_ALLOC = 10;

		static size_t defaultAlloc_;
		static double allocMult_;

		////////////////////////////////////////////////////////////////////////////

		char* content_;
		size_t size_;
		size_t reserved_;

		////////////////////////////////////////////////////////////////////////////

		__forceinline char* alloc_(size_t newSize)
		{
			reserved_ = newSize + 1;
			return new char[newSize + 1];
		}

		void realloc_(size_t newSize)
		{
			char* newContent = alloc_(newSize);
			memcpy(newContent, content_, ELEMENT_SIZE*size_);
			delete[] content_;
			content_ = newContent;
		}

		__forceinline void insert_end_()
		{
			content_[size_] = '\0';
		}

		void quicksort_(int r1, int r2)
		{
			int last = r1;
			for (int i = r1; i < r2; ++i) {
				if (content_[i] < content_[r2]) {
					if (last != i) swap(i, last);
					++last;
				}
			}
			swap(last, r2);
			if (r1 < last - 1) quicksort_(r1, last - 1);
			if (last + 1 < r2) quicksort_(last + 1, r2);
		}

	public:

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

		using type = char;

		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using value_type = char;
		using pointer = char*;
		using reference = char&;

		////////////////////////////////////////////////////////////////////////////

		struct iterator
		{
			string& obj;
			size_t pos;
			iterator(string& obj): obj{obj}, pos{0}
			{
			}
			iterator(string& obj, size_t pos): obj{obj}, pos{pos}
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
			__forceinline char& operator*()
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
		};

		////////////////////////////////////////////////////////////////////////////

		struct const_iterator
		{
			const string& obj;
			size_t pos;
			const_iterator(const string& obj): obj{obj}, pos{0}
			{
			}
			const_iterator(const string& obj, size_t pos): obj{obj}, pos{pos}
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
			__forceinline char operator*() const
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
		};

		////////////////////////////////////////////////////////////////////////////

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
	private:
		struct AdoptFlag{};

		string(char* str, AdoptFlag):
			content_{str},
			size_{strlen(str) - 1}
		{
		}

		template<class _Type>
		static constexpr bool IS_CHAR = x::is_any_of<_Type, char, char const>::value;

	public:
		string():
			content_(alloc_(defaultAlloc_)), size_(0)
		{
			insert_end_();
			//std::cout << "def ctor\n";
			////std::cout<<typeid(Shallow).name();
		}

		string(size_t size, bool useable = false):
			content_(alloc_(size)), 
			size_(useable ? size : 0)
		{
			insert_end_();
			//std::cout << "size ctor\n";
		}

		string(char* arr, size_t size, size_t extra = 0):
			content_(alloc_(size + extra)),
			size_(size)
		{
			memcpy(content_, arr, size_);
			insert_end_();
		}

		string(char ch):
			content_{alloc_(1)},
			size_{1}
		{
			insert_end_();
		}

		template<class _Arg, size_t size, 
			_concept<IS_CHAR<_Arg>>>
		string(_Arg(&str)[size]):
			content_(alloc_(size - 1)),
			size_(size - 1)
		{
			//disp "arr\n";
			memcpy(content_, str, size - 1);
			insert_end_();
		}

		template<class _Arg, 
			_concept<std::is_pointer<_Arg>::value && 
				IS_CHAR<std::remove_pointer_t<_Arg>>>>
		string(_Arg str):
			content_{alloc_(strlen(str))},
			size_{reserved_ - 1}
		{
			//disp "ptr\n";
			memcpy(content_, str, size_);
			insert_end_();
		}

		/*template<size_t size>
		string(char const(&str)[size]):
			content_(alloc_(size - 1)),
			size_(size - 1)
		{
			disp "arr\n";
			memcpy(content_, str, size - 1);
			insert_end_();
		}

		template<class _Arg>
		string(char const* str):
			content_{alloc_(strlen(str))},
			size_{reserved_ - 1}
		{
			disp "ptr\n";
			memcpy(content_, str, size_);
			insert_end_();
		}*/

		static string adopt(char* str)
		{
			return string{str,AdoptFlag{}};
		}

		/*static string force_adopt(char const* str)
		{
			return string{x::rem_const(str),AdoptFlag{}};
		}*/

		//string(std::initializer_list<char> arr):
		//	content_(alloc_(arr.size())),
		//	size_(arr.size())
		//{
		//	//std::cout << "init_list ctor\n";
		//	int k = 0;
		//	for (auto i = arr.begin(); i != arr.end(); ++i, ++k) {
		//		content_[k] = *i;
		//	}
		//}

		/*template<typename... Y,
		typename U = enable_if<every_is<char, Y...>::value>>
		vector_(Y... elements) :
		size_(sizeof...(elements)),
		reserved_(sizeof...(elements))
		{
		content_ = new char[size_]{elements...};
		}*/

		string(const string& other):
			content_{alloc_(other.size_ ? other.size_ : defaultAlloc_)},
			size_{other.size_}
		{
			//std::cout << "copy ctor\n";
			if (size_) 
				memcpy(content_, other.content_, size_);
			insert_end_();

		}

		string(string&& other):
			content_{other.content_},
			size_{other.size_}
		{
			other.content_ = nullptr;
			insert_end_();
			//std::cout << "move ctor\n";
		}

		__forceinline string&& move()
		{
			return (string&&)(*this);
		}

		////////////////////////////////////////////////////////////////////////////

		//returns const pointer to the content
		__forceinline const char* content() const
		{
			return content_;
		}

		//returns current size
		__forceinline size_t size() const
		{
			return size_;
		}

		__forceinline size_t last_pos() const
		{
			return size_ - 1;
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

		string& separate(
			x::vector<x::string>&	buffer, 
			char					delim	= ' ')
		{
			size_t firstPos = 0;
			auto ch{cbegin()};
			for ( ; ch; ++ch) {
				if (*ch == delim) {
					if (ch.pos > firstPos) {
						buffer.push_back(substr(firstPos, ch.pos - 1));
					}
					firstPos = ch.pos + 1;
				}
			}
			if (ch.pos > firstPos) {
				buffer.push_back(substr(firstPos, ch.pos - 1));
			}
		}

		//false if is empty
		__forceinline operator bool() const
		{
			return size_;
		}

		/*template<typename Y = char>
		operator enable_if<std::is_same<decay<Y>, char>::value,
		std::string>() const
		{
		return std::string(content_);
		}*/

		//returns reference to nth element
		__forceinline char& operator[](unsigned n) const
		{
			return content_[n];
		}

		//returns copy of this vector concatenated with copy of "other"
		string operator+(const string& other) const
		{
			string result(*this);
			return result.append(other);
		}

		bool operator==(const string& other) const
		{
			return size_==other.size_ && 
				!strcoll(content_, other.content_);
		}

		//returns reference to "n"th element
		char& at(size_t n) const
		{
			if (n < size_)
				return content_[n];
			throw ERR_OUT_OF_SIZE;
		}

		//returns reference to first element
		inline char& first() const
		{
			if (size_>0)
				return content_[0];
			throw(ERR_GET_EMPTY);
		}

		//returns reference to last element
		inline char& last() const
		{
			if (size_>0)
				return content_[size_ - 1];
			throw(ERR_GET_EMPTY);
		}

		//swaps content and parameters
		void swap(string& other)
		{
			std::swap(size_, other.size_);
			std::swap(reserved_, other.reserved_);
			std::swap(content_, other.content_);
		}

		//swaps two elements
		__forceinline string& swap(size_t pos1, size_t pos2)
		{
			std::swap(content_[pos1], content_[pos2]);
			insert_end_();
			return *this;
		}

		//returns vector_ containing non trivial copy of elements
		//in range <p1,p2> inclusive
		string substr(size_t p1, size_t p2) const
		{
			if (size_ && (p2 >= p1)) {
				string sub(p2 - p1 + 1, true);
				memcpy(sub.content_, content_ + p1, sub.size_);
				return sub;
			}
			return string();
		}

		//returns vector_ containing non trivial copy of elements
		//in range from p to the last element inclusive
		string substr(size_t p) const
		{
			if (size_) {
				string sub(size_ - p, true);
				memcpy(sub.content_, content_ + p, sub.size_);
				return sub;
			}
			return string();
		}

		//removes every element beyond range <"p1","p2">
		string& cut(size_t p1, size_t p2)
		{
			if (p2 >= size_) p2 = size_ - 1;
			if ((int)p2 - (int)p1 > 0) {
				memcpy(content_, content_ + p1, ELEMENT_SIZE*(p2 - p1 + 1));
				size_ = p2 - p1 + 1;
			}
			else size_ = 0;
			insert_end_();
			return *this;
		}

		//removes nth element 
		string& erase(size_t n)
		{
			if (n < size_) {
				memcpy(content_ + n, content_ + n + 1,
					(size_ - n - 1)*ELEMENT_SIZE);
				--size_;
			}
			insert_end_();
			return *this;
		}

		//replaces each element with "elem" 
		string& fill(char elem)
		{
			for (size_t i = 0; i < size_; ++i) {
				content_[i] = char(elem);
			}
			return *this;
		}

		//resizes content to "size" and replaces each element with "elem" 
		string& fill(char elem, size_t size)
		{
			delete[] content_;
			content_ = alloc_(size);
			size_ = size;
			for (size_t i = 0; i < size_; ++i) {
				content_[i] = char(elem);
			}
			return *this;
		}

		//returns string containing n front elements
		__forceinline string front(size_t n) const
		{
			return substr(0, n - 1);
		}

		//returns string containing n tail elements
		__forceinline string back(size_t n) const
		{
			return substr(size_ - n, size_ - 1);
		}

		//resizes array and does non trivial copy of "other"'s content
		string& operator=(const string &other)
		{
			if (other.size_) {
				delete[] content_;
				content_ = alloc_(other.size_);
				size_ = other.size_;
				memcpy(content_, other.content_, size_);
			}
			insert_end_();
			return *this;
		}

		string& operator=(string && other)
		{
			if (other.size_) {
				delete[] content_;
				content_ = other.content_;
				size_ = other.size_;
				other.content_ = nullptr;
			}
			insert_end_();
			return *this;
		}

		template<size_t size>
		string& operator=(char const(&arr)[size])
		{
			delete[] content_;
			content_ = alloc_(size - 1);
			size_ = size - 1;
			memcpy(content_, static_cast<const char*>(arr), size_);
			insert_end_();
			return *this;
		}

		//adds new element on the end
		string& push_back(char newElem)
		{
			if (size_ == reserved_ - 1)
				realloc_(1 + reserved_ * allocMult_);
			content_[size_++] = newElem;
			insert_end_();
			return *this;
		}

		//adds new element on the beginning
		__forceinline string& push_front(char newElem)
		{
			insert(newElem, 0);
			return *this;
		}

		//moves the part of content after "pos" one field forward
		//and pastes in new element on "pos"
		string& insert(char newElem, size_t pos)
		{
			if (size_ == reserved_ - 1) {
				char* newContent = alloc_(1 + reserved_*allocMult_);
				memcpy(newContent + pos + 1, content_ + pos, ELEMENT_SIZE*(size_ - pos));
				newContent[pos] = newElem;
				memcpy(newContent, content_, ELEMENT_SIZE*pos);
				delete[] content_;
				content_ = newContent;
			}
			else {
				memcpy(content_ + pos + 1, content_ + pos, ELEMENT_SIZE*(size_ - pos));
				content_[pos] = newElem;
			}
			++size_;
			insert_end_();
			return *this;
		}

		//moves the part of content after "pos" forward
		//and pastes in content of other array beginning on "pos"
		string& insert(const string& other, size_t pos)
		{
			if ((reserved_ - 1) < (size_ + other.size_)) {
				char* newContent = alloc_(size_ + other.size_);
				memcpy(newContent + pos + other.size_, content_ + pos, ELEMENT_SIZE*(size_ - pos));
				memcpy(newContent + pos, other.content_, other.size_);
				memcpy(newContent, content_, ELEMENT_SIZE*pos);
				delete[] content_;
				content_ = newContent;
			}
			else {
				memcpy(content_ + pos + other.size_, content_ + pos, ELEMENT_SIZE*(size_ - pos));
				memcpy(content_ + pos, other.content_, other.size_);
			}
			size_ += other.size_;
			insert_end_();
			return *this;
		}

		//removes up to "n" specified elements "toRem" if such occur.
		//if "n"=0 removes all
		//returns number of removed elements

		int remove(char val, unsigned max = 0)
		{
			int i = -1, lastp = -1, nrem = 0;
			while (++i < size_) {
				if (content_[i] == val) {
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
			insert_end_();
			return nrem;
		}

		template<typename _Func>
		int remove_if(_Func&& fn_, unsigned max = 0)
		{
			int i = -1, lastp = -1, nrem = 0;
			while (++i < size_) {
				if (fn_(content_[i])) {
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
			insert_end_();
			return nrem;
		}

		template<typename _Func>
		string copy_if(_Func&& fn_)
		{
			string copied(size_);
			for (int i = 0; i < size_; ++i) {
				if (fn_(content_[i])) 
					copied.push_back(content_[i]);
			}
			copied.shrink();
			return copied;
		}

		template<typename _Func>
		string extract_if(_Func&& fn_, unsigned max = 0)
		{
			string extracted(size_);
			int i = -1, lastp = -1, nrem = 0;
			while (++i < size_) {
				if (fn_(content_[i])) {
					extracted.push_back(content_[i]);
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
		string& throw_back(size_t which)
		{
			swap(which, --size_);
			insert_end_();
			return *this;
		}

		//removes first element
		__forceinline string& pop_front()
		{
			if (size_) erase(0);
			return *this;
		}

		//removes last element
		string& pop_back()
		{
			if (size_) --size_;
			insert_end_();
			return *this;
		}

		//uses quick sort on the whole scope
		string& sort()
		{
			quicksort_(0, size_ - 1);
			return *this;
		}

#ifdef XRND_H	
		//set elements in random order
		//if "swap_each" is disabled, each element has
		//some probability of not being moved
		string& shuffle(bool swap_each = true)
		{
			for (int i = 0; i<size_ - 2; ++i) {
				swap(i, random<size_t>(i + swap_each, size_ - 1));
			}
			return *this;
		}

		//return random element from specified range <"p1","p2"> inclusive
		char& get_random(size_t p1, size_t p2) const
		{
			return content_[random<size_t>(p1, p2)];
		}

		//return random element
		__forceinline char& get_random() const
		{
			return get_random(0, size_ - 1);
		}

		string& randomize(char r1, char r2)
		{
			for (int i = 0; i < size_; ++i) {
				content_[i] = random<char>(r1, r2);
			}
			return *this;
		}

		string& randomize(double r1, double r2)
		{
			double factor; char r = r2 - r1;
			for (int i = 0; i < size_; ++i) {
				factor = random<char>(-r, r);
				content_[i] *= (1 + (factor + r1*sgn(factor)));
			}
			return *this;
		}

		string& randomize(double r)
		{
			for (int i = 0; i < size_; ++i) {
				content_[i] *= (1 + random<char>(-r, r));
			}
			return *this;
		}

#endif

		//deallocate memory exceeding the actual size
		__forceinline void shrink()
		{
			realloc_(size_);
			insert_end_();
		}

		//only more space reservation is accepted
		void reserve(size_t newSize)
		{
			if (newSize > reserved_ - 1) {
				realloc_(newSize);
				insert_end_();
				return;
			}
			throw(ERR_RESERVE_LESS);
		}

	
		string& append(const string& other)
		{
			if ((reserved_ - 1) < (size_ + other.size_))
				realloc_(size_ + other.size_);
			memcpy(content_ + size_, other.content_, other.size_);
			size_ += other.size_;
			insert_end_();
			return *this;
		}

		//moves content forward and inserts non trivial copy
		//of "other"'s content on the beginning
		__forceinline string& prepend(const string& other)
		{
			insert(other, 0);
			return *this;
		}

		//returns number of occurances of elem
		size_t count(char elem) const
		{
			size_t n = 0;
			for (size_t i = 0; i<size_; ++i) 
				if (content_[i] == elem) ++n;
			return n;
		}

		//returns reference to element, value of which is equal to "elem"
		char& find(char elem)
		{
			for (size_t i = 0; i < size_; ++i)
				if (content_[i] == elem)
					return content_[i];
			throw(ERR_NOT_FOUND);
		}

		size_t pos_of(char elem) const
		{
			for (size_t i = 0; i < size_; ++i)
				if (content_[i] == elem)
					return i;
			throw(ERR_NOT_FOUND);
		}

		bool contain(char elem) const
		{
			for (size_t i = 0; i < size_; ++i)
				if (content_[i] == elem) return true;
			return false;
		}

		//returns reference to element, for which "fn"'s return value is equal to "val"
		template<typename cT, typename _Func>
		char& find_by(_Func&& fn_, cT val)
		{
			for (size_t i = 0; i < size_; ++i)
				if (fn_(content_[i]) == val) return content_[i];
			throw(ERR_NOT_FOUND);
		}

		//replaces each occurance of "what" element with "val"
		int replace(char what, char val, unsigned nmax = 0)
		{
			int nrep = 0;
			for (int i = 0; i < size_; ++i) {
				if (content_[i] == what) {
					content_[i] = val;
					if (++nrep == nmax) return nrep;
				}
			}
			return nrep;
		}

		//sets elements in reverse order
		string& reverse()
		{
			for (int i = size_ / 2 - 1; i >= 0; --i) {
				swap(i, size_ - i - 1);
			}
			return *this;
		}

		//moves elements "n" fields forward / backward (if "n" < 0)
		//making them to appear on the beginning / end
		string& shift(int n)
		{
			if (n %= int(size_)) {
				if (n < 0) n += size_;
				char* newContent = alloc_(size_);
				memcpy(newContent, content_ + size_ - n, ELEMENT_SIZE*n);
				memcpy(newContent + n, content_, ELEMENT_SIZE*(size_ - n));
				delete[] content_;
				content_ = newContent;
			}
			insert_end_();
			return *this;
		}

		char take(size_t n)
		{
			if (n < size_) {
				char elem = content_[n];
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

		template<class _Type>
		class NumericValidator
		{
			static x::range<char> const numbers_;
		public:
			static bool validate(string const& str);
		};

		class AlphabeticValidator
		{
			static x::range<char> const lowerCase_;
			static x::range<char> const upperCase_;
		public:
			static bool validate_alpha(string const& str);
			static bool validate_lowercase(string const& str);
			static bool validate_uppercase(string const& str);
		};


		template<class _Type, _concept<std::is_fundamental<_Type>::value>>
		__forceinline bool is() const
		{
			return NumericValidator<_Type>::validate(*this);
		}

		__forceinline bool is_number() const
		{
			return is<float>();
		}

		__forceinline bool is_word() const
		{
			return AlphabeticValidator::validate_alpha(*this);
		}

		__forceinline bool is_lowercase() const
		{
			return AlphabeticValidator::validate_lowercase(*this);
		}

		__forceinline bool is_uppercase() const
		{
			return AlphabeticValidator::validate_uppercase(*this);
		}

		//passes content to output stream
		friend std::ostream& operator<<(std::ostream &os, const string &arr);

		friend std::ostream& operator<<(std::ostream &os, const string &arr);

		//pushes back new element from input stream
		friend std::istream &operator >> (std::istream &is, string &arr);

		//calls function on every element
		template<typename _Func>
		string& call(_Func&& fn_)
		{
			for (int i = 0; i<size_; ++i) {
				fn_(content_[i]);
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
		string& clear()
		{
			size_ = 0;
			insert_end_();
			return *this;
		}

		//clears all content and allocates new block of memory of default size
		string& erase()
		{
			if (size_) {
				delete[] content_;
				content_ = alloc_(defaultAlloc_);
				size_ = 0;
				insert_end_();
			}
			return *this;
		}

		//returns std::vector with non trivial copy of content
		std::vector<char> to_std_vector()
		{
			return std::vector<char>(content_, content_ + size_ - 1);
		}

		std::list<char> to_std_list()
		{
			std::list<char> ret;
			int i = size_;
			while (i--)
				ret.push_front(content_[i]);
			return ret;
		}

		__forceinline char const* cstr() const
		{
			return content_;
		}

		/*__forceinline operator char const*() const
		{
			return content_;
		}*/

		~string()
		{
			if (content_) delete[] content_;
		}

	};


	size_t string::defaultAlloc_ = 10;

	double string::allocMult_ = 2.0;



	std::ostream& operator<<(std::ostream &os, const string &str)
	{
		return os << str.content_;
	}

	std::istream & operator >> (std::istream & is, string & arr)
	{
		if (arr.size_ == arr.reserved_ - 1)
			arr.realloc_(1 + arr.reserved_ * arr.allocMult_);
		is >> arr.content_[arr.size_++];
		arr.insert_end_();
		return is;
	}

	template<class _Type>
	x::range<char> const string::NumericValidator<_Type>::numbers_ = {'0','9'};

	template<>
	bool string::NumericValidator<char>::validate(string const& str)
	{
		return str.size_ == 1;
	}

	template<>
	bool string::NumericValidator<unsigned>::validate(string const& str)
	{
		if (!str.size_) return false;
		char const* ch = str.content_ - 1;
		while (numbers_.contains(*(++ch))) {}
		return ch - str.content_ == str.size_;
	}

	template<>
	bool string::NumericValidator<int>::validate(string const& str)
	{
		if (!str.size_) return false;
		char const* ch = str.content_ - 1;
		if (str.content_[0] == '-' || str.content_[0] == '+') ++ch;
		while (numbers_.contains(*(++ch))) {}
		return ch - str.content_ == str.size_;
	}

	template<>
	bool string::NumericValidator<double>::validate(string const& str)
	{
		if (!str.size_) return false;
		char const* ch = str.content_ - 1;
		bool dot = false;
		if (str.content_[0] == '-' || str.content_[0] == '+') ++ch;
		while (numbers_.contains(*(++ch)) || ((*ch == '.' && !dot) ? (dot = true) : false)) {}
		return ch - str.content_ == str.size_;
	}


	x::range<char> const string::AlphabeticValidator::lowerCase_ = {'a','z'};
	x::range<char> const string::AlphabeticValidator::upperCase_ = {'A','Z'};

	bool string::AlphabeticValidator::validate_alpha(string const& str)
	{
		if (!str.size_) return false;
		char const* ch = str.content_ - 1;
		while (lowerCase_.contains(*(++ch)) || upperCase_.contains(*ch)) {}
		return ch - str.content_ == str.size_;
	}

	bool string::AlphabeticValidator::validate_lowercase(string const& str)
	{
		if (!str.size_) return false;
		char const* ch = str.content_ - 1;
		while (lowerCase_.contains(*(++ch))) {}
		return ch - str.content_ == str.size_;
	}

	bool string::AlphabeticValidator::validate_uppercase(string const& str)
	{
		if (!str.size_) return false;
		char const* ch = str.content_ - 1;
		while (upperCase_.contains(*(++ch))) {}
		return ch - str.content_ == str.size_;
	}

}//namespace x

#undef enable_if
#undef decay


#endif //_X_STRING_H_
