// MyExitScope.cpp : Defines the entry point for the console application.
//
#include <vld.h>
#include <iostream>
#include <conio.h>

template<typename T>
using ExFn = void(*)(T*);

template<typename T> 
class ScopeExit 
{
public:
   explicit ScopeExit(ExFn<T> _fun, T* _x) : m_func(_fun), m_x(_x) {}
   ~ScopeExit()
   {
      if(m_func)
         m_func(m_x);
   }

private:
   ExFn<T> m_func = nullptr;
   T* m_x = nullptr;
};

#define OnExitScope(T, code, var) auto exFunc = [](T* var){ code }; ScopeExit<T> se(exFunc, var)

int main()
{
   {
      double* x = new double[100];
      OnExitScope(
         double,
         std::cout << "Free allocated memory" << std::endl;
         if(nullptr != x)
            delete[] x;
         std::cout << "Exit from scope" << std::endl;
         , x
      );
   }

   getchar();
   return 0;
}

