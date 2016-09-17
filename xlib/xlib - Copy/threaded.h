#ifndef THREADED_H
#define THREADED_H

#include <atomic>
#include <thread>
#include "xvector.h"
#include "timer.h"

#define enable_if std::enable_if_t

class Threaded
{
public:
	std::thread thread_;
	std::atomic_bool running_;

	Threaded();
	
public:
	virtual void task() abstract;
	virtual void stop() abstract;
	virtual void process();	
	inline void run();
	virtual ~Threaded();

	friend class MainThread;
};

class ThreadedTask : public Threaded
{
	void process() final override;
	void stop() final override;
};

#define __setProcess Threaded(&std::remove_pointer_t<decltype(this)>::process,this)

class ThreadedProcess: public Threaded
{	
protected:
	virtual void task() abstract override;
	virtual void stop() override;
	virtual void process() override;
public:
	ThreadedProcess();
	virtual ~ThreadedProcess();
};

class TimedProcess: public ThreadedProcess
{
	double processPeriod_;
	x::timer<std::chrono::milliseconds> processTimer_;
	virtual void process() override;
public:
	TimedProcess(double processPeriod);
};

class MainThread
{
	std::atomic_bool running_;
	x::vector<Threaded*> processes_;
	
public:
	MainThread();
	virtual void process();
	void add(Threaded* process);
	void add(Threaded& process);
	void run();
	void stop();
	void exit();
};

#endif
