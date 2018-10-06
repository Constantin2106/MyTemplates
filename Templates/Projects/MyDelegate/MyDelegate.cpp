// MyDelegate.cpp : Defines the entry point for the console application.
//
#include <vld.h>

#include <conio.h>
#include <iostream>

#include "src\Delegate.h"

class MyClass_1
{
public:
    void Method_1(char* s)
    {
        std::cout << "Call Method_1 of MyClass_1 with argument \"" << s << "\"" << std::endl;
    }
    void Method_2(int x)
    {
        std::cout << "Call Method_2 of MyClass_1 with argument " << x << std::endl;
    }
};

class MyClass_2
{
public:
    void Method_1(double pi)
    {
        std::cout << "Call Method_1 of MyClass_2 with argument " << pi << std::endl;
    }
};

int Increment(int n)
{
    return n + 1;
}

class Incrementor
{
public:
    Incrementor()
    {}
    ~Incrementor()
    {}

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
    Decrementor()
    {}
    ~Decrementor()
    {}

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
size_t getPtrSize(char *ptr)
{
    return sizeof(ptr);
}

#include "src\MyDelegate.h"

int main()
{
    MyClass_1 myClass_1;
    MyClass_2 myClass_2;

    Delegate delegate;

    delegate.Connect(&myClass_1, &MyClass_1::Method_1);
    delegate("a1b2c3");

    delegate.Connect(&myClass_1, &MyClass_1::Method_2);
    delegate(22);

    std::cout << std::endl << std::endl;

    delegate.Connect(&myClass_2, &MyClass_2::Method_1);
    delegate(3.14);

    std::cout << std::endl << std::endl;

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
    myDelegate<Decrementor, int, int> mydelegate = &Decrementor::Decrement;
    Decrementor decrementor;
    int d = (decrementor.*mydelegate)(i);

    std::cout << "My Delegate" << "\n" << i << "\n" << d << std::endl;
#pragma endregion

#pragma region SimpleDelegate
    auto del(&Decrementor::Decrement);
    Decrementor decr;
    int x = (decr.*del)(i + 1);

    std::cout << "Simple Delegate" << "\n" << i << "\n" << x << std::endl;
#pragma endregion


    _getch();

    return 0;
}

