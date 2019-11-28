
#include <cinttypes>
#include <ctime>
#include <cassert>

#include "LogUtils.h"
#include "Logger.h"

namespace Log {

template<class T>
void WriteItem(std::string& str, const char* fmt, T&& arg)
{
    constexpr size_t LOBUF = 16;
    char localbuf[LOBUF] = "";

    auto written = snprintf(localbuf, LOBUF - 1, fmt, arg);
    assert(written > 0 && written < LOBUF);

    str.append(localbuf);
}

void Write_year(std::string& str, const Data::Record&, const tm& timestamp)
{
    WriteItem(str, "%04d", timestamp.tm_year + 1900);
}

void Write_mon(std::string& str, const Data::Record&, const tm& timestamp)
{
    WriteItem(str, "%02d", timestamp.tm_mon + 1);
}

void Write_day(std::string& str, const Data::Record&, const tm& timestamp)
{
    WriteItem(str, "%02d", timestamp.tm_mday);
}

void Write_h(std::string& str, const Data::Record&, const tm& timestamp)
{
    WriteItem(str, "%02d", timestamp.tm_hour);
}

void Write_m(std::string& str, const Data::Record&, const tm& timestamp)
{
    WriteItem(str, "%02d", timestamp.tm_min);
}

void Write_s(std::string& str, const Data::Record&, const tm& timestamp)
{
    WriteItem(str, "%02d", timestamp.tm_sec);
}

void Write_tid(std::string& str, const Data::Record& rec, const tm&)
{
    WriteItem(str, "0x%04" PRIxPTR, rec.threadId);
}

void Write_file(std::string& str, const Data::Record& rec, const tm&)
{
	std::string fName("<unknown>");
	if (rec.location.file && *(rec.location.file))
	{
		fs::path p(rec.location.file);
		fName = p.filename().string();
	}

	str.append(fName.c_str());
}

void Write_line(std::string& str, const Data::Record& rec, const tm&)
{
    WriteItem(str, "%d", rec.location.line);
}

void Write_sev(std::string& str, const Data::Record& rec, const tm&)
{
    const char* sevName = "EMPTY";
    switch(rec.severity)
    {
        case Data::Severity::Error:   sevName = "ERROR"; break;
        case Data::Severity::Warning: sevName = "WARN "; break;
        case Data::Severity::Info:    sevName = "INFO "; break;
        case Data::Severity::Debug:   sevName = "DEBUG"; break;
    }

	str.append(sevName);
}

void Write_msg(std::string& str, const Data::Record& rec, const tm&)
{
	str.append(rec.message ? rec.message : "");
}

template<void (*Fun)(std::string&, const Data::Record&, const tm&)>
bool WriteSpec(std::string& fmt, const std::string& spec, std::string& str, const Data::Record& rec, const tm& timestamp)
{
    if(spec != fmt.substr(0, spec.size()))
    {
        return false;
    }

    fmt = fmt.substr(spec.size(), fmt.size());
    Fun(str, rec, timestamp);
    return true;
}

std::string FormatRecord(const Data::Record& rec, std::string fmt)
{
	static constexpr char SPEC_CH = '$';
	static std::string spec(1, SPEC_CH);
    std::string str;

	tm timestamp{};
	{
		auto t = std::chrono::system_clock::to_time_t(rec.timestamp);
		errno_t err = localtime_s(&timestamp, &t);
		// TODO: err must be handled
	}

	while (!fmt.empty())
	{
		// Find next spec start
		auto next = fmt.find(SPEC_CH);
		if (std::string::npos == next)
		{
			// SPEC_CH not found - add the rest part of format
			next = fmt.size();
		}

		// Append prefix into str and truncate format
		str.append(fmt.c_str(), next);
		fmt = fmt.substr(next, fmt.size() - next);

		if (!fmt.empty())
		{
			// Check if we match any format specifier
#define SPEC($ident) if(WriteSpec<Write_##$ident>(fmt, spec + "{"#$ident"}", str, rec, timestamp)) { continue; }
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
			str += SPEC_CH;
			fmt = fmt.substr(0, 1);
		}
	}
    return str;
}

}// namespace Log