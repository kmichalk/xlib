#ifndef DYNAMIC_PTR_H
#define DYNAMIC_PTR_H

#include "more_type_traits.h"
#include "warning.h"

#define enable_if std::enable_if_t
#define _suspend template<typename = _Type>

//namespace x
//{
//	template<typename> class dynamic_ptr;
//}
//
//template<typename _NewType, typename ..._Args>
//__forceinline x::dynamic_ptr<_NewType>(::xnew)(_Args ...args)
//{
//	return x::dynamic_ptr<_NewType>{new _NewType{args...}};
//}

namespace x
{
	template<typename _Type>
	class ref
	{
		_Type* object_;

	public:
		inline ref(ref<_Type> const&) = default;

		inline ref(ref<_Type>&&) = default;

		inline ref(_Type& object):
			object_{&object}
		{
		}

		inline operator _Type&()
		{
			return *object_;
		}

		inline ~ref() = default;
	};
	
	template<typename _Type>
	using cref = ref<std::add_const_t<_Type>>;

	template<typename _Type>
	class dynamic_ptr final
	{
		struct ForcePtrCtorFlag {};

		_Type* ptr_;

		

		dynamic_ptr<_Type>& operator=(dynamic_ptr<_Type> const&) = delete;

		template<typename _FriendType>
		friend class dynamic_ptr;

		__forceinline dynamic_ptr(ForcePtrCtorFlag, _Type* ptr) noexcept:
		ptr_{ptr}
		{
			//std::cout<< "ptr\n";
		}

	public:	
		dynamic_ptr(dynamic_ptr<_Type> const& other):
			ptr_{other.ptr_}
		{
		}

		_suspend static inline dynamic_ptr<_Type> force_nullptr()
		{
#pragma message (WARNING("Forcing dynamic_ptr to contain nullptr is unsafe and this function generally should not be used."))
			return dynamic_ptr<_Type>{ForcePtrCtorFlag{}, nullptr};
		}

		_suspend static inline dynamic_ptr<_Type> force_dynamic(_Type* ptr)
		{
#pragma message (WARNING("Forcing an existing pointer to be converted into dynamic_ptr object is unsafe and this function generally should not be used."))
			return dynamic_ptr<_Type>{ForcePtrCtorFlag{}, ptr};
		}

		template<_capture<_Type>, _concept<std::is_default_constructible<_Type>::value>>
		__forceinline dynamic_ptr():
			ptr_{new _Type{}}
		{
			//std::cout<< "def\n";
		}

		//template<typename _OtherType, 
		//	_concept<!std::is_same<_Type, _OtherType>::value>>
		//__forceinline dynamic_ptr(dynamic_ptr<_OtherType> const& other) noexcept:
		//	ptr_{static_cast<_Type*>(other.ptr_)}
		//{
		//	//std::cout<< "other\n";
		//}

		template<typename... _Args,
			_concept<!x::is_any_of<ForcePtrCtorFlag, _Args...>::value>>
		__forceinline dynamic_ptr(_Args... args):
			ptr_{new _Type{args...}}
		{
			static_assert(
				!x::is_any_of<x::dynamic_ptr<_Type>, _Args...>::value,
				"Copying of dynamic_ptr is forbidden.");

			//std::cout<< "template\n";
		}

		//template<typename... _Args/*,
		//	_concept<!x::is_any_of<_Type,_Args...>::value>,
		//	_concept<!x::is_any_of<x::dynamic_ptr<_Type>, _Args...>::value>*/>
		//__forceinline dynamic_ptr(_Args... args):
		//	ptr_{new _Type{args...}}
		//{
		//	static_assert(
		//		!x::is_any_of<x::dynamic_ptr<_Type>, _Args...>::value, 
		//		"Copying of dynamic_ptr is forbidden.");
		//		
		//	//std::cout<< "template\n";
		//}

		__forceinline dynamic_ptr(dynamic_ptr<_Type>&& other) noexcept:
			ptr_{std::move(other.ptr_)}
		{
			//std::cout<< "move\n";
		}


