#pragma once

#include <chrono>
#include <Windows.h>

namespace Log {

namespace Data {

// Severity levels
enum class Severity
{
    Empty,		// Record without any level of severity
    Error,		// Unrecoverable error
    Warning,	// Warning of invalid data state, etc.
    Info,		// Informational message
    Debug,		// State of some structure, variable, etc.
	Model,		// Logging of model's data

    _Count
};

// Location in code
struct Location
{
    const char* file;
    int         line;
};

// Time point
using Timestamp = std::chrono::system_clock::time_point;

// Record of a log
struct Record
{
	Severity severity{};
	Location location{};
	Timestamp timestamp{};
	DWORD threadId{};
	const char* message{};

    Record()
    {
        timestamp = std::chrono::system_clock::now();
        threadId = ::GetCurrentThreadId();
    }

    Record(Severity _severity, Location _location, const char* _message) : Record()
    {
        severity = _severity;
        location = _location;
        message = _message;
    }
};

}// namespace Data
}// namespace Log
