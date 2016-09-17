#ifndef LOGIC_H
#define LOGIC_H

#include <iostream>
#include "simple.h"
#include "va_handler.h"

#define enable_if std::enable_if_t

namespace logic
{
	template<typename... T>
	bool all_true(T... args)
	{
		bool result = true;
		x::expand((result &= (bool)args)...);
		return result;
	}

	template<typename... T>
	bool all_false(T... args)
	{
		bool result = true;
		x::expand((result &= !(bool)args)...);
		return result;
	}

	template<typename... T>
	bool any_true(T... args)
	{
		bool result = false;
		x::expand((result |= (bool)args)...);
		return result;
	}

	template<typename... T>
	bool any_false(T... args)
	{
		bool result = false;
		x::expand((result |= !(bool)args)...);
		return result;
	}

	//template<unsigned n>
	//class logical
	//{
	//public:
	//	enum class Value { min = 0, max = n-1 };
	//private:
	//	Value value;
	//public:

	//	logical():
	//		value{Value::min}
	//	{
	//	}
	//	logical(Value value):
	//		value{value}
	//	{
	//	}
	//	logical(logical<n> const& other):
	//		value{other.value}
	//	{
	//	}
	//	logical(bool value):
	//		value{value ? Value::max : Value::min}
	//	{
	//	}

	//	static int min()
	//	{
	//		return (int)Value::min;
	//	}
	//	static int max()
	//	{
	//		return (int)Value::max;
	//	}

	//	__forceinline logical<n>& operator=(logical<n> const& other)
	//	{
	//		value = other.value;
	//		return *this;
	//	}
	//	__forceinline bool operator==(logical<n> const& other) const
	//	{
	//		return value==other.value;
	//	}
	//	__forceinline bool operator!=(logical<n> const& other) const
	//	{
	//		return value!=other.value;
	//	}
	//	__forceinline logical<n>& operator&=(logical<n> const& other)
	//	{
	//		value = x::min(value, other.value);
	//		return *this;
	//	}
	//	__forceinline logical<n>& operator|=(logical<n> const& other)
	//	{
	//		value = x::max(value, other.value);
	//		return *this;
	//	}

	//	__forceinline logical<n> operator&(logical<n> const& other) const
	//	{
	//		return logical<n>{x::min(value, other.value)};
	//	}
	//	__forceinline logical<n> operator|(logical<n> const& other) const
	//	{
	//		return logical<n>{x::max(value, other.value)};
	//	}
	//	__forceinline logical<n> impl(logical<n> const& other) const
	//	{
	//		return logical<n>{x::min(Value::max, Value::max-value+other.value)};
	//	}
	//	__forceinline logical<n> operator!() const
	//	{
	//		return logical<n>{Value::max-value};
	//	}
	//};

	enum Method { minmax, algebraic, Hemacher, Einstein, drastic, limited };

	template<int method = logic::Method::algebraic>
	class fuzzy
	{
		double value;
		using fuzzy_t = fuzzy<method>;

	public:
		fuzzy():
			value{0.0}
		{
		}
		fuzzy(double value):
			value{x::cutr(value,0.0,1.0)}
		{
		}

		fuzzy(fuzzy const& other):
			value{other.value}
		{
		}

		fuzzy(bool value):
			value{(double)value}
		{
		}

		static constexpr double min()
		{
			return 0.0;
		}

		static constexpr double max()
		{
			return 1.0;
		}

		__forceinline fuzzy_t& operator=(fuzzy_t const& other)
		{
			value = other.value;
			return *this;
		}

		__forceinline bool operator==(fuzzy_t const& other) const
		{
			return value==other.value;
		}

