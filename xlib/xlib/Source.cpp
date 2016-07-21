#include "xvector.h"
#include "keyevent.h"
#include "SFML\Graphics.hpp"
#include "signals.h"
#include "typeenum.h"
#include "simple.h"
#include "timer.h"
#include "loops.h"
#include "error.h"
#include "va_handler.h"
#include "clarg.h"
#include "list.h"
#include "realcall.h"
#include "auto_cast.h"
#include <fstream>
//#include "logic.h"

using namespace std;

#define enable_if std::enable_if_t


#include "keyevent.h"
#include <SFML\Graphics.hpp>
#include "crd.h"

int g() { exit(0); }

#define DEBUG 1

class Object;


class Object: public sf::Drawable
{
public:
	virtual void process(float time) abstract;
	virtual void putMoveDir(sf::Vector2f dir) abstract;
	virtual void draw(sf::RenderTarget&, sf::RenderStates = sf::RenderStates{}) const override abstract;
};

//template<class ObjectType>
//class Controller: public EventHandler<KeyEvent>
//{
//	using actionFn = void(ObjectType::*)();
//public:
//	
//	ObjectType* object_;
//
//	//static actionFn actions[8];
//
//	Controller(ObjectType* object):
//		object_{object}
//	{
//		//add(new KeyPressEvent{WKey::Down, true,[object = this->object_](){object->putMoveDir({0.0,1.0}); }});
//	}
//
//	/*void setDirLeft()
//	{
//		object_->putMoveDir({-1.0,0.0});
//	}
//
//	void setDirRight()
//	{
//		object_->putMoveDir({1.0,0.0});
//	}
//
//	void setDirUp()
//	{
//		object_->putMoveDir({0.0,-1.0});
//	}
//
//	void setDirDown()
//	{
//		object_->putMoveDir({0.0,1.0});
//	}
//*/
//	void assignKey(WKey key, void(Controller::*action)())
//	{
//
//	}
//};

//Controller::actionFn Controller::actions[8]{
//	&Controller::setDirLeft,
//	&Controller::setDirRight,
//	&Controller::setDirUp,
//	&Controller::setDirDown
//};

void normalize(sf::Vector2f& vec)
{
	float len = sqrt(vec.x*vec.x+vec.y*vec.y);
	if (len) vec /= len;
}

class DynamicObject: public Object
{

};

class ControlledObject: public DynamicObject
{
	void setEvents_()
	{
		controller.add(new KeyPressEvent{WKey::Left,true,this,&ControlledObject::moveLeft});
		controller.add(new KeyReleaseEvent{WKey::Left,true,this,&ControlledObject::moveRight});
		controller.add(new KeyPressEvent{WKey::Right,true,this,&ControlledObject::moveRight});
		controller.add(new KeyReleaseEvent{WKey::Right,true,this,&ControlledObject::moveLeft});
		controller.add(new KeyPressEvent{WKey::Up,true,this,&ControlledObject::moveUp});
		controller.add(new KeyReleaseEvent{WKey::Up,true,this,&ControlledObject::moveDown});
		controller.add(new KeyPressEvent{WKey::Down,true,this,&ControlledObject::moveDown});
		controller.add(new KeyReleaseEvent{WKey::Down,true,this,&ControlledObject::moveUp});
	}
public:
	EventHandler<KeyEvent> controller;
	sf::CircleShape shape;
	sf::Vector2f dir_;
	float speed;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(shape);
	}
	virtual void putMoveDir(sf::Vector2f dir) override
	{
		dir_ = {(float)x::sgn(dir_.x),(float)x::sgn(dir_.y)};
		this->dir_ += dir;
		normalize(dir_);
	}

	void moveLeft()
	{
		putMoveDir({-1.0,0.0});
	}

	void moveRight()
	{
		putMoveDir({1.0,0.0});
	}

	void moveUp()
	{
		putMoveDir({0.0,-1.0});
	}

	void moveDown()
	{
		putMoveDir({0.0,1.0});
	}

	void process(float time) override
	{
		shape.move(dir_*(time*speed));
		//cout<<shape.getPosition().x<<" "<<shape.getPosition().y<<endl;
	}
	ControlledObject():
		shape{30},
		dir_{0,0},
		speed{400}
	{
		setEvents_();
		controller.run();
	}
};

class Application: public MainThread
{
#if DEBUG == 1 
public:
#endif
	sf::RenderWindow window_;
	x::vector<Object*> objects_;
	x::timer<> timer_;

	void processObjects(float time)
	{
		for (Object* obj : objects_) {
			obj->process(time);
			obj->draw(window_);
			window_.draw(*obj);
		}
	}

	virtual void process() override
	{
		sf::Event event;
		timer_.tic();
		while (window_.isOpen()) {
			window_.pollEvent(event);
			if (event.type==sf::Event::Closed) window_.close();
			//if (timer_.toc()<0.001) Sleep(1);
			processObjects(timer_.measure());
			window_.display();
			window_.clear();
		}
	}

public:
	EventHandler<> eventHandler;

	Application():
		window_{sf::VideoMode{800,600}, "window"}
	{
		add(eventHandler);
	}

