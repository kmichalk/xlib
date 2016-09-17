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
#include "ctmath.h"
#include "autolim.h"
#include "ctarray.h"
#include "disp.h"
#include "listed.h"
#include "geom.h"
#include "finally.h"
#include "keyevent.h"
#include <SFML\Graphics.hpp>
#include "crd.h"
#include "static_if.h"
//#include "logic.h"

using namespace std;

#define enable_if std::enable_if_t

#pragma region OLD

int g() { exit(0); }

#define DEBUG 1

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

//class DynamicObject: public Object
//{
//
//};
//
//class ControlledObject: public DynamicObject
//{
//	void setEvents_()
//	{
//		/*controller.add(new KeyPressEvent{WKey::Left,true,this,&ControlledObject::moveLeft});
//		controller.add(new KeyReleaseEvent{WKey::Left,true,this,&ControlledObject::moveRight});
//		controller.add(new KeyPressEvent{WKey::Right,true,this,&ControlledObject::moveRight});
//		controller.add(new KeyReleaseEvent{WKey::Right,true,this,&ControlledObject::moveLeft});
//		controller.add(new KeyPressEvent{WKey::Up,true,this,&ControlledObject::moveUp});
//		controller.add(new KeyReleaseEvent{WKey::Up,true,this,&ControlledObject::moveDown});
//		controller.add(new KeyPressEvent{WKey::Down,true,this,&ControlledObject::moveDown});
//		controller.add(new KeyReleaseEvent{WKey::Down,true,this,&ControlledObject::moveUp});*/
//	}
//public:
//	EventHandler<KeyEvent> controller;
//	sf::CircleShape shape;
//	sf::Vector2f dir_;
//	float speed;
//
//	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
//	{
//		target.draw(shape);
//	}
//	virtual void putMoveDir(sf::Vector2f dir) override
//	{
//		dir_ = {(float)x::sgn(dir_.x),(float)x::sgn(dir_.y)};
//		this->dir_ += dir;
//		//normalize(dir_);
//	}
//
//	void moveLeft()
//	{
//		putMoveDir({-1.0,0.0});
//	}
//
//	void moveRight()
//	{
//		putMoveDir({1.0,0.0});
//	}
//
//	void moveUp()
//	{
//		putMoveDir({0.0,-1.0});
//	}
//
//	void moveDown()
//	{
//		putMoveDir({0.0,1.0});
//	}
//
//	void process(float time) override
//	{
//		shape.move(dir_*(time*speed));
//		//cout<<shape.getPosition().x<<" "<<shape.getPosition().y<<endl;
//	}
//	ControlledObject():
//		shape{30},
//		dir_{0,0},
//		speed{400}
//	{
//		setEvents_();
//		controller.run();
//	}
//};
//
//class Application: public MainThread
//{
//#if DEBUG == 1 
//public:
//#endif
//	sf::RenderWindow window_;
//	x::vector<Object*> objects_;
//	x::timer<> timer_;
//
//	void processObjects(float time)
//	{
//		for (Object* obj : objects_) {
//			obj->process(time);
//			obj->draw(window_);
//			window_.draw(*obj);
//		}
//	}
//
//	virtual void process() override
//	{
//		sf::Event event;
//		timer_.tic();
//		while (window_.isOpen()) {
//			window_.pollEvent(event);
//			if (event.type==sf::Event::Closed) window_.close();
//			//if (timer_.toc()<0.001) Sleep(1);
//			processObjects(timer_.measure());
//			window_.display();
//			window_.clear();
//		}
//	}
//
//public:
//	EventHandler<> eventHandler;
//
//	Application():
//		window_{sf::VideoMode{800,600}, "window"}
//	{
//		add(eventHandler);
//	}
//
//	void addObject(Object* obj)
//	{
//		objects_.push_back(obj);
//	}
//};
//
//#define setKey(_key, _action) assignKey(WKey::_key, &Object::_action)
#pragma endregion


using Point = crd<double>;
using Vector = crd<double>;

#pragma region COLLISION

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
using basic_t = double;

