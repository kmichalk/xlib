#ifndef _LOG_H
#define _LOG_H_

#include <fstream>
#include <ctime>
#include "xstring.h"
#include "error.h"


namespace x
{
	class log
	{
		std::fstream file_;
		x::string name_;
		time_t time_;

	public:
		log();
		log(x::string const& name);

		template<class _Type>
		log& operator<<(_Type const& obj)
		{
			std::time(&time_);
			if (file_.is_open()) {
				file_ << std::asctime(std::localtime(time_)) << " - " << obj;
			}
			return *this;
		}
		

	};
}
#endif //_LOG_H_
