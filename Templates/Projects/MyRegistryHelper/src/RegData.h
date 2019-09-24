#pragma once
#include <winreg.h>
#include <unordered_map>

namespace Registry
{
	// Key Value Types
	enum eDataType
	{
		Binary,
		DWord,
		DWordLEnd,
		DWordBEnd,
		QWord,
		QWordLEnd,
		None,
		String,
		EnvironStr,
		Link,
		MultiStr,

		VTYPE_SIZE
	};
	
	// Key Options
	enum eOptions
	{
		Store,
		NotStore,
		BackupRestore,

		OPT_SIZE
	};
	
	// Key Access and Mask
	enum eAccessAndMask
	{
		Full,
		CreateLink,
		CreateSubKey,
		EnumSubKeys,
		ReadKey,
		Notify,
		ReadSubKey,
		Read,
		WriteSubKey,
		Write,
		Wow64_32,
		Wow64_64,

		ACC_SIZE
	};
	namespace data
	{
		static std::unordered_map<eDataType, DWORD> EnumToDataType(
			{
					{ Binary,		REG_BINARY },
					{ DWord,		REG_DWORD },
					{ DWordLEnd,	REG_DWORD_LITTLE_ENDIAN },
					{ DWordBEnd,	REG_DWORD_BIG_ENDIAN },
					{ EnvironStr,	REG_EXPAND_SZ },
					{ Link,			REG_LINK },
					{ MultiStr,		REG_MULTI_SZ },
					{ None,			REG_NONE },
					{ QWord,		REG_QWORD },
					{ QWordLEnd,	REG_QWORD_LITTLE_ENDIAN },
					{ String,		REG_SZ }
			});
		
		static std::unordered_map<DWORD, eDataType> DataTypeToEnum(
			{
					{ REG_BINARY,				Binary	 },
					{ REG_DWORD,				DWord	 },
					{ REG_DWORD_LITTLE_ENDIAN,	DWordLEnd },
					{ REG_DWORD_BIG_ENDIAN,		DWordBEnd },
					{ REG_EXPAND_SZ,			EnvironStr },
					{ REG_LINK,					Link },
					{ REG_MULTI_SZ,				MultiStr },
					{ REG_NONE,					None },
					{ REG_QWORD,				QWord },
					{ REG_QWORD_LITTLE_ENDIAN,	QWordLEnd },
					{ REG_SZ,					String }
			});
		
		static std::unordered_map<eOptions, DWORD> EnumToKeyOpt(
			{
					{ Store,			REG_OPTION_NON_VOLATILE },
					{ NotStore,			REG_OPTION_VOLATILE },
					{ BackupRestore,	REG_OPTION_BACKUP_RESTORE }
			});
		
		static std::unordered_map<eAccessAndMask, REGSAM> EnumToAccess(
			{
					{ Full,				KEY_ALL_ACCESS },
					{ CreateLink,		KEY_CREATE_LINK },
					{ CreateSubKey,		KEY_CREATE_SUB_KEY },
					{ EnumSubKeys,		KEY_ENUMERATE_SUB_KEYS },
					{ ReadKey,			KEY_EXECUTE },
					{ Notify,			KEY_NOTIFY },
					{ ReadSubKey,		KEY_QUERY_VALUE },
					{ Read,				KEY_READ },
					{ WriteSubKey,		KEY_SET_VALUE },
					{ Write,			KEY_WRITE },
					{ Wow64_32,			KEY_WOW64_32KEY},
					{ Wow64_64,			KEY_WOW64_64KEY}
			});
	}

	// Parameters of data to write.
	struct dataParam
	{
		DWORD type;		// data type (Registry Value Type)
		LPBYTE data;	// pointer to data
		DWORD size;		// data size in bytes
	};
}



