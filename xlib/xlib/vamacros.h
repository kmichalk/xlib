#ifndef VA_MACROS_H
#define VA_MACROS_H


#define SEPARATE(m, x) m x
#define NARGS_SEQ(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,N,...) N

#define NARGS(...) SEPARATE(NARGS_SEQ, (__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))

#define PRIMITIVE_CAT(x, y) x ## y
#define CAT(x, y) SEPARATE(PRIMITIVE_CAT, (x,y))

//#define APPLY(macro, ...) CAT(APPLY_, NARGS(__VA_ARGS__))(macro, __VA_ARGS__)
#define __APPLY_1 (m, x1)											m(x1)
#define __APPLY_2 (m, x1, x2)										m(x1), m(x2)
#define __APPLY_3 (m, x1, x2, x3)									m(x1), m(x2), m(x3)
#define __APPLY_4 (m, x1, x2, x3, x4)								m(x1), m(x2), m(x3), m(x4)
#define __APPLY_5 (m, x1, x2, x3, x4, x5)							m(x1), m(x2), m(x3), m(x4), m(x5)
#define __APPLY_6 (m, x1, x2, x3, x4, x5, x6)						m(x1), m(x2), m(x3), m(x4), m(x5), m(x6)
#define __APPLY_7 (m, x1, x2, x3, x4, x5, x6, x7)					m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7)
#define __APPLY_8 (m, x1, x2, x3, x4, x5, x6, x7, x8)				m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8)
#define __APPLY_9 (m, x1, x2, x3, x4, x5, x6, x7, x8, x9)			m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9)
#define __APPLY_10(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10)		m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10)

#define APPLY(macro, ...) SEPARATE(CAT(__APPLY_, NARGS(__VA_ARGS__)), (macro, __VA_ARGS__))

#define __GENERATE_VAR_1(T, baseName)	T baseName##_1;
#define __GENERATE_VAR_2(T, baseName)	__GENERATE_VAR_1(T, baseName) T baseName##_2;
#define __GENERATE_VAR_3(T, baseName)	__GENERATE_VAR_2(T, baseName) T baseName##_3;
#define __GENERATE_VAR_4(T, baseName)	__GENERATE_VAR_3(T, baseName) T baseName##_4;
#define __GENERATE_VAR_5(T, baseName)	__GENERATE_VAR_4(T, baseName) T baseName##_5;
#define __GENERATE_VAR_6(T, baseName)	__GENERATE_VAR_5(T, baseName) T baseName##_6;
#define __GENERATE_VAR_7(T, baseName)	__GENERATE_VAR_6(T, baseName) T baseName##_7;
#define __GENERATE_VAR_8(T, baseName)	__GENERATE_VAR_7(T, baseName) T baseName##_8;
#define __GENERATE_VAR_9(T, baseName)	__GENERATE_VAR_8(T, baseName) T baseName##_9;
#define __GENERATE_VAR_10(T, baseName)	__GENERATE_VAR_9(T, baseName) T baseName##_10;

#define GENERATE_VAR(T, baseName, n) static_assert(n<=10, "This marco handles up to 10 fields."); __GENERATE_VAR_##n(T, baseName)

#endif //VA_MACROS_H
