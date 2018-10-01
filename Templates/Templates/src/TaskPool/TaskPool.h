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

   TaskPool(unsigned _threads);
   
   /**
    * @fn	TaskPool::~TaskPool();
    *
    * @brief	Destructor. Notifies all threads of stop condition so that they do not take other tasks from the pool.
    * 			Waits for all threads to finish (joins all threads).
    */
   ~TaskPool();

   void Activate();
 
	/**
	 * @fn	template<class F, class... Args> auto TaskPool::enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
	 *
	 * @brief	Queues a task be executed by one of the threads in the pool.
	 *
	 * @tparam	F   	The function type as determined by the template. 
	 * @tparam	Args  Variable list of arguments as determined by the template.
	 * @param [in,out] f	The function to process. Takes std::bind, or lambda as r-value or moves all other functional containers.
	 * @param	args  Variable arguments to pass to the function call.
	 *
	 * @return	A future of the result type of the function. A .get() call to this future container returns the result after the task is finished.
	 * 			The .get() call waits synchronously for the task to finish.
	 */
   template<class F, class... Args> 
   auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of_t<F(Args...)>>
   {
      using return_type = typename std::result_of_t<F(Args...)>;

      auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
      std::future<return_type> res = task->get_future();

      {
         std::unique_lock<std::mutex> lock(m_queue_mutex);

         //Don't allow enqueueing after stopping the pool
         if (m_stop)
            throw std::runtime_error("enqueue on stopped TaskPool");

         m_tasks.emplace([task]() { (*task)(); });
      }

      m_condition.notify_one();
      return res;
   }

private:
   unsigned m_threadNum;                        // The number of threads
   std::vector<std::thread> m_threads;				// The worker threads
   std::queue<std::function<void()>> m_tasks;	// The task queue

   std::mutex m_queue_mutex;						   // The queue mutex
   std::condition_variable m_condition;			// The condition based on a lock of queue mutex

   std::atomic_bool m_stop{ false };				// false initially, true to stop
};






