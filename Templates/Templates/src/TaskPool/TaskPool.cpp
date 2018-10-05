#include "TaskPool.h"

TaskPool::TaskPool() : TaskPool(false)
{
}

TaskPool::~TaskPool()
{
   finish();
}

TaskPool::TaskPool(bool _finish) : m_finish(_finish)
{
   // Calc the optimal number of threads
   SYSTEM_INFO sysInfo;
   GetSystemInfo(&sysInfo);
   DWORD threadNum = sysInfo.dwNumberOfProcessors;// * 2;

   // Create threads
   for (DWORD i = 0; i < threadNum; ++i)
   {
      // Create thread and put it into container
      m_threads.emplace_back(
         [&] // thread function
         {
            while (true)
            {
               taskFunc task;

               {
                  std::unique_lock<std::mutex> lock(m_map_mutex);
                  
                  // Suspend this thread
                  m_thread_control.wait(lock, [&] { return m_finish || !m_tasks.empty(); });

                  if (m_finish)
                     break; 
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

std::size_t TaskPool::Size()
{ 
   std::unique_lock<std::mutex> lock(m_map_mutex);

   return m_threads.size(); 
}

std::size_t TaskPool::TaskNumber()
{
   std::unique_lock<std::mutex> lock(m_map_mutex);

   return m_tasks.size();
}

void TaskPool::finish()
{
   {
      std::unique_lock<std::mutex> lock(m_map_mutex);
      m_finish = true;

      m_tasks.clear();

      m_thread_control.notify_all();
   }

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
