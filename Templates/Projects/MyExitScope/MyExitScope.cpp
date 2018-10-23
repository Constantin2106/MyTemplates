// MyExitScope.cpp : Defines the entry point for the console application.
//
#include <vld.h>
#include <iostream>
#include <conio.h>

#include "src\ScopeExit.h"

template<typename T>
using ExFn = void(*)(T);

template<typename T> 
class ExitScope 
{
public:
   explicit ExitScope(ExFn<T> _fun, T _x) : m_func(_fun), m_x(_x) {}
   ~ExitScope()
   {
      if(m_func)
         m_func(m_x);
   }

private:
   ExFn<T> m_func = nullptr;
   T m_x = nullptr;
};

#define OnExitScope(var, code) auto __func__##__COUNTER__ = [](decltype(var) var){ code }; \
ExitScope<decltype(var)> se##__COUNTER__(__func__##__COUNTER__, var)

int main()
{
   //{
   //   double* x = new double[100];
   //   char* str = new char[10];

   //   OnExitScope(
   //      x,
   //      std::cout << "Free allocated memory" << std::endl;
   //      if(nullptr != x)
   //         delete[] x;
   //      std::cout << "Exit from scope" << std::endl;
   //   );

   //   OnExitScope(
   //      str,
   //      std::cout << "Free allocated memory" << std::endl;
   //   if(nullptr != str)
   //      delete[] str;
   //   std::cout << "Exit from scope" << std::endl;
   //   );
   //}

   {
      double* x = new double[100];
      OnScopeExit{ delete[] x; };
   }

   char* s = new char[100];
   OnScopeExit{ delete[] s; };


   getchar();

   return 0;
}

