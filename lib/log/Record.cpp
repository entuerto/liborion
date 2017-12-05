//
// Record.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
// 
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
{
}

/*!
   Constructor
 */
Record::Record(Level level, const std::string& msg)
   : _level(level)
   , _time_stamp(std::chrono::system_clock::now())
   , _message(msg)
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

Record::Record(Record&& rhs) noexcept
   : _level(rhs._level)
   , _time_stamp(rhs._time_stamp)
   , _message(std::move(rhs._message))
   , _src_location(std::move(rhs._src_location))
{
}

Level Record::level() const
{
   return _level;
}

void Record::level(Level level)
{
   _level = level;
}

TimePoint<> Record::time_stamp() const
{
   return _time_stamp;
}

std::string Record::message() const
{
   return _message.str();
}

void Record::message(const std::string& msg)
{
   _message.str(msg);
}

void Record::append_message(const std::string& text)
{
   _message << text;
}

const SourceLocation& Record::source_location() const
{
   return _src_location;
}

void Record::source_location(const SourceLocation& value)
{
   _src_location = value;
}

Record& Record::operator=(Record&& rhs) noexcept 
{
   _level        = rhs._level;
   _time_stamp   = rhs._time_stamp;
   _message      = std::move(rhs._message);
   _src_location = std::move(rhs._src_location);

   return *this;
}

} // namespace log
} // namespace orion
