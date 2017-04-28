#ifndef _RESULT_H_
#define _RESULT_H_

#include <iostream>
#include "more_type_traits.h"
#include "_void.h"
#include "static_if.h"
#include <iostream>


namespace x
{
	template<class _Type>
	class _Result
	{
		/*static_assert(std::is_fundamental<_Type>::value,
			"Result value wrapper is only available with fundamental types, pointers, references or void.");*/


		struct InvalidFlag
		{
		};

		bool const valid_;
		_Type value_;

	public:
		static const _Result<_Type> INVALID;

		inline _Result():
			valid_{true},
			value_{}
		{
		}

		inline _Result(_Type const& value_):
			valid_{true},
			value_{value_}
		{
		}

		inline constexpr _Result(InvalidFlag):
			valid_{false},
			value_{}
		{
		}

		inline _Result(_Type&& value_):
			valid_{true},
			value_{std::forward<_Type>(value_)}
		{
		}

		inline _Result<_Type>& operator=(_Type const& value)
		{
			//std::cout << "operator=\n";
			value_ = value;
			return *this;
		}

		__forceinline explicit operator bool() const
		{
			//std::cout<< "bool const\n";
			return valid_;
		}

		__forceinline explicit operator bool()
		{
			//std::cout << "bool\n";
			return valid_;
		}

		__forceinline bool operator!() const
		{
			return !valid_;
		}

		template<class _Type = _Type>
		std::enable_if_t<std::is_class<_Type>::value,
			_Type*> operator->()
		{
			return &value_;
		}

		template<class _Type = _Type>
		std::enable_if_t<std::is_class<_Type>::value,
			_Type const*> operator->() const
		{
			return &value_;
		}

		/*_Type& operator*()
		{
			return value_;
		}

		_Type const& operator*() const
		{
			return value_;
		}*/

		operator _Type&()
		{
			//std::cout<< "_Type&\n";
			return value_;
		}

		operator _Type const&() const
		{
			//std::cout<< "_Type const&\n";
			return value_;
		}

		__forceinline bool valid() const
		{
			return valid_;
		}


		friend std::ostream& operator<<(std::ostream& os, _Result<_Type> const& res)
		{
			if (!res.valid_) return os << "invalid";
			return os << res.value_;
		}
	};

	///////////////////////////////////////////////////////////////////////////////

	template<class _Type>
	class _Result<_Type&>
	{
		struct InvalidFlag
		{
		};

		bool const valid_;
		_Type* const value_;

	public:
		static const _Result<_Type&> INVALID;

		inline _Result(_Type& value_):
			valid_{true},
			value_{&value_}
		{
		}

		inline _Result(InvalidFlag):
			valid_{false},
			value_{nullptr}
		{
		}

		inline _Result(_Type&& value_):
			valid_{true},
			value_{&value_}
		{
		}
		
		__forceinline explicit operator bool() const
		{
			//std::cout << "bool const\n";
			return valid_;
		}

		__forceinline explicit operator bool()
		{
			//std::cout << "bool\n";
			return valid_;
		}

		operator _Result<_Type>() const
		{
			if (valid_) return _Result<_Type>{*value_};
			else return _Result<_Type>::INVALID;
		}

		__forceinline bool operator!() const
		{
			return !valid_;
		}


		/*template<class _Type = _Type>
		std::enable_if_t<std::is_class<_Type>::value,*/
			_Type* operator->()
		{
			return value_;
		}

		template<class _Type = _Type>
		std::enable_if_t<std::is_class<_Type>::value,
			_Type const*> operator->() const
		{
			return value_;
		}

		operator _Type&()
		{
			//std::cout << "_Type&\n";
			return *value_;
		}

		operator _Type const&() const
		{
			//std::cout << "_Type const&\n";
			return *value_;
		}

		/*_Type& operator*()
		{
			return *value_;
		}

		_Type const& operator*() const
		{
			return *value_;
		}*/

		__forceinline bool valid() const
		{
			return valid_;
		}


		friend std::ostream& operator<<(std::ostream& os, _Result<_Type&> const& res)
		{
			if (!res.valid_) return os << "invalid";
			return os << *res.value_;
		}
	};

	///////////////////////////////////////////////////////////////////////////////

	template<class _Type>
	class _Result<_Type*&>
	{
		struct InvalidFlag
		{};

