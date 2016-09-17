//#ifndef LISTED_H
//#define LISTED_H
//
//template<template<typename...> class _ListContainer, class _ThisType>
//class Listed
//{
//	using Container_ = _ListContainer<_ThisType>;
//	using Iterator_ = Container_::iterator;	
//
//	Container_& thisContainer_;
//	Iterator_ thisIter_;
//
//public:
//	Listed() = delete;
//	Listed(Listed<_ListContainer, _ThisType> const&) = delete;
//	Listed(Listed<_ListContainer, _ThisType> &&) = delete;
//
//	Listed(Container_& container, Iterator_ const& iter):
//		thisContainer_{container},
//		thisIter_{iter}
//	{
//	}
//
//	virtual ~Listed()
//	{
//		thisContainer_.erase(thisIter_);
//	}
//};
//
//#endif