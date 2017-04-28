#include "xvector.h"
#include "keyevent.h"
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
#include "crd.h"
#include "xstring.h"
#include "static_if.h"
#include "gc.h"
#include "debug.h"
#include "bitliteral.h"
#include "fnv2.h"


using namespace std;

#define enable_if std::enable_if_t

#include "strmap.h"
#include "tree.h"

#include "register.h"


#define in_class exists(this)
#include "id.h"



#undef byte

#include "assign.h"
#include "allocator.h"
#include "clipboard.h"


namespace x
{
	template<class _Type>
	class ref
	{
		_Type* ptr_;
	public:
		ref():
			ptr_{nullptr}
		{
		}

		ref(_Type& obj):
			ptr_{&obj}
		{
		}

		__forceinline operator _Type&()
		{
			return *ptr_;
		}

		_Type& operator=(_Type& obj)
		{
			return *(ptr_ = &obj);
		}
	};
}

namespace x
{
	template<size_t... _i>
	__forceinline x::string expandSetString_(unsigned value, x::seq<_i...>&)
	{
		static constexpr unsigned max10 = 1'000'000'000;
		char* buffer = new char[11];
		unsigned div = max10;
		unsigned nPow;
		x::va::expand((nPow = value / assert_constexpr(x::ct::pow(10, 9 - _i)),
			buffer[_i] = '0' + value / assert_constexpr(x::ct::pow(10, 9 - _i)),
			value -= nPow*assert_constexpr(x::ct::pow(10, 9 - _i)))...);
		return x::string::adopt(buffer, 10);
		//return x::string{buffer};
	}

	x::string to_string(unsigned value)
	{
		static constexpr unsigned max10 = 1'000'000'000;

		x::string str;
		bool begin = false;
		for (unsigned pow = max10; pow > 0; pow /= 10) {
			unsigned nPow = value / pow;
			if (nPow > 0) begin = true;
			if (begin) str.push_back('0' + nPow);
			value -= nPow*pow;
		}
		return str;
	}

	static thread_local x::string temp;

	x::string to_string(int value)
	{
		static constexpr int bit0 = 1 << 31;
		static constexpr unsigned max10 = 1'000'000'000;

		x::string str;
		if (value & bit0) str.push_back('-');
		value = abs(value);
		bool begin = false;
		for (unsigned pow = max10; pow > 0; pow /= 10) {
			unsigned nPow = value / pow;
			if (nPow > 0) begin = true;
			if (begin) str.push_back('0' + nPow);
			value -= nPow*pow;
		}
		return str;
	}

}

#include "mathfn.h"
#include "thread_safe.h"


#define GENERATE_OSTREAM_OPERATOR(_Class, _expr) \
friend std::ostream& operator<<(std::ostream& os, _Class const& obj) { return os << obj._expr ; }

#define GENERATE_EQUALITY_OPERATOR(_Class, _expr)\
bool operator==(_Class const& other) const {return _expr == other._expr ;}


namespace x
{
	template<class _Type>
	class init_list
	{
		_Type* begin_;
		size_t size_;

	public:
		init_list():
			begin_{nullptr},
			size_{0}
		{
		}

		template<class _First, class... _Rest>
		init_list(_First&& first, _Rest&&... rest):
			begin_{&first},
			size_{1 + sizeof...(_Rest)}
		{
		}

		__forceinline size_t size() const
		{
			return size_;
		}

		__forceinline _Type* begin() const
		{
			return begin_;
		}
	};
}

void ght(x::init_list<int> il)
{
	for (int i = 0; i < il.size(); ++i) {
		disp *(il.begin() + i);
	}
}


class Base
{
public:
	int i;
	Base(int i):i{i}
	{
	}
	Base(float f)
	{
	}

	virtual void f()
	{

	}

	GENERATE_EQUALITY_OPERATOR(Base, i);
};

class Der
{
public:

