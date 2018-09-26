#pragma once

#include <queue>
#include "Callback/Callback.h"

class TThread
{
public:
	TThread();
	~TThread();

	bool Initialize();
	bool Free();

   unsigned AddTask(FuncRef<void(void*)> function, void* _data);
	void Run();
	unsigned GetId() { return id; }

protected:
	struct Task
	{
		void* data;
      FuncRef<void(void*)> function;

      Task() : function(nullptr), data(nullptr) {}
      Task(FuncRef<void(void*)> f, void* d) : function(f), data(d) {}
	};

private:
	static unsigned maxThreads;
	static unsigned maxId;

	unsigned id;
	bool terminate;

	std::queue<Task> tasks;
};
