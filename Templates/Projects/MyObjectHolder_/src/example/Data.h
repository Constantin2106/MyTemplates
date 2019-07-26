#pragma once
#include "Holder.h"
/*
struct SX
{
	SX() : m_counter(0)
	{}
	void Action()
	{
		std::cout << ++m_counter << ": in action" << std::endl;
	}

	int m_counter;
};
template<>
void setObject<SX>(CSingletonHolder<SX>& holder)
{
	static SX x;
	holder = &x;
}
*/

class ICounter
{
public:
	virtual ~ICounter() {}

	virtual void Setup(int) = 0;
	virtual bool IsValid() = 0;
	virtual void Action() = 0;
	virtual bool Restore() = 0;
};

// CHolder class specialized with ICounter interface
#define USE_POST_CHECKER 1

template<typename TO>
class CCounterHolder : public CHolder<ICounter, TO>
{// TODO: The functionality can be enhancement here.
	using BASECLASS = CHolder<ICounter, TO>;

#if USE_POST_CHECKER == 1
	// It used to check the object validity 
	// just after its change.
	struct PostChecker
	{
		PostChecker(ICounter& objRef) : m_objRef(objRef) {}
		~PostChecker()
		{
			// Check the object's validity just after usage.
			std::cout << "After the object has just been used:\t";
			if (!m_objRef.IsValid())
			{
				std::cout << "Invalid state\t";
				m_objRef.Restore();
			}
			else
				std::cout << "Valid state" << std::endl;
		}
		decltype(auto) operator->()
		{
			return &m_objRef;
		}

		// Reference to the wrapped object
		ICounter& m_objRef;
	};
#endif
	void throwException(std::string str) override 
	{
		// TODO: It's possible to add specific info to str here
		BASECLASS::throwException(str);
	}
	
public:
#if USE_POST_CHECKER == 1
	PostChecker operator->()
	{ 
		return *BASECLASS::getObject();
	}
#else
	decltype(auto) operator->()
	{
		// Get the existing object or create a new one
		auto obj = BASECLASS::getObject();
		std::cout << "Before send object for usage:\t";
		// Check the object's validity before usage.
		if (!obj->IsValid())
		{
			//throwException("The object is in an invalid state.");
			std::cout << "Invalid state\t";
			obj->Restore();
		}
		else 
			std::cout << "Valid state" << std::endl;

		return obj;
	}
#endif
};

