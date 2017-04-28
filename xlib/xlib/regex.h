#ifndef _REGEX_H
#define _REGEX_H_

#include "xstring.h"


namespace x
{
	struct regex_rule
	{
		enum Type: byte
		{
			SINGLE, RANGE, SET, WORD
		};

		enum Modif: byte
		{
			ONE, MORE, POSSIBLE, ANY
		};

		union Spec{
			char ch;
			x::range<char> range;
			char const* set;
			struct {
				char const* data;
				char const* it;
			} word;

			Spec(char ch):
				ch{ch}
			{
			}

			Spec(x::range<char> const& range):
				range{range}
			{
			}

			Spec(char const* set):
				set{set}
			{
			}

			Spec(char const* word,
				char const* it):
				word{word,it}
			{
			}

			~Spec()
			{
			}
		};

		byte const type;
		Spec spec;
		byte modif;

		regex_rule(char ch, byte modif):
			type{SINGLE},
			spec{ch},
			modif{modif}
		{
		}

		regex_rule(x::range<char> const& range, byte modif):
			type{RANGE},
			spec{range},
			modif{modif}
		{
		}

		regex_rule(char const* set, byte modif):
			type{SET},
			spec{set},
			modif{modif}
		{
		}

		regex_rule(char const* set, byte modif):
			type{SET},
			spec{set},
			modif{modif}
		{
		}

		x::string::const_iterator& match(x::string::const_iterator& ch)
		{
			/*if (type == SINGLE) {

			}
			for (auto ch{str.cbegin()}; ch; ++ch) {

			}*/
			return ch;
		}

		__forceinline bool match(char ch)
		{
			switch (type) {
				case SINGLE:	
					return ch == spec.ch;
				case RANGE:		
					return spec.range.contains(ch);
				case SET:		
					return [this](char ch) {
						char const* i{spec.set - 1};
						while (*++i) if (*i == ch) return true;
						return false;
					}(ch);
			}
		}
	};

	class regex
	{
		x::vector<regex_rule> rules_;
	public:
		x::string expr;

		regex():
			expr{}
		{
		}

		regex(regex const&) = default;
		regex(regex &&) = default;
		
	};
}
#endif //_REGEX_H_
