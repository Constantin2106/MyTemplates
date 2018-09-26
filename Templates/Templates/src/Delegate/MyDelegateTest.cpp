#include <conio.h>
#include <iostream>

int Increment(int n)
{
   return n + 1;
}

class Incrementor
{
public:
   Incrementor(){}
   ~Incrementor(){}

   int Increment(int i)
   {
      n = i;
      return ++n;
   }

private:
   int n;
};

class Decrementor
{
public:
   Decrementor(){}
   ~Decrementor(){}

   int Decrement(int i)
   {
      n = i;
      return --n;
   }

private:
   int n;
};

//template<int(*func)(int)>
//struct funcPtr
//{
//   int operator()(int a)
//   {
//      return (*func)(a);
//   }
//};

#include "MyDelegate.h"
size_t getPtrSize(char *ptr)
{
   return sizeof(ptr);
}

void MyDelegateTest()
{
   int i = 5;

   #pragma region Pointer to Function
   //   functionPointer fp = Increment;
   //   int n = fp(i);
   //   std::cout << "Pointer to Function" << "\n" << i << "\n" << n << std::endl;
   //#pragma endregion
   //
   //#pragma region Pointer to method
   //   methodPointer mp = &Incrementor::Increment;
   //   Incrementor incrementor;
   //   int m = (incrementor.*mp)(i);
   //
   //   std::cout << "Pointer to method" << "\n" << i << "\n" << m << std::endl;
   //#pragma endregion
   //
   #pragma region Template of Pointer to Function
      /*funcPtr<&Increment> fun_1;
      int k = fun_1(i);*/
      funcPtr<int, int> fPtr = Increment;
      int k = fPtr(i);
      
      std::cout << "Template of Pointer to Function" << "\n" << i << "\n" << k << std::endl;
   #pragma endregion
   //
   //#pragma region Template of Pointer to Metod
   //   methodPtr<Decrementor> mPtr = &Decrementor::Decrement;
   //   Decrementor decr;
   //   int l = (decr.*mPtr)(i);
   //
   //   std::cout << "Template of Pointer to Metod" << "\n" << i << "\n" << l << std::endl;
   #pragma endregion

#pragma region MyDelegate
   myDelegate<Decrementor, int, int> delegate = &Decrementor::Decrement;
   Decrementor decrementor;
   int d = (decrementor.*delegate)(i);

   std::cout << "My Delegate" << "\n" << i << "\n" << d << std::endl;
#pragma endregion

#pragma region SimpleDelegate
   auto del(&Decrementor::Decrement);
   Decrementor decr;
   int x = (decr.*del)(i+1);

   std::cout << "Simple Delegate" << "\n" << i << "\n" << x << std::endl;
#pragma endregion

   _getch();
}