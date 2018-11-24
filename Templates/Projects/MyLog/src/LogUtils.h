#pragma once

#include <string>
#include <tuple>
#include <utility>

#include "Logger.h"

/*
Description
    Transforms provided record into string using format specifier
    Each format item is of form '${ident}', where ident means:

    year - 4-digit year
    mon  - 2-digit month
    day  - 2-digit day of the month
    h    - 24-hour format hours
    m    - 2-digit minutes
    s    - 2-digit seconds
    ms   - 3-digit milliseconds
    tid  - thread ID, 4-digit, zero-padded
    file - location.file from record
    line - location.line from record
    sev  - severity level name
    msg  - actual message text

Arguments
    buffer - the destination string
    record - the logger record that must be formatted
    format - format string

History
    Konstantin Zhelieznov              11/21/2018    Add comments to LogUtils.h
*/
void FormatRecord(std::string& buffer, const Record& record, std::string format);

/*
Description
    Transforms provided record into string using format specifier
    Each format item is of form '${ident}', where ident means:
    
    year - 4-digit year
    mon  - 2-digit month
    day  - 2-digit day of the month
    h    - 24-hour format hours
    m    - 2-digit minutes
    s    - 2-digit seconds
    ms   - 3-digit milliseconds
    tid  - thread ID, 4-digit, zero-padded
    file - location.file from record
    line - location.line from record
    sev  - severity level name
    msg  - actual message text

Arguments
    record - the logger record that must be formatted
    format - format string

Return
    Returns the record, formatted as text

History
    Konstantin Zhelieznov              11/21/2018    Add comments to LogUtils.h
*/
//#define FmtRecord(record, format)                   \
//            std::string buffer;                     \
//            FormatRecord(buffer, record, format);   \
//            return buffer;                          \

std::string FormatRecord(const Record& record, std::string format)
{
    std::string buffer;
    FormatRecord(buffer, record, format);
    return buffer;
}

/*
Description
    Composes multiple provided loggers into one multi-logger

Template
    Logs - the set of logger classes to be composed

History
    Konstantin Zhelieznov              11/21/2018    Add comments to LogUtils.h
*/
template<class... Logs>
class MultiLogger : public Logger
{
public:
    /*
    Description
        Construct by loggers
    
    Arguments
        loggers - set of loggers to be composed
    
    History
        Konstantin Zhelieznov              11/21/2018    Add comments to LogUtils.h
    */
    MultiLogger(Logs... loggers)
        : m_loggers(std::move(loggers)...)
    { }

    MultiLogger(MultiLogger const&) = delete;
    MultiLogger& operator=(MultiLogger const&) = delete;

    MultiLogger(MultiLogger &&) = default;
    MultiLogger& operator=(MultiLogger &&) = default;

    /*
    Description
        Checks if any of embedded loggers is enabled for specified metadata
    
    Arguments
        meta - the metadata to work with

    Return
        Returns true when at least one logger works with given metadata
    
    History
        Konstantin Zhelieznov              11/21/2018    Add comments to LogUtils.h
    */
    bool IsEnabled(const Metadata& meta) override
    {
        return Fold(
            false,
            [&meta](auto acc, auto& logger) { return acc || logger.IsEnabled(meta); }
        );
    }
    
    /*
    Description
        Writes the given record to every suitable logger
    
    Arguments
        record - the record to write

    Return
        Returns true when at least one logger works with given metadata
    
    History
        Konstantin Zhelieznov              11/21/2018    Add comments to LogUtils.h
    */
    void Write(const Record& record) override
    {
        Fold(
            0,
            [&record](auto acc, auto& logger) {
            return (logger.IsEnabled(record.metadata) ? logger.Write(record) : (void()) ), acc;
        }
        );
    }

private:
    std::tuple<Logs...> m_loggers;
    
    /*
    Description
        Fold function  recursive implementation
        Calls the given lambda for each logger in tuple
    
    Template
        T - the type of context object
        Fn - the type of function object
        I - the first index in private tuple
        Is - the least part of tuple indices
    
    Arguments
        acc - accumulator
        func - the function to call for each Logger object;
               It receives acc object as first parameter and Logger object as the second one.

    Return
        Returns the accumulator object
    
    History
        Konstantin Zhelieznov              11/21/2018    Add comments to LogUtils.h
    */
    template<class T, class Fn, size_t I, size_t... Is>
    T FoldImpl(T acc, Fn&& func, std::index_sequence<I, Is...>)
    {
        auto tmp = func(std::move(acc), std::get<I>(m_loggers));
        return FoldImpl(std::move(tmp), std::forward<Fn>(func), std::index_sequence<Is...>());
    }

    /*
    Description
        The specialization for an empty variadic list
    */
    template<class T, class Fn>
    T FoldImpl(T acc, Fn&&, std::index_sequence<>)
    {
        return acc;
    }
    
    /*
    Description
        Folds tuple of loggers by applying provided lambda to each logger in stored tuple.
        Lambda accepts accumulator and logger and produces new accumulator, which is then provided to next call.
    
    Template
        T - the type of accumulator
        Fn - the type of function object
    
    Arguments
        acc - an initial accumulator value
        func - the function to call for each Logger object;
               It receives acc object as first parameter and Logger object as the second one.

    Return
        Returns final value of accumulator
    
    History
        Konstantin Zhelieznov              11/21/2018    Add comments to LogUtils.h
    */
    template<class T, class Fn>
    T Fold(T acc, Fn&& func)
    {
        return FoldImpl(acc, std::forward<Fn>(func), std::make_index_sequence<sizeof...(Logs)>());
    }
};