	int i;
	Der(Base const& b):
		i{b.i}
	{
	}
	GENERATE_EQUALITY_OPERATOR(Base, i);
	GENERATE_OSTREAM_OPERATOR(Base, i);
	//GENERATE_OSTREAM_OPERATOR(Der, i);
};



template<class _Arg>
class ExecutableSequence
{
	x::vector<x::Callable<void(_Arg)>*> sequence_;

public:
	void operator()(_Arg arg) const
	{
		const_foreach(f, sequence_)
		{
			(*f)->call(arg);
		}
	}

	void push(x::Callable<void(_Arg)>* func)
	{
		sequence_.push_back(func);
	}

	void pop()
	{
		sequence_.pop_back();
	}

	void clear()
	{
		sequence_.delete_each().clear();
	}

};


void bytecpy(void* dest, void* src, byte delim)
{
	byte last;
	do {
		*(((byte*&)dest)++) = *(((byte*&)src)++);
		last = *(byte*)src;
	} while (last != 0xC3 && last != 0xCB && last != 0xC2 && last != 0xCA);
}

#include "date.h"
#include "abstract.h"

#pragma region BITDISP
template<class _Type>
void bitdisp(_Type v)
{
	using mask_t = _MinFitType<sizeof(_Type) * 8>;
	static constexpr mask_t mask = (mask_t)__int64{1};
	char c;
	for (int i = sizeof(_Type) * 8 - 1; i >= 0; --i) {
		c = ((v >> i) & mask);
	}
	//disp "\n";
}


template<size_t _size, class _Type, int... _i>
void _expandExtractBits(char(&output)[_size], _Type value, x::seq<_i...>&)
{
	static_assert(_size >= sizeof...(_i), "");
	using mask_t = _MinFitType<bitsizeof(_Type)>;
	//displn typeid(mask_t).name();
	//displn _size;
	//displn value;
	//displn BIT_SIZE<_Type>;
	//displn sizeof...(_i);
	static constexpr mask_t MASK = (mask_t)__int64{1};
	//displn MASK;
	x::va::expand((output[_size - _i - 2] = ((value >> _i)&MASK) + '0')...);
	//displn output;
}

template<class _Type>
void bitdisp_3(_Type value)
{
	char output[bitsizeof(_Type) +1]{0};
	_expandExtractBits(output, value, x::gen_seq<bitsizeof(_Type)>{});
	disp output;
}



template<class _Type, int... _i>
__forceinline void _expandDispBits(_Type value, x::seq<_i...>&)
{
	using mask_t = x::_MinFitType<bitsizeof(_Type)>;
	static constexpr mask_t MASK = (mask_t)__int64{1};
	static constexpr size_t OUTSIZE = bitsizeof(_Type) +1;
	char output[OUTSIZE]{(((reinterpret_cast<mask_t&>(value) >> (OUTSIZE - _i - 2)) & MASK) + '0')...};
	displn output;
}

template<class _Type>
void bitdisp_4(_Type value)
{
	_expandDispBits(value, x::gen_seq<bitsizeof(_Type)>{});
}


template<class _Type>
void bitdisp_2(_Type v)
{
	static constexpr size_t SIZE = sizeof(_Type) * 8;
	using mask_t = x::_MinFitType<SIZE>;
	static constexpr mask_t mask = (mask_t)__int64{1};
	char output[SIZE + 1]{0};
	for (int i = 0; i < SIZE; ++i) {
		output[SIZE - i - 1] = ((v >> i) & mask) + '0';
	}
	disp output;
}

#pragma endregion


class B
{
public:
	x::vector<int> ref;
	//virtual x::vector<int>& get() abstract;
	//B(x::vector<int>& v):ref{v}{ }
	B()
	{
	}
	virtual void f() = 0;
};

class D:virtual public B
{
	virtual void f()
	{
	}
public:
	x::vector<int> v;
	D():
		v{5,6,7}
	{
	}


	/*virtual x::vector<int>& get() override
	{
		return v;
	}*/
};


#define suspend template<class = void>
#define CLASS private: suspend inline constexpr void __member__(){}\
	using this_type = x::owner_of_t<decltype(&__member__<>)>; 

