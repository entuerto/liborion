//
// String.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_STRING_H
#define ORION_STRING_H

#include <orion/Orion-Stddefs.h>

#include <algorithm>
#include <list>
#include <string>
#include <vector>

namespace orion
{
typedef std::list<std::string> StringList;
typedef std::vector<std::string> StringVector;

/// convert UTF-8 string to wstring
API_EXPORT std::wstring utf8_to_wstring(const std::string& str);

/// convert wstring to UTF-8 string
API_EXPORT std::string wstring_to_utf8(const std::wstring& str);

///
API_EXPORT bool equals_no_case(const std::string& str1, const std::string& str2);

/// Splits a string into various substrings.
API_EXPORT StringVector split(const std::string& text, char separator);

/// Remove whitespace on the left side of the string
API_EXPORT std::string& trim_left(std::string& text);

/// Remove whitespace on the right side of the string
API_EXPORT std::string& trim_right(std::string& text);

/// Remove whitespace on both sides of the string
API_EXPORT std::string& trim(std::string& text);

/// lower case a string
API_EXPORT std::string& to_lower(std::string& text);

/// Upper case a string
API_EXPORT std::string& to_upper(std::string& text);

/// Speperate a string into different tokens
API_EXPORT void tokenize(const std::string& input,
                         std::vector<std::string>& tokens,
                         const std::string& delimiters);

inline std::string plural(std::size_t n, const std::string& s, const std::string& p)
{
   return (n > 1) ? p : s;
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
#endif // ORION_STRING_H
