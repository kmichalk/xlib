#ifndef AUTO_REF_H
#define AUTO_REF_H

namespace x
{
	template<typename T>
	__forceinline constexpr T& deref(T* var)
	{
		return *var;
	}

	template<typename T>
	__forceinline constexpr T& deref(T& var)
	{
		return var;
	}

	template<typename T>
	__forceinline constexpr auto deref(T&& var)
	{
		return std::forward<T>(var);
	}

	/*template<typename T>
	inline constexpr auto deref(T*&& var)
	{
		return *std::forward<T>(var);
	}*/

	template<typename T>
	__forceinline constexpr T* pointer(T& var)
	{
		return &var;
	}

	template<typename T>
	__forceinline constexpr T* pointer(T* var)
	{
		return var;
	}

	template<typename T>
	__forceinline constexpr auto pointer(T&& var)
	{
		return &std::forward<T>(var);
	}

	template<typename T>
	__forceinline constexpr auto pointer(T*&& var)
	{
		return std::forward<T>(var);
	}


	template<class...>
	class Member;

	template<class _Ret, class... _Args, class _Owner>
	struct Member<_Owner, _Ret(_Args...)>
	{
		_Ret(_Owner::*ptr)(_Args...);
	};

	template<class _Ret, class... _Args, class _Owner>
	__forceinline Member<_Owner, _Ret(_Args...)> wrapMember(_Ret(_Owner::*ptr)(_Args...))
	{
		return Member<_Owner, _Ret(_Args...)>{ptr};
	}


	template<class _To, class _Ret, class... _Args, class _Owner>
	__forceinline _To member_cast(_Ret(_Owner::*ptr)(_Args...))
	{
		return reinterpret_cast<_To&&>(Member<_Owner, _Ret(_Args...)>{ptr});
	}


	template<class _Owner, class _Ret, class... _Args>
	using member_t = _Ret(_Owner::*)(_Args...);


	template<class _To, class _From>
	__forceinline _To member_cast(_From* ptr)
	{
		return reinterpret_cast<Member<x::owner_of_t<_To>, x::function_type_of_t<_To>>*>(&ptr)->ptr;
	}


	/*template<class...>
	__forceinline auto member_caset();

	template<class _To, class _From, class _Ret, class... _Args, class _Owner>
	__forceinline member_t<_Owner, _Ret, _Args...> member_caset<_Ret(_Args...), _Owner, _To, _From>(_From* ptr)
	{
		return reinterpret_cast<Member<_Owner, _Ret(_Args...)>*>(ptr)->ptr;
	}*/


	//template<class _Type>
	//class _UniRef
	//{
	//	_Type* ptr_;
	//public:
	//	__forceinline _UniRef(_Type* ptr):
	//		ptr_{ptr}
	//	{
	//	}

	//	__forceinline _UniRef(_Type& ref) :
	//		ptr_{&ref}
	//	{
	//	}

	//	__forceinline operator _Type* ()
	//	{
	//		return ptr_;
	//	}

	//	__forceinline operator _Type& ()
	//	{
	//		return *ptr_;
	//	}

	//	__forceinline operator _Type const* ()
	//	{
	//		return ptr_;
	//	}

	//	__forceinline operator _Type const& ()
	//	{
	//		return *ptr_;
	//	}
	//};

	//template<class _Type>
	//class _UniRef<const _Type>
	//{
	//	const _Type* ptr_;
	//public:
	//	__forceinline _UniRef(const _Type* ptr):
	//		ptr_{ptr}
	//	{
	//	}

	//	__forceinline _UniRef(const _Type& ref) :
	//		ptr_{&ref}
	//	{
	//	}

	//	__forceinline operator _Type const* ()
	//	{
	//		return ptr_;
	//	}

	//	__forceinline operator _Type const& ()
	//	{
	//		return *ptr_;
	//	}
	//};

}

#endif //AUTO_REF_H