#define OBJECT public: virtual this_type* getThis() override { return this; }\
		virtual this_type const* getThis() const override { return this; }\
	private:

class Object
{
public:
	virtual void f()
	{
		displn "f";
	}
	int i;

	Object* get()
	{
		return this;
	}
};


//class A: public Object
//{
//public:
//	int i;
//	struct Eraser
//	{
//		virtual void execute() abstract;
//	};
//
//	template<class _Type>
//	struct Concrete: public Eraser
//	{
//		virtual void execute() override
//		{
//			displn typeid(_Type).name();
//		}
//	};
//
//	//Eraser* eraser_;
//
//	template<class _Type>
//	inline A(_Type&& arg)
//		//eraser_{new Concrete<_Type>{}}
//	{
//		/*x::Finally overrider;
//		overrider = [&,this] {displn arg; new (this) Concrete<_Type>{}; };*/
//		finally{displn arg; new (this) Concrete<_Type>{}; };
//	}
//};

auto ret()
{
	struct _Dummy
	{};
	_Dummy _d;
	return &[&, _d](int i) {return i + 1; };
}

#define GENERATE_TEST_FUNCTIONS(...)\
x::v2::Executable<void(__VA_ARGS__ )>* xep = new x::v2::Function<void(__VA_ARGS__ )>{[](__VA_ARGS__ ) {}};\
x::v2::Function<void(__VA_ARGS__ )> xf2{[](__VA_ARGS__ ) {}};\
x::Callable<void(__VA_ARGS__ )>* xcp = new x::Fn<void(__VA_ARGS__ )>{[](__VA_ARGS__ ) {}};\
x::Fn<void(__VA_ARGS__ )> xf{[](__VA_ARGS__ ) {}};\
std::function<void(__VA_ARGS__ )> sf{[](__VA_ARGS__ ) {}};


#define _DBGLINE(...) displn #__VA_ARGS__; __VA_ARGS__


void crt(x::vector<Function<void()>*>& v)
{
	auto o = new Object;
	auto l = [] {displn "lambda"; };
	//auto ol = [o] {o->f(); };
	//displn &ol;
	auto f = new Function<void()>{(decltype(l) && )(*new decltype(l){l})};
	v.push_back(f);
	v.push_back(new Function<void()>{o, &Object::f});
	v.push_back(new Function<void()>{[o] {o->f(); }});
}
#include <Windows.h>
#include "sleep.h"

namespace x
{
	//template<class _Type>
	//struct Ptr
	//{
	//	using type = _Type*;
	//};

	/*template<class _Ret, class... _Arg>
	struct Ptr<_Ret(_Arg...)>
	{
		using type = _Ret(*)(_Arg...);
	};*/

	//template<class _Type>
	//using ptr_ty = Ptr<_Type>::type;

	template<class _Ret, class... _Arg>
	using func_ptr = _Ret(*)(_Arg...);
}

template<class _Ret, class... _Arg>
x::func_ptr<_Ret, _Arg...> select_overload(_Ret(*func)(_Arg...))
{
	return func;
}

template<class _Type>
class vec: public x::vector<_Type>
{
	class Async
	{
		vec<_Type>& owner_;
		x::vector<x::Fn<void()>> queue_;
		volatile bool queueLock_ = false;

	public:
		Async(vec<_Type>& owner):
			owner_{owner}
		{
		}


		vec<_Type>& sort()
		{
			while(queueLock_){ }
			queueLock_ = true;
			queue_.push_back([this] {owner_.sort(); });
			queueLock_ = false;
			return *owner_;
		}
	};
public:

	Async async;

	vec(std::initializer_list<_Type> il):
		x::vector<_Type>(il),
		async{*this}
	{
	}
};

#include "matrix.h"


//void* operator new(size_t size, int t)
//{
//
//}

//template<class _Type>
//void* operator new(size_t size, _Type&&)
//{
//	return nullptr;
//}

