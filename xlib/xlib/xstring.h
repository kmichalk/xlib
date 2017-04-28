#ifndef _STRING_H_
#define _STRING_H_

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
#include "result.h"
#include "xrnd.h"
#include "debug.h"


#define enable_if typename std::enable_if_t
#define decay typename std::decay_t

#define X_STRING_DEBUG true;

namespace x
{
	template<typename> class _string;

	template<typename _Char = _Char>
	class _CharRanges
	{
	public:
		static x::range<_Char> const binary_;
		static x::range<_Char> const numbers_;
		static x::range<_Char> const lowerCase_;
		static x::range<_Char> const upperCase_;
		static x::range<_Char> const hexLettersUpper_;
		static x::range<_Char> const hexLettersLower_;
	};

	template<class _Type, typename _Char = _Char>
	class NumericTypeValidator
	{
	public:
		static bool validate_type(_string<_Char> const& str);
		static x::result<_Type> convert(_string<_Char> const& str, byte base = 10);
	};

	template<typename _Char = _Char>
	class NumericReprValidator
	{
	public:
		static bool validate_binary(_string<_Char> const& str);
		static bool validate_hex(_string<_Char> const& str);
	};

	template<typename _Char = _Char>
	class AlphabeticValidator
	{
	public:
		static bool validate_alpha(_string<_Char> const& str);
		static bool validate_lowercase(_string<_Char> const& str);
		static bool validate_uppercase(_string<_Char> const& str);
	};


	template<typename _Char>
	class _string
	{
#if X_STRING_DEBUG == true
	public:
#endif
		static_assert(std::is_same<_Char, char>::value || std::is_same<_Char, wchar_t>::value,
			"Ivalid template argument.");


#if defined(_M_X64) || defined(_M_IX64) || defined(_X64_) || defined(_IA64_) || defined(_AMD64_)
		using _Align = __int64;
#else
		using _Align = __int32;
#endif

	
		template<class _Char>
		friend int strcmp(_string<_Char> const&, _string<_Char> const&);

		template<class _Char>
		friend bool streq(_string<_Char> const&, _string<_Char> const&);
		//template<class _Char>
		friend result<size_t> strfind(_string<char> const&, _string<char> const&);


		static constexpr size_t ALLOC_ALIGN_SIZE_ = sizeof(_Align);
		static constexpr size_t ELEMENT_SIZE_ = sizeof(_Char);
		static constexpr size_t DEFAULT_ALLOC_ = 10;
		static constexpr size_t INPUT_BUFFER_SIZE_ = 256;
		static constexpr unsigned __int32 TERMINATE_MASK_[4] = {0xFFFFFFFF, 0x00FFFFFF, 0x0000FFFF, 0x000000FF};

		static size_t defaultAlloc_;
		static double allocMult_;

		////////////////////////////////////////////////////////////////////////////

		_Char* data_;
		size_t size_;
		size_t reserved_;

		////////////////////////////////////////////////////////////////////////////
	public:
		static __forceinline size_t round_to_align_(size_t value)
		{
			return (value / ALLOC_ALIGN_SIZE_ + bool(value % ALLOC_ALIGN_SIZE_)) * ALLOC_ALIGN_SIZE_;
		}


		__forceinline _Char* alloc_(size_t newSize)
		{
			reserved_ = round_to_align_(newSize + 1);
			return new _Char[reserved_]{};
		}

		void realloc_(size_t newSize)
		{
			_Char* newContent = alloc_(newSize);
			memcpy(newContent, data_, ELEMENT_SIZE_*size_);
			delete[] data_;
			data_ = newContent;
		}

		void terminate_()
		{
			size_t alignOffset = round_to_align_(size_) - size_;
			*reinterpret_cast<unsigned __int32*>(data_ + (size_ + alignOffset - 4)) &=
				TERMINATE_MASK_[alignOffset];
		}

