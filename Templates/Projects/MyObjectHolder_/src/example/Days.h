#pragma once
#include "Data.h"

//--------------------- DayCounter ---------------------//
class DayCounter : public ICounter
{
	const int minValid = 1;
	const int maxValid = 7;

	int m_counter;

	void init(int val) { m_counter = val; }

public:
	DayCounter() : m_counter(0)		{ init(1); }
	~DayCounter()					{ std::cout << "Destructor DayCounter has been invoked" << std::endl; }

	// Interface implementaion
	void Setup(int val) final		{ m_counter = val; std::cout << "Set value: " << m_counter << std::endl; }
	bool IsValid() final			{ return m_counter >= minValid && m_counter <= maxValid; }
	void Action() final				{ std::cout << "actual state: " << ++m_counter << std::endl; }
	bool Restore() final			
	{ 
		std::cout << "Here an invalid number of the day must be restored\t"; 
		Setup(minValid);
		return true;
	}
};

template<>
void setObject<ICounter, DayCounter>(CHolder<ICounter, DayCounter>& holder)
{
	holder = std::move(std::make_shared<DayCounter>());
}



