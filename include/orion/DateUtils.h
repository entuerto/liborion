// Date-utils.h
//
// Copyright 2013 tomas <tomasp@videotron.ca>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.

#ifndef ORION_DATE_UTILS_H
#define ORION_DATE_UTILS_H

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

#include <orion/Orion-Stddefs.h>

namespace orion
{

template<typename CharT, typename Clock>
std::basic_string<CharT> to_basic_string(const std::chrono::time_point<Clock>& tp, const CharT* fmt)
{
	auto t = Clock::to_time_t(tp);

	std::tm tm{0};

   tm = *std::localtime(&t);

   std::basic_stringstream<CharT> ostr;

	ostr << std::put_time(&tm, fmt);

	return ostr.str();
}

template <typename Clock>
std::string to_string(const std::chrono::time_point<Clock>& tp, const char* fmt) 
{
   return to_basic_string<char>(tp, fmt);
}

template <typename Clock>
std::string to_wstring(const std::chrono::time_point<Clock>& tp, const wchar_t* fmt) 
{
   return to_basic_string<wchar_t>(tp, fmt);
}

template <typename Clock>
std::string to_u16string(const std::chrono::time_point<Clock>& tp, const char16_t* fmt) 
{
   return to_basic_string<char16_t>(tp, fmt);
}

template <typename Clock>
std::string to_u32string(const std::chrono::time_point<Clock>& tp, const char32_t* fmt) 
{
   return to_basic_string<char32_t>(tp, fmt);
}

} // namespace orion

#endif // ORION_DATE_UTILS_H
