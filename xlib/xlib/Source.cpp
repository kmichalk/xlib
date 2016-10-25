#include "xvector.h"
#include "keyevent.h"
#include "SFML\Graphics.hpp"
#include "signals.h"
#include "typeenum.h"
#include "timer.h"
#include "loops.h"
#include "error.h"
#include "va_handler.h"
#include "clarg.h"
#include "list.h"
#include "realcall.h"
#include "xrnd.h"
#include "auto_cast.h"
#include "crd_cast.h"
#include <fstream>
#include "ctmath.h"
#include "autolim.h"
#include "ctarray.h"
#include "disp.h"
#include "listed.h"
#include "geom.h"
#include "finally.h"
#include "keyevent.h"
#include <SFML\Graphics.hpp>
#include "crd.h"
#include "static_if.h"
#include "xstring.h"
#include "gc.h"
#include "debug.h"


using namespace std;

#define enable_if std::enable_if_t

#include <map>

#include "strmap.h"
#include "tree.h"

#include "register.h"

//template<typename T>
//std::enable_if_t<std::is_pointer<T>::value>
//foo(T)
//{
//	std::cout << "pointer\n";
//}
//
//template<typename T, std::size_t sz>
//void foo(T(&)[sz])
//{
//	std::cout << "array\n";
//}
//
//int main()
//{
//	char const* c = "3";
//	foo(c);
//	foo("hello");
//}


namespace ns
{

	inline namespace in2
	{
		void C(int i)
		{
		}
	}
}

#define in_class exists(this)
#include "id.h"

class C
{
public:
	int val;
	char arr;
	bool b;
	long double ld;

	/*C(int val):
		val{val}
	{ }*/
	C(){ }

	C(C const& other):
		val{other.val},
		arr{other.arr},
		b{other.b},
		ld{other.ld}
	{
		//disp "copy\n";
	}

	C& operator=(C const& other)
	{
		//disp "assign\n";
		val = other.val;
		arr = other.arr;
		b = other.b;
		ld = other.ld;
		return *this;
	}
};

class Triv
{
public:
	int i;
};

class Def
{
public:
	int i;
	Def& operator=(Def const& other)
	{
		i = other.i;
	}
};

namespace x
{
	template<class _Type>
	__forceinline void objcpy2(void* dest, _Type* src)
	{
		*reinterpret_cast<_Type*>(dest) = *src;
	}

	template<class _Type>
	__forceinline void objcpy3(void* dest, _Type* src)
	{
		*reinterpret_cast<FakeType<_Type>*>(dest) = reinterpret_cast<FakeType<_Type>&&>(_Type{*src});
	}
}


int main()
{
	x::string s{"-982345226126."};
	//disp s.is<double>();

	//return 0;

	
	C* ptr = reinterpret_cast<C*>(new byte[8]);
	C c{};


	x::tic();
	repeat(100000000)
	{
		if (double val = sq2(-1)) {

		}
	}
	disp x::toc(), endl;
	
	x::tic();
	repeat(100000)
	{
		try {
			double val = sq1(-1);
		}
		catch(...){ }
	}
	disp x::toc(), endl;

	x::tic();
	repeat(100000000)
	{
		
	}
	disp x::toc(), endl;

	/*x::tic();
	repeat(1000000000)
	{
		x::objcpy2(ptr, &c);
	}
	disp x::toc(), endl;

	x::tic();
	repeat(1000000000)
	{
		x::objcpy3(ptr, &c);
	}
	disp x::toc(), endl;*/

	/*
	disp measure{
		repeat(1000000)
		for (auto d : x::_range(0, 10, 0.1)) {
			auto v = d;
		}
	}, endl;

	disp measure{
		repeat(1000000)
		for (auto&& d : x::_range(0, 10, 0.1)) {
			auto v = d;
		}
	}, endl;

	disp measure{
		repeat(1000000)
		for (auto d{x::_range(0, 10, 0.1).begin()}; d; ++d) {
			auto v = *d;
		}
	}, endl;

	auto r = x::_range(0, 10, 0.1);
	disp measure{
		repeat(1000000)
		for (auto d = r.begin(); d != r.end(); ++d) {
			auto v = *d;
		}
	}, endl;

	disp measure{
		repeat(1000000)
		for (auto d = r.begin(), end = r.end(); d != end; ++d) {
			auto v = *d;
		}
	}, endl;*/
	/*char const* cs = "krz";
	x::string s{cs};
	x::string d{"krz"};
	std::string dd;*/
	//int t = x::va::sum(1, 2, 3, 4);
	
	/*x::tic();
	repeat(100000) x::safe_ptr<C> p{1};
	disp x::toc(), endl;

	x::tic();
	repeat(100000) 
	{
		auto p = new C{1}; delete p;
	}
	disp x::toc(), endl;*/
}