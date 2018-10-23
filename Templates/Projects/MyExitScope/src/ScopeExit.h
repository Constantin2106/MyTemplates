#pragma once

#include <utility>
#include <functional>

/*
Description
   Prefix macro for constructing anonymous scope exit guards
   The 'block' prefixed is packed into lambda and assigned to anonymous scope guard object,
   which calls that lambda in destructor

Usage
   // NB: semicolon is required at the end because it's a lambda in fact
   OnScopeExit { PK_MEMORY_free(pkMemoryPointer); };
*/
#define OnScopeExit auto __scope_guard_ ## __COUNTER__ = (ScopedExitTag {}) % [&]()

template<typename T>
class ScopedExit
{
   ScopedExit(const ScopedExit&) = delete;
   void operator=(const ScopedExit&) = delete;

public:

   ScopedExit(T&& func) :
      m_enabled(true),
      m_valid(true),
      m_f(std::move(func))
   {}

   ScopedExit(ScopedExit<T>&& other) :
      m_enabled(other.m_enabled),
      m_valid(other.m_valid),
      m_f(std::move(other.m_f))
   {
      other.m_valid = false;
   }

   ~ScopedExit()
   {
      if(m_enabled && m_valid)
      {
         m_f();
      }
   }

   void Enable()
   {
      m_enabled = true;
   }
   void Disable()
   {
      m_enabled = false;
   }

   void operator=(ScopedExit<T>&& other)
   {
      m_enabled = other.m_enabled;
      m_valid = other.m_valid;
      m_f = std::move(other.m_f);

      other.m_valid = false;
   }

private:
   bool m_enabled;
   bool m_valid;
   T m_f;
};

template<typename Fn>
ScopedExit<Fn> MakeScopedExit(Fn&& func)
{
   return ScopedExit<Fn>(std::forward<Fn>(func));
}

template<typename Fn, typename T, typename... Ts>
decltype(auto) MakeScopedExit(Fn&& func, T&& arg, Ts&&... args)
{
   return MakeScopedExit(std::bind(func, std::forward<T>(arg), std::forward<Ts>(args)...));
}

template<typename T, typename... U>
void UnusedVar(T&&, U&&...) {}

/*
Description
   Helper tag type used in OnScopeExit macro

History
   Igor Baidiuk                04/07/2017  Creation
*/
struct ScopedExitTag
{};
/*
Description
   Helper operator for OnScopeExit macro which allows to use it as kind of prefix keyword

Arguments
   tag     - tag object, used in operator overload resolution
   func    - arbitrary callable object which would be packed into ScopedExit object

Return
   instance of ScopedExit guard
*/
template<class Fn>
decltype(auto) operator%(ScopedExitTag tag, Fn&& func)
{
   UnusedVar(tag);
   return MakeScopedExit(std::forward<Fn>(func));
}


