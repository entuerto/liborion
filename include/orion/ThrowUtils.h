//
// ThrowUtils.h
//
// Created by Tomas Palazuelos on 2017-04-26.
// Copyright © 2017 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_THROWUTILS_H
#define ORION_THROWUTILS_H

#include <orion/Orion-Stddefs.h>

#include <orion/SourceLocation.h>

namespace orion
{

template<typename E>
void throw_exception(const std::string& msg)
{
   throw E(msg);
}

template<typename E>
void throw_exception(const std::string& msg, const SourceLocation& src_loc)
{
   throw E(msg, src_loc);
}

template<typename E>
void throw_if(bool condition, const std::string& msg, const SourceLocation& src_loc = SourceLocation{})
{
   if (condition)
      throw_exception<E>(msg, src_loc);
}

template<typename E, typename T>
void throw_if_null(const T* value, const std::string& msg, const SourceLocation& src_loc = SourceLocation{})
{
   if (value == nullptr)
      throw_exception<E>(msg, src_loc);
}

} // namespace orion

#endif /* ORION_THROWUTILS_H */
