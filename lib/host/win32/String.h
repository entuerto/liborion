//
// String.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_HOST_WIN32_STRING_H
#define ORION_HOST_WIN32_STRING_H

#include <orion/String.h>

#include <Windows.h>

namespace orion
{
namespace win32
{

// convert UTF-8 string to wstring
inline std::wstring utf8_to_wstring(const std::string& str)
{
   // Get buffer size
   int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);

   wchar_t ws_buffer[len];

   MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, ws_buffer, len);

   return std::wstring{ws_buffer};
}

// convert wstring to UTF-8 string
inline std::string wstring_to_utf8(const std::wstring& str)
{
   // Get buffer size
   int len = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, nullptr, 0, nullptr, nullptr);

   char buffer[len];

   WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, buffer, len, nullptr, nullptr);

   return std::string{buffer};
}

inline void format_error_message(DWORD last_error_code, std::string& error_message)
{
   wchar_t* buffer = nullptr;

   FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                  FORMAT_MESSAGE_IGNORE_INSERTS,
                  nullptr,
                  last_error_code,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  buffer,
                  0,
                  nullptr);

   if (buffer != nullptr)
   {
      error_message = wstring_to_utf8(buffer);

      LocalFree(buffer);
   }
}

} // namespace win32
} // namespace orion

#endif // ORION_HOST_WIN32_STRING_H
