#include <atomic>
#include <stdexcept>
#include <assert.h>
#include <codecvt>

#include "Logger.h"
#include "LogUtils.h"

namespace Log {

namespace 
{
	// Global logger
    std::atomic<ILogger*> g_logger{nullptr};
	
	// Logger deleter
    class LoggerDestructor
    {
    public:
        LoggerDestructor() = default;
        ~LoggerDestructor() { delete g_logger.exchange(nullptr); }
    };
   
    LoggerDestructor g_loggerDestructor;
}

// Defines a logger implementation
bool DefineLogger(std::unique_ptr<ILogger> _log)
{
    if (nullptr == g_logger.load())
    {
        // Move _log to g_logger
        g_logger.exchange(_log.release());
        return true;
    }
    
    delete _log.release();
    return false;
}

// Checks if a logger is open
bool IsReady()
{
	ILogger* log(g_logger.load());
    return nullptr != log && log->IsReady();
}

// Format string
std::string FormatStr(const char* fmt, va_list args)
{
    auto buffSize = vsnprintf(nullptr, 0, fmt, args);

    if (buffSize == -1)
        return std::string{};

    buffSize++;
    auto buff = std::make_unique<char[]>(buffSize);
    vsnprintf(buff.get(), buffSize, fmt, args);

    return std::string(buff.get());
}

// Saves a record with some severity level
void Write(Data::Severity sev, Data::Location loc, const char* fmt, ...)
{
	ILogger* log(g_logger.load());
    if (nullptr == log)
        return;

	if (!log->IsReady())
		return;

	va_list args;
    va_start(args, fmt);
    auto msg = FormatStr(fmt, args);
    va_end(args);

	if (Data::Severity::Model == sev)
	{
		log->Write(msg);
		return;
	}

	Data::Record rec(sev, loc, msg.c_str());
	log->Write(FormatRecord(rec, "${sev} TId-${tid} - ${file} - line-${line} - ${year}-${mon}-${day}_${h}:${m}:${s}\n\t${msg}"));
}

// Convert from wstring to string
const std::string ConvertToCharStr(const std::wstring wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
	return cvt.to_bytes(wstr);
}

#pragma region Console Logger
//-----------------------------------------------------------------------//
//                          Console Logger                               //
//-----------------------------------------------------------------------//
void ConsoleLogger::Write(const std::string& msg)
{
	::OutputDebugStringA("/----------------------------------------------------------------/\n");
	::OutputDebugStringA(msg.c_str());
	::OutputDebugStringA("\n");
}
#pragma endregion

#pragma region File Logger
//-----------------------------------------------------------------------//
//                            File Logger                                //
//-----------------------------------------------------------------------//
FileLogger::FileLogger()
{
	Data::Record rec;
    auto name = FormatRecord(rec, "${year}-${mon}-${day}_${h}-${m}-${s}.log");

    mPath = fs::current_path() / "Log" / name;
}

bool FileLogger::IsReady()
{
    return !mPath.empty();
}

void FileLogger::Write(const std::string& msg)
{
	std::call_once(mOnceFlag, [&]() 
	{
		// Create log folder
		std::error_code errCode;
		fs::create_directories(mPath.parent_path(), errCode);
		if (errCode)
		{
			// The folder wasn't created
			// TODO: Must be handled
			return;
		}

		// Open log file
		mFile.open(mPath.c_str(), std::ios_base::out | std::ios_base::ate);
	});

	if (mFile.is_open())
	{
		mFile << msg << std::endl;
	}
}
#pragma endregion

}// namespace Log
