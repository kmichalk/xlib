#ifndef _TREE_H
#define _TREE_H_

#include "xvector.h"



namespace x
{
	template<class _Type>
	class tree
	{
		static constexpr size_t DEFAULT_CHILDREN_ALLOC_SIZE_ = 3;
		static size_t childrenAllocSize_;

		x::vector<tree<_Type>*> children_;

		void copyChildren_(x::tree<_Type> const& other)
		{
			for (auto child{other.children_.begin()}; child; ++child) {
				children_.push_back(child->copy());
			}
		}

	public:
		_Type value;

		tree():
			value{},
			children_(childrenAllocSize_)
		{
		}

		tree(_Type const& value):
			value{value},
			children_(childrenAllocSize_)
		{
		}

		/*template<class... _Args, _concept<x::every_is<_Type, _Args...>::value>>
		tree(_Args const&... children):
			children_{new tree<_Type>{children}...}
		{
		}*/

		template<class... _Args, _concept<x::every_is<_Type, _Args...>::value>>
		tree(_Type const& value, tree<_Args>*... children) :
			value{value},
			children_{children...}
		{
		}

		tree(tree<_Type> const& other):
			value{other.value},
			children_(other.children_.size())
		{
			copyChildren_(other);
		}

		tree(tree<_Type>&& other):
			value{std::move(other.value)},
			children_{other.children_.move()}
		{
			other.children_.clear();
		}

		tree<_Type>& operator=(tree<_Type> const& other)
		{
			value = other.value;
			children_.erase<x::vector_opt::PTR_DELETE>();
			copyChildren_(other);
			return *this;
		}

		tree<_Type>& operator=(tree<_Type> && other)
		{
			value = std::move(other.value);
			children_.erase<x::vector_opt::PTR_DELETE>();
			children_ = other.children_.move();
			return *this;
		}

		tree<_Type>& insert(_Type const& value)
		{
			children_.push_back(new tree<_Type>{value});
			return *this;
		}

		tree<_Type>& clear()
		{
			children_.erase<x::vector_opt::PTR_DELETE>();
			return *this;
		}

		~tree()
		{
			for (auto child{children_.begin()}; child; ++child) {
				if (*child) delete *child;
			}
		}
	};

	template<class _Type>
	size_t tree<_Type>::childrenAllocSize_ = tree<_Type>::DEFAULT_CHILDREN_ALLOC_SIZE_;

	template<class _Type, size_t _nChild>
	class _treeNode:
		public tree<_Type>
	{
		
	};
}
#endif //_TREE_H_
