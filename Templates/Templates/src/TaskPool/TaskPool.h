#pragma once

#include <vector>
#include <map>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <atomic>
#include <stdexcept>

#include <Windows.h>

class TaskPool
{
public:
   /**
    * @fn	TaskPool::TaskPool();
    *
    * @brief	Delegating constructor.
    *
   */
   TaskPool();

   /**
    * @fn	TaskPool::~TaskPool();
    *
    * @brief	Destructor. Sets the stop variable and wake up all threads to be finished them.
    * 			Waits while the all threads will be finished.
   */
   ~TaskPool();

   /**
    * @fn	   TaskPool::AddTask(func, args)
    *
    * @brief	The template function for adding a task to the queue.
    *
    * @param   func	The function of the task. Takes pointer to function or lambda.
    * @param	args  The list of functions arguments.
    *
    * @return	The future object of the result of the task execution.
    *          Use 'get' function to retrieve the results when the task will finish.
   */
   template<class F, class... Args>
   auto AddTask(UINT _priority, F&& func, Args&&... args)
   {
      using ret_type = typename std::result_of_t<F(Args...)>;

      auto task = std::make_shared<std::packaged_task<ret_type()>>
         (std::bind(std::forward<F>(func), std::forward<Args>(args)...));

      std::future<ret_type> fut = task->get_future();

      {
         std::unique_lock<std::mutex> lock(m_map_mutex);

         // If the pool is stopped, the addition of the task is forbidden.
         if (m_finish)
            throw std::runtime_error("Pool has been stopped");

         // Add task with priority
         m_tasks.emplace(_priority, [task]() { (*task)(); });
      }

      m_thread_control.notify_one();
      
      return fut;
   }

   /**
    * @fn	   TaskPool::Size();
    *
    * @brief	Return the pool size
    *
   */
   std::size_t Size() { return m_threads.size(); }

   void Destroy() { finish(); }

private:
   using taskFunc = std::function<void()>;

   /**
    * @fn	TaskPool::TaskPool(bool);
    *
    * @brief	Constructor. Defines an optimal number of threads and creates them in a suspended state.
    *
   */
   TaskPool(bool _finish);

   TaskPool(const TaskPool&) = delete;
   TaskPool(TaskPool&&) = delete;
   TaskPool& operator= (const TaskPool&) = delete;
   TaskPool&& operator= (TaskPool&&) = delete;

   void finish();

   std::vector<std::thread> m_threads;       // The container of threads   
   std::multimap<UINT, taskFunc,             // The <priority, function> multimap
      std::greater<UINT>> m_tasks;	         // More priority tasks always are on the top of the map

   std::mutex m_map_mutex;						   // The mutex is used to lock of tasks map when the task is added
   std::condition_variable m_thread_control; // The condition variable used to suspend/resume of threads

   std::atomic_bool m_finish;         		   // When true, the pool will be finished
};






