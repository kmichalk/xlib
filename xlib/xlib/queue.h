#ifndef _X_QUEUE_H_
#define _X_QUEUE_H_

namespace x
{
	template<class _Type>
	class queue
	{
		_Type* data_;
		size_t size_;
		size_t reserved_;
		size_t offset_;

	};
}
#endif //_X_QUEUE_H_
