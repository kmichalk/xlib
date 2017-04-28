#ifndef THREADED_H
#define THREADED_H

#include <atomic>
#include <thread>
#include "xvector.h"
#include "timer.h"
#include "flag.h"
#include "error.h"


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
	virtual void prepare();
	virtual void run();
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
public:
	ThreadedProcess();
	virtual void task() abstract override;
	virtual void stop() override;
	virtual void process() override;
	virtual ~ThreadedProcess();
};

class TimedProcess: public ThreadedProcess
{
	static const x::error<TimedProcess> ERROR_INVALID_PERIOD_;
	static constexpr unsigned long DEFAULT_MIN_SLEEP_TIME_ = 10;

	double processPeriod_;
	unsigned long minSleepTime_;
	double frequency_;
	x::timer<std::chrono::milliseconds> processTimer_;
public:
	enum ErrorNum
	{
		INVALID_PERIOD
	};


	x::flag continuous;

	TimedProcess();
	TimedProcess(double processPeriod, unsigned long minSleepTime = DEFAULT_MIN_SLEEP_TIME_);
	virtual void process() override;
	void setPeriod(double period);
	
	virtual ~TimedProcess();
};

class MainThread
{
	x::vector<Threaded*> processes_;

protected:
	bool running_;
	
public:
	MainThread();
	virtual void process();
	virtual void prepare();
	void add(Threaded* process);
	void add(Threaded& process);
	void run();
	void stop();
	void exit();

	virtual ~MainThread();
};

#endif
