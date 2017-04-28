#ifndef _X_CRD_CAST_H_
#define _X_CRD_CAST_H_

#include "more_type_traits.h"

#define enable_if std::enable_if_t

template<class>
struct _CrdConverter;

template<template<class> class _To, class _ToParam>
struct _CrdConverter<_To<_ToParam>>
{
	MEMBER_TEST(_has_z, z);

	template<
		template<class> class _From,
		class _FromParam,
		template<class> class _To = _To,
		_capture(_ToParam)>
		inline static enable_if<
			!_has_z<_To<_ToParam>>::value || 
			!_has_z<_From<_FromParam>>::value,
	_To<_ToParam>> _convert(_From<_FromParam> const& crdVal)
	{
		return _To<_ToParam>{(_ToParam)crdVal.x, (_ToParam)crdVal.y};
	}

	template<
		template<class> class _From,
		class _FromParam,
		template<class> class _To = _To,
		_capture(_ToParam)>
		inline static enable_if<
			_has_z<_To<_ToParam>>::value && 
			_has_z<_From<_FromParam>>::value,
	_To<_ToParam>> _convert(_From<_FromParam> const& crdVal)
	{
		return _To<_ToParam>{(_ToParam)crdVal.x, (_ToParam)crdVal.y, (_ToParam)crdVal.z};
	}
};

template<class _To, class _From>
__forceinline _To crd_cast(_From const& _crdVal) 
{
	return _CrdConverter<_To>::_convert(_crdVal);
}

#undef enable_if

#endif //_X_CRD_CAST_H_
