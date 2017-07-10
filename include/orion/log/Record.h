// Record.h
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

#ifndef ORION_LOG_RECORD_H
#define ORION_LOG_RECORD_H

#include <orion/Orion-Stddefs.h>

#include <orion/SourceLocation.h>
#include <orion/log/Level.h>

#include <boost/format.hpp>
#include <chrono>

namespace orion
{
namespace log
{
//! A Record represents an event being logged.
/*!
    A Record instance is created every time something is logged. They
    contain all the information pertinent to the event being logged.

    All usefull information is passed to the Record to log. It is
    the jog of th Formatter to format the Records information that
    will be saved by the OutputHandler.
 */
class API_EXPORT Record
{
public:
   Record();

   Record(Level level, const std::string& msg);

   Record(Level level, const std::string& msg, const SourceLocation& src_loc);

   Record(const Record& rhs);
   Record(Record&& rhs);

   virtual ~Record();

   //! Returns the level of the log record
   virtual Level level() const;

   //! Records the level of the log record
   virtual void level(Level level);

   //! Returns the time stamp of the log record
   virtual std::chrono::system_clock::time_point time_stamp() const;

   //! Returns the message recorded
   virtual std::string message() const;

   //! Records a message
   virtual void message(const std::string& msg);

   //! Returns the source location recorded
   virtual const SourceLocation& source_location() const;

   //! Records source location
   virtual void source_location(const SourceLocation& value);

   Record& operator=(const Record& rhs);
   Record& operator=(Record&& rhs);

   //! Log zero terminated strings
   Record& operator<<(const std::string& value);

   //! Log an integer
   Record& operator<<(int value);

   //! Log a double
   Record& operator<<(double value);

   //! Log a character
   Record& operator<<(char value);

   //! Log std::error_code
   Record& operator<<(const std::error_code& ec);

   //! Log boost format
   Record& operator<<(const boost::format& fmt);

   /// Log stream buffer
   Record& operator<<(std::streambuf* buf);

private:
   Level _level;
   std::chrono::system_clock::time_point _time_stamp;
   std::string _message;

   SourceLocation _src_location;
};

} // namespace log
} // namespace orion

#endif /* ORION_LOG_RECORD_H */
