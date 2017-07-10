// Date-utils.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//

#ifndef ORION_DATE_UTILS_H
#define ORION_DATE_UTILS_H

#include <orion/Orion-Stddefs.h>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

namespace orion
{

constexpr const char* ANSIC    = "%a %b %e %T %Y";    // ANSIC    = "Mon Jan _2 15:04:05 2006"
constexpr const char* UnixDate = "%a %b %e %T %Z %Y"; // UnixDate = "Mon Jan _2 15:04:05 MST 2006"
constexpr const char* RFC822   = "%d %b %y %R %Z";    // RFC822   = "02 Jan 06 15:04 MST"
constexpr const char* RFC822Z  = "%d %b %y %R %z";    // RFC822Z  = "02 Jan 06 15:04 -0700"
constexpr const char* RFC850   = "%A, %d-%b-%y %T %Z"; // RFC850   = "Monday, 02-Jan-06 15:04:05 MST"
constexpr const char* RFC1123  = "%a, %d %b %Y %T %Z"; // RFC1123  = "Mon, 02 Jan 2006 15:04:05 MST"
constexpr const char* RFC1123Z = "%a, %d %b %Y %T %z";                    // RFC1123Z = "Mon, 02 Jan 2006 15:04:05 -0700"
constexpr const char* RFC3339  = "%FT%TZ%z"; // RFC3339  = "2006-01-02T15:04:05Z07:00"
constexpr const char* ISO8601  = "%FT%TZ%z"; // ISO8601  = "2006-01-02T15:04:05Z0700"

template<typename CharT, typename Clock>
std::basic_string<CharT> to_basic_string(const std::chrono::time_point<Clock>& tp, const CharT* fmt)
{
   auto t = Clock::to_time_t(tp);

   std::tm tm{0};

#if _WIN32
   localtime_s(&tm, &t);
#else
   tm = *std::localtime(&t);
#endif

   std::basic_stringstream<CharT> ostr;

   ostr << std::put_time(&tm, fmt);

   return ostr.str();
}

template<typename Clock>
std::string to_string(const std::chrono::time_point<Clock>& tp, const char* fmt)
{
   return to_basic_string<char>(tp, fmt);
}

template<typename Clock>
std::string to_wstring(const std::chrono::time_point<Clock>& tp, const wchar_t* fmt)
{
   return to_basic_string<wchar_t>(tp, fmt);
}

template<typename Clock>
std::string to_u16string(const std::chrono::time_point<Clock>& tp, const char16_t* fmt)
{
   return to_basic_string<char16_t>(tp, fmt);
}

template<typename Clock>
std::string to_u32string(const std::chrono::time_point<Clock>& tp, const char32_t* fmt)
{
   return to_basic_string<char32_t>(tp, fmt);
}

} // namespace orion

#endif // ORION_DATE_UTILS_H
