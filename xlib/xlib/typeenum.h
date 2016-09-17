//#ifndef TYPE_ENUM_H
//#define TYPE_ENUM_H
//
//#include "more_type_traits.h"
//#include "vamacros.h"
//
//#define enable_if std::enable_if_t
//
//namespace x
//{
//	using num_t = unsigned short;
//
//	template<typename, int>
//	class _UniqueNum;
//
//	class _TypeEnum
//	{
//	public:
//		template<int group = 0>
//		static __forceinline num_t last()
//		{
//			static num_t last_ = 0;
//			return ++last_;
//		}
//		const num_t num;
//
//		_TypeEnum() = delete;
//
//	public:
//		explicit _TypeEnum(num_t num): num{num} {}
//
//
//#pragma region UNIQUENUM_FRIENDS
//		template<typename, int>
//		friend class _UniqueNum;
//
//		template<typename T>
//		friend __forceinline enable_if<std::is_base_of<x::_TypeEnum, T>::value,
//			num_t> declnum(T&);
//		template<typename T>
//		friend __forceinline enable_if<!std::is_base_of<x::_TypeEnum, T>::value,
//			num_t> declnum(T&);
//		template<typename T>
//		friend __forceinline enable_if<std::is_base_of<x::_TypeEnum, T>::value,
//			num_t> declnum(T*);
//		template<typename T>
//		friend __forceinline enable_if<!std::is_base_of<x::_TypeEnum, T>::value,
//			num_t> declnum(T*);
//#pragma endregion
//	};
//
//	template<typename T, int group = 0>
//	class _UniqueNum final
//	{
//		static const num_t typenum_;
//		//static bool nUses;
//
//		_UniqueNum() = delete;
//		_UniqueNum(_UniqueNum<T,group> const&) = delete;
//		_UniqueNum<T,group>& operator=(_UniqueNum<T,group> const&) = delete;
//
//	public:
//		static num_t num()
//		{
//			return typenum_;
//		}
//	};
//
//	//num_t _TypeEnum::last_ = 0;
//
//	template<typename T, int group>
//	const num_t _UniqueNum<T,group>::typenum_ = _TypeEnum::last<group>();
//
//	/*template<typename T>
//	bool _UniqueNum<T>::nUses = false;*/
//
//	template<typename T>
//	using UniqueNum = _UniqueNum<T>;
//
//}
//
//#define typenum(...) CAT(__typenum_, NARGS(0, __VA_ARGS__))(__VA_ARGS__)
//#define __typenum_2(T) x::_UniqueNum<T>::num()
//#define __typenum_1() _TypeEnum{__typenum_2(std::remove_pointer_t<decltype(this)>)}
//
//namespace x
//{
//	template<typename T>
//	__forceinline enable_if<std::is_base_of<x::_TypeEnum, T>::value,
//		num_t> declnum(T& obj)
//	{
//		return obj.num;
//	}
//
//	template<typename T>
//	__forceinline enable_if<!std::is_base_of<x::_TypeEnum, T>::value,
//		num_t> declnum(T& obj)
//	{
//		return _UniqueNum<T>::num();
//	}
//
//	template<typename T>
//	__forceinline enable_if<std::is_base_of<x::_TypeEnum, T>::value,
//		num_t> declnum(T* obj)
//	{
//		return obj->num;
//	}
//
//	template<typename T>
//	__forceinline enable_if<!std::is_base_of<x::_TypeEnum, T>::value,
//		num_t> declnum(T* obj)
//	{
//		return _UniqueNum<T>::num();
//	}
//
//	template<typename T, typename Y>
//	inline enable_if<std::is_base_of<x::_TypeEnum, Y>::value,
//		T> typenum_cast(Y* ptr)
//	{
//		return ptr->num==typenum(std::remove_pointer_t<T>) ?
//			static_cast<T>(ptr) :
//			nullptr;
//	}
//
//	template<typename T, typename Y>
//	inline enable_if<std::is_base_of<x::_TypeEnum, Y>::value,
//		T> typenum_cast(Y& ptr)
//	{
//		if (ptr.num==typenum(std::remove_reference_t<T>))
//			return static_cast<T>(ptr);
//		else
//			throw(std::bad_cast());
//	}
//
//	template<typename T, typename Y>
//	inline enable_if<std::is_base_of<x::_TypeEnum, Y>::value,
//		T> typenum_cast(Y const* ptr)
//	{
//		return ptr->num==typenum(std::remove_const_t<std::remove_pointer_t<T>>) ?
//			static_cast<T>(ptr) :
//			nullptr;
//	}
//
//	template<typename T, typename Y>
//	inline enable_if<std::is_base_of<x::_TypeEnum, Y>::value,
//		T> typenum_cast(Y const& ptr)
//	{
//		if (ptr.num==typenum(std::remove_const_t<std::remove_reference_t<T>>))
//			return static_cast<T>(ptr);
//		else
//			throw(std::bad_cast());
//	}
//
//	template<typename T>
//	struct _is_enumed
//	{
//		static constexpr bool value = std::is_base_of<x::_TypeEnum, T>::value;
//	};
//
//}
//
//#define enumed virtual public x::_TypeEnum
//#define typenum_cast x::typenum_cast
//#define declnum x::declnum
//
////#define typenum(T) x::_UniqueNum<T>::num()
////#define set_typenum _TypeEnum{typenum(std::remove_reference_t<decltype(*this)>)}
////#define set_typenum(T) _TypeEnum{x::_UniqueNum<T>::num()}
//
//#undef enable_if
//
//#endif //TYPE_ENUM_H