		void quicksort_(int r1, int r2)
		{
			int last = r1;
			for (int i = r1; i < r2; ++i) {
				if (data_[i] < data_[r2]) {
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

		using type = _Char;

		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using value_type = _Char;
		using pointer = _Char*;
		using reference = _Char&;

		////////////////////////////////////////////////////////////////////////////

		struct iterator
		{
			_string& obj;
			size_t pos;
			iterator(_string& obj): obj{obj}, pos{0}
			{
			}
			iterator(_string& obj, size_t pos): obj{obj}, pos{pos}
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
			__forceinline _Char& operator*()
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
			__forceinline operator bool()
			{
				return pos < obj.size_;
			}
		};

		////////////////////////////////////////////////////////////////////////////

		struct const_iterator
		{
			const _string<_Char>& obj;
			size_t pos;
			const_iterator(const _string<_Char>& obj): obj{obj}, pos{0}
			{
			}
			const_iterator(const _string<_Char>& obj, size_t pos): obj{obj}, pos{pos}
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
			__forceinline _Char operator*() const
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
		template<class, class> friend class NumericTypeValidator;
		template<class> friend class NumericReprValidator;
		template<class> friend class AlphabeticValidator;
		//friend class ::StringProcessor;

		struct AdoptFlag
		{};
		struct NoAllocFlag
		{};

		_string(_Char* str, size_t size, AdoptFlag):
			data_{str},
			size_{size},
			reserved_{size + 1}
		{
			terminate_();
		}

		template<class _Type>
		static constexpr bool IS_CHAR = x::is_any_of<_Type, _Char, std::add_const_t<_Char>>::value;

	public:
		_string(NoAlloc):
			data_{nullptr},
			size_{0},
			reserved_{0}
		{
		}

		_string():
			data_(alloc_(defaultAlloc_)), size_(0)
		{
			terminate_();
			//std::cout << "def ctor\n";
			////std::cout<<typeid(Shallow).name();
		}

		_string(size_t size, bool useable = false):
			data_(alloc_(size)),
			size_(useable ? size : 0)
		{
			terminate_();
			//std::cout << "size ctor\n";
		}

		_string(_Char* arr, size_t size, size_t extra = 0):
			data_(alloc_(size + extra)),
			size_(size)
		{
			memcpy(data_, arr, size_*ELEMENT_SIZE_);
			terminate_();
		}

		_string(_Char ch):
			data_{alloc_(1)},
			size_{1}
		{
			data_[0] = ch;
			terminate_();
		}

		template<class _Arg, size_t size,
			_concept<IS_CHAR<_Arg>>>
		_string(_Arg(&str)[size]):
			data_(alloc_(size - 1)),
			size_(size - 1)
		{
			//disp "arr\n";
			memcpy(data_, str, (size - 1)*ELEMENT_SIZE_);
			terminate_();
		}


		//TODO: repair
		//template<class _Arg, 
		//	_concept<std::is_pointer<_Arg>::value && 
		//		IS_CHAR<std::remove_pointer_t<_Arg>>>>
		//_string(_Arg str):
		//	data_{alloc_(strlen(str) / ELEMENT_SIZE)},
		//	size_{reserved_ - 1}
		//{
		//	//disp "ptr\n";
		//	memcpy(data_, str, size_*ELEMENT_SIZE);
		//	insert_end_();
		//}

		static inline _string<_Char> adopt(_Char* str, size_t size)
		{
			return _string<_Char>{str, size, AdoptFlag{}};
		}

		/*static _string force_adopt(_Char const* str)
		{
		return _string{x::rem_const(str),AdoptFlag{}};
		}*/

		_string(const _string<_Char>& other):
			data_{alloc_(other.size_ ? other.size_ : defaultAlloc_)},
			size_{other.size_ ? other.size_ : 0}
		{
			//std::cout << "copy ctor\n";
			if (size_)
				memcpy(data_, other.data_, size_*ELEMENT_SIZE_);
			terminate_();

		}

		_string(_string<_Char>&& other):
			data_{other.data_},
			size_{other.size_},
			reserved_{other.reserved_}
		{
			other.data_ = nullptr;
			terminate_();
			//std::cout << "move ctor\n";
		}

		_string(std::string const& str):
			data_{alloc_(str.size())},
			size_{str.size()}
		{
			if (size_)
				memcpy(data_, str.data(), size_);
			terminate_();
		}


		template<class... _Arg>
		static _string<_Char> build(_Arg&&... args)
		{
			_string<_Char> res((size_t)x::va::sum((args.size_)...));
			x::va::expand(res.append(args)...);
			return res;
		}


		__forceinline _string<_Char>&& move()
		{
			return (_string<_Char>&&)(*this);
		}

		////////////////////////////////////////////////////////////////////////////

		//returns const pointer to the content
		__forceinline const _Char* data() const
		{
			return data_;
		}

		//returns current size
		__forceinline size_t size() const
		{
			return size_;
		}

		__forceinline size_t lastpos() const
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
			return reserved_*ELEMENT_SIZE_;
		}

		//returns current value of defaultly allocated memory
		__forceinline size_t default_alloc() const
		{
			return defaultAlloc_;
		}

		//Set size of array defaultly allocated by constructors
		//and operations such as erase
		__forceinline static void set_default_alloc(size_t defSize = DEFAULT_ALLOC_)
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

		/*_string<_Char>& separate(
		x::vector<x::_string<_Char>>&	buffer,
		_Char					delim	= ' ')
		{
		buffer.clear();
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
		return *this;
		}*/

		_vector<x::_string<_Char>> separate(_Char delim = ' ')
		{
			x::_vector<x::_string<_Char>> buffer;
			size_t firstPos = 0;
			auto ch{cbegin()};
			for (; ch; ++ch) {
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
			return buffer;
		}

		_Char* extract_data()
		{
			_Char* prev = data_;
			data_ = alloc_(defaultAlloc_);
			size_ = 0;
			return prev;
		}

		_Char* move_data()
		{
			_Char* prev = data_;
			data_ = nullptr;
			return prev;
		}

		//false if is empty
		__forceinline operator bool() const
		{
			return size_;
		}

		operator std::string() const
		{
			return std::string(data_, size_);
		}

		//returns reference to nth element
		__forceinline _Char& operator[](size_t n) const
		{
			return data_[n];
		}

		//returns copy of this vector concatenated with copy of "other"
		_string<_Char> operator+(_string<_Char> const& other) const
		{
			_string<_Char> result(*this);
			return result.append(other);
		}

		inline _string<_Char>& operator+=(_string<_Char> const& other)
		{
			return append(other);
		}

		/*template<size_t _size>
		friend _string<_Char> operator+(_Char(&arr)[_size], _string<_Char> const& str)
		{
		return _string<_Char>{arr}.append(str);
		}
		*/
		inline bool operator==(const _string<_Char>& other) const
		{
			if (size_ != other.size_) return false;
			return strncmp(data_, other.data_, size_) == 0;
			//return x::streq(*this, other);
			/*return size_==other.size_ &&
			!strncmp(data_, other.data_, size_);*/
		}

		inline bool operator!=(const _string<_Char>& other) const
		{
			if (size_ != other.size_) return true;
			return strncmp(data_, other.data_, size_) != 0;
		}

		inline bool operator<(_string<_Char> const& other) const
		{
			return x::strcmp(*this, other) < 0;
		}

		inline bool operator<=(_string<_Char> const& other) const
		{
			return x::strcmp(*this, other) <= 0;
		}

		inline bool operator>(_string<_Char> const& other) const
		{
			return x::strcmp(*this, other) > 0;
		}

		inline bool operator>=(_string<_Char> const& other) const
		{
			return x::strcmp(*this, other) >= 0;
		}

		//returns reference to "n"th element
		_Char& at(size_t n) const
		{
			if (n < size_)
				return data_[n];
			throw ERR_OUT_OF_SIZE;
		}

		//returns reference to first element
		inline result<_Char&> first() const
		{
			if (size_)
				return data_[0];
			return result<_Char&>::INVALID;
		}

		//returns reference to last element
		inline result<_Char&> last() const
		{
			if (size_)
				return data_[size_ - 1];
			result<_Char&>::INVALID;
		}

		//swaps content and parameters
		void swap(_string<_Char>& other)
		{
			std::swap(size_, other.size_);
			std::swap(reserved_, other.reserved_);
			std::swap(data_, other.data_);
		}

		//swaps two elements
		__forceinline _string<_Char>& swap(size_t pos1, size_t pos2)
		{
			std::swap(data_[pos1], data_[pos2]);
			terminate_();
			return *this;
		}

		//returns vector_ containing non trivial copy of elements
		//in range <p1,p2> inclusive
		_string<_Char> substr(size_t p1, size_t p2) const
		{
			if ((size_ > p2) && (p2 >= p1))
				return _string<_Char>{data_ + p1, p2 - p1 + 1};

			return _string<_Char>{};
		}

		//returns vector_ containing non trivial copy of elements
		//in range from p to the last element inclusive
		_string<_Char> substr(size_t p) const
		{
			if (size_ > p)
				return _string<_Char>{data_ + p, size_ - p};
			/*_string<_Char> sub(size_ - p, true);
			memcpy(sub.data_, data_ + p, sub.size_*ELEMENT_SIZE);
			return sub;*/

			return _string<_Char>{};
		}

		//removes every element beyond range <p1,p2>
		_string<_Char>& cut(size_t p1, size_t p2)
		{
			if (p2 >= size_) p2 = size_ - 1;
			int diff = (int)p2 - (int)p1;
			if (diff >= (int)size_ - 1) return *this;
			if (diff >= 0) {
				memcpy(data_, data_ + p1, ELEMENT_SIZE_*(diff + 1));
				size_ = diff + 1;
			}
			else size_ = 0;
			terminate_();
			return *this;
		}

		//removes nth element 
		_string<_Char>& erase(size_t n)
		{
			if (n < size_) {
				memcpy(data_ + n, data_ + n + 1,
					(size_ - n - 1)*ELEMENT_SIZE_);
				--size_;
			}
			terminate_();
			return *this;
		}

		//replaces each element with "elem" 
		_string<_Char>& fill(_Char elem)
		{
			for (size_t i = 0; i < size_; ++i) {
				data_[i] = _Char(elem);
			}
			return *this;
		}

		//resizes content to "size" and replaces each element with "elem" 
		_string<_Char>& fill(_Char elem, size_t size)
		{
			delete[] data_;
			data_ = alloc_(size);
			size_ = size;
			for (size_t i = 0; i < size_; ++i) {
				data_[i] = _Char(elem);
			}
			return *this;
		}

		//returns _string containing n front elements
		__forceinline _string<_Char> front(size_t n) const
		{
			return substr(0, n - 1);
		}

		//returns _string containing n tail elements
		__forceinline _string<_Char> back(size_t n) const
		{
			return substr(size_ - n, size_ - 1);
		}

		//resizes array and does non trivial copy of "other"'s content
		_string<_Char>& operator=(const _string<_Char> &other)
		{
			if (other.size_) {
				delete[] data_;
				data_ = alloc_(other.size_);
				size_ = other.size_;
				memcpy(data_, other.data_, size_*ELEMENT_SIZE_);
			}
			terminate_();
			return *this;
		}

		_string<_Char>& operator=(_string<_Char> && other)
		{
			if (other.size_) {
				delete[] data_;
				data_ = other.data_;
				size_ = other.size_;
				other.data_ = nullptr;
			}
			terminate_();
			return *this;
		}

		template<size_t size>
		_string<_Char>& operator=(_Char const(&arr)[size])
		{
			delete[] data_;
			data_ = alloc_(size - 1);
			size_ = size - 1;
			memcpy(data_, static_cast<const _Char*>(arr), size_*ELEMENT_SIZE_);
			terminate_();
			return *this;
		}

		//adds new element on the end
		_string<_Char>& push_back(_Char newElem)
		{
			if (size_ == reserved_ - 1)
				realloc_(1 + reserved_ * allocMult_);
			data_[size_++] = newElem;
			terminate_();
			return *this;
		}

		//adds new element on the beginning
		__forceinline _string<_Char>& push_front(_Char newElem)
		{
			insert(newElem, 0);
			return *this;
		}

		//moves the part of content after "pos" one field forward
		//and pastes in new element on "pos"

		//moves the part of content after "pos" forward
		//and pastes in content of other array beginning on "pos"

		_string<_Char>& insert(_Char const* str, size_t insSize, size_t pos)
		{
			if ((reserved_ - 1) < (size_ + insSize)) {
				_Char* newContent = alloc_(size_ + insSize);
				memcpy(newContent + pos + insSize, data_ + pos, ELEMENT_SIZE_*(size_ - pos));
				memcpy(newContent + pos, str, insSize);
				memcpy(newContent, data_, ELEMENT_SIZE_*pos);
				delete[] data_;
				data_ = newContent;
			}
			else {
				memcpy(data_ + pos + insSize, data_ + pos, ELEMENT_SIZE_*(size_ - pos));
				memcpy(data_ + pos, str, insSize);
			}
			size_ += insSize;
			terminate_();
			return *this;
		}

		inline _string<_Char>& insert(_Char ch, size_t pos)
		{
			return insert(&ch, 1, pos);
		}

		inline _string<_Char>& insert(const _string<_Char>& other, size_t pos)
		{
			return insert(other.data_, other.size_, pos);
		}

		//removes up to "n" specified elements "toRem" if such occur.
		//if "n"=0 removes all
		//returns number of removed elements
		int remove(_Char val, unsigned max = 0)
		{
			int i = -1, lastp = -1, nrem = 0;
			while (++i < size_) {
				if (data_[i] == val) {
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
			terminate_();
			return nrem;
		}

		/*_string<_Char>& remove2(_Char val, unsigned max = 0)
		{
		if (!size_) return *this;

		int nrem = 0;
		int offset = 0;
		_Char* newData = alloc_(reserved_);
		for (size_t ch = 0; ch < size_; ++ch) {
		if (data_[ch] != val)
		newData[ch + offset] = data_[ch];
		else
		--offset;
		}
		size_ += offset;
		delete[] data_;
		data_ = newData;
		insert_end_();
		return *this;
		}

		_string<_Char>& remove3(_Char val, unsigned max = 0)
		{
		static thread_local _Char* buffer = new _Char[1000]{};
		if (!size_) return *this;

		int offset = 0;
		for (size_t ch = 0; ch < size_; ++ch) {
		if (data_[ch] != val)
		buffer[ch + offset] = data_[ch];
		else
		--offset;
		}
		size_ += offset;
		memcpy(data_, buffer, size_*ELEMENT_SIZE);
		insert_end_();
		return *this;
		}*/

		template<typename _Func>
		int remove_if(_Func&& fn_, unsigned max = 0)
		{
			int i = -1, lastp = -1, nrem = 0;
			while (++i < size_) {
				if (fn_(data_[i])) {
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
			terminate_();
			return nrem;
		}

		template<typename _Func>
		_string<_Char> copy_if(_Func&& fn_)
		{
			_string<_Char> copied(size_);
			for (int i = 0; i < size_; ++i) {
				if (fn_(data_[i]))
					copied.push_back(data_[i]);
			}
			copied.shrink();
			return copied;
		}

		template<typename _Func>
		_string<_Char> extract_if(_Func&& fn_, unsigned max = 0)
		{
			_string<_Char> extracted(size_);
			int i = -1, lastp = -1, nrem = 0;
			while (++i < size_) {
				if (fn_(data_[i])) {
					extracted.push_back(data_[i]);
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
			extracted.shrink();
			return extracted;
		}

		//Erases specified element fast, swapping it with the current last element. 
		//The order of contained elements is not preserved.
		_string<_Char>& throw_back(size_t which)
		{
			swap(which, --size_);
			terminate_();
			return *this;
		}

		//removes first element
		__forceinline _string<_Char>& pop_front()
		{
			if (size_) erase(0);
			return *this;
		}

		//removes last element
		_string<_Char>& pop_back()
		{
			if (size_) --size_;
			terminate_();
			return *this;
		}

		//uses quick sort on the whole scope
		_string<_Char>& sort()
		{
			quicksort_(0, size_ - 1);
			return *this;
		}

		_string<_Char>& sort(size_t r1, size_t r2)
		{
			if (r2 >= size_) r2 = size_ - 1;
			if (r1 < r2) quicksort_(r1, r2);
			return *this;
		}

#ifdef _X_RND_H_	
		//set elements in random order
		//if "swap_each" is disabled, each element has
		//some probability of not being moved
		_string<_Char>& shuffle(bool swap_each = true)
		{
			for (int i = 0; i<size_ - 2; ++i) {
				swap(i, random<size_t>(i + swap_each, size_ - 1));
			}
			return *this;
		}

		//return random element from specified range <"p1","p2"> inclusive
		_Char& get_random(size_t p1, size_t p2) const
		{
			return data_[random<size_t>(p1, p2)];
		}

		//return random element
		__forceinline _Char& get_random() const
		{
			return get_random(0, size_ - 1);
		}

		_string<_Char>& randomize()
		{
			return randomize(
				std::numeric_limits<_Char>::min(),
				std::numeric_limits<_Char>::max());
		}

		_string<_Char>& randomize(size_t size)
		{
			return randomize(
				std::numeric_limits<_Char>::min(),
				std::numeric_limits<_Char>::max(),
				size);
		}

		_string<_Char>& randomize(_Char r1, _Char r2)
		{
			using UChar = std::make_unsigned_t<_Char>;

			for (int i = 0; i < size_; ++i) {
				data_[i] = (_Char)random<UChar>((UChar)r1, (UChar)r2);
			}
			return *this;
		}

		_string<_Char>& randomize(_Char r1, _Char r2, size_t size)
		{
			if (size > reserved_) {
				delete[] data_;
				data_ = alloc_(size);
			}
			size_ = size;
			terminate_();

			for (int i = 0; i < size_; ++i) {
				data_[i] = x::random<_Char>(r1, r2);
			}
			return *this;
		}

#endif

		//deallocate memory exceeding the actual size
		__forceinline void shrink()
		{
			realloc_(size_);
			terminate_();
		}


		void reserve(size_t newSize)
		{
			if (newSize > reserved_ - 1) {
				realloc_(newSize);
				terminate_();
			}
		}


		inline void extend(size_t size)
		{
			reserve(reserved_ + size);
		}


		_string<_Char>& append(_Char const* str, size_t strSize)
		{
			if ((reserved_ - 1) < (size_ + strSize))
				realloc_(size_ + strSize);
			memcpy(data_ + size_, str, strSize*ELEMENT_SIZE_);
			size_ += strSize;
			terminate_();
			return *this;
		}

		inline _string<_Char>& append(const _string<_Char>& other)
		{
			return append(other.data_, other.size_);
		}

		__forceinline _string<_Char>& prepend(_Char const* str, size_t prepSize)
		{
			insert(other, 0);
			return *this;
		}

		//moves content forward and inserts non trivial copy
		//of "other"'s content on the beginning
		__forceinline _string<_Char>& prepend(const _string<_Char>& other)
		{
			insert(other, 0);
			return *this;
		}

		//returns number of occurances of elem
		size_t count(_Char elem) const
		{
			size_t n = 0;
			for (size_t i = 0; i<size_; ++i)
				if (data_[i] == elem) ++n;
			return n;
		}

		result<size_t> find_first(_Char elem) const
		{
			for (size_t i = 0; i < size_; ++i)
				if (data_[i] == elem) return i;
			return result<size_t>::INVALID;
		}

		//returns reference to element, value of which is equal to "elem"
		result<size_t> find_first(_Char elem, _Char off) const
		{
			for (size_t i = 0; i < size_; ++i) {
				if (data_[i] == elem) return i;
				if (data_[i] == off) return result<size_t>::INVALID;
			}
			return result<size_t>::INVALID;
		}

		result<size_t> find_last(_Char elem) const
		{
			for (size_t i = size_ - 1; i; --i)
				if (data_[i] == elem) return i;
			return result<size_t>::INVALID;
		}

		result<size_t> find_last(_Char elem, _Char off) const
		{
			for (size_t i = size_ - 1; i; --i) {
				if (data_[i] == elem) return i;
				if (data_[i] == off) return result<size_t>::INVALID;
			}
			return result<size_t>::INVALID;
		}

		result<size_t> find_first(_string<_Char> const& other) const
		{

		}

		bool contains(_Char elem) const
		{
			for (size_t i = 0; i < size_; ++i)
				if (data_[i] == elem) return true;
			return false;
		}

		/*bool contains(char const* str) const
		{
		_IF_DEBUG( disp "x::_string::contains(", str, ")\n");
		_IF_DEBUG( _DEBUG_VAR_DISP(data_), endl, endl);
		for (size_t ti = 0; ti < size_; ++ti) {
		_IF_DEBUG( _DEBUG_VAR_DISP(data_[ti]), endl);
		if (data_[ti] == str[0]) {
		for (size_t si = 1, tk = ti + 1; ; ++si, ++tk) {
		_IF_DEBUG( _DEBUG_VAR_DISP(data_[tk]), " ");
		_IF_DEBUG( _DEBUG_VAR_DISP(str[si]), endl);
		if (!str[si]) {
		_IF_DEBUG(_DEBUG_VAR_DISP(!str[si]), endl);
		return true;
		}
		if (str[si] != data_[tk]) {
		_IF_DEBUG(_DEBUG_VAR_DISP(str[si] != data_[tk]), endl);
		break;
		}
		if (tk == size_) {
		_IF_DEBUG(_DEBUG_VAR_DISP(tk == size_), endl);
		return !str[si];
		}
		}
		}
		}
		return false;
		}*/

		//returns reference to element, for which "fn"'s return value is equal to "val"
		/*template<typename _Func, typename _Cmp>
		_Char& find_by(_Func&& fn_, _Cmp val)
		{
		for (size_t i = 0; i < size_; ++i)
		if (fn_(data_[i]) == val) return data_[i];
		throw(ERR_NOT_FOUND);
		}

		template<typename _Func>
		_Char& find_by(_Func&& fn_)
		{
		for (size_t i = 0; i < size_; ++i)
		if (fn_(data_[i])) return data_[i];
		throw(ERR_NOT_FOUND);
		}*/

		//replaces each occurance of "what" element with "val"
		int replace(_Char what, _Char val, unsigned nmax = 0)
		{
			int nrep = 0;
			for (int i = 0; i < size_; ++i) {
				if (data_[i] == what) {
					data_[i] = val;
					if (++nrep == nmax) return nrep;
				}
			}
			return nrep;
		}

		//sets elements in reverse order
		_string<_Char>& reverse()
		{
			for (int i = size_ / 2 - 1; i >= 0; --i) {
				swap(i, size_ - i - 1);
			}
			return *this;
		}

		//moves elements "n" fields forward / backward (if "n" < 0)
		//making them to appear on the beginning / end
		_string<_Char>& shift(int n)
		{
			if (n %= int(size_)) {
				if (n < 0) n += size_;
				_Char* newContent = alloc_(size_);
				memcpy(newContent, data_ + size_ - n, ELEMENT_SIZE_*n);
				memcpy(newContent + n, data_, ELEMENT_SIZE_*(size_ - n));
				delete[] data_;
				data_ = newContent;
			}
			terminate_();
			return *this;
		}

		_Char take(size_t n)
		{
			if (n < size_) {
				_Char elem = data_[n];
				erase(n);
				return elem;
			}
			throw ERR_OUT_OF_SIZE;
		}


		_string<_Char>& trim()
		{
			if (!size_) return *this;

			int nFront = -1;
			int nBack = size_;
			while (is_whitespace(data_[++nFront])) {
			}
			while (is_whitespace(data_[--nBack])) {
			}
			cut(nFront, nBack);
			return *this;
		}

		/*_string<_Char>& cut(size_t sides)
		{
		if (!size_ || !sides) return *this;
		if (sides >= size_ / 2) {
		size_ = 0;
		insert_end_();
		}
		else
		cut(sides, size_ - sides - 1);
		return *this;
		}*/

		_string<_Char>& trim(_Char ch)
		{
			if (!size_) return *this;

			int nFront = -1;
			int nBack = size_;
			while (data_[++nFront] == ch) {
			}
			while (data_[--nBack] == ch) {
			}
			cut(nFront, nBack);
			return *this;
		}

		_string<_Char>& trim_back(size_t max = std::numeric_limits<size_t>::max())
		{
			if (!size_ || !max) return *this;

			int nBack = size_ - 1;
			while (max-- && is_whitespace(data_[nBack])) --nBack;
			cut(0, nBack - 1);
			return *this;
		}

		_string<_Char>& trim_back(_Char ch, size_t max = std::numeric_limits<size_t>::max())
		{
			if (!size_ || !max) return *this;

			int nBack = size_ - 1;
			while (max-- && data_[nBack] == ch) --nBack;
			cut(0, nBack);
			return *this;
		}

		_string<_Char>& trim_front(size_t max = std::numeric_limits<size_t>::max())
		{
			if (!size_ || !max) return *this;

			int nFront = 0;
			while (max-- && is_whitespace(data_[nFront])) ++nFront;
			cut(nFront, size_ - 1);
			return *this;
		}

		_string<_Char>& trim_front(_Char ch, size_t max = std::numeric_limits<size_t>::max())
		{
			if (!size_ || !max) return *this;

			int nFront = 0;
			while (max-- && data_[nFront] == ch) ++nFront;
			cut(nFront, size_ - 1);
			return *this;
		}

		_string<_Char>& trim_decimal()
		{
			if (!size_) return *this;
			if (!find_last('.')) return *this;

			trim_back('0');
			if (data_[size_ - 1] == '.') {
				--size_;
				terminate_();
			}
			return *this;
		}


		result<size_t> match_bracket(size_t brPos, _Char lbr = '(', _Char rbr = ')') const
		{
			if (brPos >= size_) return result<size_t>::INVALID;
			int nesting = 1;
			if (data_[brPos] == lbr) {
				for (int ch = brPos + 1; ch < size_; ++ch) {
					if (data_[ch] == lbr) ++nesting;
					if (data_[ch] == rbr && --nesting == 0)
						return (size_t)ch;
				}
			}
			else if (data_[brPos] == rbr) {
				for (int ch = brPos - 1; ch >= 0; --ch) {
					if (data_[ch] == rbr) ++nesting;
					if (data_[ch] == lbr && --nesting == 0)
						return (size_t)ch;
				}
			}
			return result<size_t>::INVALID;
		}

		//true if is empty
		__forceinline bool empty() const
		{
			return !bool(size_);
		}


		template<class _Type, _concept<std::is_fundamental<_Type>::value>>
		__forceinline bool is() const
		{
			return NumericTypeValidator<_Type, _Char>::validate_type(*this);
		}

		__forceinline bool is_number() const
		{
			return NumericTypeValidator<float, _Char>::validate_type(*this);
		}

		__forceinline bool is_binary() const
		{
			return NumericReprValidator<_Char>::validate_binary(*this);
		}

		__forceinline bool is_hex() const
		{
			return NumericReprValidator<_Char>::validate_hex(*this);
		}

		__forceinline bool is_word() const
		{
			return AlphabeticValidator<_Char>::validate_alpha(*this);
		}

		__forceinline bool is_lowercase() const
		{
			return AlphabeticValidator<_Char>::validate_lowercase(*this);
		}

		__forceinline bool is_uppercase() const
		{
			return AlphabeticValidator<_Char>::validate_uppercase(*this);
		}

		template<class _Conv>
		__forceinline result<_Conv> to(byte base = 10) const
		{
			return NumericTypeValidator<_Conv, _Char>::convert(*this, base);
		}

		_string<_Char>& to_lowercase()
		{
			static constexpr _Char caseInterval = 'a' - 'A';
			for (auto ch{begin()}; ch; ++ch) {
				if (_CharRanges<_Char>::upperCase_.contains(*ch))
					(*ch) += caseInterval;
			}
			return *this;
		}

		_string<_Char>& to_uppercase()
		{
			static constexpr _Char caseInterval = 'a' - 'A';
			for (auto ch{begin()}; ch; ++ch) {
				if (_CharRanges<_Char>::lowerCase_.contains(*ch))
					(*ch) -= caseInterval;
			}
			return *this;
		}

		friend std::ostream& operator<<(std::ostream &os, const _string<_Char> &str)
		{
			return os << str.data_;
		}

		friend std::istream & operator >> (std::istream & is, _string<_Char> & str)
		{
			static thread_local _Char buffer[_string<_Char>::INPUT_BUFFER_SIZE_]{};

			str.clear();
			while (is.get(buffer, _string<_Char>::INPUT_BUFFER_SIZE_))
				str.append(buffer, is.gcount());

			is.clear();
			is.seekg(is.beg);
			return is;
		}

		//pass content to output stream
		//friend std::ostream& operator<<(std::ostream &os, const _string<_Char> &arr);

		////pushes back new element from input stream
		//friend std::istream &operator >> (std::istream &is, _string<_Char> &arr);

		//calls function on every element
		template<typename _Func>
		_string<_Char>& call(_Func&& fn_)
		{
			for (int i = 0; i<size_; ++i) {
				fn_(data_[i]);
			}
			return *this;
		}

		//checks if condition is true for every element
		template<typename _Func>
		bool true_for_all(_Func&& fn_)
		{
			for (int i = 0; i<size_; ++i)
				if (!fn_(data_[i])) return false;
			return true;
		}

		//forgets the content with no memory deallocation
		_string<_Char>& clear()
		{
			size_ = 0;
			terminate_();
			return *this;
		}

		//clears all content and allocates new block of memory of default size
		_string<_Char>& erase()
		{
			if (size_) {
				delete[] data_;
				data_ = alloc_(defaultAlloc_);
				size_ = 0;
				terminate_();
			}
			return *this;
		}

		/*returns std::vector with non trivial copy of content
		std::vector<_Char> to_std_vector()
		{
		return std::vector<_Char>(content_, content_ + size_ - 1);
		}*/

		/*typename _vector<_Char>::moved to_vector()
		{
		return typename _vector<_Char>::moved{x::vector<_Char>{content_, size_}};
		}*/

		_vector<_Char> to_vector()
		{
			return _vector<_Char>{data_, size_};
		}


		_string<_Char>& flip_char(int n)
		{
			for (int i = 0; i < size_; ++i)
				data_[i] += n;
			return *this;
		}

		_string<_Char>& flip_char(_string<_Char> const& key)
		{
			for (int i = 0; i < size_; ++i)
				data_[i] += key.data_[i % key.size_];
			return *this;
		}

		_string<_Char>& flip_char(_string<_Char> const& key, int mult)
		{
			for (int i = 0; i < size_; ++i)
				data_[i] += mult * key.data_[i % key.size_];
			return *this;
		}

		size_t hash(size_t max) const
		{
			static constexpr size_t FNV_PRIME = 16777619;
			static constexpr size_t OFFSET_BASIS = 2166136261;
			size_t result = 0;
			for (int i = 0; i<size_; ++i)
				result = (result*FNV_PRIME) ^ data_[i];

			return result%max;
			//return (result + size_)%max;
		}


		__forceinline _Char const* cstr() const
		{
			return data_;
		}

		/*__forceinline operator _Char const*() const
		{
		return content_;
		}*/

		~_string()
		{
			if (data_) delete[] data_;
		}

	};

	////////////////////////////////////////////////////////////////////////////

	using string = _string<char>;
	using wstring = _string<wchar_t>;

	////////////////////////////////////////////////////////////////////////////

	template<typename _Char>
	size_t _string<_Char>::defaultAlloc_ = 10;

	template<typename _Char>
	double _string<_Char>::allocMult_ = 2.0;


	////////////////////////////////////////////////////////////////////////////


	/*template<typename _Char>
	std::ostream& operator<<(std::ostream &os, const _string<_Char> &str)
	{
	return os << str.data_;
	}

	template<typename _Char>
	std::istream & operator >> (std::istream & is, _string<_Char> & arr)
	{
	if (arr.size_ == arr.reserved_ - 1)
	arr.realloc_(1 + arr.reserved_ * arr.allocMult_);
	is >> arr.data_[arr.size_];
	arr.terminate_();
	return is;
	}*/


	////////////////////////////////////////////////////////////////////////////


	template<typename _Char>
	x::range<_Char> const _CharRanges<_Char>::numbers_ = {'0','9'};

	template<typename _Char>
	x::range<_Char> const _CharRanges<_Char>::lowerCase_ = {'a','z'};

	template<typename _Char>
	x::range<_Char> const _CharRanges<_Char>::upperCase_ = {'A','Z'};

	template<typename _Char>
	x::range<_Char> const _CharRanges<_Char>::hexLettersUpper_ = {'A','F'};

	template<typename _Char>
	x::range<_Char> const _CharRanges<_Char>::hexLettersLower_ = {'a','f'};


	////////////////////////////////////////////////////////////////////////////


	template<typename _Char>
	class NumericTypeValidator<bool, _Char>
	{
	public:
		static bool validate_type(_string<_Char> const& str)
		{
			if (!str.size_) return false;
			if (str.size_ == 1) {
				return _CharRanges<_Char>::binary_.contains(str.data_[0]) ||
					str.data_[0] == 'T' || str.data_[0] == 'F' ||
					str.data_[0] == 't' || str.data_[0] == 'f';
			}
			else return str == "true" || str == "false";
		}

		static x::result<bool> convert(_string<_Char> const& str, byte base)
		{
			if (!str.size_) return result<bool>::INVALID;
			if (str.size_ == 1) {
				_Char first = str.data_[0];
				return (first == '1' || first == 'T' || first == 't' ? true :
					first == '0' || first == 'F' || first == 'f' ? false :
					result<bool>::INVALID);
			}
			else return (str == "true" ? true : str == "false" ? false : result<bool>::INVALID);
		}
	};

	template<typename _Char>
	class NumericTypeValidator<_Char, _Char>
	{
	public:
		static bool validate_type(_string<_Char> const& str)
		{
			return str.size_ == 1;
		}

		static x::result<_Char> convert(_string<_Char> const& str)
		{
			return (str.size_ == 1 ? str.data_[0] : result<_Char>::INVALID);
		}
	};

	template<typename _Char>
	class NumericTypeValidator<unsigned, _Char>
	{
		static x::result<unsigned> convBaseLess10_(_string<_Char> const& str, byte base)
		{
			_Char const
				*ch = str.data_ - 1,
				*end = str.data_ + str.size_ - 1,
				*begin = str.data_,
				last = '0' + base;
			unsigned
				result = 0,
				mult = 1;

			while (*++ch >= '0' && *ch <= last) {
				result += mult * (*(end - (size_t)ch + (size_t)begin) - '0');
				mult *= base;
			}
			return ch > end ? result : x::result<unsigned>::INVALID;
		}

		static x::result<unsigned> convBaseMore10_(_string<_Char> const& str, byte base)
		{
			_Char const
				*ch = str.data_ - 1,
				*end = str.data_ + str.size_ - 1,
				*begin = str.data_;
			unsigned
				result = 0,
				mult = 1;
			/*x::range<_Char>
			over10Range{'A', 'A' + (byte)base - '0'},*/

			//_Char cur;
			////while(_CharRange.contains((cur = *(end - (size_t)++ch + (size_t)begin)) >= 'A' )
			//while (_CharRange.contains(
			//	cur = (*(end - (size_t)++ch + (size_t)begin) >= 'A' ? *(end - (size_t)ch + (size_t)begin) + 'A' - '0' : *(end - (size_t)ch + (size_t)begin)))) {
			//	result += mult * (cur - '0');
			//	mult *= base;
			//}
			return ch > end ? result : x::result<unsigned>::INVALID;
		}

	public:
		static bool validate_type(_string<_Char> const& str, byte base)
		{
			if (!str.size_) return false;
			_Char const* ch = str.data_ - 1;
			while (_CharRanges<_Char>::numbers_.contains(*++ch)) {
			}
			return ch - str.data_ == str.size_;
		}

		static x::result<unsigned> convert(_string<_Char> const& str, byte base)
		{
			if (!str.size_) return x::result<unsigned>::INVALID;
			return base <= 10 ? convBaseLess10_(str, base) : convBaseMore10_(str, base);
		}
	};

	template<typename _Char>
	class NumericTypeValidator<int, _Char>
	{
	public:
		static bool validate_type(_string<_Char> const& str)
		{
			if (!str.size_) return false;
			_Char const* ch = str.data_ - 1;
			if (str.data_[0] == '-' || str.data_[0] == '+') ++ch;
			while (_CharRanges<_Char>::numbers_.contains(*(++ch))) {
			}
			return ch - str.data_ == str.size_;
		}

		static x::result<int> convert(_string<_Char> const& str, byte base)
		{
			if (!str.size_) return x::result<int>::INVALID;

			/*x::range<_Char>
			_CharRange{'0', base};*/
			_Char const
				*ch = str.data_,
				*end = str.data_ + str.size_ - 1,
				*begin = str.data_;
			int
				result = 0,
				mult = 1;
			bool
				neg = false;

			if (*ch == '-') {
				neg = true; ++begin;
			}
			else --ch;
			while (_CharRanges<_Char>::numbers_.contains(*++ch)) {
				result += mult * (*(end - (size_t)ch + (size_t)begin) - '0');
				mult *= base;
			}
			return ch > end ? (neg ? -result : result) : x::result<int>::INVALID;
		}
	};

	template<typename _Char>
	class NumericTypeValidator<double, _Char>
	{
	public:
		static bool validate_type(_string<_Char> const& str)
		{
			if (!str.size_) return false;
			_Char const* ch = str.data_ - 1;
			bool dot = false;
			if (str.data_[0] == '-' || str.data_[0] == '+') ++ch;
			while (_CharRanges<_Char>::numbers_.contains(*(++ch)) ||
				((*ch == '.' && !dot) ? (dot = true) : false)) {
			}
			return ch - str.data_ == str.size_;
		}

		//static x::result<double> dispPos(_string<_Char> const& str, byte base)
		//{
		//	if (!str.size_) return x::result<double>{};

		//	/*x::range<_Char>
		//	_CharRange{'0', base};*/
		//	_Char const
		//		*ch = str.content_,
		//		*end = str.content_ + str.size_ - 1,
		//		*begin = str.content_;
		//	double
		//		result = 0,
		//		mult = 1,
		//		dotdiv = 1;
		//	bool
		//		neg = false;

		//	if (*ch == '-') {
		//		neg = true; 
		//		++begin;
		//	}
		//	else --ch;
		//	while (_CharRanges<_Char>::numbers_.contains(*++ch)) {
		//		result += mult * (*(end - (size_t)ch + (size_t)begin) - '0');
		//		mult *= base;
		//		dotdiv *= base;
		//	}
		//	if (ch > end) return neg ? -result : result;
		//	if (*ch == '.') {
		//		mult = 1.0/base;
		//		begin = ch;
		//		while (_CharRanges<_Char>::numbers_.contains(*++ch)) {
		//			result += mult * ((size_t)ch - (size_t)begin - '0');
		//			mult /= base;
		//		}
		//	}
		//	return ch > end ? (neg ? -result : result) : x::result<double>{};
		//}

		static x::result<double> convert(_string<_Char> const& str, byte base)
		{
			if (!str.size_) return x::result<double>::INVALID;

			double
				result = 0,
				mult = 0.1;
			bool
				neg = (str.data_[0] == '-');

			auto ch{str.cend()};
			for (; _CharRanges<_Char>::numbers_.contains(*--ch) && ch; ) {
				mult *= base;
				result += mult * (*ch - '0');
			}
			if (!ch) return neg ?
				x::result<double>{-result} :
				x::result<double>{result};

			if (*ch != '.') return x::result<double>::INVALID;
			//mult *= base;
			if (ch.pos != str.lastpos()) {
				result /= mult*base;
				mult = 1;
			}
			else mult *= base;
			while (_CharRanges<_Char>::numbers_.contains(*--ch)) {
				result += mult * (*ch - '0');
				mult *= base;
			}
			return !ch ?
				(neg ?
					x::result<double>{-result} :
					x::result<double>{result}) :
				x::result<double>::INVALID;
		}
	};

	template<typename _Char>
	class NumericTypeValidator<float, _Char>
	{
	public:
		static bool validate_type(_string<_Char> const& str)
		{
			if (!str.size_) return false;
			auto ch{str.cbegin() - 1};
			while (*++ch);
		}
	};


	////////////////////////////////////////////////////////////////////////////


	template<typename _Char>
	inline bool NumericReprValidator<_Char>::validate_binary(_string<_Char> const & str)
	{
		if (!str.size_) return false;
		_Char const* ch = str.data_ - 1;
		while (_CharRanges<_Char>::binary_.contains(*(++ch))) {
		}
		return ch - str.data_ == str.size_;
	}

	template<typename _Char>
	inline bool NumericReprValidator<_Char>::validate_hex(_string<_Char> const & str)
	{
		if (!str.size_) return false;
		_Char const* ch = str.data_ - 1;
		while (_CharRanges<_Char>::numbers_.contains(*(++ch)) ||
			_CharRanges<_Char>::hexLettersLower_.contains(*ch) ||
			_CharRanges<_Char>::hexLettersUpper_.contains(*ch)) {
		}
		return ch - str.data_ == str.size_;
	}


	template<typename _Char>
	bool AlphabeticValidator<_Char>::validate_alpha(_string<_Char> const& str)
	{
		if (!str.size_) return false;
		_Char const* ch = str.data_ - 1;
		while (_CharRanges<_Char>::lowerCase_.contains(*(++ch)) ||
			_CharRanges<_Char>::upperCase_.contains(*ch)) {
		}
		return ch - str.data_ == str.size_;
	}

	template<typename _Char>
	bool AlphabeticValidator<_Char>::validate_lowercase(_string<_Char> const& str)
	{
		if (!str.size_) return false;
		_Char const* ch = str.data_ - 1;
		while (_CharRanges<_Char>::lowerCase_.contains(*(++ch))) {
		}
		return ch - str.data_ == str.size_;
	}

	template<typename _Char>
	bool AlphabeticValidator<_Char>::validate_uppercase(_string<_Char> const& str)
	{
		if (!str.size_) return false;
		_Char const* ch = str.data_ - 1;
		while (_CharRanges<_Char>::upperCase_.contains(*(++ch))) {
		}
		return ch - str.data_ == str.size_;
	}

	///////////////////////////////////////////////////////////////////////////////

	template<class _Char>
	bool streq(_string<_Char> const& s1, _string<_Char> const& s2)
	{
		if (s1.size_ != s2.size_)
			return false;
		size_t max = _string<_Char>::round_to_align_(s1.size_);
		_asm {
			mov ecx, dword ptr[s1]
			mov edx, dword ptr[s2]
			mov eax, dword ptr[ecx]
			mov ebx, dword ptr[edx]
			mov edx, 0

			_WHILE_BEGIN_:
			mov ecx, dword ptr[ebx + edx]
				cmp ecx, dword ptr[eax + edx]
				jne _RETURN_FALSE_
				add edx, 4
				cmp edx, dword ptr[max]
				je _RETURN_TRUE_
				jmp _WHILE_BEGIN_
		}
	_RETURN_TRUE_:
		return true;
	_RETURN_FALSE_:
		return false;
	}


	template<class _Char>
	int strcmp(_string<_Char> const& s1, _string<_Char> const& s2)
	{
		size_t max = _string<_Char>::round_to_align_(x::min(s1.size_, s2.size_));
		int result;
		_asm {
			mov ecx, dword ptr[s1]
			mov edx, dword ptr[s2]
			mov eax, dword ptr[ecx]
			mov ebx, dword ptr[edx]
			mov edx, 0

			_WHILE_BEGIN_:
			mov ecx, dword ptr[ebx + edx]
				sub ecx, dword ptr[eax + edx]
				jne _DIFF_RETURN_
				add edx, 4
				cmp edx, dword ptr[max]
				jge _STR_END_RETURN_
				jmp _WHILE_BEGIN_
		}
		return 0;

	_STR_END_RETURN_:
		return s1.size_ - s2.size_;

	_DIFF_RETURN_:
		_asm {
			mov dword ptr[result], ecx
		}
		return -result;
	}

	/*result<size_t> strfind(_string<char> const& src, _string<char> const& what)
	{
	if (!src.size_ || !what.size_)
	return result<size_t>::INVALID;

	size_t ch = 0;
	while (ch < src.size_) {
	while (src.data_[ch] != what.data_[0]) ++ch;
	size_t fpos = ch;
	while (ch - fpos < what.size_) {
	if (src.data_[ch] == what.data_[0]) {
	fpos = ch;
	++ch;
	continue;
	}
	if (src.data_[ch] != what.data_[ch - fpos])
	break;

	++ch;
	}
	if (ch - fpos == what.size_)
	return fpos;

	++ch;
	}
	return result<size_t>::INVALID;
	}*/

	//template<class _Char>
	/*result<size_t> strfind(_string<char> const& src, _string<char> const& what)
	{
	if (!src.size_ || !what.size_)
	return result<size_t>::INVALID;

	size_t ch = 0;
	size_t fpos = 0;
	while (ch < src.size_) {
	if (src.data_[ch] == what.data_[0]) {
	fpos = ch;
	}
	if (!what.data_[ch - fpos]) {
	return fpos;
	}
	if (src.data_[ch] != what.data_[ch - fpos]) {
	ch = fpos;
	}
	++ch;
	}
	return result<size_t>::INVALID;
	}*/



	inline result<size_t> strfind(_string<char> const& src, _string<char> const& what)
	{
		if (!src.size_ || !what.size_)
			return result<size_t>::INVALID;

		size_t ch = 0;
		size_t fpos = 0;

		while (ch < src.size_) {
			if (src.data_[ch] == what.data_[0]) {
				fpos = ch;

				while (
					(++ch - fpos) < what.size_ &&
					src.data_[ch] == what.data_[ch - fpos]) {
				}
				if (ch - fpos == what.size_)
					return fpos;
				else ch = fpos;
			}
			++ch;
		}
		return result<size_t>::INVALID;
	}

}//namespace x

#undef enable_if
#undef decay


#endif //_STRING_H_
