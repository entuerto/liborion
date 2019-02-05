//
// Function.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
// 
#include <orion/log/Function.h>

#include <orion/log/Logger.h>
#include <orion/log/Record.h>

namespace orion
{
namespace log
{
/// 
/// Constructor
/// 
Function::Function(Level level, const SourceLocation& src_loc)
   : _level(level)
   , _name(src_loc.function_name())
{
   Logger& logger = default_logger();

   if (not logger.is_enabled(level))
      return;

   logger += Record(level, "", src_loc) << "Entering function: " << _name;
   logger.push_scope();
}

Function::Function(Function&& rhs) noexcept
   : _level(rhs._level)
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

/// 
/// Returns the level of the log record
/// 
Level Function::level() const
{
   return _level;
}

/// 
/// Records the level of the log record
/// 
void Function::level(Level level)
{
   _level = level;
}

/// 
/// Returns the function name recorded
/// 
std::string Function::name() const
{
   return _name;
}

/// 
/// Records a function name
/// 
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

Function& Function::operator=(Function&& rhs) noexcept
{
   _level = rhs._level;
   _name  = std::move(rhs._name);

   return *this;
}

} // namespace log
} // namespace orion
