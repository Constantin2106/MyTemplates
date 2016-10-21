#include "FactoryMethodTest.h"
#include "FactoryMethod.h"
#include <conio.h>
#include <iostream>

using namespace std;

void FactoryMethodTest()
{
	TClass_1* obj1 = (TClass_1*)TBaseClass::CreateObject("class_1");
	TClass_2* obj2 = (TClass_2*)TBaseClass::CreateObject("class_2");
	
	TBaseClass* obj3 = TBaseClass::CreateObject("11");

	cout << (obj1 != nullptr ? obj1->GetClassName().c_str() : "no obj1") << endl;
	cout << (obj2 != nullptr ? obj2->GetClassName().c_str() : "no obj2") << endl;
	cout << (obj3 != nullptr ? obj3->GetClassName().c_str() : "no obj3") << endl;

	obj2->Initialize();
	
	for (size_t n = 0, size = obj2->GetSize(); n < size; n++)
		cout << obj2->GetData(n) << endl;
	
	_getch();

	obj2->Free();

	delete obj1;
	delete obj2;
	delete obj3;
}