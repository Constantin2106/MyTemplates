#pragma once

#include <type_traits>
#include <assert.h>

template<class Signature> class FunctionRef;
/*
   Abstracts out any callable object without owning it or moving it to heap
   Useful when need to pass callback through translation unit boundary
   without exposing its internals

   !!!WARN!!! Does not prolong lifetime of wrapped object. So please keep watch of the lambdas lifetimes

   NB: only functors and normal free functions are supported. Use std::bind or lambdas to wrap member functions
*/
template<class R, class... Ts>
class FunctionRef<R(Ts...)>
{
public:
   using Pointer = R(*)(Ts...);
   using Callback = R(*)(void*, Ts...);
   /*
       Wraps any compatible functor
   */
   template<class Fn>
   FunctionRef(Fn&& func)
      : m_context(reinterpret_cast<void*>(&func))
      , m_caller(&ObjectCaller<std::decay_t<Fn>>)
   { }
   /*
       Wraps any compatible free function
   */
   FunctionRef(Pointer func)
      : m_context(reinterpret_cast<void*>(func))
      , m_caller(&FuncCaller)
   {
      assert(m_context);
   }

   FunctionRef(FunctionRef const&) = default;
   FunctionRef& operator= (FunctionRef const&) = default;
   /*
       Invoke wrapped callable
   */
   R operator () (Ts... args) const
   {
      return (*m_caller)(m_context, std::forward<Ts>(args)...);
   }

   Callback callback() const noexcept
   {
      return &CallbackCaller;
   }

   void*    context() noexcept
   {
      return this;
   }

private:
   using Caller = R(*)(void*, Ts&&... args);

   void*    m_context;
   Caller   m_caller;

   template<class Fn>
   static R ObjectCaller(void* object, Ts&&... args)
   {
      return (*reinterpret_cast<Fn*>(object))(std::forward<Ts>(args)...);
   }

   static R FuncCaller(void* func, Ts&&... args)
   {
      return (reinterpret_cast<Pointer>(func))(std::forward<Ts>(args)...);
   }

   static R CallbackCaller(void* context, Ts... args)
   {
      FunctionRef<R(Ts...)>* funcref = (FunctionRef<R(Ts...)>*)context;
      return (funcref->m_caller)(funcref->m_context, std::forward<Ts>(args)...);
   }
};