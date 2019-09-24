#pragma once

#include <optional>

#include "RegHelper.h"
#include "RegistryTools.cpp"

template<typename T>
RetType WriteKey(RKey& key, const std::wstring& name, const T& value)
{
	using namespace Registry;

	auto keyStatus = GetKey(key, eAccessAndMask::Write);
	if (ERROR_SUCCESS != keyStatus)
	{
		ExitIfError(keyStatus);
	}

	auto writeStatus = WriteValue(key.GetKey(), name.c_str(), value);
	key.Close();
	if (ERROR_SUCCESS != writeStatus)
	{
		ExitIfError(writeStatus);
	}

	return{};
}

template<typename T>
RetType ReadKey(RKey& key, const std::wstring& name, T& value)
{
	using namespace Registry;

	auto keyStatus = GetKey(key, eAccessAndMask::Read);
	if (ERROR_SUCCESS != keyStatus)
	{
		ExitIfError(keyStatus);
	}

	eDataType dataType{};
	auto readStatus = ReadValue(key.GetKey(), name.c_str(), dataType, value);
	if (ERROR_SUCCESS != readStatus)
	{
		ExitIfError(readStatus);
	}

	return{};
}
