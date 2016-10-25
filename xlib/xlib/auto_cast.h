#ifndef ANY_CAST_H
#define ANY_CAST_H

#include "typeenum.h"
#include "static_if.h"

#define enable_if std::enable_if_t
#define forceinline __forceinline

namespace x
{
	template<typename _Type>
	__forceinline constexpr _Type* rem_const(_Type const * ptr)
	{
		return const_cast<_Type*>(ptr);
	}

	template<typename _Type>
	__forceinline constexpr _Type* rem_const(_Type * ptr)
	{
		return ptr;
	}


	template<typename _Type>
	__forceinline constexpr _Type& rem_const(_Type const & ref)
	{
		return const_cast<_Type&>(ref);
	}

	template<typename _Type>
	__forceinline constexpr _Type& rem_const(_Type & ref)
	{
		return ref;
	}


	template<typename _Type>
	__forceinline constexpr _Type* add_const(_Type * ptr)
	{
		return const_cast<_Type const*>(ptr);
	}

	template<typename _Type>
	__forceinline constexpr _Type* add_const(_Type const * ptr)
	{
		return ptr;
	}


	template<typename _Type>
	__forceinline constexpr _Type& add_const(_Type & ref)
	{
		return const_cast<_Type const&>(ref);
	}

	template<typename _Type>
	__forceinline constexpr _Type& add_const(_Type const & ref)
	{
		return ref;
	}

}
//template<typename _To, typename _From, template<typename> class _Container>
//__forceinline _Container<_To>& container_cast(_Container<_From*>& container)
//{
//	static_assert(std::is_base_of<std::remove_pointer_t<_To>, _From>::value,
//		"Only certain upcasts are allowed using container_cast.");
//
//	return (_Container<_To>&)container;
//}
//
//template<typename _To, typename _From, size_t _nDim>
//__forceinline x::vector<_To>& container_cast(x::vector<_From*, _nDim>& container)
//{
//	static_assert(std::is_base_of<std::remove_pointer_t<_To>, _From>::value,
//		"Only certain upcasts are allowed using container_cast.");
//
//	return (x::vector<_To, _nDim> &)container;
//}
//template<typename T, typename Y>
//forceinline enable_if<std::is_same<std::remove_pointer_t<T>,Y>::value,
//	T> auto_cast(Y* ptr)
//{
//	std::cout<<"same\n";
//	return ptr;
//}
//
//template<typename T, typename Y>
//forceinline enable_if<std::is_base_of<std::remove_pointer_t<T>, Y>::value &&
//	!x::_is_enumed<Y>::value,
//	T> auto_cast(Y* ptr)
//{
//	std::cout<<"staic (base)\n";
//	return static_cast<T>(ptr);
//}
//
//template<typename T, typename Y>
//forceinline enable_if<std::is_base_of<Y, std::remove_pointer_t<T>>::value &&
//	x::_is_enumed<Y>::value,
//	T> auto_cast(Y* ptr)
//{
//	std::cout<<"enumed\n";
//	return typenum_cast<T>(ptr);
//}
//
//template<typename T, typename Y>
//forceinline enable_if<std::is_base_of<Y, std::remove_pointer_t<T>>::value,
//	T> auto_cast(Y* ptr)
//{
//	std::cout<<"dynamic\n";
//	return dynamic_cast<T>(ptr);
//}
//
////template<typename T, typename Y>
////forceinline enable_if<!std::is_base_of<Y, std::remove_pointer_t<T>>::value &&
////	!std::is_base_of<std::remove_pointer_t<T>,Y>::value &&
////	!std::is_same<T,Y>::value &&
////	sizeof(std::remove_pointer_t<T>)==sizeof(Y),
////	T> auto_cast(Y* ptr)
////{
////	std::cout<<"try reinterpret\n";
////	return reinterpret_cast<T>(ptr);
////}
//
//template<typename T, typename Y>
//forceinline enable_if<!std::is_base_of<Y, std::remove_pointer_t<T>>::value &&
//	!std::is_base_of<std::remove_pointer_t<T>, Y>::value &&
//	!std::is_same<T, Y>::value &&
//	sizeof(std::remove_pointer_t<T>)!=sizeof(Y),
//	T> auto_cast(Y* ptr)
//{
//	std::cout<<"failed\n";
//	return nullptr;
//}
//
//template<typename T, typename Y>
//forceinline constexpr 
//T auto_cast(Y* ptr)
//{
//	static_if((std::is_same<std::remove_pointer_t<T>, Y>::value)) {
//		std::cout<<"same\n";
//		return ptr;
//	}
//	static_if((std::is_base_of<std::remove_pointer_t<T>, Y>::value)) {
//		std::cout<<"staic (base)\n";
//		return static_cast<T>(ptr);
//	}
//	static_if((std::is_polymorphic<std::remove_pointer_t<T>>::value && std::is_polymorphic<Y>::value)) {
//		std::cout<<"dynamic\n";
//		return dynamic_cast<T>(ptr);
//	}
//	return nullptr;
//}
//
//template<typename T, typename Y>
//forceinline enable_if<std::is_base_of<std::remove_pointer_t<T>, Y>::value &&
//	!x::_is_enumed<Y>::value,
//	T> auto_cast(Y* ptr)
//{
//	std::cout<<"staic (base)\n";
//	return static_cast<T>(ptr);
//}
//
//template<typename T, typename Y>
//forceinline enable_if<std::is_base_of<Y, std::remove_pointer_t<T>>::value &&
//	x::_is_enumed<Y>::value,
//	T> auto_cast(Y* ptr)
//{
//	std::cout<<"enumed\n";
//	return typenum_cast<T>(ptr);
//}
//
//template<typename T, typename Y>
//forceinline enable_if<std::is_base_of<Y, std::remove_pointer_t<T>>::value,
//	T> auto_cast(Y* ptr)
//{
//	std::cout<<"dynamic\n";
//	return dynamic_cast<T>(ptr);
//}
//
////template<typename T, typename Y>
////forceinline enable_if<!std::is_base_of<Y, std::remove_pointer_t<T>>::value &&
////	!std::is_base_of<std::remove_pointer_t<T>,Y>::value &&
////	!std::is_same<T,Y>::value &&
////	sizeof(std::remove_pointer_t<T>)==sizeof(Y),
////	T> auto_cast(Y* ptr)
////{
////	std::cout<<"try reinterpret\n";
////	return reinterpret_cast<T>(ptr);
////}
//
//template<typename T, typename Y>
//forceinline enable_if<!std::is_base_of<Y, std::remove_pointer_t<T>>::value &&
//	!std::is_base_of<std::remove_pointer_t<T>, Y>::value &&
//	!std::is_same<T, Y>::value &&
//	sizeof(std::remove_pointer_t<T>)!=sizeof(Y),
//	T> auto_cast(Y* ptr)
//{
//	std::cout<<"failed\n";
//	return nullptr;
//}

template<typename T, typename Y>
forceinline constexpr T force_cast(Y* ptr)
{
	return static_cast<T>(static_cast<void*>(ptr));
}

template<typename T, typename Y>
forceinline constexpr T force_cast(Y& ref)
{
	return static_cast<T>(*static_cast<void*>(&ref));
}

template<typename... A>
using _void_fn = void(*)(A...);

template<typename R, typename... A>
forceinline _void_fn<A...> void_cast(R(*fn)(A...)) 
{
	return force_cast<_void_fn<A...>>(fn);
}

template<typename _Type>
__forceinline _Type force_set(_Type const& var, _Type&& val)
{
	return const_cast<_Type&>(var) = std::forward<_Type>(val);
}

#undef enable_if
#undef forceinline

#endif