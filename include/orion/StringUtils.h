// Copyright (C) 2009 by Tomas Palazuelos, Inc. All rights reserved.
// Released under the terms of the GNU General Public License version 2.
// See COPYRIGHT file for more information.
#ifndef ORION_STRINGUTILS_H
#define ORION_STRINGUTILS_H

#include <orion/Orion-Stddefs.h>

#include <algorithm>
#include <cstdint>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

namespace orion
{
typedef std::list<std::string> StringList;
typedef std::vector<std::string> StringVector;

class API_EXPORT BadConversion : public std::runtime_error
{
public:
   BadConversion(const std::string& s)
      : std::runtime_error(s)
   {
   }
};

/// convert UTF-8 string to wstring
API_EXPORT std::wstring utf8_to_wstring(const std::string& str);

/// convert wstring to UTF-8 string
API_EXPORT std::string wstring_to_utf8(const std::wstring& str);

//!
API_EXPORT bool equals_no_case(const std::string& str1, const std::string& str2);

//! Splits a string into various substrings.
API_EXPORT StringVector split(const std::string& text, char separator);

//! Remove whitespace on the left side of the string
API_EXPORT std::string& trim_left(std::string& text);

//! Remove whitespace on the right side of the string
API_EXPORT std::string& trim_right(std::string& text);

//! Remove whitespace on both sides of the string
API_EXPORT std::string& trim(std::string& text);

//! lower case a string
API_EXPORT std::string& to_lower(std::string& text);

//! Upper case a string
API_EXPORT std::string& to_upper(std::string& text);

//! Hash a string
API_EXPORT uint32_t compute_hash(const std::string& text);

// Speperate a string into different tokens
API_EXPORT void tokenize(const std::string& input,
                         std::vector<std::string>& tokens,
                         const std::string& delimiters);

inline std::string plural(int n, const std::string& s, const std::string& p)
{
   return (n > 1) ? p : s;
}

//! Converts an ostream-able type to a std::string
template<typename T>
inline std::string to_string(const T& x)
{
   std::ostringstream os;

   if (not(os << x))
      throw BadConversion("Cannot convert " + std::string(typeid(x).name()) + " to string");

   return os.str();
}

//! Converts a string to an arbitrary type that supports iostream.
/*! Inspired by the C++ FAQ.
   \param str  the string to convert
   \param x  the object to hold converted value
   \param fail_if_leftover_chars  if set to true, the function will
   throw a BadConversion exception if any characters are remaining after
   the conversion
*/
template<typename T>
inline void convert_to(const std::string& str, T& x, bool fail_if_leftover_chars = true)
{
   std::istringstream is(str.c_str());
   char c;

   if ((not(is >> x)) or (fail_if_leftover_chars and is.get(c)))
      throw BadConversion(str);
}

//! Converts a string to an arbitrary type that supports iostream by returning by value.
/*! This is a convenience function which is handy for conversions to
   primitive types.
   \param str  the string to convert
   \param fail_if_leftover_chars  if set to true, the function will
   throw a BadConversion exception if any characters are remaining after
   the conversion
   \return  the object with converted value
*/
template<typename T>
inline T convert_to(const std::string& str, bool fail_if_leftover_chars = true)
{
   T x;
   convert_to(str, x, fail_if_leftover_chars);
   return x;
}

/*!

 */
template<typename I>
struct MapKeyIterator : public I
{
   typedef typename I::value_type::first_type value_type;
   MapKeyIterator(I const& i)
      : I(i)
   {
   }
   value_type const& operator*() const { return (*this)->first; }
};

template<typename I>
struct MapValueIterator : public I
{
   typedef typename I::value_type::second_type value_type;
   MapValueIterator(I const& i)
      : I(i)
   {
   }
   value_type const& operator*() const { return (*this)->second; }
};

template<typename I>
struct join_functor : public I::value_type
{
   typedef typename I::value_type value_type;
   join_functor(value_type const& sep)
      : sep(sep)
   {
   }
   void operator()(value_type const& s)
   {
      *this += s;
      *this += sep;
   }

private:
   const value_type sep;
};

template<typename I>
typename I::value_type join(typename I::value_type const& sep, I beg, I const& end)
{
   return std::for_each(beg, end, join_functor<I>(sep));
}

template<typename I>
typename I::value_type::first_type join_keys(typename I::value_type::first_type const& sep,
                                             I const& beg,
                                             I const& end)
{
   return join(sep, MapKeyIterator<I>(beg), MapKeyIterator<I>(end));
}
template<typename I>
typename I::value_type::first_type join_keys(typename I::value_type::first_type const& sep,
                                             std::pair<I, I> const& ip)
{
   return join(sep, MapKeyIterator<I>(ip.first), MapKeyIterator<I>(ip.second));
}
template<typename I>
typename I::value_type::second_type join_values(typename I::value_type::second_type const& sep,
                                                I const& beg,
                                                I const& end)
{
   return join(sep, MapValueIterator<I>(beg), MapValueIterator<I>(end));
}
template<typename I>
typename I::value_type::second_type join_values(typename I::value_type::second_type const& sep,
                                                std::pair<I, I> const& ip)
{
   return join(sep, MapValueIterator<I>(ip.first), MapValueIterator<I>(ip.second));
}

} // namespace orion
#endif // ORION_STRINGUTILS_H
