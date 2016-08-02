#ifndef CRD_CAST
#define CRD_CAST

#include "more_type_traits.h"

#define enable_if std::enable_if_t

template<typename>
struct _CrdConverter;

template<template<typename> class _To, typename _TT>
struct _CrdConverter<_To<_TT>>
{
	MEMBER_TEST(_has_z, z);

	template<
		template<typename> class _From,
		typename _FT,
		template<typename> class _ToC = _To,
		typename _TTC = _TT>
		inline static enable_if<
			!_has_z<_ToC<_TTC>>::value || 
			!_has_z<_From<_FT>>::value,
	_To<_TT>> _convert(_From<_FT> const& crdVal)
	{
		return _To<_TT>{(_TT)crdVal.x, (_TT)crdVal.y};
	}

	template<
		template<typename> class _From,
		typename _FT,
		template<typename> class _ToC = _To,
		typename _TTC = _TT>
		inline static enable_if<
			_has_z<_ToC<_TTC>>::value && 
			_has_z<_From<_FT>>::value,
	_To<_TT>> _convert(_From<_FT> const& crdVal)
	{
		return _To<_TT>{(_TT)crdVal.x, (_TT)crdVal.y, (_TT)crdVal.z};
	}
};

#define crd_cast(_Type) _CrdConverter<_Type>::_convert

#undef enable_if

#endif
