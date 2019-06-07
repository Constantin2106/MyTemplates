// MyTemplates.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <vld.h>

#include <conio.h>
#include <iostream>

#include "Src/TTFunction.h"

bool Less(int a, int b)
{
	return a < b;
}
bool Equal(int a, int b)
{
	return a == b;
}
bool LEqu(bool a, bool b)
{
	return a == b;
}

int ISquare(int a)
{
	return a * a;
}
double DSquare(double a)
{
	return a * a;
}

bool IMoreThan100(int a, int b)
{
	return a * b > 100;
}
bool MoreThan100(double a, int b)
{
	return a * b > 100;
}

int main()
{
	std::cout << std::boolalpha;

	TComparator<int, &Less> l;
	TComparator<int, &Equal> e;
	TComparator<bool, &LEqu> le;

	std::cout << l(5, 7) << std::endl;
	std::cout << l(7, 5) << std::endl;
	std::cout << std::endl;

	std::cout << e(5, 7) << std::endl;
	std::cout << e(7, 7) << std::endl;
	std::cout << std::endl;

	std::cout << e(true, true) << std::endl;
	std::cout << e(false, true) << std::endl;
	std::cout << std::endl;


	TSquare<int, int, &ISquare> is;
	TSquare<double, double, &DSquare> ds;

	std::cout << is(3) << std::endl;
	std::cout << ds(3.0) << std::endl;
	std::cout << std::endl;


	//TTFunction<bool, int, int, &IMoreThan100> im100;

	std::cout << std::endl << "Press any key to exit..." << std::endl;
	_getch();
	return 0;
}
