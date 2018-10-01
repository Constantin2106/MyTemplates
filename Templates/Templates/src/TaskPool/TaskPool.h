#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <atomic>
#include <stdexcept>

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
	 * @param   f	The function of the task. Takes pointer to function or lambda.
	 * @param	args  The list of functions arguments.
	 *
	 * @return	The future object of the result of the task execution.
	 *          Use 'get' function to retrieve the results when the task will finish. 			
	*/
   template<class F, class... Args> 
   auto AddTask(F&& f, Args&&... args) -> std::future<typename std::result_of_t<F(Args...)>>
   {
      using ret_type = typename std::result_of_t<F(Args...)>;

      auto task = std::make_shared<std::packaged_task<ret_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
      
      std::future<ret_type> res = task->get_future();

      {
         std::unique_lock<std::mutex> lock(m_queue_mutex);

         // If the pool is stopped, the addition of the task is forbidden.
         if (m_stop)
            throw std::runtime_error("Pool has been stopped");

         // Add task to queue
         m_tasks.emplace(
            [task]()
            { 
               (*task)(); 
            });
      }

      m_condition.notify_one();
      return res;
   }

private:
   /**
    * @fn	TaskPool::TaskPool(bool);
    *
    * @brief	Constructor.  Defines an optimal number of threads and creates them in a suspended state.
    * 			
   */
   TaskPool(bool _stop);

   TaskPool(const TaskPool&) = delete;
   TaskPool(TaskPool&&) = delete;
   TaskPool& operator= (const TaskPool&) = delete;
   TaskPool&& operator= (TaskPool&&) = delete;

   std::vector<std::thread> m_threads;				// The pool of threads
   std::queue<std::function<void()>> m_tasks;	// The task queue

   std::mutex m_queue_mutex;						   // The mutex is used to lock of tasks queue when the task is added
   std::condition_variable m_condition;			// The condition variable used to lock of queue mutex

   std::atomic_bool m_stop;         				// If true, pool will be stopped
};






