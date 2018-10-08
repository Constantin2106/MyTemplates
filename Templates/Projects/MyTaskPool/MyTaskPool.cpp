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

int fastFun(int i)
{
    Message(std::cout << "fast task " << i << std::endl;)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    return i;
}

int slowFun(int i)
{
    int N = i;

    for(int n = 0; n < N; ++n)
    {
        Message(std::cout << "slow task step " << n << std::endl;)
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return i;
}


int main()
{
    std::vector<std::future<int>> results;

    TaskPool pool;
    std::future<int> fut;

    Message(std::cout << "------ Pool size " << pool.Size() << " ------" << std::endl;)

    int fastTasksNum = pool.Size() * 2;

    for(int k = 0; k < fastTasksNum; ++k)
    {
        fut = pool.AddTask(0, fastFun, k);
        results.push_back(std::move(fut));
    }
    Message(std::cout << "------ Added " << fastTasksNum << " fast tasks ------" << std::endl;)

    fut = pool.AddTask(1, slowFun, fastTasksNum);
    results.push_back(std::move(fut));
    Message(std::cout << "------ Added " << "1" << " slow task ------" << std::endl;)

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    Message(std::cout << "------ Before destroying. Task number " << pool.TaskNumber() << std::endl;)
        pool.Destroy();
    Message(std::cout << "------ After destroying. Task number " << pool.TaskNumber() << std::endl;)
    Message(std::cout << "------ The pool has been destroyed ------" << std::endl;)

    for(int k = fastTasksNum, n = fastTasksNum + 5; k < n; ++k)
    {
        fut = pool.AddTask(0, fastFun, k);
        results.push_back(std::move(fut));
    }
    Message(std::cout << "------ Added else " << "5" << " fast task ------" << std::endl;)
    Message(std::cout << "------ Final task number " << pool.TaskNumber() << std::endl;)

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

    /*for (auto && result : results)
    std::cout << result.get() << ' ';
    std::cout << std::endl;*/

    _getch();

    return 0;

}

