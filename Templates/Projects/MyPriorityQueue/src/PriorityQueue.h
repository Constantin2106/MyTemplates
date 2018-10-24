#pragma once

#include <mutex>
#include <map>
#include <assert.h>

#define Lock(m) std::lock_guard<std::mutex> lock(m)

/*
Description
   Thread-safe queue with elements arranged according to their priority.
   By default, elements arranged in descending order of their priority. 

Usage
   PriorityQueue<TypeOfElements> var;

History
   Created by Kostiantyn Zhelieznov   10/01/2018
*/
template<typename V, typename P = std::size_t, typename C = std::greater<P>>
class PriorityQueue
{
public:
   PriorityQueue() = default;
   PriorityQueue(PriorityQueue&&) = default;
   ~PriorityQueue() = default;

   PriorityQueue(const PriorityQueue&) = delete;
   PriorityQueue& operator = (const PriorityQueue&) = delete;
   PriorityQueue& operator = (PriorityQueue&&) = default;

/*
Description
   Put new pair <priority,element> into a queue using move semantics

Arguments
   val - new element
   pr  - priority of a new element (use as a key)

History
   Created by Kostiantyn Zhelieznov   10/01/2018
*/
   void Push(V&& val, P&& pr = {})
   {
      Lock(m_mutex);
      m_multimap.emplace(pr, val);
   }

/*
Description
   Check if the queue is empty.

Arguments

History
   Created by Kostiantyn Zhelieznov   10/01/2018
*/
   auto IsEmpty() const noexcept
   {
      Lock(m_mutex);
      return m_multimap.empty();
   }

/*
Description
   Retrieve the element from the top of the queue using move semantics

Arguments

Return
   Elenent of the queue

History
   Created by Kostiantyn Zhelieznov   10/01/2018
*/
   auto Pop()
   {
      Lock(m_mutex);

      if(m_multimap.empty())
         return V{};

      auto it = std::begin(m_multimap);
      auto value = std::move(it->second);
      m_multimap.erase(it);

      return std::move(value);
   }

/*
Description
   Clear the queue

Arguments

Return

History
   Created by Kostiantyn Zhelieznov   10/01/2018
*/
   void Clear()
   {
      Lock(m_mutex);
      m_multimap.clear();
   }

/*
Description
   Get length of the queue

Arguments

Return
   Number of elements in the queue

History
   Created by Kostiantyn Zhelieznov   10/01/2018
*/
   auto GetSize() const noexcept
   {
      Lock(m_mutex);
      return m_multimap.size();
   }

private:
   std::multimap<P, V, C> m_multimap{};   // multimap using to automatic sort elements acoording to their priority
   mutable std::mutex m_mutex{};          // mutex for garantee thread-safe
};

