#pragma once

#include <string>

#include "LogData.h"

namespace Log {
/*
	Converts the record into the string according to format
	Formatted item has a form of '${ident}'
	The ident is:
	year - year (4-digit)
	mon  - month (2-digit)
	day  - day (2-digit)
	h    - hours (24-hour format)
	m    - minutes (2-digit)
	s    - seconds (2-digit)
	tid  - thread ID (4-digit)
	file - file name
	line - line number
	sev  - severity level
	msg  - message text

	rec - the logger record
	fmt - format string

	Returns the record formatted as text
*/
std::string FormatRecord(const Data::Record& rec, std::string fmt);

// A location in code that comprises of the file name and the line number
#define LocationInCode Data::Location{ __FILE__, __LINE__ }

// Logging records with different severity.
#define LogError(fmt, ...) Write(Data::Severity::Error, LocationInCode, fmt, __VA_ARGS__)
#define LogWarn(fmt, ...) Write(Data::Severity::Warning, LocationInCode, fmt, __VA_ARGS__)
#define LogInfo(fmt,  ...) Write(Data::Severity::Info, LocationInCode, fmt, __VA_ARGS__)
#define LogDebug(fmt, ...) Write(Data::Severity::Debug, LocationInCode, fmt, __VA_ARGS__)

// Conditional logging records.
#define LogErrorIf(cond, fmt, ...) ( (cond) ? (LogError(fmt, __VA_ARGS__)) : (void()) )
#define LogWarnIf(cond, fmt, ...)  ( (cond) ? (LogWarn(fmt, __VA_ARGS__)) :  (void()) )
#define LogInfoIf(cond, fmt, ...)  ( (cond) ? (LogInfo(fmt, __VA_ARGS__)) :  (void()) )
#define LogDebugIf(cond, fmt, ...) ( (cond) ? (LogDebug(fmt, __VA_ARGS__)) : (void()) )

// Logging the model's data
#define LogModel(fmt, ...) Write(Data::Severity::Model, LocationInCode, fmt, __VA_ARGS__)

// Logging records and models data having a wide char string
#define LogErrorW(fmt, ...) Write(Data::Severity::Error, LocationInCode, ConvertToCharStr(fmt).c_str(), __VA_ARGS__)
#define LogWarnW(fmt, ...) Write(Data::Severity::Warning, LocationInCode, ConvertToCharStr(fmt).c_str(), __VA_ARGS__)
#define LogInfoW(fmt,  ...) Write(Data::Severity::Info, LocationInCode, ConvertToCharStr(fmt).c_str(), __VA_ARGS__)
#define LogDebugW(fmt, ...) Write(Data::Severity::Debug, LocationInCode, ConvertToCharStr(fmt).c_str(), __VA_ARGS__)

#define LogModelW(fmt, ...) Write(Data::Severity::Model, LocationInCode, ConvertToCharStr(fmt).c_str(), __VA_ARGS__)

}// namespace Log