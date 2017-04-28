#ifndef _X_SLEEP_H_
#define _X_SLEEP_H_

#include <thread>
#include "timer.h"


inline void sleep(unsigned long msec)
{
	std::this_thread::sleep_for(std::chrono::milliseconds{msec});
}

inline void sleep(double msec)
{
	std::this_thread::sleep_for(std::chrono::milliseconds{(unsigned long)msec});
}

template<class _Func>
inline void sleep_while(_Func&& condition)
{
	while (condition()) {
	}
}

template<class _Func>
inline void sleep_while(_Func&& condition, unsigned checkPeriod)
{
	while (condition()) {
		sleep(checkPeriod);
	}
}

template<class _Func>
inline void sleep_while(_Func&& condition, double secLimit)
{
	x::timer<> timer;
	timer.tic();
	while (condition() && timer.toc() < secLimit) {
	}
}

template<class _Func>
inline void sleep_while(_Func&& condition, double secLimit, unsigned checkPeriod)
{
	x::timer<> timer;
	timer.tic();
	while (condition() && timer.toc() < secLimit) {
		sleep(checkPeriod);
	}
}

#endif //_X_SLEEP_H_
