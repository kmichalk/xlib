#ifndef ERROR_H
#define ERROR_H

#include <iostream>

namespace x
{

	template<typename = void>
	class error: public error<void>
	{
	public:
		using error<void>::error;

		~error() override {}
	};


	template<>
	class error<void>
	{
	public:
		const int num;
		const char* message;

		constexpr error(int num = 0, const char* message = ""):
			num{num}, message{message}
		{}

		friend std::ostream& operator<<(std::ostream& os, error const& err)
		{
			return os<<"E"<<err.num<<": "<<err.message<<std::endl;
		}

		virtual ~error() {}
	};

}
#endif