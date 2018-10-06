#ifndef THREAD
#define THREAD

#include <queue>

class TThread
{
public:
	TThread();
	~TThread();

	bool Initialize();
	bool Free();

	unsigned AddTask(void(*_func)(void*), void* _data);
	void Run();
	unsigned GetId() { return m_id; }

protected:
	struct Task
	{
		void* m_data;
		void(*m_func)(void*);

      Task(void(*_func)(void*), void* _data) : m_func(_func), m_data(_data) {}
	};

private:
	static unsigned m_maxThreads;
	static unsigned m_maxId;

	unsigned m_id;
	bool m_terminate;

	std::queue<Task> m_tasks;
};

#endif
