// LogFunction.h
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

#ifndef ORION_LOGGING_LOGFUNCTION_H
#define ORION_LOGGING_LOGFUNCTION_H

#include <cstdint>

#include <orion/logging/Logger.h>

namespace orion
{
namespace logging
{
//! A LogFunction represents an function being logged.
/*!
 */
class LogFunction 
{
public:
   LogFunction(Logger::Level level,
               const std::string& function_name,
               const std::string& file,
               int32_t line);

   virtual ~LogFunction();

   //! Returns the level of the log record
   virtual Logger::Level level() const;

   //! Records the level of the log record
   virtual void level(Logger::Level level);

   //! Returns the function name recorded
   virtual std::string function_name() const;

   //! Records a function name
   virtual void function_name(const std::string& function);

protected:
   LogFunction(const LogFunction& rhs);
   LogFunction& operator=(const LogFunction& rhs);

private:
   Logger::Level _level;
   std::string   _function_name;

};

} // namespace logging
} // namespace orion

#endif /* ORION_LOGGING_LOGFUNCTION_H */
