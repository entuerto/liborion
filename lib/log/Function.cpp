// Function.cpp
//
// Copyright 2013 tomas <tomasp@videotron.ca>
//
// Copyright 2016 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
#include <orion/log/Function.h>

#include <orion/log/Logger.h>
#include <orion/log/Record.h>

namespace orion
{
namespace log
{
/*!
   Constructor
 */
Function::Function(Level level, const SourceLocation& src_loc)
   : _level(level)
   , _name(src_loc.function_name)
{
   Logger& logger = default_logger();

   if (not logger.is_enabled(level))
      return;

   logger += Record(level, "", src_loc) << "Entering function: " << _name;
   logger.push_scope();
}

/*
   Copy constructor
 */
Function::Function(const Function& rhs)
   : _level(rhs._level)
   , _name(rhs._name)
{
}

Function::Function(Function&& rhs)
   : _level(std::move(rhs._level))
   , _name(std::move(rhs._name))
{
}

Function::~Function()
{
   Logger& logger = default_logger();

   if (not logger.is_enabled(_level))
      return;

   logger.pop_scope();
   logger += Record(_level, "") << "Exiting function: " << _name;
}

/*!
   Returns the level of the log record
 */
Level Function::level() const
{
   return _level;
}

/*!
   Records the level of the log record
 */
void Function::level(Level level)
{
   _level = level;
}

/*!
   Returns the function name recorded
 */
std::string Function::name() const
{
   return _name;
}

/*!
   Records a function name
 */
void Function::name(const std::string& value)
{
   _name = value;
}

Function& Function::operator=(const Function& rhs)
{
   if (this == &rhs)
      return *this;

   _level = rhs._level;
   _name  = rhs._name;

   return *this;
}

Function& Function::operator=(Function&& rhs)
{
   _level = std::move(rhs._level);
   _name  = std::move(rhs._name);

   return *this;
}

} // namespace log
} // namespace orion
