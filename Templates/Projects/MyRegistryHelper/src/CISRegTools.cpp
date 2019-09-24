
#include "stdafx.h"
#include ".\regtools.h"
#include "InstallerApp.h"
#include "tools\Logging.h"
#include "tools\Tools.h"

namespace RegTools
{

Key CreateKey (HKEY hRootKey, const wchar_t * pSubKey, DWORD dwAccess, bool bNoWow64Redirect /*= true*/, bool bVolatile /*= false*/)
{
	Key hKey;
	if (pSubKey != NULL)
	{
		if (bVolatile)
		{
			std::wstring keyname = pSubKey;
			std::wstring::size_type _p = keyname.find_last_of (L'\\');
			if (_p != std::wstring::npos)
			{
				keyname.erase (_p);
				CreateKey (hRootKey, keyname.c_str (), dwAccess, bNoWow64Redirect, false);
			}
		}

		LONG lRes = RegCreateKeyEx (hRootKey, pSubKey, 0, NULL, (bVolatile ? REG_OPTION_VOLATILE : REG_OPTION_NON_VOLATILE), dwAccess | (bNoWow64Redirect ? KEY_WOW64_64KEY : 0), NULL, &hKey, NULL);
		if (lRes != ERROR_SUCCESS)
		{
			LogW (L"Cannot create key '%s': %d", pSubKey, lRes);
			hKey.Close ();
			Tools::SetErrorIfNoError (lRes);
		}
	}
	return hKey;
}

Key OpenKey (HKEY hRootKey, const wchar_t * pSubKey, DWORD dwAccess, bool bNoWow64Redirect /*= true*/)
{
	Key hKey;
	if (pSubKey != NULL)
	{
		LONG lRes = RegOpenKeyEx (hRootKey, pSubKey, 0, dwAccess | (bNoWow64Redirect ? KEY_WOW64_64KEY : 0), &hKey);
		if (lRes != ERROR_SUCCESS)
		{
			LogW (L"Cannot open key '%s': %d", pSubKey, lRes);
			hKey.Close ();
			Tools::SetErrorIfNoError (lRes);
		}
	}
	return hKey;
}

LONG CopyKeyRecursive (HKEY hSrcParent, HKEY hTargParent, wchar_t * pSrcKey, wchar_t * pTargKey, bool bNoWow64Redirect /*= true*/)
{
	Key hTargKey;
	LONG lRes = RegCreateKeyEx (hTargParent, (NULL == pTargKey ? L"" : pTargKey), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | (bNoWow64Redirect ? KEY_WOW64_64KEY : 0), NULL, &hTargKey, NULL);
	if (lRes != ERROR_SUCCESS)
		return lRes;

	Key hSrcKey;
	lRes = RegOpenKeyEx (hSrcParent, (NULL == pSrcKey ? L"" : pSrcKey), 0, KEY_ENUMERATE_SUB_KEYS | KEY_READ | (bNoWow64Redirect ? KEY_WOW64_64KEY : 0), &hSrcKey);
	if (lRes != ERROR_SUCCESS)
		return lRes;

	DWORD dwMaxKeyNameLen = 0, dwMaxValueNameLen = 0, dwMaxValueLen = 0;
	RegQueryInfoKey (hSrcKey, NULL, NULL, NULL, NULL, &dwMaxKeyNameLen, NULL, NULL, &dwMaxValueNameLen, &dwMaxValueLen, NULL, NULL);

	std::vector <wchar_t> vSubKeyName (++dwMaxKeyNameLen);
    DWORD dwIndex = 0, dwSize = dwMaxKeyNameLen;
	
	FILETIME tTime = { };
    while (RegEnumKeyEx (hSrcKey, dwIndex++, &vSubKeyName [0], &dwSize, 0, NULL, NULL, &tTime) == ERROR_SUCCESS)
    {
		lRes = CopyKeyRecursive (hSrcKey, hTargKey, &vSubKeyName [0], &vSubKeyName [0], bNoWow64Redirect);
		if (lRes != ERROR_SUCCESS)
			return lRes;

        dwSize = dwMaxKeyNameLen;
    }

	std::vector <wchar_t> vValueName (++dwMaxValueNameLen);
	std::vector <BYTE> vData (dwMaxValueLen);

    DWORD dwType = 0;
    dwIndex = 0;
    dwSize = dwMaxValueNameLen;
	DWORD dwDataSize = dwMaxValueLen;

    while (RegEnumValue (hSrcKey, dwIndex++, &vValueName [0], &dwSize, NULL, NULL, &vData [0], &dwDataSize) == ERROR_SUCCESS)
    {
		lRes = RegQueryValueEx (hSrcKey, &vValueName [0], NULL, &dwType, &vData [0], &dwDataSize);
		if (lRes != ERROR_SUCCESS)
			return lRes;

		lRes = RegSetValueEx (hTargKey, &vValueName [0], NULL, dwType, &vData [0], dwDataSize);
		if (lRes != ERROR_SUCCESS)
			return lRes;

        dwSize = dwMaxValueNameLen;
        dwDataSize = dwMaxValueLen;
    }

	return ERROR_SUCCESS;
}

Key CopyKey (HKEY hRootKeySrc, const wchar_t * pSubKeySrc, HKEY hRootKeyDest, const wchar_t * pSubKeyDest, bool bNoWow64Redirect /*= true*/)
{
	Key dest = CreateKey (hRootKeyDest, pSubKeyDest, KEY_ALL_ACCESS, bNoWow64Redirect, false);
	Key src = OpenKey (hRootKeySrc, pSubKeySrc, KEY_ALL_ACCESS, bNoWow64Redirect);
	if (dest && src)
	{
		LONG lRes = ERROR_SUCCESS;
		if (AppCheck.IsWindows_XP ())
			lRes = CopyKeyRecursive(src, dest, L"", L"", bNoWow64Redirect);
		else
			lRes = SHCopyKey (src, L"", dest, 0);
		if (lRes != ERROR_SUCCESS)
		{
			LogW (L"Cannot copy key '%s' to '%s': %d", pSubKeySrc, pSubKeyDest, lRes);
			dest.Close ();
			Tools::SetErrorIfNoError (lRes);
		}
	}
	else
		dest.Close ();
		
	return dest;
}

bool CopyKey (HKEY hRootKeySrc, const wchar_t * pSubKeySrc, HKEY hKeyDest, bool bNoWow64Redirect /*= true*/)
{
	bool bDone = false;
	Key src = OpenKey (hRootKeySrc, pSubKeySrc, KEY_ALL_ACCESS, bNoWow64Redirect);
	if (hKeyDest != NULL && src)
	{
		LONG lRes = ERROR_SUCCESS;
		if (AppCheck.IsWindows_XP ())
			lRes = CopyKeyRecursive(src, hKeyDest, L"", L"", bNoWow64Redirect);
		else
			lRes = SHCopyKey (src, L"", hKeyDest, 0);
		if (lRes != ERROR_SUCCESS)
		{
			LogW (L"Cannot copy key '%s': %d", pSubKeySrc, lRes);
			src.Close ();
			Tools::SetErrorIfNoError (lRes);
		}
		else
		{
			bDone = true;
		}
	}

	return bDone;
}

bool WriteDword (HKEY hKey, const wchar_t * pName, DWORD dwValue)
{
	bool bDone = false;
	if (hKey != NULL && pName != NULL)
	{
		LONG lRes = RegSetValueEx (hKey, pName, 0, REG_DWORD, (BYTE *)&dwValue, sizeof (DWORD));
		if (lRes != ERROR_SUCCESS)
		{
			LogW (L"Cannot save dword value %u to '%s': %d", dwValue, pName, lRes);
			Tools::SetErrorIfNoError (lRes);
		}
		else
		{
			bDone = true;
		}
	}
	return bDone;
}

bool WriteQword (HKEY hKey, const wchar_t * pName, DWORD64 dw64Value)
{
	bool bDone = false;
	if (hKey != NULL && pName != NULL)
	{
		LONG lRes = RegSetValueEx (hKey, pName, 0, REG_QWORD, (BYTE *)&dw64Value, sizeof (DWORD64));
		if (lRes != ERROR_SUCCESS)
		{
			LogW (L"Cannot save qword value %I64u to '%s': %d", dw64Value, pName, lRes);
			Tools::SetErrorIfNoError (lRes);
		}
		else
		{
			bDone = true;
		}
	}
	return bDone;
}

bool WriteString (HKEY hKey, const wchar_t * pName, const wchar_t * pValue, size_t lCharsLen /*= 0*/, DWORD dwType /*= REG_SZ*/)
{
	bool bDone = false;
	if (hKey != NULL && pName != NULL && pValue != NULL)
	{
		DWORD dwLen = 0;
		if (0 == lCharsLen)
			dwLen = static_cast <DWORD> ((wcslen (pValue) + 1) * sizeof (wchar_t));
		else
			dwLen = static_cast <DWORD> ((lCharsLen + 1) * sizeof (wchar_t));

		LONG lRes = RegSetValueEx (hKey, pName, 0, dwType, (BYTE *)pValue, dwLen);
		if (lRes != ERROR_SUCCESS)
		{
			LogW (L"Cannot save %u string value '%s' to '%s': %d", dwType, pValue, pName, lRes);
			Tools::SetErrorIfNoError (lRes);
		}
		else
		{
			bDone = true;
		}
	}
	return bDone;
}

bool WriteString (HKEY hKey, const wchar_t * pName, const std::wstring & szValue, DWORD dwType /*= REG_SZ*/)
{
	return WriteString (hKey, pName, szValue.c_str (), szValue.size (), dwType);
}

bool WriteMultiString (HKEY hKey, const wchar_t * pName, const std::list <std::wstring> & lValues)
{
	bool bDone = false;
	if (hKey != NULL && pName != NULL)
	{
		std::vector <wchar_t> v;
		for (std::list <std::wstring>::const_iterator _it = lValues.begin (); _it != lValues.end (); ++_it)
		{
			const std::wstring & str = *_it;
			v.insert (v.end (), str.begin (), str.end ());
			v.push_back (0);
		}
		v.push_back (0);

		DWORD dwLen = static_cast <DWORD> (v.size () * sizeof (wchar_t));
		LONG lRes = RegSetValueEx (hKey, pName, 0, REG_MULTI_SZ, (BYTE *)&v [0], dwLen);
		if (lRes != ERROR_SUCCESS)
		{
			LogW (L"Cannot save multistring value to '%s': %d", pName, lRes);
			Tools::SetErrorIfNoError (lRes);
		}
		else
		{
			bDone = true;
		}
	}
	return bDone;
}

bool WriteGuid (HKEY hKey, const wchar_t * pName, const GUID & guid, bool bWriteBraces /*= true*/)
{
	std::vector <wchar_t> pguid (64);
	if (0 == ::StringFromGUID2 (guid, &pguid [0], 64))
	{
		LogW (L"Cannot save guid value to '%s' (invalid guid: %u)", pName, GetLastError ());
		return false;
	}

	wchar_t * ptr = &pguid [0];
	if (!bWriteBraces)
	{
		ptr++;
		ptr [wcslen (ptr) - 1] = 0;
	}

	return WriteString (hKey, pName, ptr);
}

bool WriteData (HKEY hKey, const wchar_t * pName, const BYTE * pData, DWORD dwBytes)
{
	bool bDone = false;
	if (hKey != NULL && pName != NULL && pData != NULL)
	{
		LONG lRes = RegSetValueEx (hKey, pName, 0, REG_BINARY, pData, dwBytes);
		if (lRes != ERROR_SUCCESS)
		{
			LogW (L"Cannot save binary value to '%s': %d", pName, lRes);
			Tools::SetErrorIfNoError (lRes);
		}
		else
		{
			bDone = true;
		}
	}
	return bDone;
}

bool WriteCryptedString (HKEY hKey, const wchar_t * pName, const std::wstring & szValue)
{
	std::vector <BYTE> buff = Tools::CryptStringToBinaryValue (szValue);
	return WriteData (hKey, pName, &buff [0], static_cast <DWORD> (buff.size ()));
}

bool WriteCryptedDword (HKEY hKey, const wchar_t * pName, DWORD dwValue)
{
	std::vector <BYTE> buff = Tools::CryptDwordToBinaryValue (dwValue);
	return WriteData (hKey, pName, &buff [0], static_cast <DWORD> (buff.size ()));
}

bool WriteCryptedQword (HKEY hKey, const wchar_t * pName, DWORD64 dw64Value)
{
	std::vector <BYTE> buff = Tools::CryptQdwordToBinaryValue (dw64Value);
	return WriteData (hKey, pName, &buff [0], static_cast <DWORD> (buff.size ()));
}

DWORD ReadDword (HKEY hKey, const wchar_t * pName, DWORD dwDefValue /*= 0*/)
{
	DWORD dwValue = dwDefValue;
	if (hKey != NULL && pName != NULL)
	{
		DWORD dwType = REG_DWORD, dwSize = sizeof (DWORD);
		LONG lRes = RegQueryValueEx (hKey, pName, NULL, &dwType, (BYTE *)&dwValue, &dwSize);
		if (lRes != ERROR_SUCCESS)
		{
			LogW (L"Cannot read dword value from '%s': %d", pName, lRes);
			dwValue = dwDefValue;
			Tools::SetErrorIfNoError (lRes);
		}
		else if (dwType != REG_QWORD && dwType != REG_DWORD)
		{
			LogW (L"Required value '%s' as dword is real type of %u", pName, dwType);
			dwValue = dwDefValue;
		}
	}
	return dwValue;
}

DWORD64 ReadQword (HKEY hKey, const wchar_t * pName, DWORD64 dw64DefValue /*= 0*/)
{
	DWORD64 dw64Value = dw64DefValue;
	if (hKey != NULL && pName != NULL)
	{
		DWORD dwType = REG_QWORD, dwSize = sizeof (DWORD64);
		LONG lRes = RegQueryValueEx (hKey, pName, NULL, &dwType, (BYTE *)&dw64Value, &dwSize);
		if (lRes != ERROR_SUCCESS)
		{
			LogW (L"Cannot read qword value from '%s': %d", pName, lRes);
			dw64Value = dw64DefValue;
			Tools::SetErrorIfNoError (lRes);
		}
		else if (dwType != REG_QWORD && dwType != REG_DWORD)
		{
			LogW (L"Required value '%s' as qword is real type of %u", pName, dwType);
			dw64Value = dw64DefValue;
		}
	}
	return dw64Value;
}

std::wstring ReadString (HKEY hKey, const wchar_t * pName, std::wstring szDefault /*= L""*/)
{
	std::wstring szValue = szDefault;
	if (hKey != NULL && pName != NULL)
	{
		DWORD dwSize = 0, dwType = REG_SZ;
		LONG lRes = RegQueryValueEx (hKey, pName, NULL, &dwType, NULL, &dwSize);
		if (lRes != ERROR_SUCCESS)
		{
			LogW (L"Cannot read string value from '%s': %d", pName, lRes);
			Tools::SetErrorIfNoError (lRes);
		}
		else if (dwType != REG_SZ && dwType != REG_EXPAND_SZ)
			LogW (L"Required value '%s' as string is real type of %u", pName, dwType);
		else
		{
			std::vector <wchar_t> buff (dwSize + 1);
			lRes = RegQueryValueEx (hKey, pName, NULL, &dwType, (BYTE *)&buff [0], &dwSize);
			if (lRes != ERROR_SUCCESS)
			{
				LogW (L"Cannot read string value from '%s': %d", pName, lRes);
				Tools::SetErrorIfNoError (lRes);
			}
			else
			{
				szValue = &buff [0];
			}
		}
	}
	return szValue;
}

bool ReadMultiString (HKEY hKey, const wchar_t * pName, std::list <std::wstring> & lValues)
{
	bool bDone = false;
	if (hKey != NULL && pName != NULL)
	{
		DWORD dwSize = 0, dwType = REG_MULTI_SZ;
		LONG lRes = RegQueryValueEx (hKey, pName, NULL, &dwType, NULL, &dwSize);
		if (lRes != ERROR_SUCCESS)
		{
			LogW (L"Cannot read multistring value from '%s': %d", pName, lRes);
			Tools::SetErrorIfNoError (lRes);
		}
		else if (dwType != REG_MULTI_SZ)
			LogW (L"Required value '%s' as multistring is real type of %u", pName, dwType);
		else
		{
			std::vector <wchar_t> buff (dwSize + 1);
			lRes = RegQueryValueEx (hKey, pName, NULL, &dwType, (BYTE *)&buff [0], &dwSize);
			if (lRes != ERROR_SUCCESS)
			{
				LogW (L"Cannot read multistring value from '%s': %d", pName, lRes);
				Tools::SetErrorIfNoError (lRes);
			}
			else
			{
				bDone = true;

				const wchar_t * ptr = &buff [0];
				if (*ptr)
				{
					do
					{
						lValues.push_back (ptr);
						while (*ptr++)
							;
					}
					while (*ptr);
				}
			}
		}
	}
	return bDone;
}

GUID ReadGuid (HKEY hKey, const wchar_t * pName, bool bIgnoreBraces /*= true*/)
{
	std::wstring szguid = ReadString (hKey, pName);
	if (szguid.empty ())
		return GUID_NULL;

	if (bIgnoreBraces && szguid [0] != L'{')
		szguid.insert (0, L"{");
	if (bIgnoreBraces && szguid [szguid.size () - 1] != L'}')
		szguid += L'}';

	std::vector <wchar_t> vguid (szguid.size () + 1);
	wcscpy (&vguid [0], szguid.c_str ());

	GUID guid = GUID_NULL;
	HRESULT hr = ::CLSIDFromString (&vguid [0], &guid);
	if (hr != NOERROR)
	{
		LogW (L"Cannot format guid value '%s' from '%s': 0x%x, %u", szguid.c_str (), pName, hr, GetLastError ());
		return GUID_NULL;
	}
	return guid;
}

bool ReadData (HKEY hKey, const wchar_t * pName, std::vector <BYTE> & data)
{
	data.clear ();
	bool bDone = false;
	if (hKey != NULL && pName != NULL)
	{
		DWORD dwSize = 0, dwType = REG_BINARY;
		LONG lRes = RegQueryValueEx (hKey, pName, NULL, &dwType, NULL, &dwSize);
		if (lRes != ERROR_SUCCESS)
		{
			LogW (L"Cannot read binary value from '%s': %d", pName, lRes);
			Tools::SetErrorIfNoError (lRes);
		}
		else if (dwType != REG_BINARY)
		{
			LogW (L"Required value '%s' as binary is real type of %u", pName, dwType);
		}
		else if (0 == dwSize)
		{
			LogW (L"Required value '%s' read, size = 0", pName);
			data.clear ();
			bDone = true;
		}
		else
		{
			data.resize (dwSize);
			lRes = RegQueryValueEx (hKey, pName, NULL, &dwType, (BYTE *)&data [0], &dwSize);
			if (lRes != ERROR_SUCCESS)
			{
				LogW (L"Cannot read binary value from '%s': %d", pName, lRes);
				data.clear ();
				Tools::SetErrorIfNoError (lRes);
			}
			else 
			{
				bDone = true;
			}
		}
	}
	return bDone;
}

std::wstring ReadCryptedString (HKEY hKey, const wchar_t * pName, std::wstring szDefValue /*= L""*/)
{
	std::vector <BYTE> buff;
	if (ReadData (hKey, pName, buff))
		return Tools::DecryptBinaryValueToString (buff);
	return szDefValue;
}

DWORD ReadCryptedDword (HKEY hKey, const wchar_t * pName, DWORD dwDefValue /*= 0*/)
{
	std::vector <BYTE> buff;
	if (ReadData (hKey, pName, buff))
		return Tools::DecryptBinaryValueToDword (buff);
	return dwDefValue;
}

DWORD64 ReadCryptedQword (HKEY hKey, const wchar_t * pName, DWORD64 dw64DefValue /*= 0*/)
{
	std::vector <BYTE> buff;
	if (ReadData (hKey, pName, buff))
		return Tools::DecryptBinaryValueToQdword (buff);
	return dw64DefValue;
}

bool DeleteKey (HKEY hKey, const wchar_t * pSubkey, bool bNoWow64Redirect /*= true*/)
{
	bool bDone = false;
	if (hKey != NULL && pSubkey != NULL)
	{
		HKEY key = NULL;
		LONG lRes = RegOpenKeyEx (hKey, pSubkey, 0, KEY_ALL_ACCESS | (bNoWow64Redirect ? KEY_WOW64_64KEY : 0), &key);
		if (ERROR_SUCCESS == lRes)
		{
			lRes = SHDeleteKey (key, L"");
			RegCloseKey (key);

			if (lRes != ERROR_SUCCESS)
			{
				LogW (L"Cannot delete key '%s': %d", pSubkey, lRes);
				Tools::SetErrorIfNoError (lRes);
			}
			else 
			{
				bDone = true;
			}
		}
		else
			LogW (L"Cannot delete key '%s': %d", pSubkey, lRes);
	}
	return bDone;
}

bool DeleteValue (HKEY hKey, const wchar_t * pValueName)
{
	bool bDone = false;
	if (hKey != NULL && pValueName != NULL)
	{
		LONG lRes = RegDeleteValue (hKey, pValueName);
		if (lRes != ERROR_SUCCESS)
		{
			LogW (L"Cannot delete value '%s': %d", pValueName, lRes);
			Tools::SetErrorIfNoError (lRes);
		}
		else 
		{
			bDone = true;
		}
	}
	return bDone;
}

bool DeleteValue (HKEY hKey, const wchar_t * pSubkey, const wchar_t * pValueName, bool bNoWow64Redirect /*= true*/)
{
	bool bDone = false;
	if (hKey != NULL && pSubkey != NULL && pValueName != NULL)
	{
		HKEY key = NULL;
		LONG lRes = RegOpenKeyEx (hKey, pSubkey, 0, KEY_ALL_ACCESS | (bNoWow64Redirect ? KEY_WOW64_64KEY : 0), &key);
		if (ERROR_SUCCESS == lRes)
		{
			lRes = RegDeleteValue (key, pValueName);
			RegCloseKey (key);

			if (lRes != ERROR_SUCCESS)
			{
				LogW (L"Cannot delete value '%s' - '%s': %d", pSubkey, pValueName, lRes);
				Tools::SetErrorIfNoError (lRes);
			}
			else 
			{
				bDone = true;
			}
		}
		else
			LogW (L"Cannot delete value '%s' - '%s': %d", pSubkey, pValueName, lRes);
	}
	return bDone;
}

bool QueryValue (HKEY hKey, const wchar_t * pValue, DWORD * pdwType /*= NULL*/, DWORD * pdwSize /*= NULL*/, bool bNoWow64Redirect /*= true*/)
{
	if (hKey != NULL && pValue != NULL)
	{
		DWORD dwSize = 0, dwType = 0;
		LONG lRes = RegQueryValueEx (hKey, pValue, NULL, &dwType, NULL, &dwSize);
		if (lRes != ERROR_SUCCESS)
		{
			LogW (L"Value '%s' not found: %d", pValue, lRes);
			Tools::SetErrorIfNoError (lRes);
		}
		else
		{
			if (pdwType != NULL)
				*pdwType = dwType;
			if (pdwSize != NULL)
				*pdwSize = dwSize;
			return true;
		}
	}
	return false;
}

bool QueryValue (HKEY hRootKey, const wchar_t * pSubKey, const wchar_t * pValue, DWORD * pdwType /*= NULL*/, DWORD * pdwSize /*= NULL*/, bool bNoWow64Redirect /*= true*/)
{
	Key key = OpenKey (hRootKey, pSubKey, KEY_READ, bNoWow64Redirect);
	return QueryValue (key, pValue, pdwType, pdwSize, bNoWow64Redirect);
}

bool QueryKeyExists (HKEY hKey, const wchar_t * pSubKey, bool bNoWow64Redirect /*= true*/)
{
	Key key = OpenKey (hKey, pSubKey, KEY_READ, bNoWow64Redirect);
	return !!key;
}

bool EnumKey (HKEY hRootKey, const wchar_t * pSubKey, 
	size_t * subkeys_count /*= nullptr*/, size_t * values_count /*= nullptr*/, 
	std::list <std::wstring> * subkeys /*= nullptr*/, std::list <std::wstring> * values /*= nullptr*/, 
	bool bNoWow64Redirect /*= true*/)
{
	if (nullptr == subkeys && nullptr == values && nullptr == subkeys_count && nullptr == values_count)
	{
		LogA ("Invalid parameters in function call");
		return false;
	}

	RegTools::Key key = RegTools::OpenKey (hRootKey, pSubKey, KEY_READ | KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE, bNoWow64Redirect);
	if (!key)
		return false;

	DWORD keys = 0, key_name_len = 0;
	DWORD vals = 0, val_name_len = 0;
	LRESULT res = ::RegQueryInfoKey (key, NULL, NULL, NULL, &keys, &key_name_len, NULL, &vals, &val_name_len, NULL, NULL, NULL);
	if (res != ERROR_SUCCESS)
	{
		LogW (L"Can't query key '%s' info, error %u", pSubKey, res);
		return false;
	}

	if (subkeys_count != nullptr)
		*subkeys_count = CAST_SIZET (keys);
	if (values_count != nullptr)
		*values_count = CAST_SIZET (vals);

	if (subkeys != nullptr)
	{
		key_name_len += 2;

		for (DWORD index = 0, ki = key_name_len; index < keys; index++, ki = key_name_len)
		{
			std::vector <wchar_t> name (key_name_len);
			res = ::RegEnumKeyEx (key, index, &name [0], &ki, NULL, NULL, NULL, NULL);
			if (res != ERROR_SUCCESS)
			{
				LogW (L"Can't enum key's '%s' subkeys, error %u", pSubKey, res);
				break;
			}

			subkeys->push_back (&name [0]);
		}
	}

	if (values != nullptr)
	{
		val_name_len += 2;

		for (DWORD index = 0, vi = val_name_len; index < vals; index++, vi = val_name_len)
		{
			std::vector <wchar_t> name (val_name_len);
			res = ::RegEnumValue (key, index, &name [0], &vi, NULL, NULL, NULL, NULL);
			if (res != ERROR_SUCCESS)
			{
				LogW (L"Can't enum key's '%s' values, error %u", pSubKey, res);
				break;
			}

			values->push_back (&name [0]);
		}
	}

	return true;
}

DWORD OpenKeyAndReadDword (HKEY hRootKey, const wchar_t * pSubKey, const wchar_t * pName, DWORD dwDefValue /*= 0*/, bool bNoWow64Redirect /*= true*/)
{
	Key key = OpenKey (hRootKey, pSubKey, KEY_READ, bNoWow64Redirect);
	return ReadDword (key, pName, dwDefValue);
}


bool OpenKeyAndReadBool(HKEY hRootKey, const wchar_t * pSubKey, const wchar_t * pName, bool bDefValue /*= false*/, bool bNoWow64Redirect /*= true*/)
{
	Key key = OpenKey(hRootKey, pSubKey, KEY_READ, bNoWow64Redirect);
	return !!ReadDword(key, pName, bDefValue ? 1 : 0);
}

DWORD64 OpenKeyAndReadQword (HKEY hRootKey, const wchar_t * pSubKey, const wchar_t * pName, DWORD64 dw64DefValue /*= 0*/, bool bNoWow64Redirect /*= true*/)
{
	Key key = OpenKey (hRootKey, pSubKey, KEY_READ, bNoWow64Redirect);
	return ReadQword (key, pName, dw64DefValue);
}

std::wstring OpenKeyAndReadString (HKEY hRootKey, const wchar_t * pSubKey, const wchar_t * pName, std::wstring szDefValue /*= L""*/, bool bNoWow64Redirect /*= true*/)
{
	Key key = OpenKey (hRootKey, pSubKey, KEY_READ, bNoWow64Redirect);
	return ReadString (key, pName, szDefValue);
}

bool CreateKeyAndWriteDword (HKEY hRootKey, const wchar_t * pSubKey, const wchar_t * pName, DWORD dwValue, bool bNoWow64Redirect /*= true*/)
{
	Key key = CreateKey (hRootKey, pSubKey, KEY_WRITE, bNoWow64Redirect);
	return WriteDword (key, pName, dwValue);
}

bool CreateKeyAndWriteQword (HKEY hRootKey, const wchar_t * pSubKey, const wchar_t * pName, DWORD64 dw64Value, bool bNoWow64Redirect /*= true*/)
{
	Key key = CreateKey (hRootKey, pSubKey, KEY_WRITE, bNoWow64Redirect);
	return WriteQword (key, pName, dw64Value);
}

bool CreateKeyAndWriteString (HKEY hRootKey, const wchar_t * pSubKey, const wchar_t * pName, const wchar_t * pValue, size_t lCharsLen /*= 0*/, DWORD dwType /*= REG_SZ*/, bool bNoWow64Redirect /*= true*/)
{
	Key key = CreateKey (hRootKey, pSubKey, KEY_WRITE, bNoWow64Redirect);
	return WriteString (key, pName, pValue, lCharsLen, dwType);
}

bool CreateKeyAndWriteString(HKEY hRootKey, const wchar_t * pSubKey, const wchar_t * pName, const std::wstring & szValue, DWORD dwType /*= REG_SZ*/, bool bNoWow64Redirect /*= true*/)
{
	Key key = CreateKey (hRootKey, pSubKey, KEY_WRITE, bNoWow64Redirect);
	return WriteString (key, pName, szValue, dwType);
}

SECURITY_DESCRIPTOR * CreateKeySecurityDescriptorToAll ()
{
	SID sidEveryone = {};
	SID_IDENTIFIER_AUTHORITY sidAuth = SECURITY_WORLD_SID_AUTHORITY; 

	if (!InitializeSid (&sidEveryone, &sidAuth, 1))
	{
		LogA ("Create sec. descr. failed: %u", GetLastError ());
		return NULL; 
	}

	ULONG lACLSize = GetLengthSid (&sidEveryone) + sizeof (ACCESS_ALLOWED_ACE) + sizeof (ACL);

	ULONG lSdSize = sizeof (SECURITY_DESCRIPTOR); 
	SECURITY_DESCRIPTOR * pSD = (SECURITY_DESCRIPTOR *)calloc (1, lSdSize + lACLSize); 
	if (NULL == pSD)
		return NULL;

	if (!InitializeSecurityDescriptor (pSD, SECURITY_DESCRIPTOR_REVISION))
	{ 
		free (pSD);
		LogA ("Create sec. descr. failed: %u", GetLastError ());
		return NULL; 
	} 

	ACL * pAcl = (ACL *)(((PBYTE)pSD) + lSdSize); 
	if (!InitializeAcl (pAcl, lACLSize, ACL_REVISION))
	{ 
		free (pSD);
		LogA ("Create sec. descr. failed: %u", GetLastError ());
		return NULL; 
	}

	if (!SetSecurityDescriptorDacl (pSD, TRUE, pAcl, FALSE))
	{ 
		free (pSD);
		LogA ("Create sec. descr. failed: %u", GetLastError ());
		return NULL; 
	}

	if (!AddAccessAllowedAceEx (pAcl, ACL_REVISION, SUB_CONTAINERS_AND_OBJECTS_INHERIT, KEY_ALL_ACCESS, &sidEveryone))
	{ 
		free (pSD);
		LogA ("Create sec. descr. failed: %u", GetLastError ());
		return NULL; 
	}

	return pSD;
}

bool SetKeyAccessToAll (HKEY hRoot, const wchar_t * pKey)
{
	Key key = CreateKey (hRoot, pKey, WRITE_DAC);
	if (!key)
		return false;

	SECURITY_DESCRIPTOR * pDescriptor = CreateKeySecurityDescriptorToAll ();
	if (NULL == pDescriptor)
		return false;

	LONG lRes = RegSetKeySecurity (key, DACL_SECURITY_INFORMATION, pDescriptor);
	if (lRes != ERROR_SUCCESS)
	{
		LogA ("Create sec. descr. failed: %d", lRes);
		free (pDescriptor);
		Tools::SetErrorIfNoError (lRes);
		return false;
	}
	key.Close ();

	const wchar_t * pRoot = L"";
	if (HKEY_LOCAL_MACHINE == hRoot)
		pRoot = L"MACHINE";
	else if (HKEY_CLASSES_ROOT == hRoot)
		pRoot = L"CLASSES_ROOT";
	else if (HKEY_CURRENT_USER == hRoot)
		pRoot = L"CURRENT_USER";
	else if (HKEY_USERS == hRoot)
		pRoot = L"USERS";
	else
		return false;

	std::vector <wchar_t> pFullPath (wcslen (pRoot) + wcslen (pKey) + 3);
	wsprintf (&pFullPath [0], L"%s\\%s", pRoot, pKey);

	lRes = TreeResetNamedSecurityInfo (&pFullPath [0], SE_REGISTRY_KEY, DACL_SECURITY_INFORMATION, NULL, NULL,
		pDescriptor->Dacl, NULL, FALSE, NULL, ProgressInvokeNever, NULL);
	if (lRes != ERROR_SUCCESS)
	{
		LogA ("Create sec. descr. failed: %d", lRes);
		Tools::SetErrorIfNoError (lRes);
	}

	free (pDescriptor);
	return (ERROR_SUCCESS == lRes);
}

};


