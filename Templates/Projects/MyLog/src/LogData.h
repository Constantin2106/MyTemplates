#pragma once

#include <chrono>
#include <Windows.h>

// Importance level of log message
enum class Severity
{
    // Used to set a first level with 1
    Empty,
    // Logging level
    Error,
    Warning,
    Info,
    Debug,

    _Count
};

// Defines log location
struct Location
{
    const char* file;
    int         line;
};

// Full logging message payload.
using Timestamp = std::chrono::system_clock::time_point;
struct Record
{
    Severity    severity;
    Location    location;
    Timestamp   timestamp;
    uintptr_t   threadId;
    const char* message;

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
