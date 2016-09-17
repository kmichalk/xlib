#ifndef STATIC_IF
#define STATIC_IF

namespace x::ct
{
	template<typename IfTrueType, typename IfFalseType>
	struct _Choose
	{
		template<bool test, IfTrueType ifTrue, IfFalseType ifFalse>
		inline static constexpr auto _choose()
		{
			return test ? ifTrue : ifFalse;
		}
	};

	template<typename _Type>
	struct _ForceConstexpr
	{
		template<_Type _value>
		struct _Test
		{
			static constexpr _Type value = _value;
		};
	};
}

#define force_constexpr(_value) _ForceConstexpr<decltype(_value)>::_Test<_value>::value

//////////////////////////////////////////////////////////////////////////////

#define _STATIC_ERROR(_text) static_assert(false, _text)

#define _CREATE_USED_TAG(_name) struct _name{}
#define _STATIC_IF(_test) __if_exists(x::_StaticIf<_test>)
#define _LASTTEST(_num) __lastTest_##_num##__
#define _USED_TAG(_num) __lastTest_##_num##_used__
#define _IF_USED_TAG_EXISTS(_num) __if_exists(_USED_TAG(_num))
#define _IF_USED_TAG_NOT_EXISTS(_num) __if_not_exists(_USED_TAG(_num))
#define _IF_LASTTEST_EXISTS(_num) __if_exists(_LASTTEST(_num))
#define _IF_LASTTEST_NOT_EXISTS(_num) __if_not_exists(_LASTTEST(_num))
#define _TRY_GENERATE_LASTTEST(_num) _IF_LASTTEST_NOT_EXISTS(_num){_LASTTEST(_num)}

#define _GENERATE_LASTTEST_NAME _TRY_GENERATE_LASTTEST(1) \
_IF_LASTTEST_EXISTS(1){_TRY_GENERATE_LASTTEST(2) \
_IF_LASTTEST_EXISTS(2){_TRY_GENERATE_LASTTEST(3) \
_IF_LASTTEST_EXISTS(3){_TRY_GENERATE_LASTTEST(4) \
_IF_LASTTEST_EXISTS(4){_TRY_GENERATE_LASTTEST(5) \
_IF_LASTTEST_EXISTS(5){_TRY_GENERATE_LASTTEST(6) \
_IF_LASTTEST_EXISTS(6){_TRY_GENERATE_LASTTEST(7) \
_IF_LASTTEST_EXISTS(7){_TRY_GENERATE_LASTTEST(8) \
_IF_LASTTEST_EXISTS(8){_TRY_GENERATE_LASTTEST(9) \
_IF_LASTTEST_EXISTS(9){_TRY_GENERATE_LASTTEST(10) \
_IF_LASTTEST_EXISTS(10){_STATIC_ERROR("More than 10 static_if-else statements in one scope are impossible.");}}}}}}}}}}

#define _TRY_GET_LASTTEST(_num) _IF_LASTTEST_EXISTS(_num){_LASTTEST(_num)}
//#define _TRY_GET_LASTTEST(_num, _current) _STATIC_IF(exists(_LASTTEST(_num)) && !exists(_USED_TAG(_num))){_LASTTEST(_num)}
#define _GET_LASTTEST_NAME _TRY_GET_LASTTEST(10)\
_IF_LASTTEST_NOT_EXISTS(10){_TRY_GET_LASTTEST(9)\
_IF_LASTTEST_NOT_EXISTS(9){_TRY_GET_LASTTEST(8)\
_IF_LASTTEST_NOT_EXISTS(8){_TRY_GET_LASTTEST(7)\
_IF_LASTTEST_NOT_EXISTS(7){_TRY_GET_LASTTEST(6)\
_IF_LASTTEST_NOT_EXISTS(6){_TRY_GET_LASTTEST(5)\
_IF_LASTTEST_NOT_EXISTS(5){_TRY_GET_LASTTEST(4)\
_IF_LASTTEST_NOT_EXISTS(4){_TRY_GET_LASTTEST(3)\
_IF_LASTTEST_NOT_EXISTS(3){_TRY_GET_LASTTEST(2)\
_IF_LASTTEST_NOT_EXISTS(2){_TRY_GET_LASTTEST(1)\
_IF_LASTTEST_NOT_EXISTS(1){_STATIC_ERROR("There is no static_if statement to be else-referenced.");}}}}}}}}}}