bool RCP_2(Point vertArr[4], Point const & P, Point const& C, Point& colPos, Vector& colDir)
{
	double relArr[4];
	int imin;
	basic_t d;
	basic_t distMin = std::numeric_limits<basic_t>::max();
	for (int i = 0; i<4; ++i) {
		line<basic_t> edge{vertArr[i], vertArr[(i+1)%4]};
		relArr[i] = x::sgn(edge.relation(P));
		
		if (x::sgn(edge.relation(C))!=relArr[i] && (d = edge.dist2(P)) < distMin) {
			distMin = d;
			imin = i;
		}		
	}
		
	if (relArr[0]!=relArr[2] && relArr[1]!=relArr[3]) {
		colPos = P;
		colDir = (vertArr[(imin+1)%4]-vertArr[imin]).rot90p().setLen(sqrt(distMin)+0.0001);
		return true;
	}
	return false;
}

struct Rectangle
{
	Point pos;
	double a, b, angle;

	Rectangle(Point const& pos, double a, double b, double angle):
		pos{pos},a{a},b{b},angle{angle}{}
};

bool RCP(Point const& S, double a, double b, double angle, Point const& P)
{
	Vector SP = P-S;
	double angRel = x::norm_rf(SP.angle()-angle);
	angRel = fmod(angRel, PI);
	double dist2;
	double angLim = atan2(b, a);

	if (angRel < angLim || angRel > PI-angLim)
		dist2 = x::pow2(a)*(1+x::pow2(tan(angRel)));
	else 
		dist2 = x::pow2(b)*(1+x::pow2(tan(PI/2.0-angRel)));

	return dist2 > SP.len2();
}

#pragma endregion

template<int _num = 1, int _denom = 1>
constexpr double _PI = (double)_num / (double)_denom * PI;

#define e 2.71828182845904523536

#define rename(_from, _to) static auto& _to{_from}


class Object
{
public:
	Point position_;
	double bound_;
	sf::Color color;
	Object(Point const& position, double bound): 
		position_{position}, 
		bound_{bound},
		color{sf::Color::White}
	{}
	void display(sf::RenderWindow& window)
	{
		sf::CircleShape c{(float)bound_};
		c.setFillColor(color);
		c.setOrigin(bound_, bound_);
		c.setPosition(crd_cast<sf::Vector2f>(position_));
		window.draw(c);
	}
};

class QuadTree
{
public:
	Point boundMin_;
	Point boundMax_;
	QuadTree* children_[2][2];
	x::vector<Object*> elements_;

public:
	static constexpr size_t DEF_ELEMENTS_ALLOC_SIZE = 10;

	QuadTree(Point const& boundMin, Point const& boundMax):
		boundMin_{boundMin},
		boundMax_{boundMax},
		children_{nullptr},
		elements_{DEF_ELEMENTS_ALLOC_SIZE}
	{
	}

	bool contains(Object* obj) const
	{
		return
			obj->position_.x - obj->bound_ > boundMin_.x &&
			obj->position_.x + obj->bound_ < boundMax_.x &&
			obj->position_.y - obj->bound_ > boundMin_.y &&
			obj->position_.y + obj->bound_ < boundMax_.y;

	}

	bool align(Object* obj)
	{
		//disp endl;
		//disp obj->position_, endl;
		//disp "boundMin: ", boundMin_, " boundMax: ", boundMax_, endl;
		if (!contains(obj))
			return false;

		Point center = /*children_[0][0] ?
			children_[0][0]->boundMax_ :*/
			(boundMax_-boundMin_)/2 + boundMin_;
		//disp "center: ", center, endl;
		bool left = obj->position_.x < center.x,
			down = obj->position_.y < center.y;
		//disp left, " ", down, endl;
		QuadTree*& nextChild = children_[left][down];
		if (!nextChild) {
			nextChild =
				new QuadTree{
					{left ? boundMin_.x : center.x, down ? boundMin_.y : center.y},
					{left ? center.x : boundMax_.x, down ? center.y : boundMax_.y}};
		}
		if (!nextChild->align(obj))
			elements_.push_back(obj);
		return true;
	}

	void process()
	{
		if (elements_) {
			for (auto elem1 = elements_.begin(); elem1!=elements_.end();++elem1) {
				for (auto elem2 = elem1+1u; elem2!=elements_.end(); ++elem2) {
					if (hypot((*elem2)->position_-(*elem1)->position_) < ((*elem1)->bound_ + (*elem2)->bound_)) {
						(*elem1)->color = sf::Color::Red;
						(*elem2)->color = sf::Color::Red;
					}
				}
			}
			for (int i = 0; i<2; ++i) {
				for (int j = 0; j<2; ++j) {
					if (children_[i][j]) {
						children_[i][j]->process();
						children_[i][j]->process(elements_);
					}
				}
			}
		}
		else {
			for (int i = 0; i<2; ++i) {
				for (int j = 0; j<2; ++j) {
					if (children_[i][j]) children_[i][j]->process();
				}
			}
		}
	}
	

