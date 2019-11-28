#pragma once

#include <memory>
#include <filesystem>
#include <mutex>
#include <fstream>

#include "LogData.h"

namespace Log {

namespace fs = std::filesystem;

// Logger interface
class ILogger
{
public:
    virtual bool IsReady() = 0;
	virtual void Write(const std::string& message) = 0;

    virtual ~ILogger() {}
};

// Infers records to console
class ConsoleLogger : public ILogger
{
public:
    ConsoleLogger() {}

    bool IsReady() override { return true; };
	void Write(const std::string& message) override;
};

// Saves records to a file with a unique name
class FileLogger : public ILogger
{
	fs::path mPath;
	std::once_flag mOnceFlag;
	std::ofstream mFile;

public:
    FileLogger();
	~FileLogger() { mFile.close(); }

    FileLogger(FileLogger const&) = delete;
    FileLogger& operator=(FileLogger const&) = delete;

    FileLogger(FileLogger&&) = delete;
    FileLogger& operator=(FileLogger&&) = delete;

    bool IsReady() override;
	void Write(const std::string& message) override;
};

// Defines a logger implementation. _log is a unique pointer to global logger object
bool DefineLogger(std::unique_ptr<ILogger> _log);
#define UseLogger(Logger) DefineLogger(std::make_unique<Logger>())
// Checks if a logger is ready
bool IsReady();
// Saves a record with some severity level
void Write(Data::Severity sev, Data::Location loc, const char* fmt, ...);
// Convert from wstring to string
const std::string ConvertToCharStr(const std::wstring wstr);

}// namespace Log
