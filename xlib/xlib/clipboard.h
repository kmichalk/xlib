#ifndef _X_CLIPBOARD_H_
#define _X_CLIPBOARD_H_

namespace x
{
	template<class _Type>
	class _Clipboard
	{
		static _Type value_;

		template<class _Type>
		friend _Type& remember(_Type& value);

		template<class _Type>
		friend _Type&& remember(_Type&& value);

		template<class _Type>
		friend _Type remind();
	};


	template<class _Type>
	inline _Type & remember(_Type & value)
	{
		_Clipboard<_Type>::value_ = value;
		return value;
	}

	template<class _Type>
	inline _Type && remember(_Type && value)
	{
		_Clipboard<_Type>::value_ = (_Type&)value;
		return (_Type&&)value;
	}

	template<class _Type>
	inline _Type remind()
	{
		return _Clipboard<_Type>::value_;
	}

	template<class _Type>
	_Type _Clipboard<_Type>::value_ = {};
}

#endif //_X_CLIPBOARD_H_
