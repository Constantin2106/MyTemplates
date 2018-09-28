#include <conio.h>

#include <iostream>
#include <vector>
#include <chrono>

#include "TaskPool/TaskPool.h"

std::mutex mut;

int testFun(int i)
{
   mut.lock();
   std::cout << "task " << i << std::endl;
   mut.unlock();

   return i;
}
void TaskPoolTest()
{
   TaskPool pool(8);
   std::vector<std::future<int>> results;

   //for (int i = 0; i < 8; ++i)
   //{
   //   results.emplace_back(
   //      pool.enqueue([i]
   //   {
   //      //throw 1;

   //      std::cout << "hello " << i << std::endl;
   //      std::this_thread::sleep_for(std::chrono::seconds(i));
   //      std::cout << "world " << i << std::endl;
   //      return i;
   //   })
   //   );
   //}
   

   for (int i = 0; i < 8; ++i)
   {
      //auto fut = pool.enqueue([i]
      //{
      //   //throw 1;

      //   std::cout << "hello " << i << std::endl;
      //   std::this_thread::sleep_for(std::chrono::seconds(i));
      //   std::cout << "world " << i << std::endl;
      //   return i;
      //});

      //auto lmd = [&]
      //{
      //   //throw 1;

      //   mut.lock();
      //   std::cout << "task " << i << std::endl;
      //   mut.unlock();
      //   return i;
      //};
      //auto fut = pool.enqueue(lmd);

      auto fut = pool.enqueue(testFun, i);

      results.push_back(std::move(fut));
   }

   //for (auto && result : results)
   //   std::cout << result.get() << ' ';
   //std::cout << std::endl;

   _getch();
}