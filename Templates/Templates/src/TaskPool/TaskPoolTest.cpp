#include <conio.h>

#include <iostream>
#include <vector>
#include <chrono>

#include "TaskPool/TaskPool.h"

std::mutex mut;

int fastFun(int i)
{
   mut.lock();
   std::cout << "task " << i << std::endl;
   mut.unlock();

   return i;
}

int slowFun(int i)
{
   int N = 5 * i;

   for (int n = 0; n < N; ++n)
   {
      //mut.lock();
      std::cout << "slow task step " << n << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
      //mut.unlock();
   }

   return i;
}


void TaskPoolTest()
{
   std::vector<std::future<int>> results;

   TaskPool pool;
   std::future<int> fut;

   for (int i = 0; i < 8; ++i)
   {
      if (1 == i)
         fut = pool.AddTask(slowFun, i);
      else
         fut = pool.AddTask(fastFun, i);

      results.push_back(std::move(fut));
   }

   /*for (auto && result : results)
      std::cout << result.get() << ' ';
   std::cout << std::endl;*/

   _getch();
}