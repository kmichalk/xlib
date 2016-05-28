#pragma once

#define enable_if std::enable_if_t

template<typename... T>
class va_handler
{
	static_assert(all_same<T...>::value,
		"Arguments have to be of the same type.");
private:
	using type_ = typename select<1, T...>::type;
	type_* args_;
	size_t size_;
public:
	__forceinline size_t size() const
	{
		return size_;
	}
	va_handler(T... args) :
		size_{sizeof...(args)}
	{
		args_ = new type_[size_]{args...};
	}
	__forceinline type_& operator[](size_t i)
	{
		return args_[i];
	}
	type_& get(size_t i)
	{
		if (i<size_) {
			return args_[i];
		}
		throw "out of size";
	}
	~va_handler()
	{
		delete[] args_;
	}
};
