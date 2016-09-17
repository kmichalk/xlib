#ifndef FINALLY_H
#define FINALLY_H

#include "fn.h"

namespace x
{
	class Finally
	{
		Fn<void()> action_;
	public:
		Finally() = delete;
		Finally(Finally const&) = delete;
		Finally(Finally&&) = delete;

		inline Finally(Fn<void()>&& action):
			action_{std::move(action)}
		{
		}

		inline ~Finally()
		{
			action_();
		}
	};
}
#define finally(_code) x::Finally{[&](){_code}}

#endif //FINALLY_H
