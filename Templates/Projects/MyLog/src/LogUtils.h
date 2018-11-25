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

History
    Konstantin Zhelieznov              11/21/2018    Add comments to LogUtils.h
*/
std::string FormatRecord(const Record& record, std::string format);