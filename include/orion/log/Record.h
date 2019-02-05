//
// Record.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_LOG_RECORD_H
#define ORION_LOG_RECORD_H

#include <orion/Common.h>

#include <orion/Chrono.h>
#include <orion/log/Level.h>

#include <sstream>

namespace orion
{
namespace log
{
/// A Record represents an event being logged.
///
/// A Record instance is created every time something is logged. They
/// contain all the information pertinent to the event being logged.

/// All usefull information is passed to the Record to log. It is
/// the jog of th Formatter to format the Records information that
/// will be saved by the OutputHandler.
///
class API_EXPORT Record
{
public:
   NO_COPY(Record)
   
   Record();

   Record(Level level, const std::string& msg);

   Record(Level level, const std::string& msg, const SourceLocation& src_loc);

   Record(Record&& rhs) noexcept;

   virtual ~Record() = default;

   /// Returns the level of the log record
   Level level() const;

   /// Records the level of the log record
   void level(Level level);

   /// Returns the time stamp of the log record
   TimePoint<> time_stamp() const;

   /// Returns the message recorded
   virtual std::string message() const;

   /// Records a message
   virtual void message(const std::string& msg);

   /// Appends some text to the message
   virtual void append_message(const std::string& text);

   /// Returns the source location recorded
   const SourceLocation& source_location() const;

   /// Records source location
   void source_location(const SourceLocation& value);

   Record& operator=(Record&& rhs) noexcept;

   template<typename T>
   Record& operator<<(const T& value);

private:
   Level _level;
   TimePoint<> _time_stamp;
   std::ostringstream _message;

   SourceLocation _src_location;
};

//-------------------------------------------------------------------------------------------------

template<typename T>
Record& Record::operator<<(const T& value)
{
   _message << value;
   return *this;
}

} // namespace log
} // namespace orion

#endif /* ORION_LOG_RECORD_H */
