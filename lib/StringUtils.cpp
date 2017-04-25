// Copyright (C) 2009 by Tomas Palazuelos, Inc. All rights reserved.
// Released under the terms of the GNU General Public License version 2.
// See COPYRIGHT file for more information.

#include <orion/StringUtils.h>

#include <locale>
#include <codecvt>

namespace orion
{
inline bool no_case_char_compare(char a, char b) 
{
   return(toupper(a) == toupper(b));
}


// convert UTF-8 string to wstring
std::wstring utf8_to_wstring(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.from_bytes(str);
}

// convert wstring to UTF-8 string
std::string wstring_to_utf8(const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}

bool equals_no_case(const std::string& str1, const std::string& str2)
{
   return (str1.size() == str2.size()) and 
          std::equal(str1.begin(), str1.end(), str2.begin(), no_case_char_compare); 
}

int compare_no_case(const std::string& str1, const std::string& str2)
{
   const char* s1 = str1.c_str();
   const char* s2 = str2.c_str();

   char c2; // we need only one char outside the loop
   do
   {
      const char c1 = *s1++; // const local variable should help compiler to optimize

      c2 = *s2++;

      if (c1 != c2 and ::tolower(c1) != ::tolower(c2)) // This includes the possibility that one of the characters is the null-terminator, which implies a string mismatch.
         return ::tolower(c1) - ::tolower(c2);

   } while (c2 != '\0'); // At this point, we know c1 == c2, so there's no need to test them both.

   return 0;
}

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

   std::string::iterator it = --(text.end());

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

std::string& to_upper(std::string& text)
{
   std::transform(text.begin(), text.end(), text.begin(), ::toupper);
   return text;
}

std::string& to_lower(std::string& text)
{
   std::transform(text.begin(), text.end(), text.begin(), ::tolower);
   return text;
}

uint32_t compute_hash(const std::string& text)
{
   const uint32_t BASE = 65521; // largest prime smaller than 65536

   uint32_t checksum = 1;

   //(By Per Bothner)
   uint32_t s1 = checksum & 0xffff;
   uint32_t s2 = checksum >> 16;

   int off = 0;
   std::size_t len = text.size();
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

void tokenize(const std::string& input, std::vector<std::string>& tokens, const std::string& delimiters)
{
   // Tokenize ripped from http://www.linuxselfhelp.com/HOWTO/C++Programming-HOWTO-7.html
   // Skip delimiters at beginning.
   std::string::size_type lastPos = input.find_first_not_of(delimiters, 0);
   // Find first "non-delimiter".
   std::string::size_type pos = input.find_first_of(delimiters, lastPos);

   while (std::string::npos != pos or std::string::npos != lastPos)
   {
      // Found a token, add it to the vector.
      tokens.push_back(input.substr(lastPos, pos - lastPos));
      // Skip delimiters.  Note the "not_of"
      lastPos = input.find_first_not_of(delimiters, pos);
      // Find next "non-delimiter"
      pos = input.find_first_of(delimiters, lastPos);
   }
}

}
