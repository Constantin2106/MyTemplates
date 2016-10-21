#include "Singleton.h"

TSingleton* TSingleton::instance = nullptr;

TSingleton* TSingleton::Instance()
{
	if (instance == nullptr)
		instance = new TSingleton();
	return instance;
}

bool TSingleton::Initialize()
{
	size = 10;
	data = new int[size];
	
	return true;
}

bool TSingleton::Free()
{
	delete data;
	size = 0;

	return true;
}
