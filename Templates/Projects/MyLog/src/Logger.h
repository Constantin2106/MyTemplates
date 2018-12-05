#pragma once

#include <memory>
#include <experimental/filesystem>
#include <mutex>
#include <fstream>

#include "LogData.h"

namespace fs = std::experimental::filesystem;

/*
Description
    Common interface for all logger implementations.

History
    Konstantin Zhelieznov              11/21/2018    Created.
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
    ConsoleLogger() {}

    bool IsEnabled() override;
    void Write(const Record& record) override;
};

/*
Description
    Dumps message from record to specified file
    Does not perform any checks

    NB: it's movable, though std::once_flag is not - must settle in place before first write
*/
class FileLogger : public ILogger
{
public:
    FileLogger();

    FileLogger(FileLogger const&) = delete;
    FileLogger& operator=(FileLogger const&) = delete;

    FileLogger(FileLogger&&);
    FileLogger& operator=(FileLogger&&);

    bool IsEnabled() override;
    void Write(const Record& record) override;

private:
    fs::path m_path;
    std::once_flag m_onceFlag;
    std::ofstream m_file;
};

/*
Description
    Sets logger implementation
    Logger impl can be set only once - all other attemps will cause std::logic_error

Arguments
    logger - unique pointer to logger object
*/
bool SetLogger(std::unique_ptr<ILogger> logger);
#define UseLogger(Logger) SetLogger(std::make_unique<Logger>())

bool IsEnabled();

void Write(Severity severity, Location loc, const char* format, ...);