	void process(x::vector<Object*>& objects)
	{
		for (auto elem1 = elements_.begin(); elem1!=elements_.end(); ++elem1) {
			for (auto elem2 = objects.begin(); elem2!=objects.end(); ++elem2) {
				if (hypot((*elem2)->position_-(*elem1)->position_) < ((*elem1)->bound_ + (*elem2)->bound_)) {
					(*elem1)->color = sf::Color::Red;
					(*elem2)->color = sf::Color::Red;
				}
			}
		}
		for (auto elem1 = elements_.begin(); elem1!=elements_.end(); ++elem1) {
			for (auto elem2 = elem1+1u; elem2!=elements_.end(); ++elem2) {
				if (hypot((*elem2)->position_-(*elem1)->position_) < ((*elem1)->bound_ + (*elem2)->bound_)) {
					(*elem1)->color = sf::Color::Red;
					(*elem2)->color = sf::Color::Red;
				}
			}
		}
		for (int i = 0; i<2; ++i) {
			for (int j = 0; j<2; ++j) {
				if (children_[i][j]) {
					children_[i][j]->process(elements_);
					children_[i][j]->process(objects);
				}
			}
		}
	}

	void display(sf::RenderWindow& window)
	{
		
		/*Point center = children_[0][0] ?
			children_[0][0]->boundMax_ :
			(boundMax_-boundMin_)/2 + boundMin_;*/
		if (elements_) {
			sf::RectangleShape rect{{float(boundMax_.x-boundMin_.x), float(boundMax_.y-boundMin_.y)}};
			rect.setFillColor({0,0,0,0});
			rect.setOutlineColor(sf::Color::White);
			rect.setOutlineThickness(1);
			rect.setPosition(crd_cast<sf::Vector2f>(boundMin_));
			window.draw(rect);
			for (auto obj : elements_) {
				obj->display(window);
			}
		}
		for (int i = 0; i<2; ++i) {
			for (int j = 0; j<2; ++j) {
				if (children_[i][j]) children_[i][j]->display(window);
			}
		}
	}
};


#define literal static constexpr

#define cthis const_cast<const decltype(this)>(this)

template<typename T>
void tf(T val) {
	static_if(is_integral(T)) {
		disp "T is integral\n";
	}

	static_else{
		disp "T is not integral\n";
	}
	
	static_else{
		disp "T is not class\n";
	}
}

#define CLASS_DEFAULTS

template<typename T>
class V
{
public:
	literal int i{0};

	void t(int){}
	void f() {
		//literal int j{0};
		//(this->*static_if(j==1, &V::f, &V::t))(0);
	}
	void g() {
		//(this->*(j==1 ? &V::g : &V::t))(0);
	}
	//owner_of(&decltype(this)::g)* v = nullptr;
};
//
//constexpr int fi(int i) { return i; }
//#define struct(_Name) struct _Name{ private:using ThisType_=_Name;public:
//#define class(_Name) class _Name{using ThisType_=_Name;
//#define this_type ThisType_
//

class A
{
public:
	int ch;
	
	virtual ~A(){}
	virtual void r(){}
	virtual void u() {}

};



class B: virtual public A 
{
public:
	int cx = 'X';
	virtual void q() {}
	virtual void t() {}
	//void r() override {};
	~B() {}
};

class C: virtual public A
{
public:
	static constexpr 

	int sx = 'X';
	virtual void q() {}
	virtual void t() {}
	//void r() override {};
	~C() {}
};

class D: public C, public B
{
public:
	int p = 761;
};

template<class _Base, class _Derived>
size_t offset()
{
	_Derived* derivPtr{new _Derived};
	size_t result{size_t(force_cast<char*>(dynamic_cast<_Base*>(derivPtr))-force_cast<char*>(derivPtr))};
	delete derivPtr;
	return result;
}

struct Q
{
	static x::vector<size_t, 2> v;
	static void assign()
	{
		v[typenum(A)][typenum(D)] = offset<A, D>();
		v[typenum(A)][typenum(D)] = offset<D, A>();
	}
};

x::vector<size_t, 2> Q::v{};

