#pragma once

template<typename Value, typename Priority = std::size_t, typename Compare = std::greater<Priority>>
class CThreadSafePriorityQueue
{
   using Key = Priority;

   std::multimap<Key, Value, Compare> m_multimap{};

   mutable std::mutex m_mutex{};
public:
   CThreadSafePriorityQueue(void) = default;
   CThreadSafePriorityQueue(const CThreadSafePriorityQueue&) = delete;
   CThreadSafePriorityQueue(CThreadSafePriorityQueue&&) = default;
   CThreadSafePriorityQueue& operator = (const CThreadSafePriorityQueue&) = delete;
   CThreadSafePriorityQueue& operator = (CThreadSafePriorityQueue&&) = default;
   ~CThreadSafePriorityQueue(void) = default;

   void Push(const Value& value, const Priority& priority = {})
   {
      std::lock_guard<std::mutex> lock(this->m_mutex);

      this->m_multimap.insert({ priority, value });
   }

   void Push(Value&& value, Priority&& priority = {})
   {
      std::lock_guard<std::mutex> lock(this->m_mutex);

      this->m_multimap.emplace(priority, value);
   }

   auto IsEmpty() const noexcept
   {
      std::lock_guard<std::mutex> lock(this->m_mutex);

      return this->m_multimap.empty();
   }

   auto Pop()
   {
      std::lock_guard<std::mutex> lock(this->m_mutex);

      assert(!this->m_multimap.empty());

      auto it = std::begin(this->m_multimap);
      auto value = std::move(it->second);

      this->m_multimap.erase(it);

      return std::move(value);
   }

   void Clear()
   {
      std::lock_guard<std::mutex> lock(this->m_mutex);

      this->m_multimap.clear();
   }

   auto GetSize() const noexcept
   {
      std::lock_guard<std::mutex> lock(this->m_mutex);

      return this->m_multimap.size();
   }
};
