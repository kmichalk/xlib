#ifndef _X_ALLOCATOR_H_
#define _X_ALLOCATOR_H_

#include "more_type_traits.h"
#include "simple.h"
#include "assign.h"
#include "wrap.h"


namespace x
{
	template<class _Type>
	class allocator
	{
		static constexpr byte NULL_DATA_[sizeof(_Type)] = {0};
	public:
		inline static void clear(_Type* ptr)
		{
			assign(ptr, reinterpret_cast<_Type*>(NULL_DATA_));
		}

		///////////////////////////////////////////////////////////////////////////////

		inline static _Type* raw_allocate(size_t size)
		{
			return reinterpret_cast<_Type*>(::operator new(sizeof(_Type)*size));
		}

		////////////////////////////////////////////////////////////////////////////

		inline static _Type* allocate(size_t size)
		{
			return new _Type[size];
		}

		////////////////////////////////////////////////////////////////////////////

		__forceinline static void deallocate(_Type* ptr)
		{
			::operator delete(ptr);
		}

		/*template<_capture(_Type)>
		__forceinline static std::enable_if_t<std::is_trivially_destructible<_Type>::value,
		void> deallocate(_Type* ptr, size_t size)
		{
		::operator delete[](ptr);
		}*/

		/*template<_capture(_Type)>
		__forceinline static std::enable_if_t< ! std::is_trivially_destructible<_Type>::value,*/
		static void deallocate(_Type* ptr, size_t size)
		{
			destruct(ptr, size);
			::operator delete[](reinterpret_cast<byte*>(ptr));
		}

		static void raw_deallocate(_Type* ptr, size_t size)
		{
			::operator delete[](reinterpret_cast<byte*>(ptr));
		}

		////////////////////////////////////////////////////////////////////////////

		static void reallocate(_Type*& ptr, size_t oldsize, size_t newsize)
		{
			if (oldsize == newsize) return;
			_Type* moved = raw_allocate(newsize);
			memcpy(moved, ptr, x::min(oldsize, newsize) * sizeof(_Type));
			::operator delete[](reinterpret_cast<byte*>(ptr));
			ptr = moved;
		}

		////////////////////////////////////////////////////////////////////////////

		template<_capture(_Type)>
		__forceinline static std::enable_if_t<std::is_trivially_copy_assignable<_Type>::value,
			void> construct(_Type* dest, _Type const& obj)
		{
			*dest = obj;
		}

		template<_capture(_Type)>
		__forceinline static std::enable_if_t< !std::is_trivially_copy_assignable<_Type>::value,
			void> construct(_Type* dest, _Type const& obj)
		{
			new (dest) _Type{obj};
		}

		template<_capture(_Type)>
		__forceinline static std::enable_if_t<std::is_trivially_copy_assignable<_Type>::value,
			void> construct(_Type* dest, _Type&& obj)
		{
			*dest = (_Type&&)obj;
		}

		template<_capture(_Type)>
		__forceinline static std::enable_if_t< !std::is_trivially_copy_assignable<_Type>::value,
			void> construct(_Type* dest, _Type&& obj)
		{
			new (dest) _Type{(_Type&&)obj};
		}

		///////////////////////////////////////////////////////////////////////////////

		template<_capture(_Type), class... _Arg>
		__forceinline static std::enable_if_t<std::is_trivially_copy_assignable<_Type>::value,
			void> inline_construct(_Type* dest, _Arg&&... args)
		{
			*dest = _Type{args...};
		}

		template<_capture(_Type), class... _Arg>
		__forceinline static std::enable_if_t< !std::is_trivially_copy_assignable<_Type>::value,
			void> inline_construct(_Type* dest, _Arg&&... args)
		{
			new (dest) _Type{args...};
		}


		/*template<_capture(_Type)>
		__forceinline static std::enable_if_t<std::is_trivially_copy_assignable<_Type>::value,
		void> construct(wrap::ptr<_Type> dest, wrap::ptr<_Type const> obj)
		{
		*dest = *obj;
		}

		template<_capture(_Type)>
		__forceinline static std::enable_if_t< !std::is_trivially_copy_assignable<_Type>::value,
		void> construct(wrap::ptr<_Type> dest, wrap::ptr<_Type const> obj)
		{
		new (dest) _Type{*obj};
		}

		template<_capture(_Type)>
		__forceinline static std::enable_if_t<std::is_trivially_copy_assignable<_Type>::value,
		void> construct(wrap::ptr<_Type> dest, _Type&& obj)
		{
		*dest = (_Type&&)obj;
		}

		template<_capture(_Type)>
		__forceinline static std::enable_if_t< !std::is_trivially_copy_assignable<_Type>::value,
		void> construct(wrap::ptr<_Type> dest, _Type&& obj)
		{
		new (dest) _Type{(_Type&&)obj};
		}*/

