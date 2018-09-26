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

	unsigned AddTask(void(*_function)(void*), void* _data);
	void Run();
	unsigned GetId() { return id; }

protected:
	struct Task
	{
		void* data;
		void(*function)(void*);
	};

private:
	static unsigned maxThreads;
	static unsigned maxId;

	unsigned id;
	bool terminate;

	std::queue<Task> tasks;
};

#endif
