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
   using taskFunc = std::function<void()>;
/*
Description
   TaskPool::TaskPool(bool);
   Constructor. 
   Calls the private constructor to force the creation of the instance.
   Defines the number of a processors core.
   Creates threads and suspends them until the next task will be added to the queue.
   Then the one thread will be waked up
   and the task with max priority is retrieved from the task queue and executed.

Arguments
   
Return

History
   Created by Created by Kostiantyn Zhelieznov   10/01/2018
*/
   TaskPool();

/*
Description
   Destructor. Destroys the task pool by calling finish function.

Arguments

Return

History
   Created by Kostiantyn Zhelieznov   10/01/2018
*/
   ~TaskPool();

/*
Description
   TaskPool::AddTask(pri, func, args);
   Add new task to the task queue. 
   The added task puts into the task queue according to its priority.

Arguments
   pri   The prioruty of this task.
   func  The function of the task. Takes pointer to function or lambda.
   args  The list of functions arguments.

Return
   The future object of the result of the task execution.
   Use 'get' function to retrieve the results when the task will finish.

History
   Created by Kostiantyn Zhelieznov   10/01/2018
*/
   template<class F, class... Args>
   auto AddTask(const UINT _priority, F&& func, Args&&... args)
   {
	  using namespace std;
      using R = typename result_of_t<F(Args...)>;

	  future<R> fut{};
	  // If the pool is stopped, the addition of the task is forbidden.
	  if (m_finish)
		  return fut;

      auto task = make_shared<packaged_task<R()>>(bind(forward<F>(func), forward<Args>(args)...));
      {
         unique_lock<mutex> lock(m_map_mutex);

         fut = task->get_future();
         // Add task with priority
         m_tasks.emplace(_priority, [task]() { (*task)(); });
      }

      m_thread_control.notify_one();
      
      return fut;
   }

/*
Description
   TaskPool::Size();

Arguments

Return
   Return number of thread in the pool

History
   Created by Kostiantyn Zhelieznov   10/01/2018
*/
   std::size_t Size();

/*
Description
   TaskPool::TaskNumber();

Arguments

Return
   Return number of tasks in the pool queue

History
   Created by Kostiantyn Zhelieznov   10/01/2018
*/
   std::size_t TaskNumber();

/*
Description
   TaskPool::Destroy(); Immediately destroys the task pool by calling finish function.

Arguments

Return

History
   Created by Kostiantyn Zhelieznov   10/01/2018
*/
   void Destroy() { finish(); }

private:
/*
Description
   This private constructor is invoked by delegating constructor to force the creation of the instance.

Arguments
	Needs some kind of argument to be different from the delegating constructor.

Return

History
   Created by Kostiantyn Zhelieznov   10/01/2018
*/
   TaskPool(bool _finish);

   TaskPool(const TaskPool&) = delete;
   TaskPool(TaskPool&&) = delete;
   TaskPool& operator= (const TaskPool&) = delete;
   TaskPool&& operator= (TaskPool&&) = delete;

/*
Description
   TaskPool::finish();    
   Sets the stop variable and wake up all threads to be finished them.
   Clears the task queue and waits while all executed threads will be finished

Arguments

Return

History
   Created by Kostiantyn Zhelieznov   10/01/2018
*/
   void finish();

   std::vector<std::thread> m_threads;       // The container of threads   
   std::multimap<UINT, taskFunc,             // The <priority, function> multimap
      std::greater<UINT>> m_tasks;	         // More priority tasks always are on the top of the map

   std::mutex m_map_mutex;					 // The mutex is used to lock of tasks map when the task is added
   std::condition_variable m_thread_control; // The condition variable used to suspend/resume of threads

   std::atomic_bool m_finish;         		 // When true, the pool will be finished
};






