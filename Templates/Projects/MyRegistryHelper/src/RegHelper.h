#pragma once

#include <Windows.h>
#include <utility>

#include "RegData.h"

namespace Registry
{
	namespace getters 
	{
		template<typename T>
		dataParam getParam(const T& data)
		{
			return dataParam{ ERROR_INVALID_DATATYPE, nullptr, 0 };
		}
		template<> dataParam getParam(const DWORD& data)
		{
			return dataParam{ data::EnumToDataType[DWord], (LPBYTE)&data, (DWORD)sizeof(data) };
		}
		template<> dataParam getParam(const int& data)
		{
			return dataParam{ data::EnumToDataType[DWord], (LPBYTE)&data, (DWORD)sizeof(data) };
		}
		template<> dataParam getParam(const wchar_t* const& data)
		{
			return dataParam{ data::EnumToDataType[String], (LPBYTE)data, (DWORD)((wcslen(data) + 1) * sizeof(wchar_t)) };
		}
		//template<std::size_t N> void f(const wchar_t (&data)[N]) {}
		template<> dataParam getParam(const wchar_t (&data)[4])
		//template<> dataParam getParam(std::array_ref<const wchar_t> data)
		{
			return dataParam{ data::EnumToDataType[String], (LPBYTE)data, (DWORD)((wcslen(data) + 1) * sizeof(wchar_t)) };
		}
		template<> dataParam getParam(const std::wstring& data)
		{
			return dataParam{ data::EnumToDataType[String], (LPBYTE)data.c_str(), (DWORD)((data.length() + 1) * sizeof(wchar_t)) };
		}
		template<> dataParam getParam(const std::vector<byte>& data)
		{
			return dataParam{ data::EnumToDataType[Binary], (LPBYTE)&data[0], (DWORD)data.size() };
		}
	}
	namespace setters
	{
		template<typename T>
		LSTATUS setData(T& data, LPBYTE value, const DWORD size)
		{
			return ERROR_INVALID_DATATYPE;
		}
		template<> LSTATUS setData(DWORD& data, LPBYTE value, const DWORD size)
		{
			memmove(&data, value, size);
			return ERROR_SUCCESS;
		}
		template<> LSTATUS setData(int& data, LPBYTE value, const DWORD size)
		{
			memmove(&data, value, size);
			return ERROR_SUCCESS;
		}
		template<> LSTATUS setData(wchar_t*& data, LPBYTE value, const DWORD size)
		{
			data = static_cast<wchar_t*>(malloc(size));
			memmove(data, value, size);
			return ERROR_SUCCESS;
		}
		template<> LSTATUS setData(byte*& data, LPBYTE value, const DWORD size)
		{
			data = static_cast<byte*>(malloc(size));
			memmove(data, value, size);
			return ERROR_SUCCESS;
		}
		template<> LSTATUS setData(std::wstring& data, LPBYTE value, const DWORD size)
		{
			data.clear();
			data.resize(size);
			memmove(&data[0], value, size);
			return ERROR_SUCCESS;
		}
		template<> LSTATUS setData(std::vector<byte>& data, LPBYTE value, const DWORD size)
		{
			data.clear();
			data.resize(size);
			memmove(&data[0], value, size);
			return ERROR_SUCCESS;
		}
	}

#pragma region Check parameters validity
#define ExitIfInvalidKey						\
	if(nullptr == hKey)							\
		return ERROR_INVALID_HANDLE;

#define ExitIfInvalidSubkey						\
	if (nullptr == subKey)						\
		return ERROR_PATH_NOT_FOUND;			

#define ExitIfInvalidResultKey					\
	if (nullptr == phkResult)					\
		return ERROR_INVALID_HANDLE;			

#define ExitIfInvalidAccess						\
	if (access >= eAccessAndMask::ACC_SIZE)		\
		return ERROR_INVALID_ACCESS;			

#define ExitIfInvalidOptions					\
	if (options >= eOptions::OPT_SIZE)			\
		return ERROR_INVALID_PARAMETER;

#define ExitIfInvalidName						\
	if (nullptr == valName)						\
		return ERROR_INVALID_NAME;

#define ExitIfInvalidType						\
	if (valType >= eDataType::VTYPE_SIZE)		\
		return ERROR_INVALID_PARAMETER;

#pragma endregion

