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

   TaskPool pool(8);

   pool.Activate();

   for (int i = 0; i < 8; ++i)
   {

      if (i == 1)
         results.push_back( pool.enqueue(slowFun, i) );
      else
         results.push_back( pool.enqueue(fastFun, i) );

      /*auto fut = pool.enqueue(fastFun, i);
      results.push_back(std::move(fut));*/
   }

   /*for (auto && result : results)
      std::cout << result.get() << ' ';
   std::cout << std::endl;*/

   _getch();
}