// MyPriorityQueue.cpp : Defines the entry point for the console application.
//
#include <vld.h>
#include <iostream>
#include <conio.h>

#include "src\PriorityQueue.h"

int main()
{
   PriorityQueue<std::string> pq;

   pq.Push("First", 0);
   pq.Push("Second", 1);

   pq.Push("Higher", 10);

   std::cout << pq.Pop().c_str() << std::endl; 
   std::cout << pq.Pop().c_str() << std::endl;
   std::cout << pq.Pop().c_str() << std::endl;

   auto str = pq.Pop();

   getchar();

   return{};
}

