
#include <cinttypes>
#include <ctime>
#include <cassert>

#include "LogUtils.h"

namespace {

struct TmExt : tm
{
    int tm_msec; // milliseconds fraction
};

template<class T>
void WriteLo(std::string& buffer, const char* format, T&& arg)
{
    constexpr size_t LOBUF = 16;
    char localbuf[LOBUF] = "";

    auto written = snprintf(localbuf, LOBUF - 1, format, arg);
    assert(written > 0 && written < LOBUF);

    buffer.append(localbuf);
}

void Write_year(std::string& buffer, const Record&, const TmExt& timestamp)
{
    WriteLo(buffer, "%04d", timestamp.tm_year + 1900);
}

void Write_mon(std::string& buffer, const Record&, const TmExt& timestamp)
{
    WriteLo(buffer, "%02d", timestamp.tm_mon + 1);
}

void Write_day(std::string& buffer, const Record&, const TmExt& timestamp)
{
    WriteLo(buffer, "%02d", timestamp.tm_mday);
}

void Write_h(std::string& buffer, const Record&, const TmExt& timestamp)
{
    WriteLo(buffer, "%02d", timestamp.tm_hour);
}

void Write_m(std::string& buffer, const Record&, const TmExt& timestamp)
{
    WriteLo(buffer, "%02d", timestamp.tm_min);
}

void Write_s(std::string& buffer, const Record&, const TmExt& timestamp)
{
    WriteLo(buffer, "%02d", timestamp.tm_sec);
}

void Write_tid(std::string& buffer, const Record& rec, const TmExt&)
{
    WriteLo(buffer, "0x%04" PRIxPTR, rec.threadId);
}

void Write_file(std::string& buffer, const Record& rec, const TmExt&)
{
    buffer.append(
        rec.location.file && *(rec.location.file)
        ? rec.location.file
        : "<unknown>"
    );
}

void Write_line(std::string& buffer, const Record& rec, const TmExt&)
{
    WriteLo(buffer, "%d", rec.location.line);
}

void Write_sev(std::string& buffer, const Record& rec, const TmExt&)
{
    const char* sevName = " ??? ";
    switch(rec.metadata.severity)
    {
        case Severity::Error:   sevName = "ERROR"; break;
        case Severity::Warning: sevName = "WARN "; break;
        case Severity::Info:    sevName = "INFO "; break;
        case Severity::Debug:   sevName = "DEBUG"; break;
    }
    buffer.append(sevName);
}

void Write_msg(std::string& buffer, const Record& rec, const TmExt&)
{
    buffer.append(rec.message ? rec.message : "");
}

template<void (*Fn)(std::string&, const Record&, const TmExt&)>
bool WriteSpec(std::string& format, const std::string& spec, std::string& buffer, const Record& rec, const TmExt& timestamp)
{
    if(spec != format.substr(0, spec.size()))
    {
        return false;
    }

    format = format.substr(spec.size(), format.size());
    Fn(buffer, rec, timestamp);
    return true;
}

void FormatRecord(std::string& buffer, const Record& record, std::string format)
{
    static constexpr char SPEC_CH = '$';
    static std::string spec("$");
    
    while(!format.empty())
    {
        TmExt timestamp{};
        {
            using namespace std::chrono;
            // Compute seconds part
            auto ytime = system_clock::to_time_t(record.timestamp);
            errno_t err = localtime_s(&timestamp, &ytime);
        }

        // Find next spec start
        auto next = format.find(SPEC_CH);
        if(next == std::string::npos)
        {
            // SPEC_CH not found
            break;
        }

        // Dump prefix into string and shrink format
        buffer.append(format.c_str(), next);
        format = format.substr(next, format.size() - next);
        if(format.empty())
        {
            break;
        }

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

}// namespace

std::string FormatRecord(const Record& record, std::string format)
{
    std::string buffer;
    FormatRecord(buffer, record, format);
    return buffer;
}


