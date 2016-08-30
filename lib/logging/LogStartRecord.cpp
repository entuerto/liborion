// LogStartRecord.cpp
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

#include <logging/LogStartRecord.h>

#include <orion/DateUtils.h>

namespace orion
{
namespace logging
{
//----------------------------------------------------------------------------
// LogStartRecord
/*!
   Constructor
 */
LogStartRecord::LogStartRecord() :
   LogRecord()
{
}

/*
   Copy constructor
 */
LogStartRecord::LogStartRecord(const LogStartRecord& rhs) :
   LogRecord(rhs)
{
}

LogStartRecord::LogStartRecord(LogStartRecord&& rhs) :
   LogRecord(std::move(rhs))
{
}

LogStartRecord::~LogStartRecord()
{

}

/*!
   Returns the level of the log record
 */
Level LogStartRecord::level() const
{
   return Level::NotSet;
}

/*!
   Records the level of the log record
 */
void LogStartRecord::level(Level /* level */)
{
}

/*!
   Returns the time stamp of the log record
 */
std::string LogStartRecord::time_stamp() const
{
   return "";
}

/*!
   Returns the message recorded
 */
std::string LogStartRecord::message() const
{
   std::string start_time;
   get_current_datetime(start_time);
   return "\nLog Start Time - " + start_time + "\n";
}

/*!
   Records a message
 */
void LogStartRecord::message(const std::string& /* msg */)
{
}

/*!
   Returns the file name recorded
 */
std::string LogStartRecord::file_name() const
{
   return "";
}

/*!
   Records a file name
 */
void LogStartRecord::file_name(const std::string& /* file */)
{
}

/*!
   Returns the line number recorded
 */
int32_t LogStartRecord::line() const
{
   return -1;
}

/*!
   Record a line number
 */
void LogStartRecord::line(int32_t /* l */)
{
}

/*!
   Returns the function name recorded
 */
std::string LogStartRecord::function_name() const
{
   return "";
}

/*!
   Records a function name
 */
void LogStartRecord::function_name(const std::string& /* function */)
{
}

LogStartRecord& LogStartRecord::operator=(const LogStartRecord& rhs)
{
   if (this == &rhs)
      return *this;

   LogRecord::operator=(rhs);

   return *this;
}

LogStartRecord& LogStartRecord::operator=(LogStartRecord&& rhs)
{
   LogRecord::operator=(std::move(rhs));

   return *this;
}

} // namespace logging
} // namespace orion
