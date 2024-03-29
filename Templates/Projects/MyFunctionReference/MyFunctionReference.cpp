// MyFunctionReference.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <vld.h>
#include <conio.h>
#include <iostream>

#include "src/FuncRef.h"

int Call(int i, FuncRef<int(int)> func)
{
   std::cout << "Source i " << i << std::endl;
   return func(i + 1);
}

int f(int i)
{
   std::cout << "Next i " << i << std::endl;
   i *= 2;
   return i;
}

int main()
{
   int i = 10;
   FuncRef<decltype(f)> callFunc(f);

   int newI = Call(i, [](int i)
   {
      std::cout << "Next i " << i << std::endl;
      i *= 2;
      return i;
   });
   std::cout << "Final i " << newI << std::endl;

   getchar();

   i = 10;
   newI = Call(i, callFunc);
   std::cout << "Final i " << newI << std::endl;

   getchar();

   return{};
}

