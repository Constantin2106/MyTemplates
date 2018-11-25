#pragma once

#include <chrono>

/*
Importance level of log message
*/
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

/*
    Defines log location
*/
struct Location
{
    const char* file;
    int         line;
};

/*
    Information which participates in deciding whether to write message.
*/
struct Metadata
{
    Severity severity;
    const char* target;
};

/*
    Full logging message payload.
*/
using Timestamp = std::chrono::system_clock::time_point;
struct Record
{
    Metadata    metadata;
    Location    location;
    Timestamp   timestamp;
    uintptr_t   threadId;
    const char* message;
};
/* 
    Enables ADL-based deduction on which "log target" function to use
*/
struct AdlTag
{};
