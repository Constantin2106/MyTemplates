#include "Singleton.h"

using namespace Singleton;

TSingleton* TSingleton::instance = nullptr;

TSingleton* TSingleton::Instance()
{
	if(instance == nullptr)
	{
		instance = new TSingleton();
	}

	return instance;
}

bool TSingleton::Destroy(TSingleton* singleton)
{
	if(instance)
	{
		if(singleton->size > 0)
		{
			singleton->Free();
		}

		delete instance;
		return true;
	}

	return false;
}

bool TSingleton::Initialize()
{
	size = 10;
	m_data = new int[size];

	return true;
}

bool TSingleton::Free()
{
	if(size > 0)
	{
		delete[] m_data;
		size = 0;

		return true;
	}

	return false;
}