#ifndef TYPE_ENUM_H
#define TYPE_ENUM_H

#include "more_type_traits.h"
#include "vamacros.h"

#define enable_if std::enable_if_t

namespace x
{
	using num_t = unsigned short;

	template<typename T>
	class _UniqueNum;

	class _TypeEnum
	{
	public:
		static __forceinline num_t last() 
		{
			static num_t last_ = 0;
			return ++last_;
		}
		const num_t num;

		_TypeEnum() = delete;

	public:
		explicit _TypeEnum(num_t num): num{num} {}

		/*__forceinline num_t num() const
		{
			return num_;
		}*/
#pragma region friends
		template<typename>
		friend class _UniqueNum;

		template<typename T>
		friend __forceinline enable_if<std::is_base_of<x::_TypeEnum, T>::value,
			num_t> declnum(T&);
		template<typename T>
		friend __forceinline enable_if<!std::is_base_of<x::_TypeEnum, T>::value,
			num_t> declnum(T&);
		template<typename T>
		friend __forceinline enable_if<std::is_base_of<x::_TypeEnum, T>::value,
			num_t> declnum(T*);
		template<typename T>
		friend __forceinline enable_if<!std::is_base_of<x::_TypeEnum, T>::value,
			num_t> declnum(T*);
#pragma endregion
	};

	template<typename T>
	class _UniqueNum final
	{
		static const num_t typenum_;
		//static bool nUses;

		_UniqueNum() = delete;
		_UniqueNum(_UniqueNum<T> const&) = delete;
		_UniqueNum<T>& operator=(_UniqueNum<T> const&) = delete;

	public:
		static num_t num()
		{
			return typenum_;
		}
	};

	//num_t _TypeEnum::last_ = 0;

	template<typename T>
	const num_t _UniqueNum<T>::typenum_ = _TypeEnum::last();

	/*template<typename T>
	bool _UniqueNum<T>::nUses = false;*/

	template<typename T>
	using UniqueNum = _UniqueNum<T>;

}

#define typenum(...) CAT(__typenum_, NARGS(0, __VA_ARGS__))(__VA_ARGS__)
#define __typenum_2(T) x::_UniqueNum<T>::num()
#define __typenum_1() x::_TypeEnum{__typenum_2(std::remove_pointer_t<decltype(this)>)}

namespace x
{
	template<typename T>
	__forceinline enable_if<std::is_base_of<x::_TypeEnum, T>::value,
		num_t> declnum(T& obj)
	{
		return obj.num;
	}

	template<typename T>
	__forceinline enable_if<!std::is_base_of<x::_TypeEnum, T>::value,
		num_t> declnum(T& obj)
	{
		return _UniqueNum<T>::num();
	}

	template<typename T>
	__forceinline enable_if<std::is_base_of<x::_TypeEnum, T>::value,
		num_t> declnum(T* obj)
	{
		return obj->num;
	}

	template<typename T>
	__forceinline enable_if<!std::is_base_of<x::_TypeEnum, T>::value,
		num_t> declnum(T* obj)
	{
		return _UniqueNum<T>::num();
	}

	template<typename T, typename Y>
	inline enable_if<std::is_base_of<x::_TypeEnum, Y>::value,
		T> typenum_cast(Y* ptr)
	{
		return ptr->num==typenum(std::remove_pointer_t<T>) ?
			static_cast<T>(ptr) :
			nullptr;
	}

	template<typename T, typename Y>
	inline enable_if<std::is_base_of<x::_TypeEnum, Y>::value,
		T> typenum_cast(Y& ptr)
	{
		if (ptr.num==typenum(std::remove_reference_t<T>))
			return static_cast<T>(ptr);
		else
			throw(std::bad_cast());
	}

	template<typename T, typename Y>
	inline enable_if<std::is_base_of<x::_TypeEnum, Y>::value,
		T> typenum_cast(Y const* ptr)
	{
		return ptr->num==typenum(std::remove_const_t<std::remove_pointer_t<T>>) ?
			static_cast<T>(ptr) :
			nullptr;
	}

	template<typename T, typename Y>
	inline enable_if<std::is_base_of<x::_TypeEnum, Y>::value,
		T> typenum_cast(Y const& ptr)
	{
		if (ptr.num==typenum(std::remove_const_t<std::remove_reference_t<T>>))
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