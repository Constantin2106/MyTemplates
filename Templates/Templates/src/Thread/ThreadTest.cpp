#include "ThreadTest.h"
#include "Thread.h"

#include <iostream>
#include <conio.h>
#include <mutex>

using namespace std;

void fun1(void* _data)
{
	*(int*)_data = 55;
}
void fun2(void* _data)
{
	long unsigned n = 0;
	char buff[100];
   cout << endl;
   while (n <= 1e+5)
	{ 
		_itoa_s(n, buff, 100, 10);
		cout << buff << '\r';
		n++;
	}

	strcpy_s((char*)_data, sizeof(_data), buff);
}

void ThreadTest()
{
	mutex mutexStream;
	int* data1 = new int;
	char* data2 = new char[20];

	TThread* thread1 = new TThread();
	TThread* thread2 = new TThread();

	thread1->Initialize();
	thread2->Initialize();

	thread1->AddTask(fun1, data1);
	thread2->AddTask(fun2, data2);

	thread1->Run();
	thread2->Run();
	
   char buff[5] = { 0 };
	_itoa_s(*data1, buff, 5, 10);
	cout << buff << endl;
	
	//thread2->Run();
	//cout << data2 << endl;

	_getch();

	thread1->Free();
	thread2->Free();

	delete data1;
	delete[] data2;
	delete thread1;
	delete thread2;
}