//
// Transform.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_TRANSFORM_H
#define ORION_TRANSFORM_H

#include <orion/Config.h>

#include <algorithm>
#include <locale>
#include <string>

namespace orion
{

/// Convert to upper case
///
/// Each element of the input string is converted to upper case.
///
/// @param text A string
/// @param loc A locale used for conversion
///
template<typename StringT>
inline StringT& to_upper(StringT& text, const std::locale& loc = std::locale())
{
   using CharT = typename StringT::value_type;

   std::transform(text.begin(), text.end(), text.begin(), [&](CharT c) -> CharT {
      return std::toupper(c, loc);
   });
   return text;
}

/// Convert to lower case
///
/// Each element of the input string is converted to lower case.
///
/// @param text A string
/// @param loc A locale used for conversion
///
template<typename StringT>
inline StringT& to_lower(StringT& text, const std::locale& loc = std::locale())
{
   using CharT = typename StringT::value_type;

   std::transform(text.begin(), text.end(), text.begin(), [&](CharT c) -> CharT {
      return std::tolower(c, loc);
   });
   return text;
}

} // namespace orion
#endif // ORION_TRANSFORM_H