		bool const valid_;
		_Type** const value_;

	public:
		static const _Result<_Type*&> INVALID;

		inline _Result(_Type*& value_):
			valid_{true},
			value_{&value_}
		{
		}

		inline _Result(InvalidFlag):
			valid_{false},
			value_{nullptr}
		{
		}


		__forceinline explicit operator bool() const
		{
			//std::cout << "bool const\n";
			return valid_;
		}

		__forceinline explicit operator bool()
		{
			//std::cout << "bool\n";
			return valid_;
		}

		operator _Result<_Type*>() const
		{
			if (valid_) return _Result<_Type*>{*value_};
			else return _Result<_Type*>::INVALID;
		}

		__forceinline bool operator!() const
		{
			return !valid_;
		}


		template<class _Type = _Type>
		std::enable_if_t<std::is_class<_Type>::value,
		_Type*> operator->()
		{
			return *value_;
		}

		template<class _Type = _Type>
		std::enable_if_t<std::is_class<_Type>::value,
			_Type const*> operator->() const
		{
			return *value_;
		}

		operator _Type&()
		{
			//std::cout << "_Type&\n";
			return *value_;
		}

		operator _Type* const&() const
		{
			//std::cout << "_Type const&\n";
			return *value_;
		}

		/*_Type& operator*()
		{
		return *value_;
		}

		_Type const& operator*() const
		{
		return *value_;
		}*/

		__forceinline bool valid() const
		{
			return valid_;
		}


		friend std::ostream& operator<<(std::ostream& os, _Result<_Type*&> const& res)
		{
			if (!res.valid_) return os << "invalid";
			return os << *res.value_;
		}
	};

	///////////////////////////////////////////////////////////////////////////////
	
	template<class _Type>
	class _Result<_Type*>
	{
		struct InvalidFlag
		{
		};

		bool const valid_;
		_Type* value_;

	public:
		static const _Result<_Type*> INVALID;

		inline _Result(_Type* value_):
			valid_{true},
			value_{value_}
		{
		}

		inline _Result(InvalidFlag):
			valid_{false},
			value_{nullptr}
		{
		}

		__forceinline explicit operator bool()
		{
			//std::cout << "bool\n";
			return valid_;
		}

		__forceinline explicit operator bool() const
		{
			//std::cout << "bool const\n";
			return valid_;
		}

		__forceinline bool operator!() const
		{
			return !valid_;
		}


		template<class _Type = _Type>
		std::enable_if_t<std::is_class<_Type>::value,
			_Type*> operator->()
		{
			return value_;
		}

		template<class _Type = _Type>
		std::enable_if_t<std::is_class<_Type>::value,
			_Type const*> operator->() const
		{
			return value_;
		}

		__forceinline operator _Type*&()
		{
			//std::cout << "_Type*&\n";
			return value_;
		}

		__forceinline operator _Type const*() const
		{
			//std::cout << "_Type const*\n";
			return value_;
		}

		/*__forceinline _Type*& operator*()
		{
			return value_;
		}

		__forceinline _Type const* operator*() const
		{
			return value_;
		}*/

		__forceinline bool valid() const
		{
			return valid_;
		}

		friend std::ostream& operator<<(std::ostream& os, _Result<_Type*> const& res)
		{
			if (!res.valid_) return os << "invalid";
			return os << res.value_;
		}
	};

	///////////////////////////////////////////////////////////////////////////////

	template<class _Void>
	class _Result<_void<_Void>>
	{
		struct InvalidFlag
		{
		};

	public:
		static const _Result<_void<_Void>> INVALID;		

		bool const valid_;

		inline constexpr _Result():
			valid_{true}
		{
		}

		inline constexpr _Result(InvalidFlag) :
			valid_{false}
		{
		}

		__forceinline operator bool() const
		{
			return valid_;
		}

		__forceinline bool valid() const
		{
			return valid_;
		}
	};

	template<>
	class _Result<void>
	{
		struct InvalidFlag
		{
		};

		bool const valid_;

	public:
		inline constexpr _Result():
			valid_{true}
		{
		}

		inline constexpr _Result(InvalidFlag) :
			valid_{false}
		{
		}

		__forceinline operator bool() const
		{
			return valid_;
		}

		__forceinline bool valid() const
		{
			return valid_;
		}
	};

	//template<class _Type>
	//constexpr _Result<_Type> INVALID = {_Result<_Type>::InvalidFlag{}};

