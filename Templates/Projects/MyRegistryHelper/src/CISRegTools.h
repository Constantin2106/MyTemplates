
#pragma once
#include <list>

namespace RegTools
{

class Key
{
	mutable HKEY h_;
public:
	Key (HKEY key = NULL) : 
		h_ (key)
	{
	}
	Key (const Key & key) : 
		h_ (key.h_)
	{
		key.h_ = NULL;
	}
	~Key () 
	{ 
		Close (); 
	}
	void Close () 
	{
		if (h_ != NULL)
			RegCloseKey (h_);
		h_ = NULL;
	}
	Key & operator = (HKEY key) 
	{ 
		Close ();
		h_ = key; 
		return *this; 
	}
	Key & operator = (const Key & key) 
	{ 
		Close ();
		h_ = key.h_; 
		key.h_ = NULL;
		return *this; 
	}
	HKEY * operator & ()
	{
		return &h_;
	}
	operator HKEY () 
	{ 
		return h_; 
	}
	operator bool () 
	{
		return h_ != NULL; 
	}
};

Key CreateKey (HKEY hRootKey, const wchar_t * pSubKey, DWORD dwAccess, bool bNoWow64Redirect = true, bool bVolatile = false);
Key OpenKey (HKEY hRootKey, const wchar_t * pSubKey, DWORD dwAccess, bool bNoWow64Redirect = true);

Key CopyKey (HKEY hRootKeySrc, const wchar_t * pSubKeySrc, HKEY hRootKeyDest, const wchar_t * pSubKeyDest, bool bNoWow64Redirect = true);
bool CopyKey (HKEY hRootKeySrc, const wchar_t * pSubKeySrc, HKEY hKeyDest, bool bNoWow64Redirect = true);

bool WriteDword (HKEY hKey, const wchar_t * pName, DWORD dwValue);
bool WriteQword (HKEY hKey, const wchar_t * pName, DWORD64 dw64Value);
bool WriteString (HKEY hKey, const wchar_t * pName, const wchar_t * pValue, size_t lCharsLen = 0, DWORD dwType = REG_SZ);
bool WriteString (HKEY hKey, const wchar_t * pName, const std::wstring & szValue, DWORD dwType = REG_SZ);
bool WriteMultiString (HKEY hKey, const wchar_t * pName, const std::list <std::wstring> & lValues);
bool WriteData (HKEY hKey, const wchar_t * pName, const BYTE * pData, DWORD dwBytes);
bool WriteGuid (HKEY hKey, const wchar_t * pName, const GUID & guid, bool bWriteBraces = true);

DWORD ReadDword (HKEY hKey, const wchar_t * pName, DWORD dwDefValue = 0);
DWORD64 ReadQword (HKEY hKey, const wchar_t * pName, DWORD64 dw64DefValue = 0);
std::wstring ReadString (HKEY hKey, const wchar_t * pName, std::wstring szDefault = L"");
bool ReadMultiString (HKEY hKey, const wchar_t * pName, std::list <std::wstring> & lValues);
bool ReadData (HKEY hKey, const wchar_t * pName, std::vector <BYTE> & data);
GUID ReadGuid (HKEY hKey, const wchar_t * pName, bool bIgnoreBraces = true);

bool WriteCryptedString (HKEY hKey, const wchar_t * pName, const std::wstring & szValue);
bool WriteCryptedDword (HKEY hKey, const wchar_t * pName, DWORD dwValue);
bool WriteCryptedQword (HKEY hKey, const wchar_t * pName, DWORD64 dw64Value);

std::wstring ReadCryptedString (HKEY hKey, const wchar_t * pName, std::wstring szDefValue = L"");
DWORD ReadCryptedDword (HKEY hKey, const wchar_t * pName, DWORD dwDefValue = 0);
DWORD64 ReadCryptedQword (HKEY hKey, const wchar_t * pName, DWORD64 dw64DefValue = 0);

bool DeleteKey (HKEY hKey, const wchar_t * pSubkey, bool bNoWow64Redirect = true);
bool DeleteValue (HKEY hKey, const wchar_t * pValueName);
bool DeleteValue (HKEY hKey, const wchar_t * pSubkey, const wchar_t * pValueName, bool bNoWow64Redirect = true);

bool QueryKeyExists (HKEY hKey, const wchar_t * pSubKey, bool bNoWow64Redirect = true);
bool QueryValue (HKEY hKey, const wchar_t * pValue, DWORD * pdwType = NULL, DWORD * pdwSize = NULL, bool bNoWow64Redirect = true);
bool QueryValue (HKEY hRootKey, const wchar_t * pSubKey, const wchar_t * pValue, DWORD * pdwType = NULL, DWORD * pdwSize = NULL, bool bNoWow64Redirect = true);

bool EnumKey (HKEY hRootKey, const wchar_t * pSubKey, size_t * subkeys_count = nullptr, size_t * values_count = nullptr, std::list <std::wstring> * subkeys = nullptr, std::list <std::wstring> * values = nullptr, bool bNoWow64Redirect = true);

DWORD OpenKeyAndReadDword (HKEY hRootKey, const wchar_t * pSubKey, const wchar_t * pName, DWORD dwDefValue = 0, bool bNoWow64Redirect = true);
bool OpenKeyAndReadBool(HKEY hRootKey, const wchar_t * pSubKey, const wchar_t * pName, bool bDefValue = false, bool bNoWow64Redirect = true);
DWORD64 OpenKeyAndReadQword (HKEY hRootKey, const wchar_t * pSubKey, const wchar_t * pName, DWORD64 dw64DefValue = 0, bool bNoWow64Redirect = true);
std::wstring OpenKeyAndReadString (HKEY hRootKey, const wchar_t * pSubKey, const wchar_t * pName, std::wstring szDefValue = L"", bool bNoWow64Redirect = true);

bool CreateKeyAndWriteDword (HKEY hRootKey, const wchar_t * pSubKey, const wchar_t * pName, DWORD dwValue, bool bNoWow64Redirect = true);
bool CreateKeyAndWriteQword (HKEY hRootKey, const wchar_t * pSubKey, const wchar_t * pName, DWORD64 dw64Value, bool bNoWow64Redirect = true);
bool CreateKeyAndWriteString (HKEY hRootKey, const wchar_t * pSubKey, const wchar_t * pName, const wchar_t * pValue, size_t lCharsLen = 0, DWORD dwType = REG_SZ, bool bNoWow64Redirect = true);
bool CreateKeyAndWriteString(HKEY hRootKey, const wchar_t * pSubKey, const wchar_t * pName, const std::wstring & szValue, DWORD dwType = REG_SZ, bool bNoWow64Redirect = true);

bool SetKeyAccessToAll (HKEY hRoot, const wchar_t * pKey);
};