		__forceinline operator _Type*() noexcept
		{
			return ptr_;
		}

		__forceinline constexpr operator bool() const noexcept
		{
			return true;
		}

		template<typename _ThisType = _Type>
		__forceinline enable_if<!std::is_same<_ThisType, void>::value,
			_Type*> operator->() noexcept
		{
			return ptr_;
		}

		template<typename _ThisType = _Type>
		__forceinline enable_if<!std::is_same<_ThisType, void>::value,
			_Type&> operator*() noexcept
		{
			return *ptr_;
		}

		~dynamic_ptr()
		{
			//std::cout<< "destr\n";
		}
	};

	template<typename _Type>
	using New = dynamic_ptr<_Type>;
}

#undef enable_if
#undef _suspend

#endif //DYNAMIC_PTR_H

//#ifndef DYNAMIC_PTR_H
//#define DYNAMIC_PTR_H
//
//#include "more_type_traits.h"
//
//#define enable_if std::enable_if_t
//
////namespace x
////{
////	template<typename> class dynamic_ptr;
////}
////
////template<typename _NewType, typename ..._Args>
////__forceinline x::dynamic_ptr<_NewType>(::xnew)(_Args ...args)
////{
////	return x::dynamic_ptr<_NewType>{new _NewType{args...}};
////}
//
//namespace x
//{
//	template<typename> class New;
//
//	template<typename _Type>
//	class dynamic_ptr final
//	{
//		_Type* ptr_;
//
//		__forceinline dynamic_ptr(_Type* ptr) noexcept:
//			ptr_{ptr}
//		{
//		}
//
//	public:
//		template<typename _FriendType>
//		friend class dynamic_ptr;
//
//		dynamic_ptr() = delete;
//
//		dynamic_ptr(dynamic_ptr<_Type> const&) = delete;
//
//		dynamic_ptr<_Type>& operator=(dynamic_ptr<_Type> const&) = delete;
//
//		template<typename _OtherType, 
//			typename _ThisType = _Type, 
//			typename = enable_if<!std::is_same<_ThisType, _OtherType>::value>>
//		__forceinline dynamic_ptr(dynamic_ptr<_OtherType>& other) noexcept:
//			ptr_{static_cast<_Type*>(other.ptr_)}
//		{
//		}
//
//
//		__forceinline dynamic_ptr(dynamic_ptr<_Type>&& other) noexcept:
//			ptr_{std::move(other.ptr_)}
//		{
//		}
//
//
//		__forceinline operator _Type*() noexcept
//		{
//			return ptr_;
//		}
//
//		__forceinline constexpr operator bool() const noexcept
//		{
//			return true;
//		}
//
//		template<typename _ThisType = _Type>
//		__forceinline enable_if<!std::is_same<_ThisType,void>::value,
//			_Type>& operator*() noexcept
//		{
//			return *ptr_;
//		}
//
//		~dynamic_ptr() noexcept
//		{
//		}
//
//		friend class x::New<_Type>;
//
//		/*template<typename _NewType, typename... _Args>
//		friend x::dynamic_ptr<_NewType> (::xnew)(_Args... args);*/
//	};
//
//	template<typename _Type>
//	class New final
//	{
//		x::dynamic_ptr<_Type> ptr_;
//
//	public:
//		New(New<_Type> const&) = delete;
//
//		New(New<_Type>&&) = delete;
//
//		template<typename... _Args>
//		__forceinline New(_Args&... args):
//			ptr_{new _Type{args...}}
//		{
//		}
//
//		template<typename _Conv>
//		__forceinline operator x::dynamic_ptr<_Conv>&& () noexcept
//		{
//			return std::move(static_cast<x::dynamic_ptr<_Conv>&&>(std::move(ptr_)));
//		}
//
//		~New() noexcept
//		{
//		}
//	};
//}
//
//#define xnew x::New
//
//#undef enable_if
//
//#endif //DYNAMIC_PTR_H
