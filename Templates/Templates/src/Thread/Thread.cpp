#include "Thread.h"

#include <thread>
#include <utility>

using namespace std;

unsigned TThread::maxThreads = 0;
unsigned TThread::maxId = 0;

TThread::TThread() : terminate(false)
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

unsigned TThread::AddTask(FuncRef<void(void*)> _function, void* _data)
{
   tasks.emplace(_function, _data);

	return (unsigned)tasks.size();
}