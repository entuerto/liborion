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
//Record::Record(const Record& rhs)
//   : _level(rhs._level)
//   , _time_stamp(rhs._time_stamp)
//   , _message(rhs._message)
//   , _src_location(rhs._src_location)
//{
//}

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
TimePoint<> Record::time_stamp() const
{
   return _time_stamp;
}

/*!
   Returns the message recorded
 */
std::string Record::message() const
{
   return _message.str();
}

/*!
   Records a message
 */
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

//Record& Record::operator=(const Record& rhs)
//{
//   if (this == &rhs)
//      return *this;
//
//   _level        = rhs._level;
//   _time_stamp   = rhs._time_stamp;
//   _message      = rhs._message;
//   _src_location = rhs._src_location;
//
//   return *this;
//}

Record& Record::operator=(Record&& rhs)
{
   _level        = std::move(rhs._level);
   _time_stamp   = std::move(rhs._time_stamp);
   _message      = std::move(rhs._message);
   _src_location = std::move(rhs._src_location);

   return *this;
}

} // namespace log
} // namespace orion
