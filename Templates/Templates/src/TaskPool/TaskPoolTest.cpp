#include <conio.h>

#include <iostream>
#include <vector>
#include <chrono>

#include "TaskPool/TaskPool.h"

std::mutex mut;

int fastFun(int i)
{
   mut.lock();
   std::cout << "fast task " << i << std::endl;
   mut.unlock();
   std::this_thread::sleep_for(std::chrono::milliseconds(100));

   return i;
}

int slowFun(int i)
{
   int N = i;

   for (int n = 0; n < N; ++n)
   {
      mut.lock();
      std::cout << "slow task step " << n << std::endl;
      mut.unlock();
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
   }

   return i;
}

void TaskPoolTest()
{
   std::vector<std::future<int>> results;

   TaskPool pool;
   std::future<int> fut;

   mut.lock();
   std::cout << "------ Pool size " << pool.Size() << " ------" << std::endl;
   mut.unlock();

   int fFunNum = pool.Size() * 2;

   for (int k = 0; k < fFunNum; ++k)
   {
      fut = pool.AddTask(0, fastFun, k);
      results.push_back(std::move(fut));
   }
   mut.lock();
   std::cout << "------ Added " << fFunNum << " fast tasks ------" << std::endl;
   mut.unlock();

   fut = pool.AddTask(1, slowFun, fFunNum);
   results.push_back(std::move(fut));
   mut.lock();
   std::cout << "------ Added " << "1" << " slow task ------" << std::endl;
   mut.unlock();

   for (int k = fFunNum, n = fFunNum + 5; k < n; ++k)
   {
      fut = pool.AddTask(0, fastFun, k);
      results.push_back(std::move(fut));
   }
   mut.lock();
   std::cout << "------ Added else " << "5" << " fast task ------" << std::endl;
   mut.unlock();

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
}