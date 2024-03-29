// MyClass.cpp : Defines the entry point for the console application.
//
#include <vld.h>

#include "src\Singleton.h"
#include "src\StdSingleton.h"
#include "src\TemplateSingleton.h"
#include "src\Module.h"

#include <conio.h>
#include <iostream>
#include <thread>
#include <chrono>

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

    /*auto singl = TemplateSingleton(BaseSingleton);
    cout << "Class name: " << singl.ClassName().c_str() << endl;

    auto singl_1 = TemplateSingleton(Singleton_1);
    cout << "Class name: " << singl_1.ClassName().c_str() << endl;

    auto singl_2 = TemplateSingleton(Singleton_2);
    cout << "Class name: " << singl_2.ClassName().c_str() << endl;*/

	/*auto sMyers = CreateSingleton<Singleton_2>();
	cout << "Class name: " << sMyers.ClassName().c_str() << endl;*/

	/*cout << "singl_11\n	Class name: " << CSingleton_1::Instance()->ClassName().c_str() << "\tAddr: " << CSingleton_1::Instance() << endl;
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
	}*/

	std::cout << "Main Thread Id " << std::hex << std::this_thread::get_id() << std::endl << std::flush;
	// Access the MyClass instance using std::shared_ptr
	//auto myInstanceShared = TemplateSingleton<MyClass>::Instance();

	std::thread tr1([] 
	{
		std::cout << "===> Thread 1 has been started. ThreadId " 
			<< std::hex << std::this_thread::get_id() << std::endl << std::flush;
		std::cout.clear();
		this_thread::sleep_for(chrono::milliseconds(100));

		// Access the MyClass instance using std::shared_ptr
		auto myInstanceShared = TemplateSingleton<MyClass>::Instance();
		
		// Access the MyClass instance using std::unique_ptr
		//auto myInstanceUnique = TemplateSingleton<MyClass, std::unique_ptr>::Instance();

		// Use the TemplateSingleton instances
		myInstanceShared->doSomething();
		//myInstanceUnique->doSomething();

		std::cout << "<=== Thread 1 completed. ThreadId " 
			<< std::hex << std::this_thread::get_id() << std::endl << std::flush;
		std::cout.clear();
		this_thread::sleep_for(chrono::milliseconds(100));
	});

	std::thread tr2([]
	{
		std::cout << "===> Thread 2 has been started. ThreadId " 
			<< std::hex << std::this_thread::get_id() << std::endl << std::flush;
		std::cout.clear();
		this_thread::sleep_for(chrono::milliseconds(100));

		auto atherShared = TemplateSingleton<MyClass>::Instance();
		atherShared->doSomething();

		std::cout << "<=== Thread 2 completed. ThreadId " 
			<< std::hex << std::this_thread::get_id() << std::endl << std::flush;
		std::cout.clear();
		this_thread::sleep_for(chrono::milliseconds(100));
	});

	std::thread tr3([] 
	{
		std::cout << "===> Thread 3 has been started. ThreadId " 
			<< std::hex << std::this_thread::get_id() << std::endl << std::flush;
		std::cout.clear();
		this_thread::sleep_for(chrono::milliseconds(100));

		function();

		std::cout << "<=== Thread 3 completed. ThreadId " 
			<< std::hex << std::this_thread::get_id() << std::endl << std::flush;
		std::cout.clear();
		this_thread::sleep_for(chrono::milliseconds(100));
	});

	tr1.join();
	tr2.join();
	tr3.join();

    return 0;
}

