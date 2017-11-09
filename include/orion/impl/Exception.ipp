//
// Exception.ipp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_EXCEPTION_IPP
#define ORION_EXCEPTION_IPP

#include <string>

namespace orion
{

inline Exception::Exception(const std::string& text, const SourceLocation& src_loc)
   : std::exception()
   , _text(text)
   , _src_location(src_loc)
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

} // namespace orion
#endif // ORION_EXCEPTION_IPP