		__forceinline bool operator!=(fuzzy_t const& other) const
		{
			return value!=other.value;
		}

//#pragma region operator_and_assign
//
//		template<int m = method>
//		__forceinline enable_if<method==Method::minmax,
//			fuzzy_t&> operator&=(fuzzy_t const& other) const
//		{
//			value = x::min(value, other.value);
//			return *this;
//		}
//		template<int m = method>
//		__forceinline enable_if<method==Method::algebraic,
//			fuzzy_t&> operator&=(fuzzy_t const& other) const
//		{
//			value = value*other.value;
//			return *this;
//		}
//		template<int m = method>
//		__forceinline enable_if<method==Method::Hemacher,
//			fuzzy_t&> operator&=(fuzzy_t const& other) const
//		{
//			value = (value*other.value)/(value+other.value-value*other.value);
//			return *this;
//		}
//		template<int m = method>
//		__forceinline enable_if<method==Method::Einstein,
//			fuzzy_t&> operator&=(fuzzy_t const& other) const
//		{
//			value = (value*other.value)/(2-(value+other.value-value*other.value));
//			return *this;
//		}
//		template<int m = method>
//		__forceinline enable_if<method==Method::drastic,
//			fuzzy_t&> operator&=(fuzzy_t const& other) const
//		{
//			value = x::max(value, other.value)==1 ? x::min(value, other.value) : 0;
//			return *this;
//		}
//		template<int m = method>
//		__forceinline enable_if<method==Method::limited,
//			fuzzy_t&> operator&=(fuzzy_t const& other) const
//		{
//			value = x::max(0,value+other.value-1);
//			return *this;
//		}
//
//#pragma endregion

//#pragma region operator_or_assign
//
//		template<int m = method>
//		__forceinline enable_if<method==Method::minmax,
//			fuzzy_t&> operator|=(fuzzy_t const& other) const
//		{
//			value = x::max(value, other.value);
//			return *this;
//		}
//		template<int m = method>
//		__forceinline enable_if<method==Method::algebraic,
//			fuzzy_t&> operator|=(fuzzy_t const& other) const
//		{
//			value = value+other.value-value*other.value;
//			return *this;
//		}
//		template<int m = method>
//		__forceinline enable_if<method==Method::Hemacher,
//			fuzzy_t&> operator|=(fuzzy_t const& other) const
//		{
//			value = (value*other.value)/(value+other.value-value*other.value);
//			return *this;
//		}
//		template<int m = method>
//		__forceinline enable_if<method==Method::Einstein,
//			fuzzy_t&> operator|=(fuzzy_t const& other) const
//		{
//			value = (value*other.value)/(2-(value+other.value-value*other.value));
//			return *this;
//		}
//		template<int m = method>
//		__forceinline enable_if<method==Method::drastic,
//			fuzzy_t&> operator|=(fuzzy_t const& other) const
//		{
//			value = x::max(value, other.value)==1 ? x::min(value, other.value) : 0;
//			return *this;
//		}
//		template<int m = method>
//		__forceinline enable_if<method==Method::limited,
//			fuzzy_t&> operator|=(fuzzy_t const& other) const
//		{
//			value = x::max(0, value+other.value-1);
//			return *this;
//		}
//
//#pragma endregion

#pragma region operator_and

		template<int m = method>
		__forceinline enable_if<m==Method::minmax,
			fuzzy_t> operator&(fuzzy_t const& other) const
		{
			return fuzzy_t{x::min(value, other.value)};
		}
		template<int m = method>
		__forceinline enable_if<m==Method::algebraic,
			fuzzy_t> operator&(fuzzy_t const& other) const
		{
			return fuzzy_t{value*other.value};
		}
		template<int m = method>
		__forceinline enable_if<m==Method::Hemacher,
			fuzzy_t> operator&(fuzzy_t const& other) const
		{
			return fuzzy_t{(value*other.value)/(value+other.value-value*other.value)};
		}
		template<int m = method>
		__forceinline enable_if<m==Method::Einstein,
			fuzzy_t> operator&(fuzzy_t const& other) const
		{
			return fuzzy_t{(value*other.value)/(2-(value+other.value-value*other.value))};
		}
		template<int m = method>
		__forceinline enable_if<m==Method::drastic,
			fuzzy_t> operator&(fuzzy_t const& other) const
		{
			return fuzzy_t{x::max(value, other.value)==1 ? x::min(value, other.value) : 0};
		}
		template<int m = method>
		__forceinline enable_if<m==Method::limited,
			fuzzy_t> operator&(fuzzy_t const& other) const
		{
			return fuzzy_t{x::max(0,value+other.value-1)};
		}

#pragma endregion
#pragma region operator_or

		template<int m = method>
		__forceinline enable_if<m==Method::minmax,
			fuzzy_t> operator|(fuzzy_t const& other) const
		{
			return fuzzy_t{x::max(value, other.value)};
		}
		template<int m = method>
		__forceinline enable_if<m==Method::algebraic,
			fuzzy_t> operator|(fuzzy_t const& other) const
		{
			return fuzzy_t{value+other.value-value*other.value};
		}
		template<int m = method>
		__forceinline enable_if<m==Method::Hemacher,
			fuzzy_t> operator|(fuzzy_t const& other) const
		{
			return fuzzy_t{(value+other.value-2*value*other.value)/(1-value*other.value)};
		}
		template<int m = method>
		__forceinline enable_if<m==Method::Einstein,
			fuzzy_t> operator|(fuzzy_t const& other) const
		{
			return fuzzy_t{(value+other.value)/(1+value*other.value)};
		}
		template<int m = method>
		__forceinline enable_if<m==Method::drastic,
			fuzzy_t> operator|(fuzzy_t const& other) const
		{
			return fuzzy_t{x::min(value,other.value)==0 ? x::max(value,other.value) : 1};
		}
		template<int m = method>
		__forceinline enable_if<m==Method::limited,
			fuzzy_t> operator|(fuzzy_t const& other) const
		{
			return fuzzy_t{x::min(1,value+other.value)};
		}

#pragma endregion

