//#pragma once
#ifndef TIMER_H_
#define TIMER_H_
#include <chrono>
#include "more_type_traits.h"

namespace x {

template<typename Unit>
class static_timer_
{
	static Unit from_;
	static constexpr long double ratio_ =
		double(Unit::duration::period::num)/
		Unit::duration::period::den;

	template<typename Unit>
	friend void tic();

	template<typename Unit>
	friend long double toc();
};

template<typename Unit>
Unit static_timer_<Unit>::from_ = Unit(0);

template<typename Unit=std::chrono::microseconds>
void tic()
{
	static_timer_<Unit>::from_ = std::chrono::duration_cast<Unit>(
		std::chrono::system_clock::now().time_since_epoch());
}

template<typename Unit = std::chrono::microseconds>
long double toc()
{
	return long double((std::chrono::duration_cast<Unit>(
		std::chrono::system_clock::now().time_since_epoch())-
		static_timer_<Unit>::from_).count())*
		static_timer_<Unit>::ratio_;
}

template<typename Unit = std::chrono::microseconds>
class timer
{
	Unit from_;
	Unit to_;
	long long unsigned int time_;
	long double ratio_;

public:
	static_assert(
		is_any_of<Unit,
		std::chrono::hours,
		std::chrono::minutes,
		std::chrono::seconds,
		std::chrono::milliseconds,
		std::chrono::microseconds,
		std::chrono::nanoseconds>::value,
		"Unit type for timer must be one of std::chrono time unit types.");

	timer() :
		time_(0),
		ratio_(double(Unit::duration::period::num)/
			Unit::duration::period::den)
	{
	}

	timer(long double dispRatio) :
		time_(0),
		ratio_(dispRatio)
	{
	}

	timer(const timer<Unit>&) = delete;
	timer& operator=(const timer<Unit>&) = delete;

	static unsigned long long int epoch()
	{
		return std::chrono::duration_cast<Unit>(
			std::chrono::system_clock::now().time_since_epoch()).count();
	}

	void tic()
	{
		time_ = 0;
		from_ = std::chrono::duration_cast<Unit>(
			std::chrono::system_clock::now().time_since_epoch());
	}

	long double toc()
	{
		to_ = std::chrono::duration_cast<Unit>(
			std::chrono::system_clock::now().time_since_epoch());
		time_ =
			to_.count() -
			from_.count();
		return (*this)();
	}

	long double measure()
	{
		toc();
		tic();
		return (*this)();
	}

	void sleep(long double seconds)
	{
		tic();
		while (toc()() < seconds);
	}

	inline long double operator()() const
	{
		return long double(time_)*ratio_;
	}

	friend std::ostream& operator<<(std::ostream &os, const timer<Unit> &t)
	{
		return os<<t();
	}

	~timer()
	{}

};
//
//template<typename T>
//std::ostream &operator<<(std::ostream &os, const timer<T> &t)


};
#endif //TIMER_H_