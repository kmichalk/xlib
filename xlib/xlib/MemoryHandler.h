//#ifndef _X_MEMORY_HANDLER_H_
//#define _X_MEMORY_HANDLER_H_
//
//#include "threaded.h"
//
//
//class MemoryHandler
//{
//	byte* data_;
//	size_t size_;
//	size_t last_;
//	x::vector<size_t> free_;
//public:
//	MemoryHandler();
//
//	template<class _Type>
//	_Type* construct() throw(x::error<MemoryHandler>)
//	{
//		if (last_ + sizeof(_Type) >= size_) {
//			if (!free_.empty())
//				return new (data_ + free_.pop_back()) _Type;
//			else
//				throw new x::error<MemoryHandler>{};
//		}
//		else {
//			last_ += sizeof(_Type);
//			return new (data_ + last_ - sizeof(_Type)) _Type;
//		}
//	}
//
//	template<class _Type>
//	void destruct(_Type* object) throw(x::error<MemoryHandler>)
//	{
//		object->~_Type();
//		free_.push_back((byte*)object - data_);
//	}
//};
//
//#endif //_X_MEMORY_HANDLER_H_
