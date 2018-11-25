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

using Target = const char*;

// Enables ADL-based deduction on which "log target" function to use
struct AdlTag {};
// Returns default log target, nullptr in our case
static constexpr Target GetLogTarget(...)
{
    return nullptr;
}
/*
    Substitutes with current 'target' defined in current translation unit
*/
#define LogCurrentTarget (GetLogTarget(AdlTag {}))
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
#define LogWriteRaw($severity, $target, $location, $format, ...)        \
    (                                                                   \
        IsEnabled($severity, $target)                                   \
        ? Write($severity, $target, $location, $format, __VA_ARGS__)    \
        : (void())                                                      \
    )                                                                   \

/*
    Logging macros which are specialized by severity but allow to specify custom target and location
    May be used in cases where logging macro is invoked through some intermediate code,
    and what matters is the invocation site of that code

    @param[in] $target      Target name
    @param[in] $location    File and line where logging happens
    @param[in] $format      Format string
*/
#define LogErrorAt($target, $location, $format, ...) LogWriteRaw(Severity::Error,   $target, $location, $format, __VA_ARGS__)
#define LogWarnAt( $target, $location, $format, ...) LogWriteRaw(Severity::Warning, $target, $location, $format, __VA_ARGS__)
#define LogInfoAt( $target, $location, $format, ...) LogWriteRaw(Severity::Info,    $target, $location, $format, __VA_ARGS__)
#define LogDebugAt($target, $location, $format, ...) LogWriteRaw(Severity::Debug,   $target, $location, $format, __VA_ARGS__)

// Hard, unrecoverable error
#define LogError($format, ...) LogErrorAt(LogCurrentTarget, LogCurrentLocation, $format, __VA_ARGS__)
// An error which can be possibly handled somewhere up the code hierarchy
#define LogWarn($format,  ...) LogWarnAt( LogCurrentTarget, LogCurrentLocation, $format, __VA_ARGS__)
// Informational message
#define LogInfo($format,  ...) LogInfoAt( LogCurrentTarget, LogCurrentLocation, $format, __VA_ARGS__)
// Debug data, like state of some structure after operation
#define LogDebug($format, ...) LogDebugAt(LogCurrentTarget, LogCurrentLocation, $format, __VA_ARGS__)

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