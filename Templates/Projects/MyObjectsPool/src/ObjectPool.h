#pragma once
#include <list>
#include <memory>

template<typename T>
class ObjectPool
{
public:
/*
Description
   auto pool = ObjectPool<T>::Instance();
   auto pool = ObjectPool(T);
   Creates or gets an instance of objects pool

Arguments

Return
   The instance of objects pool as a singleton

History
   Created by Kostiantyn Zhelieznov   10/13/2018
*/
   static ObjectPool& Instance()
   {
      static ObjectPool instance;
      return instance;
   }

/*
Description
   ObjectPool(T).AddObject(N);
   Adds N elements to objects list

Arguments
   std::size_t _num  The number of objects to be added

Return
   true  If the objects have been added successfully

History
   Created by Kostiantyn Zhelieznov   10/13/2018
*/   
   bool AddObject(std::size_t _num)
   {
      m_max += _num;

      for (int n = m_objects.size(); n < m_max; ++n)
      {
         auto obj = std::make_unique<T>();
         m_objects.push_back(std::move(obj));
      }

      return true;
   }

/*
Description
   auto obj = ObjectPool(T).GetObject();
   Checks out object from list

Arguments

Return
   unique_ptr to object

History
   Created by Kostiantyn Zhelieznov   10/13/2018
*/
   std::unique_ptr<T> GetObject()
   {
      std::unique_ptr<T> obj{};

      if (m_objects.empty())
      {
         return std::move(std::make_unique<T>());
      }
      
      obj = std::move(m_objects.front());
      m_objects.pop_front();
      return std::move(obj);
   }

/*
Description
   ObjectPool(T).PutObject(obj);
   Checks in object to list

Arguments
   std::unique_ptr<T>&& obj   object as rvalue

Return

History
   Created by Kostiantyn Zhelieznov   10/13/2018
*/
   void PutObject(std::unique_ptr<T>&& obj)
   {
      if (nullptr == obj.get())
         return;
      if(m_objects.size() == m_max)
         return;

      m_objects.push_back(std::move(obj));
   }

/*
Description
   auto n = ObjectPool(T).Count();
   Gets number of objects in the pool

Arguments

Return
   std::size_t n  The objects number

History
   Created by Kostiantyn Zhelieznov   10/13/2018
*/
   std::size_t Count() { return m_objects.size(); }

   /*
Description
   ObjectPool(T).Clear()
   Clears the objects pool and calls the destructor for each object.

Arguments

Return

History
   Created by Kostiantyn Zhelieznov   10/13/2018
*/
   void Clear() { m_objects.clear(); }

private:
   std::size_t m_max;                        // Max size of the object pool
   std::list<std::unique_ptr<T>> m_objects;  // The list of objects

   ObjectPool() : m_max(0) {}
   ~ObjectPool() = default;

   ObjectPool(const ObjectPool&) = delete;
   ObjectPool(ObjectPool&&) = delete;
   ObjectPool& operator=(const ObjectPool&) = delete;
   ObjectPool&& operator=(ObjectPool&&) = delete;

};

#define ObjectPool(T) ObjectPool<T>::Instance()