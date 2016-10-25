#ifndef DEBUG_H
#define DEBUG_H

#include "timer.h"

#define static_error(_message) static_assert(false, _message)

namespace x::debug
{
	template<typename...>
	struct _FuncGroup;

	template<typename _Ret, typename... _ArgTypes>
	struct _FuncGroup<_Ret(_ArgTypes...)>
	{
		template<_Ret(&_func)(_ArgTypes...)>
		struct _CallHelper
		{
			static unsigned _timesCalled;

			template<unsigned _nTimes>
			__forceinline static _Ret _callMaxTimes(_ArgTypes&&... _args)
			{
				if (++_timesCalled > _nTimes)
					throw 0;
				return _func(_args...);
			}

			__forceinline static _Ret _callCount(_ArgTypes&&... _args)
			{
				++_timesCalled;
				return _func(_args...);
			}

		};
	};

	template<typename _Ret, typename... _ArgTypes>
	template<_Ret(&_func)(_ArgTypes...)>
	unsigned _FuncGroup<_Ret(_ArgTypes...)>::_CallHelper<_func>::_timesCalled = 0;


	template<class _TimeUnit = std::chrono::microseconds>
	class _Measure
	{
		x::timer<_TimeUnit> timer_;

	public:
		static _Measure<_TimeUnit> performer;

		template<typename _Lambda>
		auto operator->*(_Lambda&& procedure)
		{
			timer_.tic();
			procedure();
			return timer_.toc();
		}
	};

	template<class _TimeUnit>
	_Measure<_TimeUnit> _Measure<_TimeUnit>::performer = {};
}
#define measure x::debug::_Measure<>::performer ->* [&]()
#define call_times(_nTimes,_func) _FuncGroup<decltype(_func)>::_CallHelper<_func>::_callMaxTimes<_nTimes>
#define call_once(_func) call_times(1, _func)
#define count_call(_func) _FuncGroup<decltype(_func)>::_CallHelper<_func, _nTimes>::_callMaxTimes

#endif // DEBUG_H
