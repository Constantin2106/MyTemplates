#pragma once
#include <functional>

/*
Description
   Simple tool to execute some special action 
   when variables are exiting from the scope.
   Will be useful to perform some actions 
   regardless of the point of exit from the scope.

Usage
   CleanScope(
      if()
      {
      ...
      }
      else
      {
      ...
      }

      delete ...
      delete[] ...
      ...
   );

History
   Created by Kostiantyn Zhelieznov   10/24/2018
*/
using CleanFunc = std::function<void()>;

class ScopeCleaner
{
   CleanFunc m_func{};

public:
   explicit ScopeCleaner(CleanFunc _func) : m_func(_func) {}

   ~ScopeCleaner()
   {
      // Execute s
      if (nullptr != m_func)
         m_func();
   }

   ScopeCleaner(const ScopeCleaner&) = delete;
   ScopeCleaner(ScopeCleaner&&) = delete;
   ScopeCleaner& operator= (const ScopeCleaner&) = delete;
   ScopeCleaner&& operator= (ScopeCleaner&&) = delete;
};

#define CleanScope(code) CleanFunc func = [&](){ code }; ScopeCleaner cs(func)

template<typename T, std::size_t N>
constexpr std::size_t arraySize(T (&)[N]) noexcept
{
   return N;
}
/*
int a[5];
const int N = arraySize(a);
int b[arraySize(a)];
*/