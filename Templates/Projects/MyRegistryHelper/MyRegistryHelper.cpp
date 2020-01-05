// RegistryHelper.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <vld.h>
#include <iostream>

#include "src/RegistryTools.h"


#define BASE_KEY		HKEY_CURRENT_USER

#define ROOT_KEY		L"Software"
#define TEST_KEY		ROOT_KEY L"\\Test"
#define MAIN_KEY		TEST_KEY L"\\First Test\\SubTest"


int main()
{
	std::wcout << std::boolalpha << std::endl;

	RKey testKey(HKEY_CURRENT_USER, TEST_KEY);
	auto delStatus = DeleteKeysTree(testKey);
	std::wcout << L"Delete keys tree: ";
	if (delStatus)
	{
		std::wcout << delStatus.value().first << std::endl;
	}
	else
		std::wcout << true << std::endl;

	RKey key(HKEY_CURRENT_USER, MAIN_KEY);

	RetType errorStatus;

	errorStatus = IsKeyExist(key);
	std::wcout << L"Key exist: ";
	if(errorStatus)
		std::wcout << errorStatus.value().first << std::endl;
	else
		std::wcout << true << std::endl;

	errorStatus = OpenKey(key);
	std::wcout << L"Key open: ";
	if (errorStatus)
	{
		std::wcout << errorStatus.value().first << std::endl;
	}
	else
		std::wcout << true << std::endl;
	
	errorStatus = DeleteKey(key);
	std::wcout << L"Delete key: ";
	if (errorStatus)
	{
		std::wcout << errorStatus.value().first << std::endl;
	}
	else
		std::wcout << true << std::endl;
	std::wcout << std::endl;

	errorStatus = WriteKey(key, L"Char", char(1));
	std::wcout << L"Char key write: ";
	if (errorStatus)
	{
		std::wcout << errorStatus.value().first << std::endl;
	}
	else
		std::wcout << true << std::endl;

	errorStatus = WriteKey(key, L"DWord", 123);
	std::wcout << L"DWord key write: ";
	if (errorStatus)
	{
		std::wcout << errorStatus.value().first << std::endl;
	}
	else
		std::wcout << true << std::endl;

	std::wstring wstr{ L"123456" };
	errorStatus = WriteKey(key, L"String", wstr);
	std::wcout << L"String key write: ";
	if (errorStatus)
	{
		std::wcout << errorStatus.value().first << std::endl;
	}
	else
		std::wcout << true << std::endl;

	errorStatus = WriteKey(key, L"Binary", std::vector<byte>{ 100, 101, 102 });
	std::wcout << L"Binary key write: ";
	if (errorStatus)
	{
		std::wcout << errorStatus.value().first << std::endl;
	}
	else
		std::wcout << true << std::endl;
	std::wcout << std::endl;

	int dwValue{};
	errorStatus = ReadKey(key, L"DWord", dwValue);
	std::wcout << L"DWORD value read: ";
	if (errorStatus)
	{
		std::wcout << errorStatus.value().first << std::endl;
	}
	else
		std::wcout << dwValue << std::endl;

	double dValue;
	errorStatus = ReadKey(key, L"DWord", dValue);
	std::wcout << L"Double value read: ";
	if (errorStatus)
	{
		std::wcout << errorStatus.value().first << std::endl;
	}
	else
		std::wcout << dValue << std::endl;

	std::wstring strValue{};
	errorStatus = ReadKey(key, L"String", strValue);
	std::wcout << L"String value read: ";
	if (errorStatus)
	{
		std::wcout << errorStatus.value().first << std::endl;
	}
	else
		std::wcout << strValue.c_str() << std::endl;
	
	std::vector<byte> binValue{};
	errorStatus = ReadKey(key, L"Binary", binValue);
	std::wcout << L"Binary value read: ";
	if (errorStatus)
	{
		std::wcout << errorStatus.value().first << std::endl;
	}
	else
	{
		for (DWORD i = 0; i < binValue.size(); ++i)
			std::wcout << binValue[i] << ' ';
		std::wcout << std::endl;
	}

	goto exit;

#pragma region  Registry functions usage
	/*using namespace Registry;

    std::wcout << "Hello World!\n\n"; 

	HKEY openedKey;
	auto openStatus = Registry::OpenKey(BASE_KEY, TEST_KEY, &openedKey);
	std::wcout << L"Open keys tree: " << openStatus << std::endl;
	auto delStatus = Registry::DeleteKeysTree(openedKey);
	std::wcout << L"Delete keys tree: " << delStatus << std::endl;
	auto closeStatus = Registry::CloseKey(openedKey);
	std::wcout << "Close key: " << closeStatus << std::endl;
	std::wcout << std::endl;

	HKEY hResKey;
	auto res = CreateKey(BASE_KEY, MAIN_KEY, &hResKey);
	std::wcout << "Create key: " << res << std::endl;

	res = CloseKey(hResKey);
	std::wcout << "Close key: " << res << std::endl;

	res = OpenKey(BASE_KEY, MAIN_KEY, &hResKey);
	std::wcout << "Open key: " << res << std::endl;

	int intData = 12345;
	res = WriteValue(hResKey, L"DwordValue", intData, eDataType::DWord);
	WCHAR astrData[] = { L"890" };
	res = WriteValue(hResKey, L"AStrValue", astrData, eDataType::String);
	const WCHAR* cstrData = L"67";
	res = WriteValue(hResKey, L"CStrValue", cstrData, eDataType::String);
	std::wstring strData(L"12345");
	res = WriteValue(hResKey, L"StrValue", strData, eDataType::String);
	std::vector<byte> byteArray{ 1, 2, 3, 4, 50 };
	res = WriteValue(hResKey, L"BinValue", byteArray, eDataType::Binary);
	std::wcout << "SetValue key: " << res << std::endl;
	std::wcout << std::endl;

	eDataType valType{};
	DWORD size{};

	DWORD dwData{};
	res = ReadValue(hResKey, L"DwordValue", valType, dwData);
	std::wcout << "GetValue key: " << res << std::endl;
	std::wcout << "ValueType: " << valType << std::endl;
	std::wcout << "data: " << dwData << std::endl;
	std::wcout << "size: " << size << std::endl;
	
	std::wstring wstrData{};
	res = ReadValue(hResKey, L"AStrValue", valType, wstrData);
	std::wcout << "GetValue key: " << res << std::endl;
	std::wcout << "ValueType: " << valType << std::endl;
	std::wcout << "data: " << wstrData.c_str() << std::endl;
	std::wcout << "size: " << size << std::endl;

	res = ReadValue(hResKey, L"CStrValue", valType, wstrData);
	std::wcout << "GetValue key: " << res << std::endl;
	std::wcout << "ValueType: " << valType << std::endl;
	std::wcout << "data: " << wstrData.c_str() << std::endl;
	std::wcout << "size: " << size << std::endl;

	res = ReadValue(hResKey, L"StrValue", valType, wstrData);
	std::wcout << "GetValue key: " << res << std::endl;
	std::wcout << "ValueType: " << valType << std::endl;
	std::wcout << "data: " << wstrData.c_str() << std::endl;
	std::wcout << "size: " << size << std::endl;

	std::vector<byte> wbyteArray{};
	res = ReadValue(hResKey, L"BinValue", valType, wbyteArray);
	std::wcout << "GetValue key: " << res << std::endl;
	std::wcout << "ValueType: " << valType << std::endl;
	std::wcout << "data: ";
	for(DWORD i = 0; i < wbyteArray.size(); ++i)
		std::wcout << wbyteArray[i] << ' ';
	std::wcout << std::endl;
	std::wcout << "size: " << size << std::endl;

	byte* pByte{};
	res = ReadValue(hResKey, L"BinValue", valType, pByte, size);
	std::wcout << "GetValue key: " << res << std::endl;
	std::wcout << "ValueType: " << valType << std::endl;
	std::wcout << "data: ";
	for (DWORD i = 0; i < size; ++i)
		std::wcout << *(pByte+i) << ' ';
	std::wcout << std::endl;
	std::wcout << "size: " << size << std::endl;
	std::wcout << std::endl;
	free(pByte);

	//res = DeleteKey(hResKey);
	//std::wcout << "Delete key: " << res << std::endl;
	
	res = CloseKey(hResKey);
	std::wcout << "Close key: " << res << std::endl;

	std::wcout << "\nBy World!\n";
	goto exit;*/
#pragma endregion	



exit:
	system("pause");
	return{};
}