	/*template<class _Type>
	_Type const* const _Result<_Type>::INVALID_ELEM_ = nullptr;
	*/
	template<class _Type>
	const _Result<_Type> _Result<_Type>::INVALID = {_Result<_Type>::InvalidFlag{}};

	template<class _Void>
	const _Result<_void<_Void>> _Result<_void<_Void>>::INVALID = {_Result<_void<_Void>>::InvalidFlag{}};

	template<class _Type>
	const _Result<_Type&> _Result<_Type&>::INVALID = {_Result<_Type&>::InvalidFlag{}};

	template<class _Type>
	const _Result<_Type*> _Result<_Type*>::INVALID = {_Result<_Type*>::InvalidFlag{}};

	template<class _Type>
	const _Result<_Type*&> _Result<_Type*&>::INVALID = {_Result<_Type*&>::InvalidFlag{}};

	//const _Result<void> _Result<void>::INVALID = {_Result<void>::InvalidFlag{}};

	template<class _Type>
	using result = _Result<std::conditional_t<std::is_same<_Type, void>::value, _void<>, _Type>>;
}

#endif //_RESULT_H_


//#ifndef _RESULT_H_
//#define _RESULT_H_
//
//#include <iostream>
//#include "more_type_traits.h"
//#include "_void.h"
//#include "static_if.h"
//
//
//namespace x
//{
//	template<class _Type>
//	class _Result
//	{
//		struct InvalidFlag
//		{};
//
//		bool const valid_;
//		_Type value_;
//
//	public:
//		static const _Result<_Type> INVALID;
//
//		inline _Result():
//			valid_{true},
//			value_{}
//		{
//		}
//
//		inline _Result(_Type const& value_):
//			valid_{true},
//			value_{value_}
//		{
//		}
//
//		inline constexpr _Result(InvalidFlag):
//			valid_{false},
//			value_{}
//		{
//		}
//
//		inline _Result(_Type&& value_):
//			valid_{true},
//			value_{std::forward<_Type>(value_)}
//		{
//		}
//
//		__forceinline operator bool()
//		{
//			return valid_;
//		}
//
//		__forceinline bool operator!() const
//		{
//			return !valid_;
//		}
//
//		template<class _Type = _Type>
//		std::enable_if_t<std::is_class<_Type>::value,
//			_Type*> operator->()
//		{
//			return &value_;
//		}
//
//		template<class _Type = _Type>
//		std::enable_if_t<std::is_class<_Type>::value,
//			_Type const*> operator->() const
//		{
//			return &value_;
//		}
//
//		__forceinline operator _Type&()
//		{
//			return value_;
//		}
//
//		__forceinline operator() const
//		{
//			return value_;
//		}
//
//		__forceinline bool valid() const
//		{
//			return valid_;
//		}
//
//
//		friend std::ostream& operator<<(std::ostream& os, _Result<_Type> const& res)
//		{
//			if (!res.valid_) return os << "invalid";
//			return os << res.value_;
//		}
//	};
//
//	///////////////////////////////////////////////////////////////////////////////
//
//	template<class _Type>
//	class _Result<_Type&>
//	{
//		struct InvalidFlag
//		{};
//
//		bool const valid_;
//		_Type* const value_;
//
//	public:
//		static const _Result<_Type&> INVALID;
//
//		inline _Result(_Type& value_):
//			valid_{true},
//			value_{&value_}
//		{
//		}
//
//		inline _Result(InvalidFlag):
//			valid_{false},
//			value_{nullptr}
//		{
//		}
//
//		inline _Result(_Type&& value_):
//			valid_{true},
//			value_{&value_}
//		{
//		}
//
//		__forceinline operator bool() const
//		{
//			return valid_;
//		}
//
//		operator _Result<_Type>() const
//		{
//			if (valid_) return _Result<_Type>{*value_};
//			else return _Result<_Type>::INVALID;
//		}
//
//		__forceinline bool operator!() const
//		{
//			return !valid_;
//		}
//
//
//		template<class _Type = _Type>
//		std::enable_if_t<std::is_class<_Type>::value,
//			_Type*> operator->()
//		{
//			return value_;
//		}
//
//		template<class _Type = _Type>
//		std::enable_if_t<std::is_class<_Type>::value,
//			_Type const*> operator->() const
//		{
//			return value_;
//		}
//
//		_Type& operator*()
//		{
//			return *value_;
//		}
//
//		_Type const& operator*() const
//		{
//			return *value_;
//		}
//
//		__forceinline bool valid() const
//		{
//			return valid_;
//		}
//
//
//		friend std::ostream& operator<<(std::ostream& os, _Result<_Type&> const& res)
//		{
//			if (!res.valid_) return os << "invalid";
//			return os << *res.value_;
//		}
//	};
//
//	///////////////////////////////////////////////////////////////////////////////
//
//	template<class _Type>
//	class _Result<_Type*>
//	{
//		struct InvalidFlag
//		{};
//
//		bool const valid_;
//		_Type* value_;
//
//	public:
//		static const _Result<_Type*> INVALID;
//
//		inline _Result(_Type* value_):
//			valid_{true},
//			value_{value_}
//		{
//		}
//
//		inline _Result(InvalidFlag):
//			valid_{false},
//			value_{nullptr}
//		{
//		}
//
//		__forceinline operator bool() const
//		{
//			return valid_;
//		}
//
//		__forceinline bool operator!() const
//		{
//			return !valid_;
//		}
//
//
//		template<class _Type = _Type>
//		std::enable_if_t<std::is_class<_Type>::value,
//			_Type*> operator->()
//		{
//			return value_;
//		}
//
//		template<class _Type = _Type>
//		std::enable_if_t<std::is_class<_Type>::value,
//			_Type const*> operator->() const
//		{
//			return value_;
//		}
//
//		__forceinline operator _Type*& ()
//		{
//			return value_;
//		}
//
//		__forceinline operator _Type const*() const
//		{
//			return value_;
//		}
//
//		__forceinline bool valid() const
//		{
//			return valid_;
//		}
//
//		friend std::ostream& operator<<(std::ostream& os, _Result<_Type*> const& res)
//		{
//			if (!res.valid_) return os << "invalid";
//			return os << res.value_;
//		}
//	};
//
//	///////////////////////////////////////////////////////////////////////////////
//
//	template<class _VoidType>
//	class _Result<_void<_VoidType>>
//	{
//		struct InvalidFlag
//		{};
//
//	public:
//		static const _Result<_void<_VoidType>> INVALID;
//
//		bool const valid_;
//
//		inline constexpr _Result():
//			valid_{true}
//		{
//		}
//
//		inline constexpr _Result(InvalidFlag) :
//			valid_{false}
//		{
//		}
//
//		__forceinline operator bool() const
//		{
//			return valid_;
//		}
//
//		__forceinline bool valid() const
//		{
//			return valid_;
//		}
//	};
//
//	template<>
//	class _Result<void>
//	{
//		struct InvalidFlag
//		{};
//
//		bool const valid_;
//
//	public:
//		inline constexpr _Result():
//			valid_{true}
//		{
//		}
//
//		inline constexpr _Result(InvalidFlag) :
//			valid_{false}
//		{
//		}
//
//		__forceinline operator bool() const
//		{
//			return valid_;
//		}
//
//		__forceinline bool valid() const
//		{
//			return valid_;
//		}
//	};
//
//	//template<class _Type>
//	//constexpr _Result<_Type> INVALID = {_Result<_Type>::InvalidFlag{}};
//
//	/*template<class _Type>
//	_Type const* const _Result<_Type>::INVALID_ELEM_ = nullptr;
//	*/
//	template<class _Type>
//	const _Result<_Type> _Result<_Type>::INVALID = {_Result<_Type>::InvalidFlag{}};
//
//	template<class _Void>
//	const _Result<_void<_Void>> _Result<_void<_Void>>::INVALID = {_Result<_void<_Void>>::InvalidFlag{}};
//
//	template<class _Type>
//	const _Result<_Type&> _Result<_Type&>::INVALID = {_Result<_Type&>::InvalidFlag{}};
//
//	template<class _Type>
//	const _Result<_Type*> _Result<_Type*>::INVALID = {_Result<_Type*>::InvalidFlag{}};
//
//
//	//const _Result<void> _Result<void>::INVALID = {_Result<void>::InvalidFlag{}};
//
//	template<class _Type>
//	using result = _Result<std::conditional_t<std::is_same<_Type, void>::value, _void<>, _Type>>;
//}
//
//#endif //_RESULT_H_
