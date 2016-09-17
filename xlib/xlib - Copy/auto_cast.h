#ifndef ANY_CAST_H
#define ANY_CAST_H

#include "typeenum.h"

#define enable_if std::enable_if_t
#define forceinline __forceinline

template<typename T, typename Y>
forceinline enable_if<std::is_same<std::remove_pointer_t<T>,Y>::value,
	T> auto_cast(Y* ptr)
{
	std::cout<<"same\n";
	return ptr;
}

template<typename T, typename Y>
forceinline enable_if<std::is_base_of<std::remove_pointer_t<T>, Y>::value &&
	!x::_is_enumed<Y>::value,
	T> auto_cast(Y* ptr)
{
	std::cout<<"staic (base)\n";
	return static_cast<T>(ptr);
}

template<typename T, typename Y>
forceinline enable_if<std::is_base_of<Y, std::remove_pointer_t<T>>::value &&
	x::_is_enumed<Y>::value,
	T> auto_cast(Y* ptr)
{
	std::cout<<"enumed\n";
	return typenum_cast<T>(ptr);
}

template<typename T, typename Y>
forceinline enable_if<std::is_base_of<Y, std::remove_pointer_t<T>>::value,
	T> auto_cast(Y* ptr)
{
	std::cout<<"dynamic\n";
	return dynamic_cast<T>(ptr);
}

//template<typename T, typename Y>
//forceinline enable_if<!std::is_base_of<Y, std::remove_pointer_t<T>>::value &&
//	!std::is_base_of<std::remove_pointer_t<T>,Y>::value &&
//	!std::is_same<T,Y>::value &&
//	sizeof(std::remove_pointer_t<T>)==sizeof(Y),
//	T> auto_cast(Y* ptr)
//{
//	std::cout<<"try reinterpret\n";
//	return reinterpret_cast<T>(ptr);
//}

template<typename T, typename Y>
forceinline enable_if<!std::is_base_of<Y, std::remove_pointer_t<T>>::value &&
	!std::is_base_of<std::remove_pointer_t<T>, Y>::value &&
	!std::is_same<T, Y>::value &&
	sizeof(std::remove_pointer_t<T>)!=sizeof(Y),
	T> auto_cast(Y* ptr)
{
	std::cout<<"failed\n";
	return nullptr;
}

template<typename T, typename Y>
forceinline T force_cast(Y* ptr)
{
	return static_cast<T>(static_cast<void*>(ptr));
}

template<typename... A>
using _void_fn = void(*)(A...);

template<typename R, typename... A>
forceinline _void_fn<A...> void_cast(R(*fn)(A...)) 
{
	return force_cast<_void_fn<A...>>(fn);
}

#undef enable_if
#undef forceinline

#endif