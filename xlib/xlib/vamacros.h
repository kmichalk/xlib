#ifndef VA_MACROS_H
#define VA_MACROS_H

#define VA_EXECUTE(macro, VA) macro VA

#define NARGS_SEQ(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,N,...) N
//#define VA_TO_SEQ(VA) NARGS_SEQ VA
#define NARGS(...) VA_EXECUTE(NARGS_SEQ, (__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))
//#define NARGS(...) VA_TO_SEQ((__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1))

#define PRIMITIVE_CAT(x, y) x ## y
//#define VA_TO_CONCAT(VA) PRIMITIVE_CAT VA
#define CAT(x, y) VA_EXECUTE(PRIMITIVE_CAT, (x,y))
//#define CAT(x, y) VA_TO_CONCAT((x, y))

#define SEPARATE(m, x) m x
#define APPLY(macro, ...) SEPARATE(CAT(APPLY_, NARGS(__VA_ARGS__)), (macro, __VA_ARGS__))

#define APPLY(macro, ...) CAT(APPLY_, NARGS(__VA_ARGS__))(macro, __VA_ARGS__)
#define APPLY_1(m, x1) m(x1)
#define APPLY_2(m, x1, x2) m(x1), m(x2)
#define APPLY_3(m, x1, x2, x3) m(x1), m(x2), m(x3)
#define APPLY_4(m, x1, x2, x3, x4) m(x1), m(x2), m(x3), m(x4)
#define APPLY_5(m, x1, x2, x3, x4, x5) m(x1), m(x2), m(x3), m(x4), m(x5)
#define APPLY_6(m, x1, x2, x3, x4, x5, x6) m(x1), m(x2), m(x3), m(x4), m(x5), m(x6)
#define APPLY_7(m, x1, x2, x3, x4, x5, x6, x7) m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7)
#define APPLY_8(m, x1, x2, x3, x4, x5, x6, x7, x8) m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8)
#define APPLY_9(m, x1, x2, x3, x4, x5, x6, x7, x8, x9) m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9)
#define APPLY_10(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10)

#endif //VA_MACROS_H