//#define _TRY_GET_USED_TAG(_num) _IF_USED_TAG_EXISTS(_num){_USED_TAG(_num)}
//
//#define _GET_LAST_USED_TAG _TRY_GET_USED_TAG(10)\
//_IF_USED_TAG_NOT_EXISTS(10){_TRY_GET_USED_TAG(9)\
//_IF_USED_TAG_NOT_EXISTS(9){_TRY_GET_USED_TAG(8)\
//_IF_USED_TAG_NOT_EXISTS(8){_TRY_GET_USED_TAG(7)\
//_IF_USED_TAG_NOT_EXISTS(7){_TRY_GET_USED_TAG(6)\
//_IF_USED_TAG_NOT_EXISTS(6){_TRY_GET_USED_TAG(5)\
//_IF_USED_TAG_NOT_EXISTS(5){_TRY_GET_USED_TAG(4)\
//_IF_USED_TAG_NOT_EXISTS(4){_TRY_GET_USED_TAG(3)\
//_IF_USED_TAG_NOT_EXISTS(3){_TRY_GET_USED_TAG(2)\
//_IF_USED_TAG_NOT_EXISTS(2){_TRY_GET_USED_TAG(1)\
//_IF_USED_TAG_NOT_EXISTS(1){_STATIC_ERROR("");}}}}}}}}}}

//#define _GET_LASTTEST_NAME _TRY_GET_LASTTEST(9,10)\
//_STATIC_IF(!exists(_LASTTEST(10))) {_TRY_GET_LASTTEST(8,9)\
//_STATIC_IF(!exists(_LASTTEST(9) )) {_TRY_GET_LASTTEST(7,8)\
//_STATIC_IF(!exists(_LASTTEST(8) )) {_TRY_GET_LASTTEST(6,7)\
//_STATIC_IF(!exists(_LASTTEST(7) )) {_TRY_GET_LASTTEST(5,6)\
//_STATIC_IF(!exists(_LASTTEST(6) )) {_TRY_GET_LASTTEST(4,5)\
//_STATIC_IF(!exists(_LASTTEST(5) )) {_TRY_GET_LASTTEST(3,4)\
//_STATIC_IF(!exists(_LASTTEST(4) )) {_TRY_GET_LASTTEST(2,3)\
//_STATIC_IF(!exists(_LASTTEST(3) )) {_TRY_GET_LASTTEST(1,2)\
//_STATIC_IF(!exists(_LASTTEST(2) )) {_TRY_GET_LASTTEST(1)\
//_STATIC_IF(!exists(_LASTTEST(1) )) {_STATIC_ERROR("There is no static_if statement to be else-referenced.");}}}}}}}}}}

#define _SET_LASTTEST(_test) static constexpr bool _GENERATE_LASTTEST_NAME {_test}

#define exists(_identifier) (__if_exists(_identifier) {true} __if_not_exists(_identifier){false})

#define _STATIC_CHOOSE(_test, _ifTrue, _ifFalse) x::ct::_Choose<decltype(_ifTrue), decltype(_ifFalse)>::_choose<_test,_ifTrue,_ifFalse>()
#define _STATIC_IF_1(_test) /*_SET_LASTTEST(_test);*/ _STATIC_IF(_test)
//#define _static_if_1(_test) __if_exists(x::_StaticIf<_test>)
#define _STATIC_IF_3(_test, _ifTrue, _ifFalse) _STATIC_CHOOSE(_test, _ifTrue, _ifFalse)
#define static_if(...) CAT(_STATIC_IF_, NARGS(__VA_ARGS__))(__VA_ARGS__)

#define static_else /*_CREATE_USED_TAG(_GET_LASTTEST_NAME##_used__);*/ _STATIC_IF(!_GET_LASTTEST_NAME)


#define is_constexpr(_value) exists( x::_StaticIf<bool(_value) || true> )

//#define If(...) static_if(is_constexpr(__VA_ARGS__)){__if_exists(x::_StaticIf<(__VA_ARGS__)>)} static_else{if(__VA_ARGS__)}

#endif //STATIC_IF

