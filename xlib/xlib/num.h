#ifndef NUM_H
#define NUM_H

#include <string>
#include <limits>

template<typename T>
class Num final
{
	T val_;
public:
	Num():
		val_{0}
	{}
	Num(T val):
		val_{val}
	{}
	template<typename Y>
	Num(Num<Y> const& other) :
		val_{other.val_}
	{}
	Num(std::string const& str) try: 
		val_{(T)stod(str)}
	{}
	catch (...) {
		val_ = 0;
	}

	inline Num& operator=(Num const& other)
	{
		val_ = other.val_;
	}
	inline bool operator==(Num const& other) const
	{
		return val_==other.val_;
	}

	inline operator bool() const
	{
		return static_cast<bool>(val_);
	}

	inline Num& operator++()
	{
		++val_;
		return *this;
	}
	inline Num operator++(int)
	{
		return Num{val_++};
	}
	inline Num& operator--()
	{
		--val_;
		return *this;
	}
	inline Num operator--(int)
	{
		return Num{val_--};
	}
	inline Num operator+(Num const& other) const
	{
		return Num{val_+other.val_};
	}
	inline Num operator-(Num const& other) const
	{
		return Num{val_-other.val_};
	}
	inline Num operator*(Num const& other) const
	{
		return Num{val_*other.val_};
	}
	inline Num operator/(Num const& other) const
	{
		return Num{val_/other.val_};
	}
	inline Num operator%(Num const& other) const
	{
		return Num{val_%other.val_};
	}
	inline Num operator^(Num const& other) const
	{
		return Num{pow(val_,other.val_)};
	}

	inline Num& operator+=(Num const& other)
	{
		val_ += other.val_;
		return *this;
	}
	inline Num& operator-=(Num const& other)
	{
		val_ -= other.val_;
		return *this;
	}
	inline Num& operator*=(Num const& other)
	{
		val_ *= other.val_;
		return *this;
	}
	inline Num& operator/=(Num const& other)
	{
		val_ /= other.val_;
		return *this;
	}
	inline Num& operator%=(Num const& other)
	{
		val_ %= other.val_;
		return *this;
	}
	inline Num& operator^=(Num const& other) const
	{
		val_ = pow(val_, other.val_);
		return *this;
	}

	inline operator std::string() const
	{
		return std::to_string(val_);
	}
	inline std::string str() const
	{
		return std::to_string(val_);
	}

	static inline T min() const
	{
		return std::numeric_limits<T>::lowest();
	}
	static inline T max() const
	{
		return std::numeric_limits<T>::max();
	}

};


using Byte = Num<char>;
using Short = Num<short>;
using Int = Num<int>;
using Real = Num<double>;
using Float = Num<float>;
using UInt = Num<unsigned int>;
using UShort = Num<unsigned short>;
using UChar = Num<unsigned char>;
using ULong = Num<unsigned long>;
using Long = Num<long>;

#endif //NUM_H
