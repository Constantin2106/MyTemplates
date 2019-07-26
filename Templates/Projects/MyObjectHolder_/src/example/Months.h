#pragma once
#include "Data.h"

//--------------------- MonthCounter ---------------------//
class MonthCounter : public ICounter
{
	const int minValid = 1;
	const int maxValid = 12;

	int m_counter;

	void init(int val) { m_counter = val; }

public:
	MonthCounter() : m_counter(0)	{ init(1); }
	~MonthCounter()					{ std::cout << "Destructor MonthCounter has been invoked" << std::endl; }

	// Interface implementaion
	void Setup(int val) final		{ m_counter = val; std::cout << "Set value: " << m_counter << std::endl; }
	bool IsValid() final			{ return m_counter >= minValid && m_counter <= maxValid; }
	void Action() final				{ std::cout << "actual state: " << ++m_counter << std::endl; }
	bool Restore() final			
	{ 
		std::cout << "Here an invalid number of the month must be restored\t"; 
		Setup(minValid);
		return true;
	}
};

template<>
void setObject<ICounter, MonthCounter>(CHolder<ICounter, MonthCounter>& holder)
{
	holder = std::move(std::make_shared<MonthCounter>());
}




