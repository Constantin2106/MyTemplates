#pragma once

#include <type_traits>
#include <assert.h>

template<typename Signature> class FuncRef;
/*
   Abstracts out any callable object without owning it or moving it to heap
   Useful when need to pass callback through translation unit boundary
   without exposing its internals

   !!!WARN!!! Does not prolong lifetime of wrapped object. So please keep watch of the lambdas lifetimes

   NB: only functors and normal free functions are supported. Use std::bind or lambdas to wrap member functions
*/
template<typename R, typename... Args>
class FuncRef<R(Args...)>
{
public:
   using Pointer = R(*)(Args...);
   using Callback = R(*)(void*, Args...);
   /*
       Wraps any compatible functor
   */
   template<typename Fn>
   FuncRef(Fn&& func)
      : m_context(reinterpret_cast<void*>(&func))
      , m_caller(&ObjectCaller<std::decay_t<Fn>>)
   { }
   /*
       Wraps any compatible free function
   */
   FuncRef(Pointer func)
      : m_context(reinterpret_cast<void*>(func))
      , m_caller(&FuncCaller)
   {
      assert(m_context);
   }

   FuncRef(FuncRef const&) = default;
   FuncRef& operator= (FuncRef const&) = default;
   /*
       Invoke wrapped callable
   */
   R operator() (Args... args) const
   {
      return (*m_caller)(m_context, std::forward<Args>(args)...);
   }

   Callback callback() const noexcept
   {
      return &CallbackCaller;
   }

   void* context() noexcept
   {
      return this;
   }

private:
   using Caller = R(*)(void*, Args&&... args);

   void*  m_context;
   Caller m_caller;

   template<typename Fn>
   static R ObjectCaller(void* object, Args&&... args)
   {
      return (*reinterpret_cast<Fn*>(object))(std::forward<Args>(args)...);
   }

   static R FuncCaller(void* func, Args&&... args)
   {
      return (reinterpret_cast<Pointer>(func))(std::forward<Args>(args)...);
   }

   static R CallbackCaller(void* context, Args... args)
   {
      FuncRef<R(Args...)>* funcref = (FuncRef<R(Args...)>*)context;
      return (funcref->m_caller)(funcref->m_context, std::forward<Args>(args)...);
   }
};