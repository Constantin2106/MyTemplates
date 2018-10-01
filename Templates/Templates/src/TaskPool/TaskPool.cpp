#include "TaskPool.h"

TaskPool::TaskPool(unsigned _threads) : m_threadNum(_threads) 
{
}

TaskPool::~TaskPool()
{
   m_stop = true;

   m_condition.notify_all();
   for (auto& tread : m_threads)
      tread.join();
}

void TaskPool::Activate()
{
   // Create m_threadNum threads
   for (unsigned i = 0; i < m_threadNum; ++i)
   {
      // Put thread into container
      m_threads.emplace_back(
         [this] // thread function
      {
         while (!m_stop)
         {
            std::function<void()> task;

            {
               std::unique_lock<std::mutex> lock(m_queue_mutex);
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