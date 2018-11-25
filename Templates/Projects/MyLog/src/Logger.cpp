#include <atomic>
#include <stdexcept>

#include "Logger.h"
#include "LogUtils.h"

/*
    Globally shared logger instance, with side-attached destructor
*/
namespace 
{
    std::atomic<ILogger*> g_logger{nullptr};

    class LoggerDestructor
    {
    public:
        LoggerDestructor() = default;
        ~LoggerDestructor()
        {
            // extract pointer from g_logger 
            // and drop it, replacing with nullptr
            // TODO: select proper ordering
            delete g_logger.exchange(nullptr);
        }
    };
   
    LoggerDestructor g_loggerDestructor;
}

/*
    Sets logger implementation
*/
void SetLogger(std::unique_ptr<ILogger> logger)
{
    if(!logger)
        throw std::invalid_argument("logger");

    ILogger* expected = nullptr;
    if(g_logger.compare_exchange_weak(expected, logger.get()))
    {
        logger.release();
    }
    else
    {
        throw std::logic_error("Logger already initialized");
    }
}

/*
    Checks the logger enabled
*/
bool IsEnabled(Severity severity, const char* target)
{
    // Obtain local pointer
    ILogger* logger = g_logger;
    return logger != nullptr && logger->IsEnabled(Metadata{ severity, target });
}

std::string FormatStr(const char* format, va_list args)
{
    auto buff_size = vsnprintf(nullptr, 0, format, args);

    if (buff_size == -1)
        return std::string{};

    buff_size++;
    auto buff = std::make_unique<char[]>(buff_size);
    vsnprintf(buff.get(), buff_size, format, args);

    return std::string(buff.get());
}

void Write(Severity severity, const char* target, Location loc, const char* format, ...)
{
    // Obtain local pointer
    ILogger* logger = g_logger; 
    if (logger == nullptr)
    {
        return;
    }

    va_list args;
    va_start(args, format);
    auto message =  FormatStr(format, args);
    va_end(args);

    Record rec{
        Metadata { severity, target },
        loc,
        std::chrono::system_clock::now(),
        ::GetCurrentThreadId(),
        message.c_str()
    };

    logger->Write(rec);
}

//-----------------------------------------------------------------------//
//                          Console Logger                               //
//-----------------------------------------------------------------------//
void ConsoleLogger::Write(const Record& record)
{
    auto str = FormatRecord(record, "ThreadId-${tid} -- ${file} -- line-${line} -- ${sev} -- ${msg}");

    ::OutputDebugStringA("/****************************************************************/\r\n");
    ::OutputDebugStringA(str.c_str());
    ::OutputDebugStringA("\r\n");
}

//-----------------------------------------------------------------------//