constexpr __int64 SIGN_M	= 0x8000'0000'0000'0000;
constexpr size_t SIGN_BEG = 0;
constexpr size_t SIGN_LEN = 1;
constexpr __int64 EXP_M		= 0x7FF0'0000'0000'0000;
constexpr size_t EXP_BEG = 1;
constexpr size_t EXP_LEN = 11;
constexpr __int64 MANT_M	= 0x000F'FFFF'FFFF'FFFF;
constexpr size_t MANT_BEG = 12;
constexpr size_t MANT_LEN = 52;

template<class _Type>
_Type bit_reverse(_Type value)
{

}


x::string tos(double value, size_t prec = 12)
{
	bitdisp_4(value);
	displn value;
	__int64 expval = (reinterpret_cast<__int64&>(value) & EXP_M) >> (64 - EXP_BEG - EXP_LEN);
	bitdisp_4(expval);
	displn expval;
	__int64 mantval = (reinterpret_cast<__int64&>(value) & MANT_M) >> (MANT_LEN - prec);
	bitdisp_4(mantval);
	x::string mants = x::to_string((unsigned)mantval);
	displn mants;
	unsigned __int64 ival = (pow(2,prec) + mantval)*pow(2, expval-1023);
	displn ival;
	
	return{};
}


x::result<double> convert(x::string const& str, byte base)
{
	if (!str.size_) return x::result<double>::INVALID;

	double
		result = 0,
		mult = 0.1;
	bool
		neg = (str.data_[0] == '-');

	auto ch{str.cend()};
	for (; x::_CharRanges<char>::numbers_.contains(*--ch) && ch; ) {
		mult *= base;
		result += mult * (*ch - '0');
	}
	if (!ch) return neg ?
		x::result<double>{-result} :
		x::result<double>{result};

	if (*ch != '.') return x::result<double>::INVALID;
	//mult *= base;
	if (ch.pos != str.lastpos()) {
		result /= mult*base;
		mult = 1;
	}
	else mult *= base;
	while (x::_CharRanges<char>::numbers_.contains(*--ch)) {
		result += mult * (*ch - '0');
		mult *= base;
	}
	return !ch ?
		(neg ?
			x::result<double>{-result} :
			x::result<double>{result}) :
		x::result<double>::INVALID;
}

#include "pow10_table.h"

x::result<unsigned> convert_u(x::string const& str)
{
	char const
		*ch = str.data_ - 1,
		*end = str.data_ + str.size_ - 1,
		*begin = str.data_;
	unsigned
		result = 0,
		mult = 1;

	while (*++ch >= '0' && *ch <= '9') {
		result += mult * (*(end - (size_t)ch + (size_t)begin) - '0');
		mult *= 10;
	}
	return ch > end ? result : x::result<unsigned>::INVALID;
}



x::result<unsigned> convert_u_t(x::string const& str)
{
	//size_t ch = 0;
	if (str.size_ > 10)
		return x::result<unsigned>::INVALID;

	unsigned result = 0;
	size_t offset = 10 - str.size_;
	for (size_t c = 0; c < str.size_; ++c) {
		result += POW10_TABLE[str.data_[c] - '0'][c + offset];
	}
	return result;
}

x::result<unsigned> convert_u_t_2(x::string const& str)
{
	char const
		*ch = str.data_ - 1,
		*end = str.data_ + str.size_ - 1,
		*offset = str.data_ - 10 + str.size_;
	unsigned
		result = 0;

	while (*++ch >= '0' && *ch <= '9') {
		result += POW10_TABLE[*ch - '0'][ch - offset];
	}
	return ch > end ? result : x::result<unsigned>::INVALID;
}

#define REG_LOOP_BEGIN(_reg, _begin, _end)\
	_asm{mov	_reg, _begin};	\
	_LOOP_BEGIN_:				\
	_asm{cmp	_reg, _end};	\
	_asm{jg		_LOOP_END_};	\
	_asm{inc	_reg};			



