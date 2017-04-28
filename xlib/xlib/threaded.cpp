#include "threaded.h"
#include "simple.h"
#include "sleep.h"

Threaded::Threaded() :
	running_{false}//, thread_{&Threaded::process,this}
{
}

void Threaded::process()
{
	//this->prepare();
	this->process();
}

void Threaded::prepare()
{
}

void Threaded::run()
{
	thread_ = std::thread(&Threaded::process, this);
	/*if (!running_)
		thread_.join();*/
}

Threaded::~Threaded()
{
}

void ThreadedTask::process()
{
	running_ = true;
	task();
	running_ = false;
}

void ThreadedTask::stop()
{
}

ThreadedProcess::ThreadedProcess()
{
}

void ThreadedProcess::process()
{
	running_ = true;
	while (running_) {
		task();
	}
}

void ThreadedProcess::stop()
{
	running_ = false;
	thread_.join();
	//thread_.detach();
}

ThreadedProcess::~ThreadedProcess()
{
}

MainThread::MainThread() :
	running_{false}
{}

void MainThread::process()
{
}

void MainThread::prepare()
{
}

void MainThread::add(Threaded * process)
{
	processes_.push_back(process);
}

void MainThread::add(Threaded & process)
{
	processes_.push_back(&process);
}

void MainThread::run()
{
	prepare();
	running_ = true;
	for (Threaded*& p : processes_)
		p->run();
	this->process();
	//while (running_) this->process();
	exit();
}

void MainThread::stop()
{
	running_ = false;
}

void MainThread::exit()
{
	for (Threaded*& p : processes_) {
		p->stop();
	}
}

MainThread::~MainThread()
{
}

void TimedProcess::process()
{
	prepare();
	running_ = true;
	unsigned long sleepTime;
	processTimer_.tic();
	while (running_) {
		this->task();
		if (!continuous) {
			sleepTime = x::cutl((processPeriod_ - processTimer_.measure()) * 1000);
			if (sleepTime > minSleepTime_) sleep(sleepTime);
		}
	}
}

void TimedProcess::setPeriod(double period)
{
	if (period <= 0) throw ERROR_INVALID_PERIOD_;
	processPeriod_ = period;
	continuous.off();
}

TimedProcess::~TimedProcess()
{
}

TimedProcess::TimedProcess()
	:
	continuous{true},
	processTimer_{},
	processPeriod_{0.0},
	minSleepTime_{DEFAULT_MIN_SLEEP_TIME_}
{
}

TimedProcess::TimedProcess(double processPeriod, unsigned long minSleepTime):
	continuous{false},
	processTimer_{},
	processPeriod_{processPeriod},
	minSleepTime_{minSleepTime}
{
}

const x::error<TimedProcess> TimedProcess::ERROR_INVALID_PERIOD_ = {
	INVALID_PERIOD, 
	"TimedProcess: tried to set invalid period value."};