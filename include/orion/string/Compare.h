//
// Compare.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_COMPARE_H
#define ORION_COMPARE_H

#include <orion/Orion-Stddefs.h>

#include <algorithm>
#include <locale>
#include <string>

namespace orion
{

//--------------------------------------------------------------------------------------------------
// Functors

/// is_equal functor
struct is_equal
{
   /// Compare two operands for equality
   template<typename T1, typename T2>
   bool operator()(const T1& arg1, const T2& arg2) const
   {
      return arg1 == arg2;
   }
};

/// Case insensitive version of is_equal
struct is_iequal
{
   /// Constructor
   /// @param loc locale used for comparison
   explicit is_iequal(const std::locale& loc = std::locale())
      : _loc(loc)
   {
   }

   /// Compare two operands. Case is ignored.
   template<typename T1, typename T2>
   bool operator()(const T1& arg1, const T2& arg2) const
   {
      return std::toupper<T1>(arg1, _loc) == std::toupper<T2>(arg2, _loc);
   }

   std::locale _loc;
};

/// is_less functor
struct is_less
{
   /// Compare two operands using > operator
   template<typename T1, typename T2>
   bool operator()(const T1& arg1, const T2& arg2) const
   {
      return arg1 < arg2;
   }
};

/// Case insensitive version of is_less
struct is_iless
{
   /// Constructor
   /// @param loc locale used for comparison
   explicit is_iless(const std::locale& loc = std::locale())
      : _loc(loc)
   {
   }

   /// Compare two operands. Case is ignored.
   template<typename T1, typename T2>
   bool operator()(const T1& arg1, const T2& arg2) const
   {
      return std::toupper<T1>(arg1, _loc) < std::toupper<T2>(arg2, _loc);
   }

private:
   std::locale _loc;
};

//--------------------------------------------------------------------------------------------------
// Predicates

template<typename StringT>
inline bool equals(const StringT& str1, const StringT& str2)
{
   return (str1.size() == str2.size()) and
          std::equal(str1.begin(), str1.end(), str2.begin(), is_equal{});
}

template<typename StringT>
inline bool iequals(const StringT& str1,
                    const StringT& str2,
                    const std::locale& loc = std::locale())
{
   return (str1.size() == str2.size()) and
          std::equal(str1.begin(), str1.end(), str2.begin(), is_iequal{loc});
}

template<typename StringT>
inline bool less(const StringT& str1, const StringT& str2)
{
   return (str1.size() == str2.size()) and
          std::lexicographical_compare(
             str1.begin(), str1.end(), str2.begin(), str2.end(), is_less{});
}

template<typename StringT>
inline bool iless(const StringT& str1, const StringT& str2, const std::locale& loc = std::locale())
{
   return (str1.size() == str2.size()) and
          std::lexicographical_compare(
             str1.begin(), str1.end(), str2.begin(), str2.end(), is_iless{loc});
}

template<typename StringT>
inline bool starts_with(const StringT& str1, const StringT& str2)
{
   if (str1.end() - str1.begin() < str2.end() - str2.begin())
      return false;

   return std::equal(str2.begin(), str2.end(), str1.begin(), is_equal{});
}

template<typename StringT>
inline bool istarts_with(const StringT& str1, const StringT& str2)
{
   if (str1.end() - str1.begin() < str2.end() - str2.begin())
      return false;

   return std::equal(str2.begin(), str2.end(), str1.begin(), is_iequal{});
}

} // namespace orion
#endif // ORION_COMPARE_H
