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
   int N = 10 * i;

   for (int n = 0; n < N; ++n)
   {
      mut.lock();
      std::cout << n << std::endl;
      mut.unlock();
   }

   return i;
}


void TaskPoolTest()
{
   TaskPool pool(8);
   std::vector<std::future<int>> results;

   for (int i = 0; i < 8; ++i)
   {

      if (i == 1)
         results.push_back( pool.enqueue(slowFun, i) );
      else
         results.push_back( pool.enqueue(fastFun, i) );

      /*auto fut = pool.enqueue(fastFun, i);
      results.push_back(std::move(fut));*/
   }

   for (auto && result : results)
      std::cout << result.get() << ' ';
   std::cout << std::endl;

   _getch();
}