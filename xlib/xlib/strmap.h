#ifndef _STRING_MAP_H_
#define _STRING_MAP_H_

#include "xvector.h"
#include "range.h"

namespace x
{
	template<class _Type>
	class strmap
	{
	public:
		x::vector<x::two<char const*, _Type>> data_[256];

		class Element
		{
			_Type* ptr_;

		public:
			Element(_Type* ptr):
				ptr_{ptr}
			{
			}

			Element(Element const&) = default;

			Element(Element&&) = default;

			Element& operator=(Element const&) = default;

			__forceinline _Type& operator=(_Type const& value)
			{
				*ptr_ = value;
			}

			__forceinline operator bool() const
			{
				return ptr_;
			}

			__forceinline operator _Type&()
			{
				return *ptr_;
			}
			
		};

	public:
		strmap()
			:
			data_{(x::vector<_Type>::set_default_alloc(0),
				std::move(x::vector<_Type>{}))}
		{
			x::vector<_Type>::set_default_alloc();
		}

		strmap<_Type>& insert(char const* str, _Type const& elem)
		{
			if (str) {
				data_[str[0]].push_back({str,elem});
			}
			return *this;
		}

		Element operator[](char const* str)
		{
			if (str) {
				for (auto elem{data_[str[0]].begin()}; elem; ++elem) {
					if ( ! strcoll(elem->a, str))
						return {&elem->b};
				}
			}
			return {nullptr};
		}
	};

}

#endif //_STRING_MAP_H_
