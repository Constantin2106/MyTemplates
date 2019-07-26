// MyObjectHolder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "vld.h"
#include <iostream>

#include ".\example\Days.h"
#include ".\example\Months.h"

#define StartBlock(O)				\
	cout << "<------------ ";		\
	cout << O;						\
	cout << " ----------->" << endl
#define EndBlock cout << "<-------------------------->" << endl

int main()
{
	using namespace std;

	StartBlock("X");
	//CHolder<ICounter, MonthCounter> month;
	CCounterHolder<MonthCounter> month;
	try
	{
		//month->Setup(50);
		month->Action();
		month->Action();
		month.Clear();

		month->Setup(11);
		month->Action();
		month->Action();
		month->Action();
		month->Action();
	}
	catch (std::exception& e)
	{
		cout << e.what() << endl;
	}
	month.Clear();
	EndBlock;

	StartBlock("Y");
	//CHolder<ICounter, DayCounter> day;
	CCounterHolder<DayCounter> day;
	try
	{
		day->Setup(6);
		day->Action();
		day->Action();
		day->Action();
		day->Action();
	}
	catch (std::exception& e)
	{
		cout << e.what() << endl;
	}
	day.Clear();
	EndBlock;

	//system("pause");
}