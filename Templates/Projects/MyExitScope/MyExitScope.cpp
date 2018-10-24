// MyExitScope.cpp : Defines the entry point for the console application.
//
#include <vld.h>

#include <iostream>
#include <conio.h>

#include "src/ScopeCleaner.h"

class MyClass
{
public:
   MyClass()
   {
      m_data = new int[10];
   }
   ~MyClass()
   {
      if (nullptr != m_data)
         delete[] m_data;
   }

   int* m_data{};

private:
};


int main()
{
   {
      double* x = new double[10];

      MyClass* mc = new MyClass();

      CleanScope(
         std::cout << "Free allocated memory" << std::endl << std::endl;

         if (nullptr != x)
            delete[] x;

         std::cout << "Call destructor" << std::endl << std::endl;
         delete mc;

         std::cout << __FUNCTION__ << std::endl;
         std::cout << __FUNCSIG__ << std::endl << std::endl;

         std::cout << "Exit from scope" << std::endl;
      );

      for (unsigned i = 0; i < 10; ++i)
      {
         x[i] = i * i;
         std::cout << x[i] << " ";
         mc->m_data[i] = i;
         std::cout << mc->m_data[i] << " ";
      }
      std::cout << std::endl << std::endl;
   }


   getchar();

   return 0;
}