template<class _To, class _From>
__forceinline _To offset_cast(_From* ptr)
{
	return force_cast<_To>(force_cast<byte*>(ptr)-Q::v[typenum(_From)][typenum(std::remove_pointer_t<_To>)]);
}

template<typename _Type>
class arr
{
	_Type* data_;
	size_t size_;
public:
	static constexpr size_t typeSize{sizeof(_Type)};

	__forceinline constexpr arr():
		data_{nullptr},
		size_{0}
	{
	}

	__forceinline arr(size_t size):
		data_{new _Type[size]},
		size_{size}
	{
	}
	
	template<typename... _InitArgs>
	__forceinline arr(_InitArgs... args):
		data_{new _Type[sizeof...(args)]{args...}},
		size_{sizeof...(args)}
	{
	}

	__forceinline arr(arr<_Type> const& other):
		data_{new _Type[other.size_]},
		size_{other.size_}
	{
		memcpy(data_, other.data_, size_*typeSize);
	}

	__forceinline arr(arr<_Type>&& other):
		data_{other.data_},
		size_{other.size_}
	{
	}

	__forceinline size_t size() const
	{
		return size_;
	}

	__forceinline operator bool() const
	{
		return bool(size_);
	}

	__forceinline bool empty() const
	{
		return !bool(size_);
	}

	__forceinline _Type& operator[](size_t n)
	{
		return data_[n];
	}

	__forceinline ~arr()
	{
		if (data_) delete[] data_;
	}
};

int main()
{
	arr<int> a{1,2,3,4,5,6,7};
	x::vector<int> v{1,2,3,4,5,6,7};

	x::tic();
	repeat(1000000) {
		arr<int> ac{a};
	}
	disp x::toc(), endl;

	x::tic();
	repeat(1000000) {
		x::vector<int> vc{v};
	}
	disp x::toc(), endl;
	/*A* ap{new D};
	disp offset<D, A>();*/
	//disp dynamic_cast<D*>(ap)->p;
	//disp offset_cast<D*>(ap)->p;
}

