#pragma once

#include <chrono>
#include <memory>
#include <Windows.h>

#include "Log.h"
//#include "LogUtils.h"

using Timestamp = std::chrono::system_clock::time_point;

/*
Description
    Information which participates in deciding whether to write message.

History
    Konstantin Zhelieznov              11/21/2018    Add comments to Logger.h
*/
struct Metadata
{
    Severity severity;
    const char* target;
};

/*
Description
    Full logging message payload.

History
    Konstantin Zhelieznov              11/21/2018    Add comments to Logger.h
*/
struct Record
{
    Metadata    metadata;
    Location    location;
    Timestamp   timestamp;
    uintptr_t   threadId;
    const char* message;
};

/*
Description
    Common interface for all logger implementations.

History
    Konstantin Zhelieznov              11/21/2018    Add comments to Logger.h
*/
class ILogger
{
public:
    virtual bool IsEnabled(const Metadata&) = 0;
    virtual void Write(const Record&) = 0;

    virtual ~ILogger() {}
};

/*
Description
    Dumps message from record to debug console
    Does not perform any checks
*/
class ConsoleLogger : public ILogger
{
public:
    ConsoleLogger() { }

    bool IsEnabled(const Metadata&) override { return true; }
    void Write(const Record& record) override 
    {
        // FIXME: desyncs during output?
        ::OutputDebugStringA("\r\n");
        ::OutputDebugStringA("\t\t");
        ::OutputDebugStringA(record.message);
        ::OutputDebugStringA("\r\n");
    }
};

/*
Description
    Sets logger implementation
    Logger impl can be set only once - all other attemps will cause std::logic_error

Arguments
    logger - unique pointer to logger object

History
    Konstantin Zhelieznov              11/21/2018    Add comments to Logger.h
*/
void SetLogger(std::unique_ptr<ILogger> logger);
#define UseLogger(Logger) SetLogger(std::make_unique<Logger>())

bool IsEnabled(Severity severity, const char* target);

void Write(Severity severity, const char* target, Location loc, const char* format, ...);