		__forceinline fuzzy_t& operator&=(fuzzy_t const& other)
		{
			operator=((*this)&other);
		}

		__forceinline fuzzy_t& operator|=(fuzzy_t const& other)
		{
			operator=((*this)|other);
		}

		__forceinline fuzzy_t operator!() const
		{
			return fuzzy_t{1-value};
		}

		friend std::ostream& operator<<(std::ostream& os, fuzzy_t const& f)
		{
			return os<<f.value;
		}
	};

	//template<>
	//class fuzzy<Method::algebraic>
	//{
	//	double value;
	//	using fuzzy_t = fuzzy<Method::algebraic>;
	//public:	
	//	fuzzy():
	//		value{0.0}
	//	{
	//	}
	//	fuzzy(double value):
	//		value{x::cutr(value,0.0,1.0)}
	//	{
	//	}

	//	fuzzy(fuzzy const& other):
	//		value{other.value}
	//	{
	//	}

	//	fuzzy(bool value):
	//		value{(double)value}
	//	{
	//	}

	//	static double min()
	//	{
	//		return 0.0;
	//	}

	//	static double max()
	//	{
	//		return 1.0;
	//	}

	//	__forceinline fuzzy_t& operator=(fuzzy_t const& other)
	//	{
	//		value = other.value;
	//		return *this;
	//	}

	//	__forceinline bool operator==(fuzzy_t const& other) const
	//	{
	//		return value==other.value;
	//	}

	//	__forceinline bool operator!=(fuzzy_t const& other) const
	//	{
	//		return value!=other.value;
	//	}

	//	__forceinline fuzzy_t& operator&=(fuzzy_t const& other)
	//	{
	//		value *= other.value;
	//		return *this;
	//	}

	//	__forceinline fuzzy_t& operator|=(fuzzy_t const& other)
	//	{
	//		value += (other.value-value*other.value);
	//		return *this;
	//	}

	//	__forceinline fuzzy_t operator&(fuzzy_t const& other) const
	//	{
	//		return fuzzy_t{value*other.value};
	//	}

	//	__forceinline fuzzy_t operator|(fuzzy_t const& other) const
	//	{
	//		return fuzzy_t{value+other.value-value*other.value};
	//	}

	//	__forceinline fuzzy_t operator!() const
	//	{
	//		return fuzzy_t{1-value};
	//	}

	//	friend std::ostream& operator<<(std::ostream& os, fuzzy_t const& f)
	//	{
	//		return os<<f.value;
	//	}
	//};

	//template<>
	//class fuzzy<Method::Hemacher>
	//{
	//	double value;
	//	using fuzzy_t = fuzzy<Method::Hemacher>;

	//public:
	//	fuzzy():
	//		value{0.0}
	//	{
	//	}
	//	fuzzy(double value):
	//		value{x::cutr(value,0.0,1.0)}
	//	{
	//	}

	//	fuzzy(fuzzy const& other):
	//		value{other.value}
	//	{
	//	}

	//	fuzzy(bool value):
	//		value{(double)value}
	//	{
	//	}

	//	static double min()
	//	{
	//		return 0.0;
	//	}

	//	static double max()
	//	{
	//		return 1.0;
	//	}

	//	__forceinline fuzzy_t& operator=(fuzzy_t const& other)
	//	{
	//		value = other.value;
	//		return *this;
	//	}

	//	__forceinline bool operator==(fuzzy_t const& other) const
	//	{
	//		return value==other.value;
	//	}

	//	__forceinline bool operator!=(fuzzy_t const& other) const
	//	{
	//		return value!=other.value;
	//	}

	//	__forceinline fuzzy_t& operator&=(fuzzy_t const& other)
	//	{
	//		value = (value*other.value)/(value+other.value-value*other.value);
	//		return *this;
	//	}

	//	__forceinline fuzzy_t& operator|=(fuzzy_t const& other)
	//	{
	//		value = (value+other.value-2*value*other.value)/(1-value*other.value);
	//		return *this;
	//	}

	//	__forceinline fuzzy_t operator&(fuzzy_t const& other) const
	//	{
	//		return fuzzy_t{(value*other.value)/(value+other.value-value*other.value)};
	//	}

	//	__forceinline fuzzy_t operator|(fuzzy_t const& other) const
	//	{
	//		return fuzzy_t{(value+other.value-2*value*other.value)/(1-value*other.value)};
	//	}

	//	__forceinline fuzzy_t operator!() const
	//	{
	//		return fuzzy_t{1-value};
	//	}

	//	friend std::ostream& operator<<(std::ostream& os, fuzzy_t const& f)
	//	{
	//		return os<<f.value;
	//	}
	//};
}

#undef enable_if

#endif //LOGIC_H