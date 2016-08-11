#include "threaded.h"
#include <Windows.h>
#include "simple.h"

Threaded::Threaded() :
	running_{false}//, thread_{&Threaded::process,this}
{
}

void Threaded::process()
{
	this->process();
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
	running_ = true;
	for (Threaded*& p : processes_)
		p->run();
	this->process();
	//while (running_);// Sleep(10);
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

void TimedProcess::process()
{
	running_ = true;
	processTimer_.tic();
	while (running_) {
		this->task();
		Sleep(x::cutl(processPeriod_-processTimer_.measure()));
	}
}

TimedProcess::TimedProcess(double processPeriod):
	processTimer_{0.001},
	processPeriod_{processPeriod}
{
}
