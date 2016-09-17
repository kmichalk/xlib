#ifndef BRIDGE_H
#define BRIDGE_H

#include "observer.h"

namespace x
{
	template<typename _GetType, typename _SetType>
	class bridge
	{
		x::Observer<_GetType> getValue_;
		x::Observer<_SetType> setValue_;

	public:
		bridge() = delete;

		bridge(_GetType& getValue, _SetType& setValue):
			getValue_{getValue},
			setValue_{setValue}
		{
		}

		/*inline bool operator()() const
		{
			return (getValue_() ? (setValue_) : false);
		}*/
	};
}

#define bridge(_getVar, _setVar, ...) x::bridge<decltype(_getVar), decltype(_setVar)> __VA_ARGS__{_getVar, _setVar}

#endif //BRIDGE_H
