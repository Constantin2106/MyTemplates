#include <thread>
#include <chrono>

#include "TemplateSingleton.h"

using namespace std;
using namespace Singleton;

void function()
{
	std::cout << "=========> Function started" << std::endl << std::flush;
	std::cout.clear();
	this_thread::sleep_for(chrono::milliseconds(100));
	
	auto shared = TemplateSingleton<MyClass>::Instance();
	shared->doSomething();

	//auto unique = TemplateSingleton<MyClass, std::unique_ptr>::Instance();
	//unique->doSomething();

	std::cout << "<========= Function completed" << std::endl << std::flush;
	std::cout.clear();
	this_thread::sleep_for(chrono::milliseconds(100));
}