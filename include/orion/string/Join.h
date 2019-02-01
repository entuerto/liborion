//
// Join.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_JOIN_H
#define ORION_JOIN_H

#include <orion/Orion-Stddefs.h>

#include <fmt/format.h>

#include <locale>
#include <sstream>
#include <string>

namespace orion
{
//--------------------------------------------------------------------------------------------------
class BaseFormatter;
class CharArrayFormatter;
class StreamFormatter;

struct NoFormatter
{
};

template<typename ValueType>
struct Formatter
{
   using Type = BaseFormatter;
};

template<>
struct Formatter<char*>
{
   using Type = CharArrayFormatter;
};

template<>
struct Formatter<const char*>
{
   using Type = CharArrayFormatter;
};

template<>
struct Formatter<std::string>
{
   using Type = NoFormatter;
};

//--------------------------------------------------------------------------------------------------

class BaseFormatter
{
public:
   template<typename T>
   std::string operator()(const T& t) const
   {
      return fmt::to_string(t);
   }
};

class CharArrayFormatter
{
public:
   std::string operator()(char* t) const { return std::string{t}; }

   std::string operator()(const char* t) const { return std::string{t}; }
};

/// Formats types to strings using a string stream.
class StreamFormatter
{
public:
   template<typename T>
   std::string operator()(const T& t)
   {
      _stream.clear();
      _stream << t;
      return _stream.str();
   }

private:
   std::ostringstream _stream;
};

//--------------------------------------------------------------------------------------------------

template<typename StrintT>
class Joinner
{
public:
   constexpr explicit Joinner(StrintT s)
      : _separator(std::move(s))
   {
   }

   template<typename IteratorT>
   StrintT operator()(IteratorT begin, IteratorT end, NoFormatter) const // NOLINT
   {
      // Construct container to hold the result
      StrintT result;

      // Append first element
      if (begin != end)
      {
         result.append(*begin);
         ++begin;
      }

      for (; begin != end; ++begin)
      {
         // Add separator
         result.append(_separator);
         // Add element
         result.append(*begin);
      }

      return result;
   }

   template<typename IteratorT, typename FormatterT>
   StrintT operator()(IteratorT begin, IteratorT end, FormatterT&& f) const
   {
      // Construct container to hold the result
      StrintT result;

      // Append first element
      if (begin != end)
      {
         result.append(f(*begin));
         ++begin;
      }

      for (; begin != end; ++begin)
      {
         // Add separator
         result.append(_separator);
         // Add element
         result.append(f(*begin));
      }

      return result;
   }

   template<typename IteratorT>
   StrintT operator()(IteratorT begin, IteratorT end) const
   {
      using ValueType = typename std::iterator_traits<IteratorT>::value_type;
      using Formatter = typename Formatter<ValueType>::Type;

      return operator()(begin, end, Formatter());
   }

   template<typename RangeT>
   StrintT operator()(const RangeT& input) const
   {
      return operator()(std::begin(input), std::end(input));
   }

   template<typename T>
   StrintT operator()(std::initializer_list<T> l) const
   {
      return operator()(std::begin(l), std::end(l));
   }

private:
   StrintT _separator;
};

template<typename StrintT, typename RangeT>
StrintT operator|(const Joinner<StrintT>& j, const RangeT& input)
{
   return j(input);
}

//--------------------------------------------------------------------------------------------------

using Join = Joinner<std::string>;

//--------------------------------------------------------------------------------------------------

/// Join all elements in a range into a single string
template<typename StrintT, typename RangeT>
inline StrintT join(const RangeT& input, StrintT separator)
{
   return Joinner<StrintT>{std::move(separator)}(input);
}

/// Join all elements in a range (start, end) into a single string
template<typename StrintT, typename IteratorT>
inline StrintT join(IteratorT start, IteratorT end, StrintT separator)
{
   return Joinner<StrintT>{std::move(separator)}(start, end);
}

/// Join all elements in a initializer list into a single string
template<typename StrintT, typename T>
StrintT join(std::initializer_list<T> l, StrintT separator)
{
   return Joinner<StrintT>{std::move(separator)}(std::move(l));
}

} // namespace orion
#endif // ORION_JOIN_H