		////////////////////////////////////////////////////////////////////////////

		static void reconstruct(_Type* dest, _Type const& obj)
		{
			destruct(dest);
			construct(dest, obj);
		}

		static void reconstruct(_Type* dest, _Type&& obj)
		{
			destruct(dest);
			construct(dest, (_Type&&)obj);
		}

		/*static void reconstruct(_Type& dest, wrap::ptr<_Type const> obj)
		{
		destruct(dest);
		construct(dest, *obj);
		}

		static void reconstruct(_Type& dest, _Type&& obj)
		{
		destruct(dest);
		construct(dest, (_Type&&)obj);
		}*/

		////////////////////////////////////////////////////////////////////////////

		template<_capture(_Type)>
		static std::enable_if_t<std::is_trivially_copy_assignable<_Type>::value,
			void> copy(_Type* dest, _Type const* src, size_t size)
		{
			memcpy(dest, src, sizeof(_Type)*size);
		}

		template<_capture(_Type)>
		static std::enable_if_t< !std::is_trivially_copy_assignable<_Type>::value,
			void> copy(_Type* dest, _Type const* src, size_t size)
		{
			while (size--) {
				destruct(dest + size);
				construct(dest + size, src[size]);
			}
		}

		////////////////////////////////////////////////////////////////////////////

		template<_capture(_Type)>
		static std::enable_if_t<std::is_trivially_copy_assignable<_Type>::value,
			void> raw_copy(_Type* dest, _Type const* src, size_t size)
		{
			//std::cout << "raw trivial\n";
			memcpy(dest, src, sizeof(_Type)*size);
		}

		template<_capture(_Type)>
		static std::enable_if_t< !std::is_trivially_copy_assignable<_Type>::value,
			void> raw_copy(_Type* dest, _Type const* src, size_t size)
		{
			//std::cout << "raw non-trivial\n";
			while (size--) {
				construct(dest + size, src[size]);
			}
		}

		////////////////////////////////////////////////////////////////////////////

		template<_capture(_Type)>
		static std::enable_if_t<std::is_trivially_copy_assignable<_Type>::value,
			void> swap(_Type& l, _Type& r)
		{
			_Type temp = l;
			l = r;
			r = temp;
		}

		template<_capture(_Type)>
		static std::enable_if_t< !std::is_trivially_copy_assignable<_Type>::value,
			void> swap(_Type& l, _Type& r)
		{
			static thread_local byte temp[sizeof(_Type)];
			assign(reinterpret_cast<_Type*>(temp), &l);
			assign(&l, &r);
			assign(&r, reinterpret_cast<_Type*>(temp));
		}

		////////////////////////////////////////////////////////////////////////////

		template<_capture(_Type)>
		__forceinline static std::enable_if_t< !std::is_trivially_destructible<_Type>::value,
			void> destruct(_Type& obj)
		{
			obj.~_Type();
		}

		template<_capture(_Type)>
		__forceinline static std::enable_if_t<std::is_trivially_destructible<_Type>::value,
			void> destruct(_Type& obj)
		{
		}

		template<_capture(_Type)>
		__forceinline static std::enable_if_t< !std::is_trivially_destructible<_Type>::value,
			void> destruct(_Type* ptr)
		{
			ptr->~_Type();
		}

		template<_capture(_Type)>
		__forceinline static std::enable_if_t<std::is_trivially_destructible<_Type>::value,
			void> destruct(_Type* ptr)
		{
		}

		template<_capture(_Type)>
		__forceinline static std::enable_if_t< !std::is_trivially_destructible<_Type>::value,
			void> destruct(_Type* ptr, size_t size)
		{
			while (size--) ptr[size].~_Type();
		}

		template<_capture(_Type)>
		__forceinline static std::enable_if_t<std::is_trivially_destructible<_Type>::value,
			void> destruct(_Type* ptr, size_t size)
		{
		}
	};
}

#endif //_X_ALLOCATOR_H_
