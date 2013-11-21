// LogSystemInfoRecord.cpp
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

#include <logging/LogSystemInfoRecord.h>

#include <string>

namespace orion
{
namespace logging
{
//----------------------------------------------------------------------------
// LogSystemInfoRecord
/*!
   Constructor
 */
LogSystemInfoRecord::LogSystemInfoRecord() :
   LogRecord()
{
}

LogSystemInfoRecord::LogSystemInfoRecord(const std::string& msg) :
   LogRecord(),
   _message(msg)
{
}

/*
   Copy constructor
 */
LogSystemInfoRecord::LogSystemInfoRecord(const LogSystemInfoRecord& rhs) :
   LogRecord(rhs)
{
}

LogSystemInfoRecord::~LogSystemInfoRecord()
{

}

/*!
   Returns the level of the log record
 */
Logger::Level LogSystemInfoRecord::level() const
{
   return Logger::NotSet;
}

/*!
   Records the level of the log record
 */
void LogSystemInfoRecord::level(Logger::Level /* level */)
{
}

/*!
   Returns the time stamp of the log record
 */
std::string LogSystemInfoRecord::time_stamp() const
{
   return "";
}

/*!
   Returns the message recorded
 */
std::string LogSystemInfoRecord::message() const
{
   return _message;
}

/*!
   Records a message
 */
void LogSystemInfoRecord::message(const std::string& /* msg */)
{
}

/*!
   Returns the file name recorded
 */
std::string LogSystemInfoRecord::file_name() const
{
   return "";
}

/*!
   Records a file name
 */
void LogSystemInfoRecord::file_name(const std::string& /* file */)
{
}

/*!
   Returns the line number recorded
 */
int32_t LogSystemInfoRecord::line() const
{
   return -1;
}

/*!
   Record a line number
 */
void LogSystemInfoRecord::line(int32_t /* l */)
{
}

/*!
   Returns the function name recorded
 */
std::string LogSystemInfoRecord::function_name() const
{
   return "";
}

/*!
   Records a function name
 */
void LogSystemInfoRecord::function_name(const std::string& /* function */)
{
}

LogSystemInfoRecord& LogSystemInfoRecord::operator=(const LogSystemInfoRecord& rhs)
{
   if (this == &rhs)
      return *this;

   LogRecord::operator=(rhs);

   return *this;
}

} // namespace logging
} // namespace orion
