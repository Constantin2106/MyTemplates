#pragma once
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
#define LogWriteRaw($severity, $target, $location, $format, ...) ( \
    IsEnabled($severity, $target) ? \
    Write($severity, $target, $location, $format, __VA_ARGS__) \
    : (void()) )

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

// Hard, unrecoverable error
#define LogError($format, ...) LogErrorAt(LogCurrentTarget, LogCurrentLocation, $format, __VA_ARGS__)