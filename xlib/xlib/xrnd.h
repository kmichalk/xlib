//#pragma once
#ifndef XRND_H
#define XRND_H
#include <random>
#include <cmath>
#include "Timer.h"
#include "more_type_traits.h"

#define enable_if typename std::enable_if_t
#define forceinline __forceinline
//#define decay typename std::decay_t
namespace x {

#define _CurrentEngine _RandomEngine<T,Distribution,Engine>

template<
	typename T = int,
	typename Distribution = std::normal_distribution<>,
	typename Engine = std::mt19937>
class _RandomEngine
{
	template<typename Y = T>
	static forceinline constexpr enable_if<std::is_integral<Y>::value,
		Y> dT_() { return 1; }
	template<typename Y = T>
	static forceinline constexpr enable_if<std::is_floating_point<Y>::value,
		Y> dT_() { return 0.0; }

	static long long int rand_helper_;
	static Engine engine_;
	static Distribution dist_;
	static bool always_seed_;

	template<
		typename T,
		typename Distribution,
		typename Engine>
		friend T random(T, T);

	template<
		typename T,
		typename Distribution,
		typename Engine>
		friend T random();

	template<
		typename T,
		typename Distribution,
		typename Engine>
		friend inline void seed();

	template<
		typename T,
		typename Distribution,
		typename Engine>
		friend inline void always_seed(bool);

	template<
		typename T,
		typename Distribution,
		typename Engine>
		friend inline bool decide(double);
};

template<
	typename T = int,
	typename Distribution = std::normal_distribution<>,
	typename Engine = std::mt19937>
	T random(T r1, T r2)
{
	if (_CurrentEngine::always_seed_)
		seed<T, Distribution, Engine>();
	return T((long double)_CurrentEngine::engine_() / (long double)_CurrentEngine::engine_.max() *
		(r2 - r1 + _CurrentEngine::dT_())) + r1;

	/*return T(fmod(long double(_CurrentEngine::engine_()),
		long double(r2 - r1 + _CurrentEngine::dT_()))) + r1;*/
}

template<
	typename T = int,
	typename Distribution = std::normal_distribution<>,
	typename Engine = std::mt19937>
	T random()
{
	if (_CurrentEngine::always_seed_)
		seed<T, Distribution, Engine>();
	return T(_CurrentEngine::engine_());
}

template<
	typename T = int,
	typename Distribution = std::normal_distribution<>,
	typename Engine = std::mt19937>
	void seed()
{
	_CurrentEngine::engine_.seed(
		timer<std::chrono::microseconds>::epoch()+
		++_CurrentEngine::rand_helper_);
}

template<
	typename T = int,
	typename Distribution = std::normal_distribution<>,
	typename Engine = std::mt19937>
	void always_seed(bool value)
{
	_CurrentEngine::always_seed_ = value;
}

template<
	typename T = double,
	typename Distribution = std::normal_distribution<>,
	typename Engine = std::mt19937>
	bool decide(double probability = 0.5)
{
	return random(0.0, 1.0) < probability;
}

template<typename T, typename Distribution, typename Engine>
long long int _CurrentEngine::rand_helper_ =
	x::timer<std::chrono::seconds>::epoch();

template<typename T, typename Distribution, typename Engine>
Engine _CurrentEngine::engine_ = Engine();

template<typename T, typename Distribution, typename Engine>
Distribution _CurrentEngine::dist_ = Distribution();

template<typename T, typename Distribution, typename Engine>
bool _CurrentEngine::always_seed_ = true;

};

#undef enable_if
#undef forceinline
//#undef decay

#endif //XRND_H