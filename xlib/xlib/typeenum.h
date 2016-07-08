#ifndef TYPE_ENUM_H
#define TYPE_ENUM_H

#include "more_type_traits.h"
#include "vamacros.h"

#define enable_if std::enable_if_t

namespace x {

template<typename T>
class _UniqueNum;

class _TypeEnum
{
public:
	static unsigned short last_;
	const unsigned short num_;

	_TypeEnum() = delete;

public:
	explicit _TypeEnum(unsigned short num) : num_{num} {}

	__forceinline unsigned short num() const
	{
		return num_;
	}
#pragma region friends
	template<typename>
	friend class _UniqueNum;

	template<typename T>
	friend __forceinline enable_if<std::is_base_of<x::_TypeEnum, T>::value,
		unsigned short> declnum(T&);
	template<typename T>
	friend __forceinline enable_if<!std::is_base_of<x::_TypeEnum, T>::value,
		unsigned short> declnum(T&);
	template<typename T>
	friend __forceinline enable_if<std::is_base_of<x::_TypeEnum, T>::value,
		unsigned short> declnum(T*);
	template<typename T>
	friend __forceinline enable_if<!std::is_base_of<x::_TypeEnum, T>::value,
		unsigned short> declnum(T*);
#pragma endregion
};

template<typename T>
class _UniqueNum final
{
	static const unsigned short typenum_;
	static bool used_;

	_UniqueNum() = delete;
	_UniqueNum(_UniqueNum<T> const&) = delete;
	_UniqueNum<T>& operator=(_UniqueNum<T> const&) = delete;

public:
	static unsigned short num()
	{
		return typenum_;
	}
};

unsigned short _TypeEnum::last_ = 0;

template<typename T>
const unsigned short _UniqueNum<T>::typenum_ = ++_TypeEnum::last_;

template<typename T>
bool _UniqueNum<T>::used_ = false;

template<typename T>
using UniqueNum = _UniqueNum<T>;

}

#define typenum(...) CAT(__typenum_, NARGS(0, __VA_ARGS__))(0, __VA_ARGS__)
#define __typenum_2(__empty, T) x::_UniqueNum<T>::num()
#define __typenum_1(__empty) _TypeEnum{__typenum_2(__empty,std::remove_pointer_t<decltype(this)>)}

namespace x {
template<typename T>
__forceinline enable_if<std::is_base_of<x::_TypeEnum, T>::value,
	unsigned short> declnum(T& obj)
{
	return obj.num();
}

template<typename T>
__forceinline enable_if<!std::is_base_of<x::_TypeEnum, T>::value,
	unsigned short> declnum(T& obj)
{
	return _UniqueNum<T>::num();
}

template<typename T>
__forceinline enable_if<std::is_base_of<x::_TypeEnum, T>::value,
	unsigned short> declnum(T* obj)
{
	return obj->num();
}

template<typename T>
__forceinline enable_if<!std::is_base_of<x::_TypeEnum, T>::value,
	unsigned short> declnum(T* obj)
{
	return _UniqueNum<T>::num();
}

template<typename T, typename Y>
enable_if<std::is_base_of<x::_TypeEnum, Y>::value,
	T> typenum_cast(Y* ptr)
{
	return ptr->num()==typenum(std::remove_pointer_t<T>) ?
		static_cast<T>(ptr) :
		nullptr;
}

template<typename T, typename Y>
enable_if<std::is_base_of<x::_TypeEnum, Y>::value,
	T> typenum_cast(Y& ptr)
{
	if (ptr.num()==typenum(std::remove_reference_t<T>))
		return static_cast<T>(ptr);
	else
		throw(std::bad_cast());
}

template<typename T, typename Y>
enable_if<std::is_base_of<x::_TypeEnum, Y>::value,
	T> typenum_cast(Y const* ptr)
{
	return ptr->num()==typenum(std::remove_const_t<std::remove_pointer_t<T>>) ?
		static_cast<T>(ptr) :
		nullptr;
}

template<typename T, typename Y>
enable_if<std::is_base_of<x::_TypeEnum, Y>::value,
	T> typenum_cast(Y const& ptr)
{
	if (ptr.num()==typenum(std::remove_const_t<std::remove_reference_t<T>>))
		return static_cast<T>(ptr);
	else
		throw(std::bad_cast());
}

template<typename T>
struct _is_enumed
{
	static constexpr bool value = std::is_base_of<x::_TypeEnum, T>::value;
};

}

#define enumed virtual public x::_TypeEnum
#define typenum_cast x::typenum_cast
#define declnum x::declnum

//#define typenum(T) x::_UniqueNum<T>::num()
//#define set_typenum _TypeEnum{typenum(std::remove_reference_t<decltype(*this)>)}
//#define set_typenum(T) _TypeEnum{x::_UniqueNum<T>::num()}

#undef enable_if

#endif //TYPE_ENUM_H