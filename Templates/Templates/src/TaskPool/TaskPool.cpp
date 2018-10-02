#include "TaskPool.h"

TaskPool::TaskPool() : TaskPool(false)
{
}

TaskPool::~TaskPool()
{
   m_finish = true;

   m_thread_control.notify_all();
   for (auto& thread : m_threads)
   {
      if (thread.joinable())
      {
         thread.join();
      }
      else
      {
         // ???
      }
   }
}

TaskPool::TaskPool(bool _finish) : m_finish(_finish)
{
   // Calc the optimal number of threads
   SYSTEM_INFO sysInfo;
   GetSystemInfo(&sysInfo);
   UINT threadNum = /*2 * */sysInfo.dwNumberOfProcessors;

   // Create threads
   for (UINT i = 0; i < threadNum; ++i)
   {
      // Create thread and put it into container
      m_threads.emplace_back(
         [&] // thread function
         {
            while (!m_finish)
            {
               taskFun task;

               {
                  std::unique_lock<std::mutex> lock(m_map_mutex);
                  // Suspend this thread
                  m_thread_control.wait(lock, [&]
                  { 
                     return m_finish || !m_tasks.empty(); 
                  });

                  if (m_tasks.empty())
                     continue;

                  auto it = m_tasks.cbegin();
                  task = std::move(it->second);
                  m_tasks.erase(it);
               }

               task();
            }
         });
   }
}
