//
// Chrono.ipp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_CHRONO_IPP
#define ORION_CHRONO_IPP

#include <orion/Orion-Stddefs.h>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace orion
{
//-------------------------------------------------------------------------------------------------

template<typename CharT>
inline std::basic_string<CharT> to_basic_string(const std::time_t& t, const CharT* fmt)
{
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

//-------------------------------------------------------------------------------------------------

template<typename Clock>
inline std::string to_string(const std::chrono::time_point<Clock>& tp, const char* fmt)
{
   auto t = Clock::to_time_t(tp); 

   return to_basic_string<char>(t, fmt);
}

inline std::string to_string(const TimePoint<>& tp, const char* fmt)
{
   using namespace std::chrono;

   auto t = system_clock::to_time_t(time_point_cast<system_clock::time_point::duration>(tp));

   return to_basic_string<char>(t, fmt);
}

template<typename Clock>
inline std::wstring to_wstring(const std::chrono::time_point<Clock>& tp, const wchar_t* fmt)
{
   auto t = Clock::to_time_t(tp);

   return to_basic_string<wchar_t>(t, fmt);
}

inline std::wstring to_wstring(const TimePoint<>& tp, const wchar_t* fmt)
{
   using namespace std::chrono;

   auto t = system_clock::to_time_t(time_point_cast<system_clock::time_point::duration>(tp));

   return to_basic_string<wchar_t>(t, fmt);
}

template<typename Clock>
inline std::u16string to_u16string(const std::chrono::time_point<Clock>& tp, const char16_t* fmt)
{
   auto t = Clock::to_time_t(tp);

   return to_basic_string<char16_t>(t, fmt);
}
/*
inline std::u16string to_u16string(const TimePoint<>& tp, const char16_t* fmt)
{
   using namespace std::chrono;

   auto t = system_clock::to_time_t(time_point_cast<system_clock::time_point::duration>(tp));

   return to_basic_string<char16_t>(t, fmt);
}
*/
template<typename Clock>
inline std::u32string to_u32string(const std::chrono::time_point<Clock>& tp, const char32_t* fmt)
{
   auto t = Clock::to_time_t(tp);

   return to_basic_string<char32_t>(t, fmt);
}
/*
inline std::u32string to_u32string(const TimePoint<>& tp, const char32_t* fmt)
{
   using namespace std::chrono;

   auto t = system_clock::to_time_t(time_point_cast<system_clock::time_point::duration>(tp));

   return to_basic_string<char32_t>(t, fmt);
}
*/

//-------------------------------------------------------------------------------------------------
//
inline static std::string unit_names(std::type_index i)
{
   static std::unordered_map<std::type_index, std::string> names{
      {std::type_index(typeid(std::chrono::hours::period)), "h"},
      {std::type_index(typeid(std::chrono::minutes::period)), "min"},
      {std::type_index(typeid(std::chrono::seconds::period)), "s"},
      {std::type_index(typeid(std::chrono::milliseconds::period)), "ms"},
      {std::type_index(typeid(std::chrono::microseconds::period)), "\xB5s"},
      {std::type_index(typeid(std::chrono::nanoseconds::period)), "ns"},
      {std::type_index(typeid(Days::period)), "Days"},
      {std::type_index(typeid(Weeks::period)), "weeks"},
      {std::type_index(typeid(Months::period)), "Months"},
      {std::type_index(typeid(Years::period)), "Years"}};

   return names[i];
}

template<class CharT, class Traits, class Rep, class Period>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                              const std::chrono::duration<Rep, Period>& d)
{
   using namespace std::chrono;
   return os << d.count() << unit_names(std::type_index(typeid(typename Period::type)));
}

template<class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                     const SystemDays& dp)
{
   auto y = std::chrono::duration_cast<Years>(dp.time_since_epoch());
   auto m = std::chrono::duration_cast<Months>(dp.time_since_epoch());
   auto d = std::chrono::duration_cast<Days>(dp.time_since_epoch());

   return os << y << '/' << m << '/' << d;
}

template<class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                     const LocalDays& dp)
{
   auto y = std::chrono::duration_cast<Years>(dp.time_since_epoch());
   auto m = std::chrono::duration_cast<Months>(dp.time_since_epoch());
   auto d = std::chrono::duration_cast<Days>(dp.time_since_epoch());

   return os << y << '/' << m << '/' << d;
}

//-------------------------------------------------------------------------------------------------

inline void Timer::start()
{
   _start = ClockType::now();
}

inline void Timer::stop()
{
   _end = ClockType::now();
}

inline void Timer::reset()
{
   start();
}

inline std::chrono::nanoseconds Timer::elapsed() const
{
   using namespace std::chrono;

   return duration_cast<nanoseconds>(_end - _start);
}

} // namespace orion
#endif // ORION_CHRONO_IPP
