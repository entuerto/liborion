// LogEndRecord.cpp
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

#include <logging/LogEndRecord.h>

#include <orion/DateUtils.h>

namespace orion
{
namespace logging
{
//----------------------------------------------------------------------------
// LogEndRecord
/*!
   Constructor
 */
LogEndRecord::LogEndRecord() :
   LogRecord()
{
}

/*
   Copy constructor
 */
LogEndRecord::LogEndRecord(const LogEndRecord& rhs) :
   LogRecord(rhs)
{
}

LogEndRecord::~LogEndRecord()
{

}

/*!
   Returns the level of the log record
 */
Logger::Level LogEndRecord::level() const
{
   return Logger::NotSet;
}

/*!
   Records the level of the log record
 */
void LogEndRecord::level(Logger::Level /* level */)
{
}

/*!
   Returns the time stamp of the log record
 */
std::string LogEndRecord::time_stamp() const
{
   return "";
}

/*!
   Returns the message recorded
 */
std::string LogEndRecord::message() const
{
   std::string date_time;
   get_current_datetime(date_time);
   return "\nLog End Time - " + date_time;
}

/*!
   Records a message
 */
void LogEndRecord::message(const std::string& /* msg */)
{
}

/*!
   Returns the file name recorded
 */
std::string LogEndRecord::file_name() const
{
   return "";
}

/*!
   Records a file name
 */
void LogEndRecord::file_name(const std::string& /* file */)
{
}

/*!
   Returns the line number recorded
 */
int32_t LogEndRecord::line() const
{
   return -1;
}

/*!
   Record a line number
 */
void LogEndRecord::line(int32_t /* l */)
{
}

/*!
   Returns the function name recorded
 */
std::string LogEndRecord::function_name() const
{
   return "";
}

/*!
   Records a function name
 */
void LogEndRecord::function_name(const std::string& /* function */)
{
}

LogEndRecord& LogEndRecord::operator=(const LogEndRecord& rhs)
{
   if (this == &rhs)
      return *this;

   LogRecord::operator=(rhs);

   return *this;
}

} // namespace logging
} // namespace orion
