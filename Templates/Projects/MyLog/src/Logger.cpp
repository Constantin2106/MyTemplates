#include <atomic>
#include <stdexcept>
#include <assert.h>

#include "Logger.h"
#include "LogUtils.h"

// Globally shared logger instance, with side-attached destructor
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

// Sets logger implementation
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

// Checks the logger enabled
bool IsEnabled()
{
    // Obtain local pointer
    ILogger* logger = g_logger;
    return logger != nullptr && logger->IsEnabled();
}

// Format string
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

// Write message into log
void Write(Severity severity, Location loc, const char* format, ...)
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
        severity,
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
bool ConsoleLogger::IsEnabled()
{
    return true;
}

void ConsoleLogger::Write(const Record& record)
{
    auto message = FormatRecord(record, "${sev} ThreadId-${tid} -- ${file} -- line-${line}\n\t\t${msg}");

    ::OutputDebugStringA("/****************************************************************/\n");
    ::OutputDebugStringA(message.c_str());
    ::OutputDebugStringA("\n");
}


//-----------------------------------------------------------------------//
//                            File Logger                                //
//-----------------------------------------------------------------------//
FileLogger::FileLogger()
{
    Record rec;
    rec.timestamp = std::chrono::system_clock::now();
    auto name = FormatRecord(rec, "${year}-${mon}-${day}_${h}-${m}-${s}.log");

    m_path = fs::current_path() / "Log" / name;
}

FileLogger::FileLogger(const std::string& path) : m_path(std::move(path))
{}

FileLogger::FileLogger(FileLogger&& other) : m_path(std::move(other.m_path)),
                                             m_file(std::move(other.m_file))
{
    assert(!m_file.is_open());
}

FileLogger& FileLogger::operator=(FileLogger&& other)
{
    assert(!m_file.is_open() && !other.m_file.is_open());
    if (this != &other)
    {
        m_path = std::move(other.m_path);
        m_file = std::move(other.m_file);
    }
    return *this;
}

bool FileLogger::IsEnabled()
{
    return !m_path.empty();
}

void FileLogger::Write(const Record& record)
{
    std::call_once(m_onceFlag, 
                   [&]() {
                       // Create folder for log file
                       std::error_code errCode;
                       fs::create_directories(m_path.parent_path(), errCode);
                       if (errCode)
                       {
                           // TODO: errCode should be handled
                           // Return without opening file, means we can't create target dir properly
                           return;
                       }

                       // Open log file; if it fails, file will not be open - and so subsequent reads will not happen
                       m_file.open(m_path.c_str(), std::ios_base::ate | std::ios_base::out);
                   });

    if (m_file.is_open())
    {
        auto message = FormatRecord(record, "${sev} ThreadId-${tid} -- ${file} -- line-${line}\n\t\t${msg}");
        m_file << message << std::endl;
    }
}

//-----------------------------------------------------------------------//