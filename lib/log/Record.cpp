// Record.cpp
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

#include <orion/log/Record.h>

#include <orion/Chrono.h>

#include <string>

namespace orion
{
namespace log
{
/*!
   Constructor
 */
Record::Record()
   : _level(Level::NotSet)
   , _time_stamp(std::chrono::system_clock::now())
   , _message()
   , _src_location()
{
}

/*!
   Constructor
 */
Record::Record(Level level, const std::string& msg)
   : _level(level)
   , _time_stamp(std::chrono::system_clock::now())
   , _message(msg)
   , _src_location()
{
}

/*!
   Constructor
 */
Record::Record(Level level, const std::string& msg, const SourceLocation& src_loc)
   : _level(level)
   , _time_stamp(std::chrono::system_clock::now())
   , _message(msg)
   , _src_location(src_loc)
{
}

/*
   Copy constructor
 */
Record::Record(const Record& rhs)
   : _level(rhs._level)
   , _time_stamp(rhs._time_stamp)
   , _message(rhs._message)
   , _src_location(rhs._src_location)
{
}

Record::Record(Record&& rhs)
   : _level(std::move(rhs._level))
   , _time_stamp(std::move(rhs._time_stamp))
   , _message(std::move(rhs._message))
   , _src_location(std::move(rhs._src_location))
{
}

Record::~Record()
{
}

/*!
   Returns the level of the log record
 */
Level Record::level() const
{
   return _level;
}

/*!
   Records the level of the log record
 */
void Record::level(Level level)
{
   _level = level;
}

/*!
   Returns the time stamp of the log record
 */
std::chrono::system_clock::time_point Record::time_stamp() const
{
   return _time_stamp;
}

/*!
   Returns the message recorded
 */
std::string Record::message() const
{
   return _message;
}

/*!
   Records a message
 */
void Record::message(const std::string& msg)
{
   _message = msg;
}

const SourceLocation& Record::source_location() const
{
   return _src_location;
}

void Record::source_location(const SourceLocation& value)
{
   _src_location = value;
}

Record& Record::operator=(const Record& rhs)
{
   if (this == &rhs)
      return *this;

   _level        = rhs._level;
   _time_stamp   = rhs._time_stamp;
   _message      = rhs._message;
   _src_location = rhs._src_location;

   return *this;
}

Record& Record::operator=(Record&& rhs)
{
   _level        = std::move(rhs._level);
   _time_stamp   = std::move(rhs._time_stamp);
   _message      = std::move(rhs._message);
   _src_location = std::move(rhs._src_location);

   return *this;
}

/*!
   \param value string value to write
 */
Record& Record::operator<<(const std::string& value)
{
   _message += value;
   return *this;
}

/*!
   \param value int value to write
 */
Record& Record::operator<<(int value)
{
   _message += std::to_string(value);
   return *this;
}

/*!
   \param value double value to write
 */
Record& Record::operator<<(double value)
{
   _message += std::to_string(value);
   return *this;
}

/*!
   \param value char value to write
 */
Record& Record::operator<<(char value)
{
   _message += std::to_string(value);
   return *this;
}

Record& Record::operator<<(const std::error_code& ec)
{
   auto fmt = boost::format("%s (%d): %s") % ec.category().name() % ec.value() % ec.message();
   _message += boost::str(fmt);
   return *this;
}

Record& Record::operator<<(const boost::format& fmt)
{
   _message += fmt.str();
   return *this;
}

Record& Record::operator<<(std::streambuf* buf)
{
   std::ostringstream ostr;

   ostr << buf;

   _message += ostr.str();

   return *this;
}

} // namespace log
} // namespace orion
