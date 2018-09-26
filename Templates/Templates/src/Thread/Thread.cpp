#include "Thread.h"

#include <thread>

using namespace std;

unsigned TThread::maxThreads = 0;
unsigned TThread::maxId = 0;

TThread::TThread() :
terminate(false)
{
	id = ++maxId;

	unsigned supportingTreads = thread::hardware_concurrency();
	maxThreads = supportingTreads > 1 ? supportingTreads : 1;
}
TThread::~TThread()
{
	while (!tasks.empty())
		tasks.pop();
}

bool TThread::Initialize()
{

	return true;
}
bool TThread::Free()
{

	return true;
}

void TThread::Run()
{
	while (!tasks.empty())
	{
		Task task = tasks.front();
		tasks.pop();

		thread thr(task.function, task.data);
		//if (thr.joinable())
		//	thr.join();
		thr.detach();
	}
}

unsigned TThread::AddTask(void(*_function)(void*), void* _data)
{
	Task task;
	task.data = _data;
	task.function = _function;

	tasks.push(task);

	return (unsigned)tasks.size();
}