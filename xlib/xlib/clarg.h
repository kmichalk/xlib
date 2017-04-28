#ifndef _X_ARG_H_
#define _X_ARG_H_

#include "xvector.h"
#include "range.h"
#include "fn.h"


class Value
{
public:
	const x::string name_;
	Value(x::string const& name):
		name_(name)
	{
	}

	virtual bool pass(x::string const& arg)
	{
		return arg==name_;
	}

	virtual ~Value() {}
};

class BasicArg final: public Value
{
public:
	x::Fn<void()> action_;

	template<typename F>
	BasicArg(x::string const& name, F&& fn):
		Value(name),
		action_{std::forward<F>(fn)}
	{}
	bool pass(x::string const& arg) final override
	{
		if (Value::pass(arg)) {
			action_();
			return true;
		}
		return false;
	}
};

class StringArg final : public Value
{
public:
	x::Fn<void(x::string const&)> action_;
	const char assignCh_;
	x::string str_;

	template<typename F>
	StringArg(x::string name, char assignCh, F&& fn):
		Value(name),
		assignCh_{assignCh},
		action_{std::forward<F>(fn)}
	{
		std::cout<<name_;
	}
	bool pass(x::string const& arg) final override
	{
		try {
			if (auto ascPos = arg.find_first(assignCh_)) {
				if (Value::pass(arg.substr(0, ascPos - 1)) &&
					(str_ = arg.substr(ascPos + 1))) {
					action_(str_);
					return true;
				}
			}
		}
		catch (...) {}
		return false;
	}
};

template<typename T>
class ValueArg final: public Value
{
public:
	const char assignCh_;
	T value_;
	x::range<T> range_;
	x::Fn<void(T)> action_;

	template<typename F>
	ValueArg(x::string const& name, char assignCh, x::range<T>&& range, F&& fn):
		Value(name),
		assignCh_{assignCh},
		range_{range},
		action_{std::forward<F>(fn)}
	{}

	template<typename F>
	ValueArg(x::string const& name, char assignCh, F&& fn):
		Value(name),
		assignCh_{assignCh},
		range_{std::numeric_limits<T>::lowest(), std::numeric_limits<T>::max()},
		action_{std::forward<F>(fn)}
	{}

	bool pass(x::string const& arg) final override
	{
		try {
			size_t ascPos = arg.pos_of(assignCh_);
			if (Value::pass(arg.subset(0, ascPos-1)) &&
				range_.contains(value_ = 
					T(std::stod(arg.subset(ascPos+1))))) 
			{
				action_(value_);
				return true;
			}
		}
		catch (...) {}
		return false;
	}
};


class ArgHandler
{
	x::vector<Value*> args_;
public:
	ArgHandler() {}

	void process(int argc, char* argv[])
	{
		for (size_t i = 1; i<argc; ++i) {
			std::cout<<argv[i]<<std::endl;
			for (Value*& a : args_) {
				if (a->pass(x::string(argv[i])))
					break;
			}
		}
	}

	void add(Value* arg)
	{
		args_.push_back(arg);
	}

	~ArgHandler()
	{
		args_.delete_each();
	}
};

#endif //ARG_H
