#pragma once
#include <list>
#include <memory>

template<typename T>
class ObjectPool
{
public:
   static ObjectPool& Instance()
   {
      static ObjectPool instance;
      return instance;
   }

   bool Generate(int _num)
   {
      for (int n = 0; n < _num; ++n)
      {
         auto obj = std::make_unique<T>();
         m_objects.push_back(std::move(obj));
      }

      return true;
   }

   std::unique_ptr<T> GetObject()
   {
      std::unique_ptr<T> obj = {};

      if (m_objects.empty())
      {
         return std::move(std::make_unique<T>());
      }
      
      obj = std::move(m_objects.front());
      m_objects.pop_front();
      return std::move(obj);
   }

   void ReturnObject(std::unique_ptr<T>&& obj)
   {
      if (nullptr == obj.get())
         return;

      m_objects.push_back(std::move(obj));
   }

private:
   std::list<std::unique_ptr<T>> m_objects;

   ObjectPool() = default;
   ~ObjectPool() = default;

   ObjectPool(const ObjectPool&) = delete;
   ObjectPool(ObjectPool&&) = delete;
   ObjectPool& operator=(const ObjectPool&) = delete;
   ObjectPool&& operator=(ObjectPool&&) = delete;

};
#define ObjectPool(T) ObjectPool<T>::Instance()