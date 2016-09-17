#pragma once

#define enable_if std::enable_if_t
#include "more_type_traits.h"

//template<typename... T>
//class va_handler
//{
//	static_assert(all_same<T...>::value,
//		"Arguments have to be of the same type.");
//private:
//	using type_ = x::select_t<1, T...>;
//	type_* args_;
//	size_t size_;
//public:
//	__forceinline size_t size() const
//	{
//		return size_;
//	}
//	va_handler(T... args) :
//		size_{sizeof...(args)}
//	{
//		args_ = new type_[size_]{args...};
//	}
//	__forceinline type_& operator[](size_t i)
//	{
//		return args_[i];
//	}
//	type_& get(size_t i)
//	{
//		if (i<size_) {
//			return args_[i];
//		}
//		throw "out of size";
//	}
//	~va_handler()
//	{
//		delete[] args_;
//	}
//};

namespace x
{

	template<typename... A>
	class va_handler
	{
	public:
		static constexpr unsigned size = sizeof...(A);

	private:
		template<int i>
		using ArgT_ = x::select_t<i, A...>;

		class Eraser_ {};

		template<typename T>
		class FnConcrete_: public Eraser_
		{
		public:
			T value;
			FnConcrete_(T const& value):
				value{value}
			{
			}
		};

		Eraser_* args_[size];

		/*template<int... i>
		void assign_(seq<i...>&, A const&... args)
		{
			args_ = new Eraser_*[size] { new Concrete_<A>{args}... };
		}*/

	public:
		va_handler(A const&... args):
			args_{(new FnConcrete_<A>{args}) ...}
		{
		}

		template<unsigned i>
		__forceinline ArgT_<i>& get()
		{
			static_assert(i <= size,
				"Index exceeds parameter pack size.");
			return static_cast<FnConcrete_<ArgT_<i>>*>(args_[i-1])->value;
		}

		~va_handler()
		{
			for (int i = 0; i<size; ++i) delete args_[i];
		}
	};

	template<typename... A>
	inline va_handler<A...> unpack(A const&... args)
	{
		return va_handler<A...>{args...};
	}

	template<typename... T>
	__forceinline void expand(T...) {}
}