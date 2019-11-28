#include <iostream>

#include "src/StdSingleton.h"


Singleton::CSingleton_1* CreateSingleton()
{
	using namespace std;

	auto singl_13 = Singleton::CSingleton_1::Instance();
	cout << "singl_13\n	Class name: " << singl_13->ClassName().c_str() << "\tAddr: " << singl_13 << endl;

	singl_13->SetClassName("Singleton_13");

	return singl_13;
}
