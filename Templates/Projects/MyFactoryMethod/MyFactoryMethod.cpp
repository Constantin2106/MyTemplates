// MyFactoryMethod.cpp : Defines the entry point for the console application.
//
#include <vld.h>

#include <conio.h>
#include <iostream>

#include "src\FactoryMethod.h"
#include "src\GenericFactory.h"

class Class1
{
public:
    Class1(int x, double y) : m_x(x), m_y(y)
    {};
	~Class1()
	{
        m_x = 0;  m_y = 0.;
	}

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
	~Class2()
	{
		m_type.clear();
	}

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


class Furniture
{
	std::wstring m_Name;

public:
	virtual ~Furniture() {}
	virtual void Show() = 0;
};
class Table : public Furniture
{
public:
	void Show() override
	{
		std::wcout << L"The big black wooden table" << std::endl;
	}
};
class Chair : public Furniture
{
public:
	void Show() override
	{
		std::wcout << L"The old rocking chair" << std::endl;
	}
};

void GenericFactoryTest()
{
	GenericFactory<Furniture> factory;

	factory.Register<Table>(L"Table");
	factory.Register<Chair>(L"Chair");

	auto f1 = factory.Create(L"Table");
	auto f2 = factory.Create(L"Chair");
	auto f3 = factory.Create(L"Chair");

	std::wcout << std::endl;
	f1->Show();
	f2->Show();
	f3->Show();
	std::wcout << std::endl;

}

void TemplateFactoryTest()
{

    auto cl1 = Factory<Class1, int, double>::Create(5, 2.6);
    auto cl2 = Factory<Class2, std::string>::Create("type 1");
    std::cout << cl1->GetClassName().c_str() << '\t' << cl1->GetX() << '\t' << cl1->GetY() << std::endl;
	std::cout << cl2->GetClassName().c_str() << '\t' << cl2->GetType().c_str() << std::endl;

    auto* cl11 = FactoryFun<Class1, int, double>(50, 12.6);
    auto* cl21 = FactoryFun<Class2, std::string>("type 2");
	std::cout << cl11->GetClassName().c_str() << '\t' << cl11->GetX() << '\t' << cl11->GetY() << std::endl;
	std::cout << cl21->GetClassName().c_str() << '\t' << cl21->GetType().c_str() << std::endl;
    
    auto c = _getch();

	Factory<Class1>::Destroy(cl1);
	Factory<Class2>::Destroy(cl2);

	DesroyFun(cl11);
	DesroyFun(cl21);


    /*delete cl1;
    delete cl2;

    delete cl11;
    delete cl21;*/
}

void FactoryMethodTest()
{
    auto* obj1 = static_cast<TClass_1*>(TBaseClass::CreateObject("class_1"));
    TClass_2* obj2 = static_cast<TClass_2*>(TBaseClass::CreateObject("class_2"));

    TBaseClass* obj3 = TBaseClass::CreateObject("11");

	std::cout << (obj1 != nullptr ? obj1->GetClassName().c_str() : "no obj1") << std::endl;
	std::cout << (obj2 != nullptr ? obj2->GetClassName().c_str() : "no obj2") << std::endl;
	std::cout << (obj3 != nullptr ? obj3->GetClassName().c_str() : "no obj3") << std::endl;

    obj2->Initialize();

    for(size_t n = 0, size = obj2->GetSize(); n < size; n++)
		std::cout << obj2->GetData(n) << std::endl;

    auto c = _getch();

    obj2->Free();

    delete obj1;
    delete obj2;
    delete obj3;
}

int main()
{
	GenericFactoryTest();

    /*TemplateFactoryTest();
    cout << endl;
    FactoryMethodTest();*/

	return{};
}

