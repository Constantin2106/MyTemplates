#pragma once

#include <string>

#include "LogData.h"

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
*/
std::string FormatRecord(const Record& record, std::string format);


/*
    Substitutes with current loation object, which contains current file and line
*/
#define LogCurrentLocation ( Location{ __FILE__, __LINE__ } )

/*
    The most explicit log writing macro. Does not infer any info from its current context

    @param[in] $severity    log severity level
    @param[in] $target      log target location, like the name of current module
    @param[in] $location    file and line which should be used in log message as location
    @param[in] $format      formatting string for message
*/
#define LogWriteRaw($severity, $location, $format, ...)        \
    (                                                          \
        IsEnabled()                                   \
        ? Write($severity, $location, $format, __VA_ARGS__)    \
        : (void())                                             \
    )                                                          \

/*
    Logging macros which are specialized by severity but allow to specify custom target and location
    May be used in cases where logging macro is invoked through some intermediate code,
    and what matters is the invocation site of that code

    @param[in] $target      Target name
    @param[in] $location    File and line where logging happens
    @param[in] $format      Format string
*/
#define LogErrorAt($location, $format, ...) LogWriteRaw(Severity::Error,   $location, $format, __VA_ARGS__)
#define LogWarnAt( $location, $format, ...) LogWriteRaw(Severity::Warning, $location, $format, __VA_ARGS__)
#define LogInfoAt( $location, $format, ...) LogWriteRaw(Severity::Info,    $location, $format, __VA_ARGS__)
#define LogDebugAt($location, $format, ...) LogWriteRaw(Severity::Debug,   $location, $format, __VA_ARGS__)

// Hard, unrecoverable error
#define LogError($format, ...) LogErrorAt(LogCurrentLocation, $format, __VA_ARGS__)
// An error which can be possibly handled somewhere up the code hierarchy
#define LogWarn($format,  ...) LogWarnAt( LogCurrentLocation, $format, __VA_ARGS__)
// Informational message
#define LogInfo($format,  ...) LogInfoAt( LogCurrentLocation, $format, __VA_ARGS__)
// Debug data, like state of some structure after operation
#define LogDebug($format, ...) LogDebugAt(LogCurrentLocation, $format, __VA_ARGS__)

/*
    Conditional logging macros - use these to write messages to log under certain condition
*/
#define LogErrorIf($cond, $format, ...) (($cond) ? LogError($format, __VA_ARGS__) : (void()) )
// An error which can be possibly handled somewhere up the code hierarchy
#define LogWarnIf($cond, $format,  ...) (($cond) ? LogWarn( $format, __VA_ARGS__) : (void()) )
// Informational message
#define LogInfoIf($cond, $format,  ...) (($cond) ? LogInfo( $format, __VA_ARGS__) : (void()) )
// Debug data, like state of some structure after operation
#define LogDebugIf($cond, $format, ...) (($cond) ? LogDebug($format, __VA_ARGS__) : (void()) )

