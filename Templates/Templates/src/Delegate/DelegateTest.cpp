#include <conio.h>
#include <iostream>

#include "Delegate.h"

void DelegateTest()
{
	class MyClass_1
	{
	public:
		void Method_1(char* s)
		{
			std::cout << "Call Method_1 of MyClass_1 with argument \"" << s << "\"" << std::endl;
		}
		void Method_2(int x)
		{
			std::cout << "Call Method_2 of MyClass_1 with argument " << x << std::endl;
		}
	};

	MyClass_1 myClass_1;

	Delegate delegate;
	
	delegate.Connect(&myClass_1, &MyClass_1::Method_1);
	delegate("a1b2c3");

	delegate.Connect(&myClass_1, &MyClass_1::Method_2);
	delegate(22);


	std::cout << std::endl << std::endl;


	class MyClass_2
	{
	public:
		void Method_1(double pi)
		{
			std::cout << "Call Method_1 of MyClass_2 with argument " << pi << std::endl;
		}
	};

	MyClass_2 myClass_2;

	delegate.Connect(&myClass_2, &MyClass_2::Method_1);
	delegate(3.14);

   _getch();
}