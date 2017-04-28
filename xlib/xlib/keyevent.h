//#ifndef KEY_EVENT_H
//#define KEY_EVENT_H
//
//#include <thread>
//#include <atomic>
//#include "threaded.h"
//#include "signals.h"
//
//#include "typeenum.h"
//#include "SFML\Graphics.hpp"
//#include "error.h"
//
//
//class Event: enumed
//{
//protected:	
//	signal() action;
//
//	Event() : typenum()
//	{}
//
//public:	
//	virtual bool trigger() const abstract;
//	virtual void process() abstract;
//	virtual void operator()() abstract;
//	virtual bool operator==(Event const& other)
//	{
//		return num_==other.num_;
//	}
//
//	friend class EventHandler;
//};
//
//class KeyEvent: public Event, enumed
//{
//	static const WKey DEFAULT_KEY = WKey::Unknown;
//
//	WKey key;
//	bool pressed;
//	bool once;
//
//public:
//	KeyEvent() : typenum(),
//		key{DEFAULT_KEY}, pressed{false}, once{true}
//	{}
//
//	template<typename F>
//	KeyEvent(
//		WKey key,
//		bool once,
//		F slot_fn) : typenum(),
//		key{key}, pressed{false}, once{once}
//	{
//		static_assert(is_free_func_ptr<F>::value,
//			"This KeyEvent constructor only accepts function pointers.");
//		action.attach(slot_fn);
//	}
//
//	template<typename T, typename F>
//	KeyEvent(
//		WKey key,
//		bool once,
//		T* obj,
//		F slot_fn) : typenum(),
//		key{key}, pressed{false}, once{once}
//	{
//		static_assert(is_member_func_ptr<F>::value,
//			"This KeyEvent constructor only accepts member function pointers.");
//		action.attach(obj, slot_fn);
//	}
//
//	/*template<typename... S>
//	KeyEvent(
//		WKey key,
//		bool once,
//		S*... slot_fn) :
//		action{slot_fn...}, key{key}, pressed{false}, once{once}
//	{
//		static_assert(
//			all_true<std::is_base_of<_Slot<>, S>::value...>::value,
//			"This KeyEvent constructor only accepts pointers to Slots.");
//	}*/
//
//	KeyEvent& operator=(const KeyEvent& other)
//	{
//		action = other.action;
//		key = other.key;
//		pressed = other.pressed;
//		once = other.once;
//		return *this;
//	}
//
//	virtual bool operator==(Event const& other) override
//	{
//		try { 
//			KeyEvent const& ke = typenum_cast<KeyEvent const&>(other); 
//			return ke.key==key;
//		}
//		catch (std::bad_cast) {
//			return false;
//		}
//	}
//
//	virtual bool trigger() const override
//	{
//		return sf::Keyboard::isKeyPressed(key);
//	}
//
//	virtual void process() override
//	{
//		if (trigger()) {
//			if (!pressed) {
//				pressed = true;
//				emit action();
//			}
//			else if (!once)	emit action();
//		}
//		else if (pressed)
//			pressed = false;
//	}
//
//	virtual void operator()() override
//	{
//		emit action();
//	}
//
//	inline operator WKey() const
//	{
//		return key;
//	}
//
//	friend class EventHandler;
//};
//
//
//class EventHandler
//{
//	std::atomic_bool running_;
//	x::vector<Event*> events_;
//
//public:
//	class error_k: public error
//	{
//	public:
//		enum type {
//			KEY_REPEATED
//		};
//
//		static const EventHandler::error_k key_repeated;
//
//		const int num;
//		const char* message;
//
//		error_k() = delete;
//		error_k(int num, const char* message = ""):
//			num{num}, message{message}
//		{}
//
//		~error_k()
//		{
//			//delete[] message;
//		}
//	};
//
//	EventHandler()
//	{}
//	//EventHandler(std::initializer_list<Event*> events):
//	//	events_{events}
//	//{}
//	void task()
//	{
//		for (auto& event : events_) {
//			event->process();
//		}
//	}
//
//	void process()
//	{
//		while (running_) task();
//	}
//
//	void run()
//	{
//		if (!running_) {
//			running_ = true;
//			std::thread processThread(&EventHandler::process, this);
//			processThread.join();
//		}
//	}
//
//	void add(Event* event)
//	{
//		try {
//			Event* same = events_.find<x::vector_opt::PTR_DEREF>(event);
//			same->action.attach(event->action);
//		}
//		catch (x::vector<Event*>::error) {
//			events_.push_back(event);
//		}
//	}
//
//	/*template<typename F>
//	void assign_key(WKey key, bool once, F action)
//	{
//		try {
//			KeyEvent& event = events_.find(key);
//			event.action.attach(action);
//		}
//		catch (x::vector<KeyEvent>::error_k) {
//			events_.push_back(new KeyEvent{key,once, action});
//		}
//	}*/
//
//	/*void clear_key(WKey key)
//	{
//		events_.remove(key);
//	}*/
//};
//
//const EventHandler::error_k EventHandler::error_k::key_repeated = 
//	EventHandler::error_k(EventHandler::error_k::type::KEY_REPEATED, 
//		"A key has been tried to assign more than once.");
//
//#endif //KEY_EVENT_H

#ifndef _X_KEY_EVENT_H_
#define _X_KEY_EVENT_H_

#include <thread>
#include <atomic>
#include "threaded.h"
#include "signals.h"

#include "typeenum.h"
#include "fn.h"
#include "wkey.h"


