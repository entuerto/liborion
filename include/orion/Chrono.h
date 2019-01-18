// Chrono.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_CHRONO_H
#define ORION_CHRONO_H

#include <orion/Orion-Stddefs.h>

#include <chrono>
#include <string>

namespace orion
{

//-------------------------------------------------------------------------------------------------
//
// Types

//
// Durations
//
using Days = std::chrono::duration<int, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;
using Weeks = std::chrono::duration<int, std::ratio_multiply<std::ratio<7>, Days::period>>;
using Years = std::chrono::duration<int, std::ratio_multiply<std::ratio<146097, 400>, Days::period>>;
using Months = std::chrono::duration<int, std::ratio_divide<Years::period, std::ratio<12>>>;

//
// Time points
//
template<typename D = std::chrono::nanoseconds>
using TimePoint = std::chrono::time_point<std::chrono::system_clock, D>;

using HighResTimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

template <class Duration>
using SystemTime    = std::chrono::time_point<std::chrono::system_clock, Duration>;
using SystemSeconds = SystemTime<std::chrono::seconds>;
using SystemDays    = SystemTime<Days>;

struct local_t {};
template<class Duration>
using LocalTime    = std::chrono::time_point<local_t, Duration>;
using LocalSeconds = LocalTime<std::chrono::seconds>;
using LocalDays    = LocalTime<Days>;

//-------------------------------------------------------------------------------------------------
//
// Format strings to convert to string type

constexpr const char* ANSIC    = "%a %b %e %T %Y";     // ANSIC    = "Mon Jan _2 15:04:05 2006"
constexpr const char* UnixDate = "%a %b %e %T %Z %Y";  // UnixDate = "Mon Jan _2 15:04:05 MST 2006"
constexpr const char* RFC822   = "%d %b %y %R %Z";     // RFC822   = "02 Jan 06 15:04 MST"
constexpr const char* RFC822Z  = "%d %b %y %R %z";     // RFC822Z  = "02 Jan 06 15:04 -0700"
constexpr const char* RFC850   = "%A, %d-%b-%y %T %Z"; // RFC850   = "Monday, 02-Jan-06 15:04:05 MST"
constexpr const char* RFC1123  = "%a, %d %b %Y %T %Z"; // RFC1123  = "Mon, 02 Jan 2006 15:04:05 MST"
constexpr const char* RFC1123Z = "%a, %d %b %Y %T %z"; // RFC1123Z = "Mon, 02 Jan 2006 15:04:05 -0700"
constexpr const char* RFC3339  = "%FT%TZ%z";           // RFC3339  = "2006-01-02T15:04:05Z07:00"
constexpr const char* ISO8601  = "%FT%TZ%z";           // ISO8601  = "2006-01-02T15:04:05Z0700"

//-------------------------------------------------------------------------------------------------

template<typename Clock>
inline std::string to_string(const std::chrono::time_point<Clock>& tp, const char* fmt);

inline std::string to_string(const TimePoint<>& tp, const char* fmt);

template<typename Clock>
inline std::wstring to_wstring(const std::chrono::time_point<Clock>& tp, const wchar_t* fmt);

inline std::wstring to_wstring(const TimePoint<>& tp, const wchar_t* fmt);

template<typename Clock>
inline std::u16string to_u16string(const std::chrono::time_point<Clock>& tp, const char16_t* fmt);

// inline std::u16string to_u16string(const TimePoint<>& tp, const char16_t* fmt);

template<typename Clock>
inline std::u32string to_u32string(const std::chrono::time_point<Clock>& tp, const char32_t* fmt);

// inline std::u32string to_u32string(const TimePoint<>& tp, const char32_t* fmt);

//-------------------------------------------------------------------------------------------------
//

template<class CharT, class Traits, class Rep, class Period>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                              const std::chrono::duration<Rep, Period>& d);
                                              
//-------------------------------------------------------------------------------------------------

class Timer
{
public:
   using ClockType     = std::chrono::high_resolution_clock;
   using TimePointType = std::chrono::time_point<ClockType>;

   void start();
   void stop();
   void reset();

   // Get the elapsed time.
   // @return The value in nanoseconds.
   std::chrono::nanoseconds elapsed() const;

private:
   TimePointType _start;
   TimePointType _end;
};

} // namespace orion

#include <orion/impl/Chrono.ipp>

#endif // ORION_CHRONO_H
