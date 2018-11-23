// MySingleton.cpp : Defines the entry point for the console application.
//
#include <vld.h>

#include "src\Singleton.h"

#include <conio.h>
#include <iostream>

using namespace std;
using namespace Singleton;

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

    auto singl = Singleton(BaseSingleton);
    cout << "Class name: " << singl.ClassName().c_str() << endl;

    auto singl_1 = Singleton(Singleton_1);
    cout << "Class name: " << singl_1.ClassName().c_str() << endl;

    auto singl_2 = Singleton(Singleton_2);
    cout << "Class name: " << singl_2.ClassName().c_str() << endl;

    _getch();

    return 0;
}

