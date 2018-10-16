//
// Throw.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_THROW_H
#define ORION_THROW_H

#include <orion/Orion-Stddefs.h>
#include <orion/SourceLocation.h>

#include <string>

namespace orion
{

template<typename E>
[[noreturn]] void throw_exception(const std::string& msg)
{
   throw E(msg);
}

template<typename E>
[[noreturn]] void throw_exception(const std::string& msg, const SourceLocation& src_loc)
{
   throw E(msg, src_loc);
}

template<typename E>
void throw_if(bool condition,
              const std::string& msg,
              const SourceLocation& src_loc = SourceLocation{})
{
   if (condition)
      throw_exception<E>(msg, src_loc);
}

template<typename E, typename T>
void throw_if_null(const T* value,
                   const std::string& msg,
                   const SourceLocation& src_loc = SourceLocation{})
{
   if (value == nullptr)
      throw_exception<E>(msg, src_loc);
}

} // namespace orion

#endif /* ORION_THROW_H */
