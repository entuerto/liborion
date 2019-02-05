//
// Trim.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_TRIM_H
#define ORION_TRIM_H

#include <orion/Config.h>

#include <locale>
#include <string>

namespace orion
{

/// Remove whitespace on the left side of the string
template<typename StringT>
inline StringT& trim_left(StringT& text, const std::locale& loc = std::locale())
{
   if (text.empty())
      return text;

   auto it  = text.begin();
   auto end = text.end();

   for (; it != end; ++it)
   {
      if (not std::isspace(*it, loc))
         break;
   }

   if (it == text.end())
      text.clear();
   else
      text.erase(text.begin(), it);

   return text;
}

/// Remove whitespace on the right side of the string
template<typename StringT>
inline StringT& trim_right(StringT& text, const std::locale& loc = std::locale())
{
   if (text.empty())
      return text;

   auto it = --(text.end());

   for (;; --it)
   {
      if (not std::isspace(*it, loc))
      {
         auto it_adv{it};
         text.erase(++it_adv, text.end());
         break;
      }

      if (it == text.begin())
      {
         text.clear();
         break;
      }
   }
   return text;
}

/// Remove whitespace on both sides of the string
template<typename StringT>
inline StringT& trim(StringT& text, const std::locale& loc = std::locale())
{
   return trim_right(trim_left(text, loc), loc);
}

} // namespace orion
#endif // ORION_TRIM_H