//int main(int argc, char* argv[])
//{
//	
//	return 0;
//
//	V<int> v;
//	repeat(1000000000) {
//		int i = 0;
//	}
//
//	x::tic();
//	repeat(1000000000) {
//		v.f();
//	}
//	disp x::toc(), endl;
//
//	x::tic();
//	repeat(1000000000) {
//		v.g();
//	}
//	disp x::toc(), endl;
//
//	/*x::tic();
//	repeat(100000000) {
//	}
//	disp x::toc(), endl;
//
//	x::tic();
//	repeat(100000000) {
//	}
//	disp x::toc(), endl;
//
//	x::tic();*/
//	/*repeat(100000000) {
//		int o = -1;
//		for(auto&& i : arr) {
//			i = ++o;
//		}
//	}
//	disp x::toc(), endl;*/
//	return 0;
//
//	//disp o.get<3>(), " ", o.get<2>(),  " ", o.get<1>(), endl;
//	sf::RenderWindow window{sf::VideoMode{1000,1000},"w"};
//	Point vertArr[4]{{2,1},{1,4},{7,6},{8,3}};
//	Point C{100,1};
//
//	/*x::vector<Object*> vo;
//	for (auto&& i:vo) {
//		i = new Object{{x::random<double>(0,1000),x::random<double>(0,1000)},x::random<double>(0,10)};
//	}
//
//	QuadTree tree{{0,0},{1000,1000}};
//	for (int i = 0; i<200; ++i) {
//		tree.align(new Object{{x::random<double>(0,1000),x::random<double>(0,1000)},x::random<double>(0,30)});
//	}
//	tree.process();
//	tree.display(window);
//	window.display();
//	system("pause");*/
//	x::tic();
//	for (double i=0;i<10;i+=0.15){
//		for (double j = 0; j<10; j += 0.15) {
//			Point P{i,j};
//			Point colPos;
//			Vector colDir;
//			sf::CircleShape c{2};
//			c.setOrigin(1, 1);
//			c.setPosition(P.x*100, P.y*-100+1000);
//			//sf::Vertex line[2]{sf::Vector2f{100,100},sf::Vector2f{200,200}};
//			
//			if (RCP_2(vertArr,P,C,colPos,colDir)) {
//				sf::Vertex line[2]{c.getPosition(),c.getPosition()+sf::Vector2f{(float)colDir.x*100,(float)colDir.y*-100}};
//				window.draw(line, 2, sf::PrimitiveType::Lines);
//				c.setFillColor(sf::Color::Red);
//			}
//			else {
//				c.setFillColor({100,100,100});
//			}
//			window.draw(c);
//			//window.display();
//		}
//	}
//	disp x::toc(), endl;
//	window.display();
//	system("pause");
//	return 0;
//	
//	/*Application app;
//	ControlledObject elem;
//	elem.shape.setPosition(100, 100);
//	app.addObject(&elem);*/
//
////	app.eventHandler.add(new KeyReleaseEvent(WKey::End, true, g));
//
//	//app.eventHandler.add(new KeyPressEvent(WKey::Right, true, [&elem]() {elem.dir += sf::Vector2f{1.0f, 0.0f}; /*normalize(elem.dir);*/ }));
//	//app.eventHandler.add(new KeyReleaseEvent(WKey::Right, true, [&elem]() {elem.dir -= sf::Vector2f{1.0f, 0.0f}; /*normalize(elem.dir);*/ }));
//	//app.eventHandler.add(new KeyPressEvent(WKey::Left, true, [&elem]() {elem.dir += sf::Vector2f{-1.0f, 0.0f}; /*normalize(elem.dir);*/ }));
//	//app.eventHandler.add(new KeyReleaseEvent(WKey::Left, true, [&elem]() {elem.dir -= sf::Vector2f{-1.0f, 0.0f}; /*normalize(elem.dir);*/ }));
//	//app.eventHandler.add(new KeyPressEvent(WKey::Up, true, [&elem]() {elem.dir += sf::Vector2f{0.0f, -1.0f}; /*normalize(elem.dir);*/ }));
//	//app.eventHandler.add(new KeyReleaseEvent(WKey::Up, true, [&elem]() {elem.dir -= sf::Vector2f{0.0f, -1.0f}; /*normalize(elem.dir);*/ }));
//	//app.eventHandler.add(new KeyPressEvent(WKey::Down, true, [&elem]() {elem.dir += sf::Vector2f{0.0f, 1.0f}; /*normalize(elem.dir);*/ }));
//	//app.eventHandler.add(new KeyReleaseEvent(WKey::Down, true, [&elem]() {elem.dir -= sf::Vector2f{0.0f, 1.0f}; /*normalize(elem.dir);*/ }));
//
//	//app.eventHandler.add(new KeyPressEvent(WKey::D, true, [&elem]() {elem.dir += sf::Vector2f{1.0f, 0.0f}; /*normalize(elem.dir);*/ }));
//	//app.eventHandler.add(new KeyReleaseEvent(WKey::D, true, [&elem]() {elem.dir -= sf::Vector2f{1.0f, 0.0f}; /*normalize(elem.dir);*/ }));
//	//app.eventHandler.add(new KeyPressEvent(WKey::A, true, [&elem]() {elem.dir += sf::Vector2f{-1.0f, 0.0f}; /*normalize(elem.dir);*/ }));
//	//app.eventHandler.add(new KeyReleaseEvent(WKey::A, true, [&elem]() {elem.dir -= sf::Vector2f{-1.0f, 0.0f}; /*normalize(elem.dir);*/ }));
//	//app.eventHandler.add(new KeyPressEvent(WKey::W, true, [&elem]() {elem.dir += sf::Vector2f{0.0f, -1.0f}; /*normalize(elem.dir);*/ }));
//	//app.eventHandler.add(new KeyReleaseEvent(WKey::W, true, [&elem]() {elem.dir -= sf::Vector2f{0.0f, -1.0f}; /*normalize(elem.dir);*/ }));
//	//app.eventHandler.add(new KeyPressEvent(WKey::S, true, [&elem]() {elem.dir += sf::Vector2f{0.0f, 1.0f}; /*normalize(elem.dir);*/ }));
//	//app.eventHandler.add(new KeyReleaseEvent(WKey::S, true, [&elem]() {elem.dir -= sf::Vector2f{0.0f, 1.0f}; /*normalize(elem.dir);*/ }));
//
//	//app.run();
//	return 0;	
//}

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

//Functor F;
//auto lam1 = [](int i) {};
//auto f1 = Fn<void(int)>{lam1};
//auto f2 = Fn<void(*)(int)>{&test};
//auto f3 = Fn<Functor, void(int)>{new Functor,&Functor::operator()};
//auto f4 = signal(int) {
//	new Fn<void(int)>{void_cast(&test2)}
//};

 