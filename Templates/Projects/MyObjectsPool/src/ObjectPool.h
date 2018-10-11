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

   ~ObjectPool()
   {
      while(!m_objects.empty())
      {
         delete m_objects.front();
         m_objects.pop_front();
      }
   }

   T* GetObject()
   {
      if (m_objects.empty())
      {
         return new T();
      }
      
      T* obj = m_objects.front();
      m_objects.pop_front();
      return obj;
   }

   void ReturnObject(T* obj)
   {
      if (nullptr == obj)
         return;

      m_objects.push_back(obj);
   }

private:
   std::list<T*> m_objects;

   ObjectPool() {}
   //ObjectPool(const ObjectPool&) = delete;
   //ObjectPool(ObjectPool&&) = delete;
   //ObjectPool& operator=(const ObjectPool&) = delete;
   //ObjectPool&& operator=(ObjectPool&&) = delete;

};
