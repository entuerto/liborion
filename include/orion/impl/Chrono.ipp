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

namespace orion
{
//-------------------------------------------------------------------------------------------------

template<typename CharT, typename Clock>
inline std::basic_string<CharT> to_basic_string(const std::chrono::time_point<Clock>& tp, const CharT* fmt)
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

//-------------------------------------------------------------------------------------------------

template<typename Clock>
inline std::string to_string(const std::chrono::time_point<Clock>& tp, const char* fmt)
{
   return to_basic_string<char>(tp, fmt);
}

template<typename Clock>
inline std::wstring to_wstring(const std::chrono::time_point<Clock>& tp, const wchar_t* fmt)
{
   return to_basic_string<wchar_t>(tp, fmt);
}

template<typename Clock>
inline std::u16string to_u16string(const std::chrono::time_point<Clock>& tp, const char16_t* fmt)
{
   return to_basic_string<char16_t>(tp, fmt);
}

template<typename Clock>
inline std::u32string to_u32string(const std::chrono::time_point<Clock>& tp, const char32_t* fmt)
{
   return to_basic_string<char32_t>(tp, fmt);
}

//-------------------------------------------------------------------------------------------------

inline void Timer::start()
{
   _start = std::chrono::high_resolution_clock::now();
}

inline void Timer::stop()
{
   _end = std::chrono::high_resolution_clock::now();
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