class Event
{
public:
	Event() = delete;
	Event(x::Callable<void()> const& action):
		action_{action.copy()}
	{}

protected:
	x::Callable<void()>* action_;

public:	

	virtual bool trigger() abstract;
	virtual void process() abstract;
	virtual void operator()()
	{
		action_->call();
	}

	template<x::ExceptionHandling>
	friend class EventHandler;

	virtual ~Event()
	{
		delete action_;
	}
};

class DynamicEvent:
	public Event
{
public:
	DynamicEvent() = delete;
	DynamicEvent(
		x::Callable<void()> const& action,
		x::Callable<bool()> const& trigger)
		:
		Event{action},
		trigger_{trigger.copy()}
	{
	}

	x::Callable<bool()>* trigger_;
public:

	virtual bool trigger() override
	{
		return trigger_->call();
	}
	virtual void process()
	{
		if (trigger_->call()) {
			action_->call();
		}
	}

	template<x::ExceptionHandling>
	friend class EventHandler;

	virtual ~DynamicEvent()
	{
		delete trigger_;
	}
};

class KeyEvent: public Event
{
protected:
	static const WKey DEFAULT_KEY = WKey::Empty;
	
	bool pressed;
	bool once;

public:
	KeyEvent() = delete;

	KeyEvent(
		bool once,
		x::Callable<void()> const& action)
		:
		Event{action},
		pressed{false}, 
		once{once}
	{
	}

	KeyEvent& operator=(const KeyEvent& other)
	{
		if (action_) delete action_;
		action_= other.action_->copy();
		//key = other.key;
		pressed = other.pressed;
		once = other.once;
		return *this;
	}

	/*inline operator WKey() const
	{
		return key;
	}*/

	template<x::ExceptionHandling>
	friend class EventHandler;
};

class KeyPressEvent: public KeyEvent
{
public:
	WKey key;

	KeyPressEvent() = delete;

	KeyPressEvent(
		WKey key,
		bool once,
		x::Callable<void()> const& fn)
		:
		KeyEvent{once, fn},
		key{key}
	{
		//std::cout<<"e";
	}

	virtual bool trigger() override
	{
		return isPressed(key);
	}

	virtual void process() override
	{
		if (trigger()) {
			if (!pressed) {
				pressed = true;
				action_->call();
			}
			else if (!once)	action_->call();
		}
		else if (pressed)
			pressed = false;
	}
};

class KeyReleaseEvent: public KeyEvent
{
public:
	WKey key;

	KeyReleaseEvent() = delete;

	KeyReleaseEvent(
		WKey key,
		bool once,
		x::Callable<void()> const& fn) 
		: 
		KeyEvent{once, fn},
		key{key}
	{
	}

	virtual bool trigger() override
	{
		return !isPressed(key);
	}

	virtual void process() override
	{
		if (trigger()) {
			if (pressed) {
				pressed = false;
				action_->call();
			}
			else if (!once)	action_->call();
		}
		else if (!pressed)
			pressed = true;
	}
};

class KeyCombinationEvent: public KeyEvent
{
public:
	x::vector<WKey> keys;

	KeyCombinationEvent() = delete;

	KeyCombinationEvent(
		std::initializer_list<WKey> keys,
		bool once,
		x::Callable<void()> const& fn)
		:
		KeyEvent{once, fn},
		keys{keys}
	{
	}

	virtual bool trigger() override
	{
		for (auto key{keys.cbegin()}; key; ++key) {
			if (!isPressed(*key))
				return false;
		}
		return true;
	}

	virtual void process() override
	{
		if (trigger()) {
			if (!pressed) {
				pressed = true;
				action_->call();
			}
			else if (!once)	action_->call();
		}
		else if (pressed)
			pressed = false;
	}
};


template<x::ExceptionHandling _exceptHndl = x::ExceptionHandling::NONE>
class EventHandler: public TimedProcess
{
private:
	x::vector<Event*> events_;


	template<x::ExceptionHandling _exceptHndl = _exceptHndl>
	inline std::enable_if_t<_exceptHndl == x::ExceptionHandling::NONE,
		void> task_()
	{
		for (auto& event : events_)
			event->process();
	}

	template<x::ExceptionHandling _exceptHndl = _exceptHndl>
	inline std::enable_if_t<_exceptHndl == x::ExceptionHandling::X_ERRORS, 
		void> task_()
	{
		for (auto& event : events_) {
			try {
				event->process();
			}
			catch (x::error<>& e) {
				std::cout << e;
			}
		}
	}

	template<x::ExceptionHandling _exceptHndl = _exceptHndl>
	inline std::enable_if_t<_exceptHndl == x::ExceptionHandling::ALL,
		void> task_()
	{
		for (auto& event : events_) {
			try {
				event->process();
			}
			catch (x::error<>& e) {
				std::cout << e;
			}
			catch (...) {
				std::cout << "Unknown exception.\n";
			}
		}
	}

public:	
	static constexpr double DEFAULT_PROCESS_PERIOD = 0.025;

	EventHandler(double processPeriod = DEFAULT_PROCESS_PERIOD):
		TimedProcess{processPeriod}
	{
	}

	virtual void task() override
	{
		task_();
	}

	void add(Event* event)
	{
		events_.push_back(event);
	}

	~EventHandler()
	{
		events_.delete_each();
		events_.clear();
		std::cout<<"~EvHan\n";
	}
};

#endif //_X_KEY_EVENT_H_
