// MyLog.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <vld.h>

#include "src\Logger.h"

#include <conio.h>
#include <iostream>
#include <atomic>

int main()
{
    UseLogger(ConsoleLogger);
    /*// 4. Construct file logger path
    std::wstring fileLogPath;
    {
        auto logsPath = appDataPath + L"\\Logs\\";
        // 4a. Form name for log file; use pattern YYYY-MM-DD_HH-MM-SS_MMM.log
        // We have time formatting in diag::FormatRecord, so simply use it
        diag::Record rec;
        rec.timestamp = std::chrono::system_clock::now();
        auto name = diag::FormatRecord(rec, "${year}-${mon}-${day}_${h}-${m}-${s}_${ms}.log");
        // 4b. Append file name to overall path
        string::EncodingConverter<string::Encoding::Utf8, string::Encoding::Wide> utf16;
        utf16.Convert(name, &logsPath);
        fileLogPath = std::move(logsPath);
    }*/

    int val = 10;
    LogError("Value is: %d", val);
    
    Record rec;
    rec.timestamp = std::chrono::system_clock::now();

    //auto str = FormatRecord(rec, "${year}-${mon}-${day}_${h}-${m}-${s}_${ms}.log");

    _getch();

    return 0;
}