	void addObject(Object* obj)
	{
		objects_.push_back(obj);
	}
};

#define setKey(_key, _action) assignKey(WKey::_key, &Object::_action)

using namespace std;
using namespace x;

int main(int argc, char* argv[])
{
	Application app;
	ControlledObject elem;
	elem.shape.setPosition(100, 100);
	app.addObject(&elem);

	app.eventHandler.add(new KeyReleaseEvent(WKey::End, true, g));

	//app.eventHandler.add(new KeyPressEvent(WKey::Right, true, [&elem]() {elem.dir += sf::Vector2f{1.0f, 0.0f}; /*normalize(elem.dir);*/ }));
	//app.eventHandler.add(new KeyReleaseEvent(WKey::Right, true, [&elem]() {elem.dir -= sf::Vector2f{1.0f, 0.0f}; /*normalize(elem.dir);*/ }));
	//app.eventHandler.add(new KeyPressEvent(WKey::Left, true, [&elem]() {elem.dir += sf::Vector2f{-1.0f, 0.0f}; /*normalize(elem.dir);*/ }));
	//app.eventHandler.add(new KeyReleaseEvent(WKey::Left, true, [&elem]() {elem.dir -= sf::Vector2f{-1.0f, 0.0f}; /*normalize(elem.dir);*/ }));
	//app.eventHandler.add(new KeyPressEvent(WKey::Up, true, [&elem]() {elem.dir += sf::Vector2f{0.0f, -1.0f}; /*normalize(elem.dir);*/ }));
	//app.eventHandler.add(new KeyReleaseEvent(WKey::Up, true, [&elem]() {elem.dir -= sf::Vector2f{0.0f, -1.0f}; /*normalize(elem.dir);*/ }));
	//app.eventHandler.add(new KeyPressEvent(WKey::Down, true, [&elem]() {elem.dir += sf::Vector2f{0.0f, 1.0f}; /*normalize(elem.dir);*/ }));
	//app.eventHandler.add(new KeyReleaseEvent(WKey::Down, true, [&elem]() {elem.dir -= sf::Vector2f{0.0f, 1.0f}; /*normalize(elem.dir);*/ }));

	//app.eventHandler.add(new KeyPressEvent(WKey::D, true, [&elem]() {elem.dir += sf::Vector2f{1.0f, 0.0f}; /*normalize(elem.dir);*/ }));
	//app.eventHandler.add(new KeyReleaseEvent(WKey::D, true, [&elem]() {elem.dir -= sf::Vector2f{1.0f, 0.0f}; /*normalize(elem.dir);*/ }));
	//app.eventHandler.add(new KeyPressEvent(WKey::A, true, [&elem]() {elem.dir += sf::Vector2f{-1.0f, 0.0f}; /*normalize(elem.dir);*/ }));
	//app.eventHandler.add(new KeyReleaseEvent(WKey::A, true, [&elem]() {elem.dir -= sf::Vector2f{-1.0f, 0.0f}; /*normalize(elem.dir);*/ }));
	//app.eventHandler.add(new KeyPressEvent(WKey::W, true, [&elem]() {elem.dir += sf::Vector2f{0.0f, -1.0f}; /*normalize(elem.dir);*/ }));
	//app.eventHandler.add(new KeyReleaseEvent(WKey::W, true, [&elem]() {elem.dir -= sf::Vector2f{0.0f, -1.0f}; /*normalize(elem.dir);*/ }));
	//app.eventHandler.add(new KeyPressEvent(WKey::S, true, [&elem]() {elem.dir += sf::Vector2f{0.0f, 1.0f}; /*normalize(elem.dir);*/ }));
	//app.eventHandler.add(new KeyReleaseEvent(WKey::S, true, [&elem]() {elem.dir -= sf::Vector2f{0.0f, 1.0f}; /*normalize(elem.dir);*/ }));

	app.run();
	return 0;	
}

//FnCall<void(int, double, int)> call(fn, 0,0,0);
//call.args_[0] = new x::Value<int>{0};
//call.args_[1] = new x::Value<int>{0};
//call.args_[2] = new x::Value<int>{0};

//x::tic();
//repeat(1000000) {
//	call();
//}
//cout<<x::toc()<<endl;

//x::tic();
//repeat(1000000) {
//	fn(0, 0, 0);
//}
//cout<<x::toc()<<endl;

//return 0;
//Application A;
//A.run();	



template<typename F>
using owner_of = decltype(_owner_of(F));

template<typename T, typename R, typename... A>
constexpr T _owner_of(R(T::*)(A...));

template<typename...>
struct _result;

template<typename R, typename... A>
struct _result<R(A...)>
{
	using type = R;
};

#define result(_fn) _result<std::remove_pointer_t<decltype(_fn)>>::type


//Functor F;
//auto lam1 = [](int i) {};
//auto f1 = Fn<void(int)>{lam1};
//auto f2 = Fn<void(*)(int)>{&test};
//auto f3 = Fn<Functor, void(int)>{new Functor,&Functor::operator()};
//auto f4 = signal(int) {
//	new Fn<void(int)>{void_cast(&test2)}
//};