#define REG_LOOP_END			\
	_asm{jmp	_LOOP_BEGIN_};	\
	_LOOP_END_:
	

#define LAB lab:

#define GOTOLAB _asm{ jg lab}


struct QR
{
	x::vector<int> v[10];
	QR():
		v{(x::vector<int>::set_default_alloc(1), x::vector<int>{})}
	{
	}
};


bool __cdecl streq(char const* s1, char const* s2, size_t n) {
	int const* i1 = reinterpret_cast<int const*>(s1) - 1;
	int const* i2 = reinterpret_cast<int const*>(s2) - 1;
	//reinterpret_cast<int const*>(s1 + n);
	//(n >> 2);
	//int const* end = i1+ (n >> 2);
	//int res = 0;
	BEGIN:
	if (*++i1 == *++i2)
		goto BEGIN;
	return false;


	/*while (*++i1 == *++i2) {
	}*/
	//RETURN:
	return reinterpret_cast<char const*>(i1) >= (s1 + n);
}

#define CLASS(_Name) template<class> class _##_Name; using _Name = _##_Name<void>; template<> class _##_Name<void>


//template<class> class _Name; 
//using Name = _Name<void>; 
//template<>
//class _Name<void>
//{
//
//};

CLASS(HO) {
public:
	int i;
};

int main()
{
	HO h;

	auto s1 = "krzysztophor";
	auto s2 = "krzysztophor";
	displn streq(s1, s2, 12);
	x::tic();
	repeat(100000000) {}
	displn x::toc();


	x::tic();
	repeat(100000000) streq(s1, s2, 12);
	displn x::toc();

	x::tic();
	repeat(100000000) strncmp(s1, s2, 12);
	displn x::toc();

	return 0;


	x::tic();
	REG_LOOP_BEGIN(edi, 0, 1000000000);
	REG_LOOP_END;
	displn x::toc();
	
	x::tic();
	for (int ni=0;ni<1000000000;++ni){ }
	displn x::toc();

	return 0;

	displn convert_u_t_2("123056");
	x::string s{"1124075991"};
	std::string ss{"1124075991"};

	x::tic();
	repeat(10000000) convert_u(s);
	displn x::toc();

	x::tic();
	repeat(10000000) convert_u_t(s);
	displn x::toc();

	x::tic();
	repeat(10000000) convert_u_t_2(s);
	displn x::toc();

	x::tic();
	repeat(10000000) stoul(ss);
	displn x::toc();

	return 0;

	auto i = 0xEFFF'FFFF'FFFF'FFFF;
	double d = reinterpret_cast<double&>(i);
	//displn d;
	tos(256.64);

	displn "-------";
	bitdisp_4(SIGN_M);
	bitdisp_4(EXP_M);
	bitdisp_4(MANT_M);
	return 0;

}









///////////////////////////////////////////////////////////////////////////////

//int main()
//{
	//t3.stop();


	/*MemoryHandler handler;
	Cmp* p = (Cmp*)::operator new(sizeof(Cmp));

	x::tic();
	repeat(1000000) delete new Cmp{0};
	displn x::toc();

	x::tic();
	repeat(1000000)	{
		new(p) Cmp{0};
		p->~Cmp();
	}
	displn x::toc();


	x::tic();
	repeat(1000000)
	{
		handler.destruct(handler.construct<Cmp>());
	}
	displn x::toc();
*/
/*x::string key(4098, true);
key.randomize();
displn key;

x::string s{"1222223222"};
disp s.flip_char("01");*/
//}

//int main() {
//
//	byte code[] = {
//		0x48, 0xb8, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00,
//		0x00, 0x00, 0x48, 0xbf, 0x02, 0x00, 0x00, 0x00,
//		0x00, 0x00, 0x00, 0x00, 0x0f, 0x05
//	};
//
//	byte* buf = new byte[sizeof(code)];
//	unsigned long oldProtect;
//	memcpy(buf, code, sizeof(code));
//
//	VirtualProtect(buf, sizeof(code), PAGE_EXECUTE_READWRITE, &oldProtect);
//	//disp oldProtect;
//	((void(*) (void))buf)();
//
//	return 0;
//}

