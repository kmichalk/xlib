#ifndef ANY_CAST_H
#define ANY_CAST_H

#include "typeenum.h"
#include "static_if.h"

#define enable_if std::enable_if_t
#define forceinline __forceinline

namespace x
{
	/// <summary>
	/// Simplifies removal of constness from pointer.
	/// </summary>
	/// <typeparam name="_Type"> - type of pointer</typeparam>
	/// <param name="ptr">pointer to remove constness</param>
	/// <returns>non-const pointer to the same object</returns>
	/// /// <example><code>int const* icp = new int{0};
	/// *x::rem_comst(icp) = 1;</code></example>
	template<typename _Type>
	__forceinline constexpr _Type* rem_const(_Type const * ptr) noexcept
	{
		return const_cast<_Type*>(ptr);
	}

	/*template<typename _Type>
	__forceinline constexpr _Type* rem_const(_Type * ptr) noexcept
	{
		return ptr;
	}*/

	/// <summary>
	/// Simplifies removal of constness from reference.
	/// </summary>
	/// <typeparam name="_Type"> - type of reference</typeparam>
	/// <param name="ref"> - reference to remove constness</param>
	/// <returns>non-const reference to the same object</returns>
	/// <example><code>int i = 0;
	/// int const& icr = i;
	/// x::rem_comst(icr) = 1;</code></example>
	template<typename _Type>
	__forceinline constexpr _Type& rem_const(_Type const & ref) noexcept
	{
		return const_cast<_Type&>(ref);
	}

	/*template<typename _Type>
	__forceinline constexpr _Type& rem_const(_Type & ref) noexcept
	{
		return ref;
	}*/

	/// <summary>
	/// Simplifies addition of constness to pointer.
	/// </summary>
	/// <typeparam name="_Type"> - type of pointer</typeparam>
	/// <param name="ptr">pointer to add constness</param>
	/// <returns>const pointer to the same object</returns>
	template<typename _Type>
	__forceinline constexpr _Type const* add_const(_Type * ptr) noexcept
	{
		return const_cast<std::remove_const_t<_Type> const*>(ptr);
	}

	/*template<typename _Type>
	__forceinline constexpr _Type const* add_const(_Type const * ptr) noexcept
	{
		return ptr;
	}*/

	/// <summary>
	/// Simplifies addition of constness to reference.
	/// </summary>
	/// <typeparam name="_Type"> - type of reference</typeparam>
	/// <param name="ptr">reference to add constness</param>
	/// <returns>const reference to the same object</returns>
	template<typename _Type>
	__forceinline constexpr _Type const& add_const(_Type & ref) noexcept
	{
		return const_cast<std::remove_const_t<_Type> const&>(ref);
	}

	/*template<typename _Type>
	__forceinline constexpr _Type const& add_const(_Type const & ref) noexcept
	{
		return ref;
	}*/

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

/// <summary>
/// Uses two static casts through void to erase type information 
/// and forces cast from pointer of any type to any another.
/// </summary>
/// <typeparam name="_To"> - desired type specified explicitly</typeparam>
/// <typeparam name="_From"> - source type to ba casted, deduced implicitly</typeparam>
/// <param name="ptr"> - pointer to be casted</param>
/// <returns>pointer to the same location</returns>
template<typename _To, typename _From>
forceinline constexpr _To force_cast(_From* ptr) noexcept
{
	return static_cast<_To>(static_cast<void*>(ptr));
}


/// <summary>
/// Uses two static casts through void to erase type information 
/// and forces cast from reference of any type to any another.
/// </summary>
/// <typeparam name="_To"> - desired type specified explicitly</typeparam>
/// <typeparam name="_From"> - source type to ba casted, deduced implicitly</typeparam>
/// <param name="ref"> - reference to be casted</param>
/// <returns>reference of the same location</returns>
template<typename _To, typename _From>
forceinline constexpr _To force_cast(_From& ref) noexcept
{
	return static_cast<_To>(*static_cast<void*>(&ref));
}

template<typename... A>
using _void_fn = void(*)(A...);

template<typename R, typename... A>
forceinline _void_fn<A...> void_cast(R(*fn)(A...)) 
{
	return force_cast<_void_fn<A...>>(fn);
}


/// <summary>
///Forces assignment to a const object.
/// </summary>
/// <typeparam name="_Type"> - type of both object and value to be assigned</typeparam>
/// <param name="obj"> - object to which val will be assigned</param>
/// <param name="val"> - value to be assigned</param>
/// <returns>non-const reference to obj</returns>
template<typename _Type>
__forceinline _Type& force_set(_Type const& obj, _Type const& val)
{
	return const_cast<_Type&>(obj) = val;
}

#undef enable_if
#undef forceinline

#endif