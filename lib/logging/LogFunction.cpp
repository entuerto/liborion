// LogFunction.cpp
//
// Copyright 2013 tomas <tomasp@videotron.ca>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.

#include <string>

#include <orion/logging/LogFunction.h>
#include <orion/logging/LogRecord.h>

namespace orion
{
namespace logging
{
/*!
   Constructor
 */
LogFunction::LogFunction(Logger::Level level, const std::string& function_name,
                                              const std::string& file,
                                                    int32_t line) :
   _level(level),
   _function_name(function_name)
{
   Logger& logger = Logger::get_logger();

   if (not logger.is_enabled(level))
      return;

   logger += LogRecord(level, file, line, "") << "Entering function: "
                                              << function_name;
   logger.push_scope();                                                            
}

/*
   Copy constructor
 */
LogFunction::LogFunction(const LogFunction& rhs) :
   _level(rhs._level),
   _function_name(rhs._function_name)
{
}

LogFunction::~LogFunction()
{
   Logger& logger = Logger::get_logger();

   if (not logger.is_enabled(_level))
      return;

   logger.pop_scope();                                                            
   logger += LogRecord(_level, "") << "Exiting function: "
                                   << _function_name; 
}

/*!
   Returns the level of the log record
 */
Logger::Level LogFunction::level() const
{
   return _level;
}

/*!
   Records the level of the log record
 */
void LogFunction::level(Logger::Level level)
{
   _level = level;
}

/*!
   Returns the function name recorded
 */
std::string LogFunction::function_name() const
{
   return _function_name;
}

/*!
   Records a function name
 */
void LogFunction::function_name(const std::string& function)
{
   _function_name = function;
}

LogFunction& LogFunction::operator=(const LogFunction& rhs)
{
   if (this == &rhs)
      return *this;

   _level = rhs._level;
   _function_name = rhs._function_name;

   return *this;
}


} // namespace logging
} // namespace orion

