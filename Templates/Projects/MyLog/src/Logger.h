#pragma once

#include <memory>
#include <Windows.h>

#include "LogData.h"

/*
Description
    Common interface for all logger implementations.

History
    Konstantin Zhelieznov              11/21/2018    Add comments to Logger.h
*/
class ILogger
{
public:
    virtual bool IsEnabled() = 0;
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

    bool IsEnabled() override { return true; }
    void Write(const Record& record) override;
};

/*
Description
    Sets logger implementation
    Logger impl can be set only once - all other attemps will cause std::logic_error

Arguments
    logger - unique pointer to logger object
*/
void SetLogger(std::unique_ptr<ILogger> logger);
#define UseLogger(Logger) SetLogger(std::make_unique<Logger>())

bool IsEnabled();

void Write(Severity severity, Location loc, const char* format, ...);
