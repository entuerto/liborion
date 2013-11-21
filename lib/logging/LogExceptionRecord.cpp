// LogExceptionRecord.cpp
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

#include <orion/logging/LogExceptionRecord.h>

#include <orion/Exception.h>

namespace orion
{
namespace logging
{

LogExceptionRecord::LogExceptionRecord(const std::exception& except) :
   LogRecord(Logger::Exception, except.what()),
   _thrown_file_name(),
   _thrown_line_number(-1),
   _thrown_function_name()
{
   try 
   {
      const Exception& ex = dynamic_cast<const Exception&>(except);

      _thrown_file_name = ex.file_name();
      _thrown_line_number = ex.line_number();
      _thrown_function_name = ex.function_name();
   } 
   catch(std::bad_cast&) 
   {
   }
}

LogExceptionRecord::LogExceptionRecord(const std::exception& except,
                                       const std::string& file,
                                             int32_t line,
                                       const std::string& function) :
	LogRecord(Logger::Exception, except.what(), file, line, function),
   _thrown_file_name(),
   _thrown_line_number(-1),
   _thrown_function_name()
{
   try 
   {
      const Exception& ex = dynamic_cast<const Exception&>(except);

      _thrown_file_name = ex.file_name();
      _thrown_line_number = ex.line_number();
      _thrown_function_name = ex.function_name();
   } 
   catch(std::bad_cast&) 
   {
   }
}

/*
   Copy constructor
 */
LogExceptionRecord::LogExceptionRecord(const LogExceptionRecord& rhs) :
   LogRecord(rhs),
   _thrown_file_name(rhs._thrown_file_name),
   _thrown_line_number(rhs._thrown_line_number),
   _thrown_function_name(rhs._thrown_function_name)
{
}

LogExceptionRecord::~LogExceptionRecord()
{

}

std::string LogExceptionRecord::thrown_file_name() const
{
   return _thrown_file_name;
}

int32_t LogExceptionRecord::thrown_line_number() const
{
   return _thrown_line_number;
}

std::string LogExceptionRecord::thrown_function_name() const
{
   return _thrown_function_name;
}

LogExceptionRecord& LogExceptionRecord::operator=(const LogExceptionRecord& rhs)
{
   if (this == &rhs)
      return *this;

   LogRecord::operator=(rhs);

   _thrown_file_name = rhs._thrown_file_name;
   _thrown_line_number = rhs._thrown_line_number;
   _thrown_function_name = rhs._thrown_function_name;

   return *this;
}

} // namespace logging
} // namespace orion
