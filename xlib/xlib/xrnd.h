//#pragma once
#ifndef _X_RND_H_
#define _X_RND_H_
#include <random>
#include <cmath>
#include "timer.h"
#include "more_type_traits.h"

#define enable_if typename std::enable_if_t
#define forceinline __forceinline
//#define decay typename std::decay_t
namespace x
{

#define _CurrentEngine _RandomEngine<T,Distribution,Engine>

	template<
		typename T = int,
		typename Distribution = std::normal_distribution<>,
		typename Engine = std::mt19937>
		class _RandomEngine
	{
		template<typename Y = T>
		static forceinline constexpr enable_if<std::is_integral<Y>::value,
			Y> dT_()
		{
			return 1;
		}
		template<typename Y = T>
		static forceinline constexpr enable_if<std::is_floating_point<Y>::value,
			Y> dT_()
		{
			return 0.0;
		}

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
			timer<std::chrono::microseconds>::epoch() +
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
		timer<std::chrono::seconds>::epoch();

	template<typename T, typename Distribution, typename Engine>
	Engine _CurrentEngine::engine_ = []() {Engine engine{}; engine.seed(timer<std::chrono::microseconds>::epoch()); return engine; }();

	template<typename T, typename Distribution, typename Engine>
	Distribution _CurrentEngine::dist_ = Distribution();

	template<typename T, typename Distribution, typename Engine>
	bool _CurrentEngine::always_seed_ = true;



	template<size_t... _i>
	__forceinline void _expand_randomize(char* str, x::seq<_i...>&)
	{
		x::va::expand((str[_i] = x::random<char>('a', 'z'))...);
	}

	template<size_t _size>
	char* random_str()
	{
		
		char* str = new char[_size + 1]{};
	
		str[_size] = '\0';
		
		_expand_randomize(str, x::gen_seq<_size>{});
	
		return str;
	}

	template<class=void> char* random_str(size_t size)
	{
		char* str = new char[size + 1]{};
		str[size] = '\0';
		for (size_t ch = 0; ch < size; ++ch) {
			str[ch] = x::random<char>('a', 'z');
		}
		return str;
	}

}

#undef enable_if
#undef forceinline
//#undef decay

#endif //_X_RND_H_
