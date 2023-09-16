// MySingleton.cpp : Defines the entry point for the console application.
//
//#include <vld.h>

#include "src\Singleton.h"
#include "src\StdSingleton.h"

#include <conio.h>
#include <iostream>

using namespace std;
using namespace Singleton;

class CSingleton_1 : public CSingleton<CSingleton_1>
{
	friend class CSingleton<CSingleton_1>;
	std::string m_class_name{ "Singleton_1" };
protected:
	CSingleton_1() {}
	/*CSingleton_1(const CSingleton_1&) = delete;
	CSingleton_1& operator=(const CSingleton_1&) = delete;*/

public:
	~CSingleton_1()
	{
		m_class_name.clear();
	}
	auto ClassName() { return m_class_name; }
	void SetClassName(std::string name) { m_class_name = name; }
};
class CSingleton_2 : public CSingleton<CSingleton_2>
{
	std::string m_class_name{ "Singleton_2" };

public:
	~CSingleton_2()
	{
		m_class_name.clear();
	}
	auto ClassName() { return m_class_name; }
	void SetClassName(std::string name) { m_class_name = name; }
};

int main()
{
    /*TSingleton* singleton = TSingleton::Instance();

    cout << (singleton != nullptr ? singleton->GetClassName().c_str() : "no singleton") << endl;

    singleton->Initialize();

    int size = singleton->GetSize();
    char buff[100];
    _itoa_s(size, buff, 100, 10);
    cout << "Data number: " << (singleton != nullptr ? buff : "no singleton") << endl;

    singleton->Free();

    TSingleton::Destroy(singleton);*/

    /*auto singl = Singleton(BaseSingleton);
    cout << "Class name: " << singl.ClassName().c_str() << endl;

    auto singl_1 = Singleton(Singleton_1);
    cout << "Class name: " << singl_1.ClassName().c_str() << endl;

    auto singl_2 = Singleton(Singleton_2);
    cout << "Class name: " << singl_2.ClassName().c_str() << endl;*/

	/*auto sMyers = CreateSingleton<Singleton_2>();
	cout << "Class name: " << sMyers.ClassName().c_str() << endl;*/

	cout << "singl_11\n	Class name: " << CSingleton_1::Instance()->ClassName().c_str() << "\tAddr: " << CSingleton_1::Instance() << endl;
	auto singl_11 = CSingleton_1::Instance();
	cout << "singl_11\n	Class name: " << singl_11->ClassName().c_str() << "\tAddr: " << singl_11 << endl;
	auto& singl_12 = CSingleton_1::Instance();
	cout << "singl_12\n	Class name: " << singl_12->ClassName().c_str() << "\tAddr: " << singl_12 << endl;

	singl_11->SetClassName("Class_11");
	cout << "singl_11\n	Class name: " << singl_11->ClassName().c_str() << "\tAddr: " << singl_11 << endl;
	cout << "singl_12\n	Class name: " << singl_12->ClassName().c_str() << "\tAddr: " << singl_12 << endl;
	
	{
		auto singl_21 = CSingleton_2::Instance();
		cout << "singl_21\n	Class name: " << singl_21->ClassName().c_str() << "\tAddr: " << singl_21 << endl;
		auto singl_22 = CSingleton_2::Instance();
		cout << "singl_22\n	Class name: " << singl_22->ClassName().c_str() << "\tAddr: " << singl_22 << endl;

		singl_21->SetClassName("Class_21");
		cout << "singl_21\n	Class name: " << singl_21->ClassName().c_str() << "\tAddr: " << singl_21 << endl;
		cout << "singl_22\n	Class name: " << singl_22->ClassName().c_str() << "\tAddr: " << singl_22 << endl;
	}

    //_getch();

    return 0;
}

