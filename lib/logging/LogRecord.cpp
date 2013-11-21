// LogRecord.cpp
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

#include <orion/DateUtils.h>
#include <orion/logging/LogRecord.h>

namespace orion
{
namespace logging
{
/*!
   Constructor
 */
LogRecord::LogRecord() :
   _level(Logger::NotSet),
   _time_stamp(),
   _message(),
   _file_name(),
   _line(-1),
   _function_name()
{
   get_current_time(_time_stamp);
}

/*!
   Constructor
 */
LogRecord::LogRecord(Logger::Level level, const std::string& msg) :
   _level(level),
   _time_stamp(),
   _message(msg),
   _file_name(),
   _line(-1),
   _function_name()
{
   get_current_time(_time_stamp);
}

/*!
   Constructor
 */
LogRecord::LogRecord(Logger::Level level,
                     const std::string& file,
                     int32_t line,
                     const std::string& function) :
   _level(level),
   _time_stamp(),
   _message(),
   _file_name(file),
   _line(line),
   _function_name(function)
{
   get_current_time(_time_stamp);
}

/*!
   Constructor
 */
LogRecord::LogRecord(Logger::Level level,
                     const std::string& msg,
                     const std::string& file,
                     int32_t line,
                     const std::string& function) :
   _level(level),
   _time_stamp(),
   _message(msg),
   _file_name(file),
   _line(line),
   _function_name(function)
{
   get_current_time(_time_stamp);
}
/*
   Copy constructor
 */
LogRecord::LogRecord(const LogRecord& rhs) :
   _level(rhs._level),
   _time_stamp(rhs._time_stamp),
   _message(rhs._message),
   _file_name(rhs._file_name),
   _line(rhs._line),
   _function_name(rhs._function_name)
{
}

LogRecord::~LogRecord()
{

}

/*!
   Returns the level of the log record
 */
Logger::Level LogRecord::level() const
{
   return _level;
}

/*!
   Records the level of the log record
 */
void LogRecord::level(Logger::Level level)
{
   _level = level;
}

/*!
   Returns the time stamp of the log record
 */
std::string LogRecord::time_stamp() const
{
   return _time_stamp;
}

/*!
   Returns the message recorded
 */
std::string LogRecord::message() const
{
   return _message;
}

/*!
   Records a message
 */
void LogRecord::message(const std::string& msg)
{
   _message = msg;
}

/*!
   Returns the file name recorded
 */
std::string LogRecord::file_name() const
{
   return _file_name;
}

/*!
   Records a file name
 */
void LogRecord::file_name(const std::string& file)
{
   _file_name = file;
}

/*!
   Returns the line number recorded
 */
int32_t LogRecord::line() const
{
   return _line;
}

/*!
   Record a line number
 */
void LogRecord::line(int32_t l)
{
   _line = l;
}

/*!
   Returns the function name recorded
 */
std::string LogRecord::function_name() const
{
   return _function_name;
}

/*!
   Records a function name
 */
void LogRecord::function_name(const std::string& function)
{
   _function_name = function;
}

LogRecord& LogRecord::operator=(const LogRecord& rhs)
{
   if (this == &rhs)
      return *this;

   _level = rhs._level;
   _time_stamp = rhs._time_stamp;
   _message = rhs._message;
   _file_name = rhs._file_name;
   _line = rhs._line;
   _function_name = rhs._function_name;

   return *this;
}

/*!
   \param value string value to write
 */
LogRecord& LogRecord::operator<<(const std::string& value)
{
   _message += value;
   return *this;
}

/*!
   \param value int value to write
 */
LogRecord& LogRecord::operator<<(int value)
{
   _message += std::to_string(value);
   return *this;
}

/*!
   \param value double value to write
 */
LogRecord& LogRecord::operator<<(double value)
{
   _message += std::to_string(value);
   return *this;
}

/*!
   \param value char value to write
 */
LogRecord& LogRecord::operator<<(char value)
{
   _message += std::to_string(value);
   return *this;
}

LogRecord::SharedPtr LogRecord::create()
{
   return LogRecord::SharedPtr(new LogRecord);
}

LogRecord::SharedPtr LogRecord::create(Logger::Level level, const std::string& msg)
{
   return LogRecord::SharedPtr(new LogRecord(level, msg));
}

LogRecord::SharedPtr LogRecord::create(Logger::Level level,
                                       const std::string& file,
                                             int32_t line,
                                       const std::string& function)
{
   return LogRecord::SharedPtr(new LogRecord(level, file, line, function));
}

LogRecord::SharedPtr LogRecord::create(Logger::Level level,
                                       const std::string& msg,
                                       const std::string& file,
                                             int32_t line,
                                       const std::string& function)
{
   return LogRecord::SharedPtr(new LogRecord(level, msg, file, line, function));
}

} // namespace logging
} // namespace orion
