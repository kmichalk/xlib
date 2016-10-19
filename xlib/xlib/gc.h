#ifndef _GARBAGE_COLLECTOR_
#define _GARBAGE_COLLECTOR_H

#include "more_type_traits.h"

namespace x
{
	template<typename _Type>
	class ptr;

	template<typename _Type>
	class _ObjData
	{
		_Type* object_;
		size_t size_;
		size_t references_;
		bool is_array_;

		inline void removeRef_()
		{
			if (--references_ == 0) {
				delete this;
			}
		}

		inline void addRef_()
		{
			++references_;
		}

		_ObjData& operator=(const _ObjData&) = delete;

		_ObjData(_Type* object, size_t size):
			object_{object}, size_{size}, references_{1}
		{
		}

		_ObjData(size_t size = 1):
			object_{new _Type[size]}, size_{size}, references_{1}
		{
		}

		_ObjData(const _ObjData&) = delete;

		~_ObjData()
		{
			delete[] object_;
		}

	public:
		friend class ptr<_Type>;
	};

	//template<typename _Type>
	//class ptr
	//{
	//private:
	//	_ObjData<_Type>* data_;

	//public:
	//	explicit ptr(size_t size = 1):
	//		data_{new _ObjData<_Type>{size}}
	//	{
	//	}

	//	template<typename... A>
	//	explicit ptr(A... args):
	//		data_{new _ObjData<_Type>{new _Type[1]{_Type{args...}}, 1}}
	//	{
	//	}

	//	explicit ptr(const ptr& other):
	//		data_{other.data_}
	//	{
	//		data_->addRef_();
	//	}

	//	explicit ptr(const _Type& value):
	//		data_{new _ObjData<_Type>{new _Type[1]{_Type{value}}, 1}}
	//	{
	//	}

	//	ptr& operator = (const ptr& other)
	//	{
	//		data_ = other.data_;
	//		data_->addRef_();
	//		return *this;
	//	}

	//	ptr& operator=(const _Type& value)
	//	{
	//		(*data_->object_) = value;
	//	}

	//	bool operator == (const ptr& other) const
	//	{
	//		return data_->object_ == other.data_->object_;
	//	}

	//	__forceinline _Type& operator*()
	//	{
	//		return *data_->object_;
	//	}

	//	template<_capture(_Type)>
	//	__forceinline enable_if<std::is_class<_Type>::value,
	//		_Type*> operator->()
	//	{
	//		return data_->object_;
	//	}

	//	~ptr()
	//	{
	//		data_->removeRef_();
	//	}
	//};

#define _ASSERT_VOID_DEREF \
	static_assert(\
		!IS_VOID_,\
		"Dereference of void pointer is forbidden.")

#define _DECR(_ptr) (-- *_ptr)
#define _INCR(_ptr) (++ *_ptr)

	template<typename _Type>
	class ptr
	{
	private:
		static constexpr bool IS_VOID_ = 
			std::is_same<_Type, void>::value;

		_Type*		data_;
		unsigned*	refCount_;

		inline explicit ptr(
			_Type* data,
			unsigned* refCount)
			:
			data_{data},
			refCount_{refCount}
		{
		}

	public:
		inline explicit ptr()
			:
			data_		{new _Type},
			refCount_	{new unsigned{0}}
		{
		}

		template<typename... _Args>
		__forceinline explicit ptr(_Args... args)
			:
			data_		{new _Type{args...}},
			refCount_	{new unsigned{0}}
		{
		}

		inline explicit ptr(ptr<_Type> const& other)
			:
			data_		{other.data_},
			refCount_	{&_INCR(other.refCount_)}
		{
		}

		inline explicit ptr(ptr<_Type>&& other)
			:
			data_		{other.data_},
			refCount_	{other.refCount_}
		{
		}

		ptr<_Type>& operator = (ptr<_Type> const& other)
		{
			if (!_DECR(refCount_)) {
				delete data_;
				delete refCount_;
			}
			data_		= other.data_;
			refCount_	= &_INCR(other.refCount_);
			return *this;
		}

		ptr<_Type>& operator = (ptr<_Type>&& other)
		{
			if (!_DECR(refCount_)) {
				delete data_;
				delete refCount_;
			}
			data_		= other.data_;
			refCount_	= other.refCount_;
		}

		inline bool operator == (ptr<_Type> const& other) const
		{
			return data_ == other.data_;
		}

		__forceinline _Type& operator*()
		{
			_ASSERT_VOID_DEREF;
			return *data_;
		}

		__forceinline _Type const& operator*() const
		{
			_ASSERT_VOID_DEREF;
			return *data_;
		}

		__forceinline _Type* operator->()
		{
			_ASSERT_VOID_DEREF;
			return data_->object_;
		}

		__forceinline _Type const* operator->() const
		{
			_ASSERT_VOID_DEREF;
			return data_->object_;
		}

		__forceinline operator bool () const
		{
			return bool(data_);
		}

		template<typename _Other>
		__forceinline operator _Other* ()
		{
			return static_cast<_Other*>(data_);
		}

		template<typename _Other>
		__forceinline operator _Other const* () const
		{
			return static_cast<_Other const*>(data_);
		}


		~ptr()
		{
			if (!_DECR(refCount_)) {
				delete data_;
				delete refCount_;
			}
		}
	};
}

#undef _ASSERT_VOID_DEREF
#undef _DECR
#undef _INCR

#endif //_GARBAGE_COLLECTOR_H
