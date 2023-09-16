#pragma once

#include <string>
#include <memory>

#include "StrategyClasses.h"

template<typename T>
void Handle(std::string str)
{
	T manipulator(str);
	manipulator.Step_1();
	manipulator.Step_2();
	manipulator.Step_3();
}	

template <typename T>
class Strategy
{
	std::unique_ptr<T> mStrategy{};

public:
	Strategy(std::string data)
	{
		mStrategy.reset(new T(data));
	}

	~Strategy()
	{
		mStrategy.reset();
	}


	void Run()
	{
		mStrategy->Step_1();
		mStrategy->Step_2();
		mStrategy->Step_3();
	}

};

