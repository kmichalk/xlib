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
#include "xrnd.h"
#include "auto_cast.h"
#include "crd_cast.h"
#include <fstream>
#include "geom.h"
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

using Point = crd<double>;
using Vector = crd<double>;


double RelationPointLSegment(Point const& P, Point const& begin, Point const& end)
{
	//cout<<"line "<<begin<<"  -  "<<end<<endl;
	double slope = abs(end.y-begin.y)/(end.x-begin.x);
	return (P.y - slope*P.x + slope*begin.x - begin.y) /
		(1 + x::pow2(slope));
}

bool RectContainsPoint(Point vertArr[4], Point const & P)
{
	//cout<<P<<endl;
	double field = 0;
	for (int i = 0; i<4; ++i) {
		field += triangleField(vertArr[i]-P, vertArr[(i+1)%4]-P);
	}
	return field <12.001;
}

bool RCP_2(Point vertArr[4], Point const & P)
{
	int in1 = 0, in2 = 0;
	for (int i = 0; i<4; ++i) {
		if (line<double>{vertArr[i], vertArr[(i+1)%4]}.relation(P) < 0) {
			if (in2) return false;
			(in1 ? in2 : in1) = i+1;
		}
	}
	return in2-in1==1 || (in2==4 && in1==1);
}

bool RCP(Point const& S, double a, double b, double angle, Point const& P)
{
	Vector SP = P-S;
	double angRel = norm_rf(SP.angle()-angle);
	angRel = fmod(angRel, PI);
	double dist2;
	double angLim = atan2(b, a);

	if (angRel < angLim || angRel > PI-angLim)
		dist2 = pow2(a)*(1+pow2(tan(angRel)));
	else 
		dist2 = pow2(b)*(1+pow2(tan(PI/2.0-angRel)));

	return dist2 > SP.len2();
}

struct S
{
	S(int) { cout<<"int"; }
	S(double) { cout<<"double"; }
	S(float) { cout<<"float"; }
};


template<int _num = 1, int _denom = 1>
constexpr double _PI = (double)_num / (double)_denom * PI;

#include "ct_algorithm.h"
#include "autolim.h"


int main(int argc, char* argv[])
{
	sf::RenderWindow window{sf::VideoMode{1000,1000},"w"};
	S s{5.0f};
	Point vertArr[4]{{2,2},{3,8},{7,4},{4,1}};


	/*double p;
	tic();
	repeat(10000000000) {
		tan(100);
	}
	cout<<toc()<<endl; 
	tic();
	repeat(10000000) {
		p = PI/7.0;
	}
	cout<<toc()<<endl;
*/
	//return 0;
	/*tic();
	for (double i = 0; i<10; i += 0.003) {
		for (double j = 0; j<10; j += 0.003) {
			RectContainsPoint(vertArr, {i,j});
		}
	}
	cout<<toc()<<endl;

	tic();
	for (double i = 0; i<10; i += 0.003) {
		for (double j = 0; j<10; j += 0.003) {
			RCP_2(vertArr, {i,j});
		}
	}
	cout<<toc()<<endl;*/

	for (double i=0;i<10;i+=0.05){
		for (double j = 0; j<10; j += 0.05) {
			Point P{i,j};
			sf::CircleShape c{1};
			c.setPosition(P.x*100, P.y*-100+1000);
			if (RCP_2(vertArr,P)) {
			//if (RCP({5,3}, 1, 2, 3, P)) {
				c.setFillColor(sf::Color::Red);
			}
			else {
				c.setFillColor({100,100,100});
			}
			window.draw(c);
			//window.display();
		}
	}
	window.display();
	system("pause");
	return 0;
	tic();
	repeat(10000000) {

	}
	cout<<toc()<<endl;

	tic();
	repeat(10000000) {
		
	}
	cout<<toc()<<endl;
	
	tic();
	repeat(10000000) {

	}
	cout<<toc()<<endl;

	return 0;
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

