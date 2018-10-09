#include "FactoryMethod.h"

TBaseClass::TCreators TBaseClass::Creators;

//---------------------------------------------------------------------------//
TBaseClass* TBaseClass::CreateObject(std::string name)
{
	if (Creators.empty())
	{
		Creators["class_1"] = &TClass_1::Create;
		Creators["class_2"] = &TClass_2::Create;
	}

	TBaseClass* object = nullptr;

	TCreators::iterator it = Creators.find(name);
	if (it != Creators.end())
		object = it->second();

	return object;
}

TBaseClass::~TBaseClass()
{
	Creators.clear();
}

//---------------------------------------------------------------------------//
bool TClass_2::Initialize()
{
	for (int n = 0; n < 10; n++)
		data.push_back(n);
	return true;
}

bool TClass_2::Free()
{ 
	data.clear();
	return true; 
}