	auto OpenKey(const HKEY& hKey, const wchar_t* subKey, HKEY* const phkResult,
		eAccessAndMask access = eAccessAndMask::Full)
	{
		ExitIfInvalidKey
		ExitIfInvalidSubkey
		ExitIfInvalidResultKey
		ExitIfInvalidAccess

		return ::RegOpenKeyEx(hKey, subKey, 0, data::EnumToAccess[access], phkResult);
	}
	auto CloseKey(const HKEY& hKey)
	{
		ExitIfInvalidKey
		return ::RegCloseKey(hKey);
	}
	auto CreateKey(const HKEY& hKey, const wchar_t* subKey, HKEY* const phkResult,
		eOptions options = eOptions::Store,
		eAccessAndMask access = eAccessAndMask::Full,
		const LPSECURITY_ATTRIBUTES sAttributes = nullptr,
		LPDWORD const disp = nullptr)
	{
		ExitIfInvalidKey
		ExitIfInvalidSubkey
		ExitIfInvalidResultKey
		ExitIfInvalidAccess
		ExitIfInvalidOptions

		return ::RegCreateKeyEx(hKey, subKey, 0, nullptr, options, data::EnumToAccess[access], sAttributes, phkResult, disp);
	}
	auto DeleteKey(const HKEY& hKey, 
		const wchar_t* subKey = L"",
		eAccessAndMask access = eAccessAndMask::Wow64_64)
	{
		ExitIfInvalidKey
		ExitIfInvalidSubkey
		ExitIfInvalidAccess

		return ::RegDeleteKeyEx(hKey, subKey, data::EnumToAccess[access], 0);
	}
	auto DeleteKeysTree(const HKEY& hKey)
	{
		ExitIfInvalidKey

		auto status = ::RegDeleteTree(hKey, NULL);
		if (ERROR_SUCCESS != status)
			return status;

		return DeleteKey(hKey);
	}

	template<typename T> class Ref
	{
	public:
		Ref(const T value) : m_Value(value) {}
		operator T&() const { return (T&)m_Value; }
	private:
		T m_Value;
	};
	
	template<typename T>
	auto WriteValue(const HKEY& hKey, const wchar_t* valName, const T& data, 
		const eDataType& valType = Ref<eDataType>(eDataType::DWord))
	{
		ExitIfInvalidKey
		ExitIfInvalidName
		ExitIfInvalidType

		auto param = getters::getParam(data);
		if (ERROR_INVALID_DATATYPE == param.type
			|| nullptr == param.data
			|| 0 == param.size)
			return ERROR_INVALID_DATATYPE;

		return ::RegSetValueEx(hKey, valName, 0, param.type, param.data, param.size);
	}
	
	//static DWORD defSize = -1;
	template<typename T>
	auto ReadValue(const HKEY& hKey, const wchar_t* valName, eDataType& valType, T& data, DWORD& dSize = Ref<DWORD>(-1))
	{
		ExitIfInvalidKey
		ExitIfInvalidName

		DWORD vType{};
		DWORD size{};

		auto sizeStatus = ::RegQueryValueEx(hKey, valName, nullptr, &vType, nullptr, (LPDWORD)&size);
		if (ERROR_SUCCESS != sizeStatus)
			return sizeStatus;
		if (0 == size)
			return ERROR_INVALID_PARAMETER;

		valType = data::DataTypeToEnum[vType];

		ExitIfInvalidType

		auto value = (LPBYTE)malloc(size);

		auto readStatus = ::RegQueryValueEx(hKey, valName, nullptr, &vType, value, (LPDWORD)&size);
		if (ERROR_SUCCESS != readStatus)
			return readStatus;
		// TODO: Is it necessary to check the new size after the data has been read?

		dSize = size;
		auto setStatus = setters::setData(data, value, size);
		free(value);

		return setStatus;
	}
}// namespace Registry
