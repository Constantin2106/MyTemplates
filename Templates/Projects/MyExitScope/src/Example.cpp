// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <functional>
#include <sstream>

template<typename T, typename Deleter>
class AutoFreeResource
{

	AutoFreeResource(const AutoFreeResource&) = delete;
	AutoFreeResource(AutoFreeResource&&) = delete;
	AutoFreeResource& operator = (const AutoFreeResource&) = delete;
	AutoFreeResource& operator = (AutoFreeResource&&) = delete;

	T m_t{};
	Deleter m_d{};

public:

	explicit AutoFreeResource(const T& t,
		const Deleter& d) noexcept : m_t(t), m_d(d) { }

	~AutoFreeResource(void) noexcept
	{
		try
		{
			if(m_d)
				m_d(m_t);
		}
		catch (...) { }
	}

	operator T (void) const noexcept
	{
		return m_t;
	}
};

int main()
{
	AutoFreeResource<SC_HANDLE, decltype(&::CloseServiceHandle)> obj(::OpenSCManagerW({}, {}, SC_MANAGER_ALL_ACCESS), &::CloseServiceHandle);
	if (!obj)
	{
		auto dwError = ::GetLastError();
#ifdef _DEBUG
		std::wostringstream wos{};
		wos << L"Error OpenSCManagerW = " << dwError << std::endl;
		::OutputDebugStringW(wos.str().c_str()); // See screen after me
#endif	// _DEBUG
		return dwError;
	}
	return {};
}