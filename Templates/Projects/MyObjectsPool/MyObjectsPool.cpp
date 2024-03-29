// MyObjectsPool.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <vld.h>
#include <iostream>
#include <conio.h>

#include "src/ObjectPool.h"

class Class
{
public:
   Class() : m_state("Free") {}
   Class(const Class&) = default;
   Class(Class&&) = default;
   ~Class() {}
   
   void Set()
   {
      m_state = "Busy";
   }
   void Reset()
   {
      m_state = "Free";
   }

   std::string& State()
   {
      return m_state;
   }

private:
   std::string m_state;
};

int main()
{
   auto& objPool = ObjectPool(Class);
   objPool.AddObject(2);

   auto obj1 = objPool.GetObject();
   
   if (nullptr == obj1.get())
   {
      std::cout << "The object is empty" << std::endl;
      return{};
   }
   
   obj1->Set();
   std::cout << obj1->State().c_str() << std::endl;
   std::cout << "Working with object" << std::endl;
   obj1->Reset();
   std::cout << obj1->State().c_str() << std::endl;
   objPool.PutObject(std::move(obj1));

   std::cout << std::endl;

   objPool.AddObject(3);
   auto obj2 = objPool.GetObject();
   obj2->Set();
   std::cout << obj2->State().c_str() << std::endl;
   std::cout << "Working with object" << std::endl;
   obj2->Reset();
   std::cout << obj2->State().c_str() << std::endl;
   objPool.PutObject(std::move(obj2));

   std::cout << "Total objects " << objPool.Count() << std::endl;
   objPool.Clear();
   std::cout << "Total objects " << objPool.Count() << std::endl;

   getchar();

   return{};
}
