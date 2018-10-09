// MySingleton.cpp : Defines the entry point for the console application.
//
#include <vld.h>

#include "src\Singleton.h"

#include <conio.h>
#include <iostream>

using namespace std;


int main()
{
    TSingleton* singleton = TSingleton::Instance();

    cout << (singleton != nullptr ? singleton->GetClassName().c_str() : "no singleton") << endl;

    singleton->Initialize();

    int size = singleton->GetSize();
    char buff[100];
    _itoa_s(size, buff, 100, 10);
    cout << (singleton != nullptr ? buff : "no singleton") << endl;

    singleton->Free();

    TSingleton::Destroy(singleton);

    _getch();

    return 0;
}

