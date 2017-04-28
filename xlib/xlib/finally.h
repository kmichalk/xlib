#ifndef _X_FINALLY_H_
#define _X_FINALLY_H_

#include "fn.h"
#include "static_if.h"

namespace x
{
	class Finally
	{
		Fn<void()> action_;
	public:
		inline Finally()
		{
		}

		Finally(Finally const&) = delete;
		Finally(Finally&&) = delete;

		template<class _Func>
		inline void operator=(_Func&& action)
		{
			action_ = std::forward<_Func>(action);
		}

		inline ~Finally()
		{
			action_();
		}
	};
}

#define finally x::Finally __finally__;\
	__finally__ = [& __if_exists(this){, this}]()

#endif //_X_FINALLY_H_
