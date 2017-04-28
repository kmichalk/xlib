#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include "xstring.h"

namespace x
{

	template<typename _Owner = void>
	class error: public error<void>
	{
	public:
		using error<void>::error;

		virtual x::string message() const override
		{
			return x::string{typeid(_Owner).name()} +x::string{": "} + message_;
		}

		~error() override {}
	};


	template<>
	class error<void>
	{
	protected:
		const x::string message_;

	public:
		const int num;

		error()
			:
			num{0},
			message_{""}
		{
		}

		error(
			int num,
			x::string const& message)
			:
			num{num}, 
			message_{message}
		{
		}

		error(
			int num,
			x::string && message)
			:
			num{num},
			message_{std::forward<x::string>(message)}
		{
		}

		error(
			x::string const& message)
			:
			num{0},
			message_{message}
		{
		}

		error(
			x::string && message)
			:
			num{0},
			message_{std::forward<x::string>(message)}
		{
		}

		virtual x::string message() const
		{
			return message_;
		}

		template<class _Owner>
		friend std::ostream& operator<<(std::ostream& os, error<_Owner> const& err)
		{
			return os << err.message()<<std::endl;
		}

		virtual ~error() {}
	};

	enum class ExceptionHandling
	{
		NONE, X_ERRORS, ALL
	};

}
#endif