/*
Description
    This wrapper uses automatic type deduction to make MultiLogger construction less verbose

Template
    Args - loggers types

Arguments
    args - loggers objects

Return
    Returns MultiLogger instance

History
    Konstantin Zhelieznov              11/21/2018    Add comments to LogUtils.h
*/
template<class... Args>
MultiLogger<std::decay_t<Args>...> MakeMultiLogger(Args&&... args)
{
    return MultiLogger<std::decay_t<Args>...>(std::forward<Args>(args)...);
}

/*
Description
    Initializes current logger with provided logger instance.
    One-argument overload of multi-argument SetLoggers which doesn't use
    MultiLogger inside

Template
    Logger - type of logger being packed

Arguments
    logger - instance of logger
*/
template<class Logger>
void SetLoggers(Logger&& logger)
{
    sere::diag::SetLogger(std::make_unique<std::decay_t<Logger>>(std::forward<Logger>(logger)));
}
/*
Description
    Packs all provided loggers into single multi-logger,
    then sets it as current logger via SetLogger
    Requires at least two loggers

Template
    Logs    - types of loggers being packed

Arguments
    logs    - instances of loggers which are then packed
*/
template<class Log0, class Log1, class... Logs>
void SetLoggers(Log0&& log0, Log1&& log1, Logs&&... logs)
{
    sere::diag::SetLoggers(
        sere::diag::MakeMultiLogger(std::forward<Log0>(log0), std::forward<Log1>(log1), std::forward<Logs>(logs)...)
    );
};
/*
Description
    Wrapper type which adds severity filtering to another logger

Template
    L - an actual logger type

History
    Konstantin Zhelieznov              11/21/2018    Add comments to LogUtils.h
*/
template<class L>
class LoggerSeverityFilter: public Logger
{
public:
    /*
    Description
        Constructs the filter by severity and logger object.
    
    Arguments
        severity - the lowest severity to log
        logger - the logger object
    
    History
        Konstantin Zhelieznov              11/21/2018    Add comments to LogUtils.h
    */
    LoggerSeverityFilter(Severity severity, L logger)
        : m_logger(std::move(logger))
        , m_severity(severity)
    { }

    
    /*
    Description
        Checks that severity condition is satisfied, and that wrapped logger is also enabled
    
    Arguments
        meta - metadata object

    Return
        Ruturns false if meta doesn't pass the filtering
    
    History
        Konstantin Zhelieznov              11/21/2018    Add comments to LogUtils.h
    */
    bool IsEnabled(const Metadata& meta) override { return meta.severity <= m_severity && m_logger.IsEnabled(meta); }
    
    /*
    Description
        Writes the record to the internal logger
    
    Arguments
        record - the record to be written
    
    History
        Konstantin Zhelieznov              11/21/2018    Add comments to LogUtils.h
    */
    void Write(const Record& record) override { m_logger.Write(record); }

private:
    L m_logger;
    Severity m_severity;
};

/*
Description
    Automatically constructs the severity filter for logger.
    This wrapper uses automatic type deduction to make LoggerSeverityFilter construction less verbose.

Template
    T - logger type

Arguments
    sev - the lowest severity to log. 
    logger - the logger object

Return
    Returns LoggerSeverityFilter instance

History
    Konstantin Zhelieznov              11/21/2018    Add comments to LogUtils.h
*/
template<class T>
LoggerSeverityFilter<std::decay_t<T>> MakeSeverityFilter(Severity sev, T&& logger)
{
    return LoggerSeverityFilter<std::decay_t<T>>(sev, std::forward<T>(logger));
}

/*
Description
    The combination for logger object and it's format
    Formats full message from record and passes it to nested logger as Record::message

Template
    L - logger type

History
    Konstantin Zhelieznov              11/21/2018    Add comments to LogUtils.h
*/
template<class L>
class LoggerFormatter : public Logger
{
public:
    /*
    Description
        Constructs the logger formatter by string format and logger object
    
    Arguments
        str - the format for logger
        logger - the logger object
    
    History
        Konstantin Zhelieznov              11/21/2018    Add comments to LogUtils.h
    */
    LoggerFormatter(std::string format, L logger)
        : m_logger(std::move(logger))
        , m_format(format.data(), format.size())
    { }
    
    /*
    Description
        Calls IsEnabled for an internal logger and just returns the result
    
    Arguments
        meta - metadata object
    
    Return
        Returns true if this logger is enabled for this metadata
    
    History
        Konstantin Zhelieznov              11/21/2018    Add comments to LogUtils.h
    */
    bool IsEnabled(const Metadata& meta) override { return m_logger.IsEnabled(meta); }

    /*
    Description
        Formats full message from record and passes it to nested logger as Record::message
    
    Arguments
        record - the record to be written
    
    History
        Konstantin Zhelieznov              11/21/2018    Add comments to LogUtils.h
    */
    void Write(const Record& record) override
    {
        std::string newMessage = FormatRecord(record, m_format);
        Record proxy = record;
        proxy.message = newMessage.c_str();

        m_logger.Write(proxy);
    }
private:
    L m_logger;
    std::string m_format;
};


/*
Description
    Automatically constructs the logger formatter.
    This wrapper just uses automatic type deduction to make LoggerFormatter construction less verbose.

Template
    T - logger type

Arguments
    format - the format for one record. 
    logger - the logger object

Return
    Returns LoggerFormatter instance

History
    Konstantin Zhelieznov              11/21/2018    Add comments to LogUtils.h
*/
template<class T>
LoggerFormatter<std::decay_t<T>> MakeFormatter(std::string format, T&& logger)
{
    return LoggerFormatter<std::decay_t<T>>(format, std::forward<T>(logger));
}