//int main() {
//	
//	int val = 1;
//	//v(val);
//	auto b = new byte[31]{
//	0x55, 0x8B, 0xEC, 0x8B, 0x45, 0x08, 0xC7, 0x00, 0x63, 0x00, 0x00, 0x00, 0x5D, 0xC3};
//
//	//bytecpy(b, a, 0);
//	//iter(i, 0, 9) disp (int)b[i], " ";
//
//	auto fp = func;
//	//LPVOID exeMem = VirtualAlloc((LPVOID)0x00111111, 100, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
//	//memcpy(fcp, fp, 20);
//	//((byte*)exeMem)[1] = 0;
//	unsigned long oldProtect;
//	//VirtualProtect(exeMem, 100, PAGE_EXECUTE, &oldProtect);
//	fp(1);
//	VirtualProtect((LPVOID)b, 31, PAGE_EXECUTE_READWRITE, &oldProtect);
//
//	//displn "ok";
//	//fp(1);
//	disp val;
//	((void(*)(int&))b)(val);
//	disp val;
//
//}



//return 0;
///*x::tic();
//repeat(1000000)
//{
//x::strcmp(s1, s2);
//x::strcmp(s1, s2);
//x::strcmp(s1, s2);
//x::strcmp(s1, s2);
//x::strcmp(s1, s2);
//x::strcmp(s1, s2);
//x::strcmp(s1, s2);
//x::strcmp(s1, s2);
//x::strcmp(s1, s2);
//x::strcmp(s1, s2);
//}
//disp x::toc(), endl;
//
//x::tic();
//repeat(1000000)
//{
//x::strcmp2(s1, s2);
//x::strcmp2(s1, s2);
//x::strcmp2(s1, s2);
//x::strcmp2(s1, s2);
//x::strcmp2(s1, s2);
//x::strcmp2(s1, s2);
//x::strcmp2(s1, s2);
//x::strcmp2(s1, s2);
//x::strcmp2(s1, s2);
//x::strcmp2(s1, s2);
//}
//disp x::toc(), endl;*/
//
//x::tic();
//repeat(100000000)
//{
//	x::strcmp3(s1, s2);
//	x::strcmp3(s1, s2);
//	x::strcmp3(s1, s2);
//	x::strcmp3(s1, s2);
//	x::strcmp3(s1, s2);
//	x::strcmp3(s1, s2);
//	x::strcmp3(s1, s2);
//	x::strcmp3(s1, s2);
//	x::strcmp3(s1, s2);
//	x::strcmp3(s1, s2);
//}
//disp x::toc(), endl;
//
///*x::tic();
//repeat(1000000)
//{
//s3==s4;
//s3==s4;
//s3==s4;
//s3==s4;
//s3==s4;
//s3==s4;
//s3==s4;
//s3==s4;
//s3==s4;
//s3==s4;
//}
//disp x::toc(), endl;*/
//
//x::tic();
//repeat(100000000)
//{
//	strcmp(d1, d2);
//	strcmp(d1, d2);
//	strcmp(d1, d2);
//	strcmp(d1, d2);
//	strcmp(d1, d2);
//	strcmp(d1, d2);
//	strcmp(d1, d2);
//	strcmp(d1, d2);
//	strcmp(d1, d2);
//	strcmp(d1, d2);
//}
//disp x::toc(), endl;
//
//x::tic();
//repeat(100000000)
//{
//	strncmp(d1, d2, 24);
//	strncmp(d1, d2, 24);
//	strncmp(d1, d2, 24);
//	strncmp(d1, d2, 24);
//	strncmp(d1, d2, 24);
//	strncmp(d1, d2, 24);
//	strncmp(d1, d2, 24);
//	strncmp(d1, d2, 24);
//	strncmp(d1, d2, 24);
//	strncmp(d1, d2, 24);
//}
//disp x::toc(), endl;