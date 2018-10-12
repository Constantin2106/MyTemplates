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
   //objPool.Generate(5);

   std::unique_ptr<Class> obj = objPool.GetObject();

   if (nullptr == obj.get())
   {
      std::cout << "The object is empty" << std::endl;
      goto exit;
   }
   
   obj->Set();
   std::cout << obj->State().c_str() << std::endl;

   std::cout << "Working with object" << std::endl;

   obj->Reset();
   std::cout << obj->State().c_str() << std::endl;

   objPool.ReturnObject(std::move(obj));
   
exit:
   getchar();

   return{};
}
