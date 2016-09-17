#ifndef AUTO_REF_H
#define AUTO_REF_H

namespace x
{
	template<typename T>
	inline constexpr T& deref(T* var)
	{
		return *var;
	}

	template<typename T>
	inline constexpr T& deref(T& var)
	{
		return var;
	}

	template<typename T>
	inline constexpr auto deref(T&& var)
	{
		return std::forward<T>(var);
	}

	template<typename T>
	inline constexpr auto deref(T*&& var)
	{
		return *std::forward<T>(var);
	}

	template<typename T>
	inline constexpr T* pointer(T& var)
	{
		return &var;
	}

	template<typename T>
	inline constexpr T* pointer(T* var)
	{
		return var;
	}

	template<typename T>
	inline constexpr auto pointer(T&& var)
	{
		return &std::forward<T>(var);
	}

	template<typename T>
	inline constexpr auto pointer(T*&& var)
	{
		return std::forward<T>(var);
	}
}

#endif //AUTO_REF_H
