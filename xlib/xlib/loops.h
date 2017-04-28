#ifndef _LOOPS_H_
#define _LOOPS_H_

#include "size.h"


/// @file

/// <summary>
/// Simplified version of for loop.
/// Repeats underlying code \a _n times.
/// </summary>
/// <param name="_n"> - number of iterations</param>
#define repeat(_n) for (auto __i__=0;__i__<_n;++__i__)


/// <summary>
/// Simplified iterator for loop, only valid when used with objects
/// from x namespace since they are using non-standard, optimized iteration syntax. 
/// </summary>
/// <param name="_i"> - name of the iterator</param>
/// <param name="_cnt"> - name of the container</param>
/// <example><code>x::vector<int> iv{1,2,3};
/// foreach(i, iv) ++*i;</code></example>
#define foreach(_i, _cnt) for (auto _i{_cnt.begin()}; _i; ++_i)


/// <summary>
/// Simplified const_iterator for loop, only valid when used with objects
/// from x namespace since they are using non-standard, optimized iteration syntax. 
/// </summary>
/// <param name="_i"> - name of the const_iterator</param>
/// <param name="_cnt"> - name of the container</param>
/// <example><code>int sum(x::vector<int> const& iv) {
///		int result = 0;
///		const_foreach(i, iv) result += *i;
///		return result;
/// }</code></example>
#define const_foreach(_i, _cnt) for (auto _i{_cnt.cbegin()}; _i; ++_i)


/// <summary>
/// Expands to infinite loop <code>while (true)</code>.
/// </summary>
#define loop while(true)


/// <summary>
/// While loop with negated condition resulting in loop executing until
/// given condition is fulfilled.
/// </summary>
/// <param name="">any statement to be checked</param>
#define until(...) while(! (__VA_ARGS__))
//#define booliter(_iterName) for (int _iterName=0;_iterName<2;++_iterName)


#define booliter(_iterName, _CODE) \
	{\
		static constexpr bool _iterName = false;\
		_CODE\
	}{\
		static constexpr bool _iterName = false;\
		_CODE\
	}

#define iter(_iterName, _from, _to) for (auto _iterName{_from}; _iterName <= _to; ++_iterName) 
#endif //_LOOPS_H_