#pragma once
#include <iostream>
#include "xstring.h"

class MenuOption
{
public:
	char select;
	x::string descr;

	MenuOption() {}
	MenuOption(char select, x::string descr) :
		select{select}, descr{descr}
	{}
	MenuOption(const MenuOption& other) :
		select{other.select}, descr{other.descr}
	{}
	MenuOption& operator=(const MenuOption& other)
	{
		select = other.select;
		descr = other.descr;
	}
	friend std::ostream& operator<<(std::ostream& os, const MenuOption& mo)
	{
		os<<std::toupper(mo.select)<<" - "<<mo.descr<<std::endl;
	}
};

class Menu
{
	x::vector<MenuOption> options_;
	using c_opt_iter = x::vector<MenuOption>::const_iterator;
	using opt_iter = x::vector<MenuOption>::iterator;

	MenuOption* validate_select_(char select)
	{
		for (auto& o : options_) {
			if (o.select == select)
				return &o;
		}
		return nullptr;
	}

public:
	Menu()
	{}
	bool add_option(char select, x::string descr)
	{
		if (!validate_select_(select)) {
			options_.push_back({select,descr});
			return true;
		}
		return false;
	}
	//void operator()()
	//{
	//	std::cout<<(*this);
	//	char input;
	//	MenuOption* chosen;
	//	std::cin>>input;
	//	while (!(chosen = validate_select_(input))) {
	//		std::cout<<"invalid input... ";
	//		std::cin>>input;
	//	}
	//	chosen


	//}
	friend std::ostream& operator<<(std::ostream& os, const Menu& m);
};

#define _for(iter, container) for (decltype(container)::iterator iter = container.begin(); iter != container.end(); ++iter)

std::ostream& operator<<(std::ostream& os, const Menu& m)
{
	for (Menu::c_opt_iter o = m.options_.cbegin(); o!=m.options_.cend(); ++o) {
		os<<o;
	}
	return os;
}
