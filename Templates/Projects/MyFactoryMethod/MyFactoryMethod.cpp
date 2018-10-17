// MyFactoryMethod.cpp : Defines the entry point for the console application.
//
#include <vld.h>

#include <conio.h>
#include <iostream>

#include "src\FactoryMethod.h"

using namespace std;

class Class1
{
public:
    Class1(int x, double y) : m_x(x), m_y(y)
    {};

    std::string GetClassName()
    {
        return m_name;
    }
    int GetX()
    {
        return m_x;
    }
    double GetY()
    {
        return m_y;
    }

private:
    const std::string m_name = "Class1";

    int m_x;
    double m_y;
};
class Class2
{
public:
    Class2(std::string type) : m_type(type)
    {};

    std::string GetClassName()
    {
        return m_name;
    }
    std::string GetType()
    {
        return m_type;
    }

private:
    const std::string m_name = "Class2";

    std::string m_type;
};

void TemplateFactoryTest()
{

    auto cl1 = Factory<Class1, int, double>::Create(5, 2.6);
    auto cl2 = Factory<Class2, std::string>::Create("type");
    cout << cl1->GetClassName().c_str() << '\t' << cl1->GetX() << '\t' << cl1->GetY() << endl;
    cout << cl2->GetClassName().c_str() << '\t' << cl2->GetType().c_str() << endl;

    auto* cl11 = FactoryFun<Class1, int, double>(50, 12.6);
    auto* cl21 = FactoryFun<Class2, std::string>("type");
    cout << cl11->GetClassName().c_str() << '\t' << cl11->GetX() << '\t' << cl11->GetY() << endl;
    cout << cl21->GetClassName().c_str() << '\t' << cl21->GetType().c_str() << endl;
    
    _getch();

    delete cl1;
    delete cl2;

    delete cl11;
    delete cl21;
}

void FactoryMethodTest()
{
    auto* obj1 = static_cast<TClass_1*>(TBaseClass::CreateObject("class_1"));
    TClass_2* obj2 = static_cast<TClass_2*>(TBaseClass::CreateObject("class_2"));

    TBaseClass* obj3 = TBaseClass::CreateObject("11");

    cout << (obj1 != nullptr ? obj1->GetClassName().c_str() : "no obj1") << endl;
    cout << (obj2 != nullptr ? obj2->GetClassName().c_str() : "no obj2") << endl;
    cout << (obj3 != nullptr ? obj3->GetClassName().c_str() : "no obj3") << endl;

    obj2->Initialize();

    for(size_t n = 0, size = obj2->GetSize(); n < size; n++)
        cout << obj2->GetData(n) << endl;

    _getch();

    obj2->Free();

    delete obj1;
    delete obj2;
    delete obj3;
}

int main()
{
    TemplateFactoryTest();
    cout << endl;
    FactoryMethodTest();

    return 0;
}

