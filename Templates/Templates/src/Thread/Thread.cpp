#include "Thread.h"

#include <thread>

using namespace std;

unsigned TThread::m_maxThreads = 0;
unsigned TThread::m_maxId = 0;

TThread::TThread() :
m_terminate(false)
{
	m_id = ++m_maxId;

	unsigned supportingTreads = thread::hardware_concurrency();
	m_maxThreads = supportingTreads > 1 ? supportingTreads : 1;
}
TThread::~TThread()
{
	while (!m_tasks.empty())
		m_tasks.pop();
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
	while (!m_tasks.empty())
	{
		Task task = m_tasks.front();
		m_tasks.pop();

		thread thr(task.m_func, task.m_data);
		//if (thr.joinable())
		//	thr.join();
		thr.detach();
	}
}

unsigned TThread::AddTask(void(*_func)(void*), void* _data)
{
   m_tasks.emplace(_func, _data);

	return static_cast<unsigned>(m_tasks.size());
}