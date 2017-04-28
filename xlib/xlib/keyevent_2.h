////#ifndef KEY_EVENT_H
////#define KEY_EVENT_H
////
////#include <thread>
////#include <atomic>
////#include "threaded.h"
////#include "signals.h"
////#include "loops.h"
////#include "typeenum.h"
////#include "SFML\Graphics.hpp"
////#include "error.h"
////
////
////class Event: enumed
////{
////protected:	
////	signal() action;
////
////	Event() : typenum()
////	{}
////
////public:	
////	virtual bool trigger() const abstract;
////	virtual void process() abstract;
////	virtual void operator()() abstract;
////	virtual bool operator==(Event const& other)
////	{
////		return num_==other.num_;
////	}
////
////	friend class EventHandler;
////};
////
////class KeyEvent: public Event, enumed
////{
////	static const sf::Keyboard::Key DEF_KEY = sf::Keyboard::Key::Unknown;
////
////	sf::Keyboard::Key key;
////	bool pressed;
////	bool once;
////
////public:
////	KeyEvent() : typenum(),
////		key{DEF_KEY}, pressed{false}, once{true}
////	{}
////
////	template<typename F>
////	KeyEvent(
////		sf::Keyboard::Key key,
////		bool once,
////		F slot_fn) : typenum(),
////		key{key}, pressed{false}, once{once}
////	{
////		static_assert(is_free_func_ptr<F>::value,
////			"This KeyEvent constructor only accepts function pointers.");
////		action.attach(slot_fn);
////	}
////
////	template<typename T, typename F>
////	KeyEvent(
////		sf::Keyboard::Key key,
////		bool once,
////		T* obj,
////		F slot_fn) : typenum(),
////		key{key}, pressed{false}, once{once}
////	{
////		static_assert(is_member_func_ptr<F>::value,
////			"This KeyEvent constructor only accepts member function pointers.");
////		action.attach(obj, slot_fn);
////	}
////
////	/*template<typename... S>
////	KeyEvent(
////		sf::Keyboard::Key key,
////		bool once,
////		S*... slot_fn) :
////		action{slot_fn...}, key{key}, pressed{false}, once{once}
////	{
////		static_assert(
////			all_true<std::is_base_of<_Slot<>, S>::value...>::value,
////			"This KeyEvent constructor only accepts pointers to Slots.");
////	}*/
////
////	KeyEvent& operator=(const KeyEvent& other)
////	{
////		action = other.action;
////		key = other.key;
////		pressed = other.pressed;
////		once = other.once;
////		return *this;
////	}
////
////	virtual bool operator==(Event const& other) override
////	{
////		try { 
////			KeyEvent const& ke = typenum_cast<KeyEvent const&>(other); 
////			return ke.key==key;
////		}
////		catch (std::bad_cast) {
////			return false;
////		}
////	}
////
////	virtual bool trigger() const override
////	{
////		return sf::Keyboard::isKeyPressed(key);
////	}
////
////	virtual void process() override
////	{
////		if (trigger()) {
////			if (!pressed) {
////				pressed = true;
////				emit action();
////			}
////			else if (!once)	emit action();
////		}
////		else if (pressed)
////			pressed = false;
////	}
////
////	virtual void operator()() override
////	{
////		emit action();
////	}
////
////	inline operator sf::Keyboard::Key() const
////	{
////		return key;
////	}
////
////	friend class EventHandler;
////};
////
////
////class EventHandler
////{
////	std::atomic_bool running_;
////	x::vector<Event*> events_;
////
////public:
////	class error_k: public error
////	{
////	public:
////		enum type {
////			KEY_REPEATED
////		};
////
////		static const EventHandler::error_k key_repeated;
////
////		const int num;
////		const char* message;
////
////		error_k() = delete;
////		error_k(int num, const char* message = ""):
////			num{num}, message{message}
////		{}
////
////		~error_k()
////		{
////			//delete[] message;
////		}
////	};
////
////	EventHandler()
////	{}
////	//EventHandler(std::initializer_list<Event*> events):
////	//	events_{events}
////	//{}
////	void task()
////	{
////		for (auto& event : events_) {
////			event->process();
////		}
////	}
////
////	void process()
////	{
////		while (running_) task();
////	}
////
////	void run()
////	{
////		if (!running_) {
////			running_ = true;
////			std::thread processThread(&EventHandler::process, this);
////			processThread.join();
////		}
////	}
////
////	void add(Event* event)
////	{
////		try {
////			Event* same = events_.find<x::vector_opt::PTR_DEREF>(event);
////			same->action.attach(event->action);
////		}
////		catch (x::vector<Event*>::error) {
////			events_.push_back(event);
////		}
////	}
////
////	/*template<typename F>
////	void assign_key(sf::Keyboard::Key key, bool once, F action)
////	{
////		try {
////			KeyEvent& event = events_.find(key);
////			event.action.attach(action);
////		}
////		catch (x::vector<KeyEvent>::error_k) {
////			events_.push_back(new KeyEvent{key,once, action});
////		}
////	}*/
////
////	/*void clear_key(sf::Keyboard::Key key)
////	{
////		events_.remove(key);
////	}*/
////};
////
////const EventHandler::error_k EventHandler::error_k::key_repeated = 
////	EventHandler::error_k(EventHandler::error_k::type::KEY_REPEATED, 
////		"A key has been tried to assign more than once.");
////
////#endif //KEY_EVENT_H
//
//#ifndef KEY_EVENT_H
//#define KEY_EVENT_H
//
//#include <thread>
//#include <atomic>
//#include "threaded.h"
//#include "signals.h"
//#include "loops.h"
//#include "typeenum.h"
//#include "SFML\Graphics.hpp"
//#include "error.h"
//
//
//class Event : enumed
//{
//protected:
//	Event() : typenum()
//	{}
//	Event(signal() const& action):
//		action{action}
//	{}
//
//public:
//	signal() action;
//
//	virtual bool trigger() const abstract;
//	virtual void process() abstract;
//	virtual void operator()()
//	{
//		emit action();
//	}
//	virtual bool operator==(Event const& other)
//	{
//		return num_==other.num_;
//	}
//
//	friend class EventHandler;
//};
//
//class KeyEvent : public Event, enumed
//{
//	static const sf::Keyboard::Key DEF_KEY = sf::Keyboard::Key::Unknown;
//
//	sf::Keyboard::Key key;
//	bool pressed;
//	bool once;
//
//public:
//	KeyEvent() : typenum(),
//		key{DEF_KEY}, pressed{false}, once{true}
//	{}
//
//	template<typename F>
//	KeyEvent(
//		sf::Keyboard::Key key,
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
//		sf::Keyboard::Key key,
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
//	sf::Keyboard::Key key,
//	bool once,
//	S*... slot_fn) :
//	action{slot_fn...}, key{key}, pressed{false}, once{once}
//	{
//	static_assert(
//	all_true<std::is_base_of<_Slot<>, S>::value...>::value,
//	"This KeyEvent constructor only accepts pointers to Slots.");
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
//	inline operator sf::Keyboard::Key() const
//	{
//		return key;
//	}
//
//	friend class EventHandler;
//};
//
//class EventHandler: public ThreadedProcess
//{
//	x::vector<Event*> events_;
//
//public:
//	EventHandler()
//	{
//	}
//
//	virtual void task() override
//	{
//		for (auto& event : events_) {
//			event->process();
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
//	~EventHandler()
//	{
//		events_.clear<x::vector_opt::PTR_DELETE>();
//	}
//};
//
//#endif //KEY_EVENT_H
