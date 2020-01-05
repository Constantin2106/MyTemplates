// MyTaskPool.cpp : Defines the entry point for the console application.
//
#include <vld.h>

#include <conio.h>
#include <iostream>
#include <vector>
#include <chrono>

#include "src\TaskPool.h"

std::mutex mut;

#define  Message(msg)               \
{                                   \
    std::unique_lock<std::mutex>    \
        lock(mut);                  \
    msg                             \
}
#define OutMessage(cmn, val)		\
{									\
	std::unique_lock<std::mutex>	\
		lock(mut);					\
	std::cout <<					\
		cmn << val <<				\
	std::endl;						\
}

int fastFun(int i)
{
	OutMessage("started fast task ", i);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
	OutMessage("finished fast task ", i);
    return i;
}

int slowFun(int i)
{
    int N = 10;
	OutMessage("!!!slow task started!!! ", N);

    for(int n = 0; n < N; ++n)
    {
		OutMessage("slow task step ", n);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return i;
}


int main()
{
    std::vector<std::future<int>> results;

    TaskPool pool;

	OutMessage("------ Pool size ", pool.Size());

    int fastTasksNum = static_cast<int>(pool.Size() * 2);

    for(int k = 0; k < fastTasksNum; ++k)
    {
        auto fut = pool.AddTask(0, fastFun, k);
        results.push_back(std::move(fut));
    }
	Message(std::cout << "------ Added " << fastTasksNum << " fast tasks ------" << std::endl;);

	auto fut = pool.AddTask(100, slowFun, 100);
    results.push_back(std::move(fut));
	OutMessage("------ Added 1", " slow task ------");

    /*std::this_thread::sleep_for(std::chrono::milliseconds(500));
	OutMessage("------ Before destroying. Task number ", pool.TaskNumber());
    pool.Destroy();
	OutMessage("------ After destroying. Task number ", pool.TaskNumber())
	OutMessage("------ The pool has been destroyed ------", "");*/

    for(int k = fastTasksNum, n = fastTasksNum + 5; k < n; ++k)
    {
		auto fut = pool.AddTask(0, fastFun, k);
        results.push_back(std::move(fut));
    }
	OutMessage("------ Added another 5", " fast task ------");
	OutMessage("------ Final task number ", pool.TaskNumber());

    //int sFuncInd = 10;
    //for (int i = 0; i < 12; ++i)
    //{
    //   if (sFuncInd == i)
    //   {
    //      fut = pool.AddTask(1, slowFun, i);
    //      results.push_back(std::move(fut));
    //   }
    //   else if (i < sFuncInd)
    //   {
    //      for (int k = i, n = i + sFuncInd; k < n; ++k)
    //      {
    //         fut = pool.AddTask(0, fastFun, k);
    //         results.push_back(std::move(fut));
    //         ++i;
    //      }
    //   }
    //   else
    //   {
    //      fut = pool.AddTask(0, fastFun, i);
    //      results.push_back(std::move(fut));
    //   }

    //   //results.push_back(std::move(fut));
    //}

	{
		OutMessage("Total Results number: ", results.size());
		for (auto && result : results)
		{
			try
			{
				//result.wait();
				auto r = result.get();
				if (r >= 0)
				{
					OutMessage("\t\tresult: ", r);
				}
				else
				{
					OutMessage("\t\tinvalid result: ", r);
				}
			}
			catch (const std::exception& e)
			{
				OutMessage("\tDon't have the result!\t", e.what());
			}
		}
		std::cout << std::endl;
	}

	system("pause");
    return 0;

}

