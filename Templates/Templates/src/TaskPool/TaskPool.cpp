#include <Windows.h>

#include "TaskPool.h"

TaskPool::TaskPool() : TaskPool(false)
{
}

TaskPool::~TaskPool()
{
   m_stop = true;

   m_condition.notify_all();
   for (auto& tread : m_threads)
      tread.join();
}

TaskPool::TaskPool(bool _stop) : m_stop(_stop)
{
   // Calc the optimal number of threads
   SYSTEM_INFO sysInfo;
   GetSystemInfo(&sysInfo);
   unsigned threadNum = 2 * sysInfo.dwNumberOfProcessors;

   // Create threads
   for (unsigned i = 0; i < threadNum; ++i)
   {
      // Create thread and put it into container
      m_threads.emplace_back(
         [this] // thread function
         {
            while (!m_stop)
            {
               std::function<void()> task;

               {
                  std::unique_lock<std::mutex> lock(m_queue_mutex);
                  // Suspend this thread
                  m_condition.wait(lock, [this] { return m_stop || !m_tasks.empty(); });

                  if (m_tasks.empty())
                     continue;

                  task = std::move(m_tasks.front());
                  m_tasks.pop();
               }

               task();
            }
         });
   }
}