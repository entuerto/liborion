//
// Common.ipp
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_COMMON_IPP
#define ORION_COMMON_IPP

#include <iostream>

namespace orion
{

inline SourceLocation::SourceLocation(std::string_view fn, int ln)
   : _file_name(fn.begin(), fn.end())
   , _line_number(ln)
{
}

inline SourceLocation::SourceLocation(std::string_view fn, int ln, std::string_view fun)
   : _file_name(fn.begin(), fn.end())
   , _line_number(ln)
   , _function_name(fun.begin(), fun.end())
{
}

inline const std::string& SourceLocation::file_name() const
{
   return _file_name;
}

inline constexpr int SourceLocation::line_number() const
{
   return _line_number;
}

inline const std::string& SourceLocation::function_name() const
{
   return _function_name;
}

//-------------------------------------------------------------------------------------------------
// Exception

inline Exception::Exception(std::string text)
   : _text(std::move(text))
{
}

inline Exception::Exception(std::string text, SourceLocation src_loc)
   : _text(std::move(text))
   , _src_location(std::move(src_loc))
{
}

inline const SourceLocation& Exception::source_location() const
{
   return _src_location;
}

inline const char* Exception::what() const noexcept
{
   return _text.c_str();
}

//-------------------------------------------------------------------------------------------------
// SourceLocation

inline std::ostream& operator<<(std::ostream& out, const SourceLocation& sl)
{
   out << sl.file_name() << ":" << sl.line_number();

   if (not sl.function_name().empty())
   {
      out << ":" << sl.function_name();
   }
   return out;
}

} // namespace orion
#endif // ORION_COMMON_IPP
