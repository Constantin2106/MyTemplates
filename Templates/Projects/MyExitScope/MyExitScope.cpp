// MyExitScope.cpp : Defines the entry point for the console application.
//
#include <vld.h>

#include <iostream>
#include <conio.h>
#include <memory>

#include "src/ScopeCleaner.h"
#include "src/ScopeGuard.h"
#include "src/ScopeExit.h"

class MyClass
{
public:
   MyClass()
   {
      m_data = new int[10];
	  std::cout << "Contructor is performed" << std::endl << std::endl;
   }
   ~MyClass()
   {
      if (nullptr != m_data)
         delete[] m_data;
	  std::cout << "Destructor is performed" << std::endl << std::endl;
   }

   int* m_data{};

private:
};


int main()
{
   {
      double* x = new double[10];
	
	  //auto mc = std::make_unique<MyClass>();
	  MyClass* mc = new MyClass();

      /*CleanScope(
         std::cout << "Free allocated memory" << std::endl << std::endl;

         if (nullptr != x)
            delete[] x;

         std::cout << "Call destructor" << std::endl << std::endl;
         delete mc;

         std::cout << __FUNCTION__ << std::endl;
         std::cout << __FUNCSIG__ << std::endl << std::endl;

         std::cout << "Exit from scope" << std::endl;
      );*/
	  
	  SCOPE_EXIT
	  {
		 std::cout << "Free allocated memory" << std::endl << std::endl;
		 if (nullptr != x)
			delete[] x;

		 std::cout << "Call destructor" << std::endl << std::endl;
		 delete mc;

		 std::cout << __FUNCTION__ << std::endl;
		 std::cout << __FUNCSIG__ << std::endl << std::endl;
		 std::cout << "Exit from scope" << std::endl;
	  };

      for (unsigned i = 0; i < 10; ++i)
      {
         x[i] = i;
         std::cout << x[i] << " ";
         mc->m_data[i] = i*i;
         std::cout << mc->m_data[i] << " ";
      }
      std::cout << std::endl << std::endl;
   }

   _getch();

   return 0;
}

