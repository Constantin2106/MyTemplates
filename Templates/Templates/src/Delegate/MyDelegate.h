#pragma once

// Pointer to Function
using functionPointer = int(*)(int);
// Template of Pointer to Function
template<typename R, typename A>
using funcPtr = R(*)(A);

// Pointer to method
using methodPointer = int(Incrementor::*)(int);
// Template of Pointer to Metod 1
template<typename C>
using methodPtr = int(C::*)(int);
// Template of Pointer to Metod 2
template<typename C, typename R, typename A>
using myDelegate = R(C::*)(A);

template<typename C, typename R, typename A>
class SimpleDelegate
{
public:
   using methodPtr = R(C::*)(A);

   /*static SimpleDelegate<C, R, A>& Connect(R(C::*metod)(A))
   {
   SimpleDelegate<C, R, A> dlg = metod;
   return dlg;
   }*/

   SimpleDelegate(methodPtr metod)
   {
      dlg = metod;
   }
   ~SimpleDelegate()
   {};

private:
   methodPtr dlg;
};

//class MyDelegate
//{
//public:
//   template<typename R, typename A>
//   static MyDelegate Delegate(R(*method)(A))
//   {
//      MyDelegate d;
//      d.arg = 
//
//      return My
//
//   }
//
//   template<typename A>
//   void operator()(A arg)
//   {
//      /*Arguments<A> a(arg);
//      m_parameters->Call(&a);*/
//      call(arg);
//   }
//
//   ~MyDelegate()
//   {}
//
//private:
//   typedef void(*m_call)(int arg);
//   int arg;
//
//   MyDelegate() : m_call(nullptr){}
//};

