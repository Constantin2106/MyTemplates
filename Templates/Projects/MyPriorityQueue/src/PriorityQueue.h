#pragma once

#include <mutex>
#include <map>
#include <assert.h>

#define Lock(m) std::lock_guard<std::mutex> lock(m)

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

   void Push(V&& value, P&& priority = {})
   {
      Lock(m_mutex);
      m_multimap.emplace(priority, value);
   }

   auto IsEmpty() const noexcept
   {
      Lock(m_mutex);
      return m_multimap.empty();
   }

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

   void Clear()
   {
      Lock(m_mutex);
      m_multimap.clear();
   }

   auto GetSize() const noexcept
   {
      Lock(m_mutex);
      return m_multimap.size();
   }

private:
   std::multimap<P, V, C> m_multimap{};
   mutable std::mutex m_mutex{};
};

