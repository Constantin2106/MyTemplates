// MySingleton.cpp : Defines the entry point for the console application.
//
#include <vld.h>

#include "src\Singleton.h"
#include "src\StdSingleton.h"

#include <conio.h>
#include <iostream>

using namespace std;
using namespace Singleton;

class CSingleton_1 : public CSingleton<CSingleton_1>
{
	std::string m_class_name{ "Singleton_1" };

public:
	auto ClassName() { return m_class_name; }
};
class CSingleton_2 : public CSingleton<CSingleton_2>
{
	std::string m_class_name{ "Singleton_2" };

public:
	auto ClassName() { return m_class_name; }
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

	auto singl_1 = CSingleton_1::Instance();
	cout << "Class name: " << singl_1.ClassName().c_str() << endl;

	auto singl_2 = CSingleton_2::Instance();
	cout << "Class name: " << singl_2.ClassName().c_str() << endl;

	auto singl_22 = CSingleton_2::Instance();
	cout << "Class name: " << singl_22.ClassName().c_str() << endl;

    _getch();

    return 0;
}

