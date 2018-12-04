
#include <cinttypes>
#include <ctime>
#include <cassert>

#include "LogUtils.h"

namespace {

template<class T>
void WriteLo(std::string& buffer, const char* format, T&& arg)
{
    constexpr size_t LOBUF = 16;
    char localbuf[LOBUF] = "";

    auto written = snprintf(localbuf, LOBUF - 1, format, arg);
    assert(written > 0 && written < LOBUF);

    buffer.append(localbuf);
}

void Write_year(std::string& buffer, const Record&, const tm& timestamp)
{
    WriteLo(buffer, "%04d", timestamp.tm_year + 1900);
}

void Write_mon(std::string& buffer, const Record&, const tm& timestamp)
{
    WriteLo(buffer, "%02d", timestamp.tm_mon + 1);
}

void Write_day(std::string& buffer, const Record&, const tm& timestamp)
{
    WriteLo(buffer, "%02d", timestamp.tm_mday);
}

void Write_h(std::string& buffer, const Record&, const tm& timestamp)
{
    WriteLo(buffer, "%02d", timestamp.tm_hour);
}

void Write_m(std::string& buffer, const Record&, const tm& timestamp)
{
    WriteLo(buffer, "%02d", timestamp.tm_min);
}

void Write_s(std::string& buffer, const Record&, const tm& timestamp)
{
    WriteLo(buffer, "%02d", timestamp.tm_sec);
}

void Write_tid(std::string& buffer, const Record& rec, const tm&)
{
    WriteLo(buffer, "0x%04" PRIxPTR, rec.threadId);
}

void Write_file(std::string& buffer, const Record& rec, const tm&)
{
    buffer.append(
        rec.location.file && *(rec.location.file)
        ? rec.location.file
        : "<unknown>"
    );
}

void Write_line(std::string& buffer, const Record& rec, const tm&)
{
    WriteLo(buffer, "%d", rec.location.line);
}

void Write_sev(std::string& buffer, const Record& rec, const tm&)
{
    const char* sevName = " ??? ";
    switch(rec.severity)
    {
        case Severity::Error:   sevName = "ERROR"; break;
        case Severity::Warning: sevName = "WARN "; break;
        case Severity::Info:    sevName = "INFO "; break;
        case Severity::Debug:   sevName = "DEBUG"; break;
    }

    buffer.append(sevName);
}

void Write_msg(std::string& buffer, const Record& rec, const tm&)
{
    buffer.append(rec.message ? rec.message : "");
}

template<void (*Fun)(std::string&, const Record&, const tm&)>
bool WriteSpec(std::string& format, const std::string& spec, std::string& buffer, const Record& rec, const tm& timestamp)
{
    if(spec != format.substr(0, spec.size()))
    {
        return false;
    }

    format = format.substr(spec.size(), format.size());
    Fun(buffer, rec, timestamp);
    return true;
}

void FormatRecord(std::string& buffer, const Record& record, std::string format)
{
    static constexpr char SPEC_CH = '$';
    static std::string spec("$");
    tm timestamp{};
    {
        using namespace std::chrono;
        // Compute seconds part
        auto ytime = system_clock::to_time_t(record.timestamp);
        errno_t err = localtime_s(&timestamp, &ytime); // TODO: err should be handled
    }

    while (!format.empty())
    {
        // Find next spec start
        auto next = format.find(SPEC_CH);
        if (std::string::npos == next)
        {
            // SPEC_CH not found - add the rest part of format
            next = format.size();
        }

        // Append prefix into buffer and truncate format
        buffer.append(format.c_str(), next);
        format = format.substr(next, format.size() - next);
        
        if (!format.empty())
        {
            // Check if we match any format specifier
#define SPEC($ident) if(WriteSpec<Write_##$ident>(format, spec + "{"#$ident"}", buffer, record, timestamp)) { continue; }
            SPEC(year)
            SPEC(mon)
            SPEC(day)
            SPEC(h)
            SPEC(m)
            SPEC(s)
            SPEC(tid)
            SPEC(file)
            SPEC(line)
            SPEC(sev)
            SPEC(msg)
#undef SPEC
            // In case of no match, append '$' and go forward
            buffer += SPEC_CH;
            format = format.substr(0, 1);
        }
    }
}

}// namespace

std::string FormatRecord(const Record& record, std::string format)
{
    std::string buffer;
    FormatRecord(buffer, record, format);
    return buffer;
}
