// Copyright (C) 2009 by Tomas Palazuelos, Inc. All rights reserved.
// Released under the terms of the GNU General Public License version 2.
// See COPYRIGHT file for more information.

#include <orion/StringUtils.h>

#include <locale>

namespace orion
{
/*!
   This is a convenience function that wraps the Glib::Regex::split_simple()
   function. Use the Glib::Regex::split_simple() function iif possible.
 */
StringVector split(const std::string& text, char separator)
{
   // return Glib::Regex::split_simple(separator, text);
   StringVector v;

   if (text.empty())
      return v;

   std::string::size_type i = 0;
   std::string::size_type j = text.find(separator);

   while (j != std::string::npos) 
   {
      v.push_back(text.substr(i, j - i));

      i = ++j;
      j = text.find(separator, j);

      if (j == std::string::npos) 
         v.push_back(text.substr(i, text.length()));
   }   
   return v;
}

std::string& trim_left(std::string& text)
{
   if (text.empty())
      return text;

   std::string::iterator it  = text.begin();
   std::string::iterator end = text.end();

   for ( ; it != end; ++it) 
   {
      if (not std::isspace(*it, std::locale()))
         break;
   }

   if (it == text.end())
      text.clear();
   else
      text.erase(text.begin(), it);
   return text;
}

std::string& trim_right(std::string& text)
{
   if (text.empty())
      return text;

   std::string::iterator end = text.end();
   std::string::iterator it  = --(text.end());

   for ( ; ; --it) 
   {
      if (not std::isspace(*it, std::locale())) 
      {
         std::string::iterator it_adv(it);
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

std::string& trim(std::string& text)
{
   return trim_right(trim_left(text));
}

uint32_t compute_hash(const std::string& text)
{
   const uint32_t BASE = 65521; // largest prime smaller than 65536

   uint32_t checksum = 1;

   //(By Per Bothner)
   uint32_t s1 = checksum & 0xffff;
   uint32_t s2 = checksum >> 16;

   int off = 0;
   int len = text.size();
   const uint8_t* buf = (const uint8_t*)text.data();

   while (len > 0) {
      // We can defer the modulo operation:
      // s1 maximally grows from 65521 to 65521 + 255 * 3800
      // s2 maximally grows by 3800 * median(s1) = 2090079800 < 2^31
      int n = 3800;
      if (n > len)
         n = len;
      len -= n;
      while (--n >= 0) {
         s1 = s1 + (buf[off++] & 0xFF);
         s2 = s2 + s1;
      }
      s1 %= BASE;
      s2 %= BASE;
   }

   checksum = (s2 << 16) | s1;

   return checksum & 0xffffffffL;
}

}
