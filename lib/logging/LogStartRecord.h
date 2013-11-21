// LogStartRecord.h
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


#ifndef ORION_LOGGING_LOGSTARTRECORD_H
#define ORION_LOGGING_LOGSTARTRECORD_H

#include <cstdint>
#include <string>

#include <orion/logging/Logger.h>
#include <orion/logging/LogRecord.h>

namespace orion
{
namespace logging
{
//! First LogRecord sent when logging
/*!
    This LogRecord tells the user that the logging
    process is about to start.
 */
class LogStartRecord : public LogRecord
{
public:
   LogStartRecord();

   LogStartRecord(const LogStartRecord& rhs);

   virtual ~LogStartRecord();

   virtual Logger::Level level() const;

   virtual void level(Logger::Level level);

   virtual std::string time_stamp() const;

   virtual std::string message() const;

   virtual void message(const std::string& msg);

   virtual std::string file_name() const;

   virtual void file_name(const std::string& file);

   virtual int32_t line() const;

   virtual void line(int32_t l);

   virtual std::string function_name() const;

   virtual void function_name(const std::string& function);

   LogStartRecord& operator=(const LogStartRecord& rhs);

private:

};

} // namespace logging
} // namespace orion

#endif /* ORION_LOGGING_LOGSTARTRECORD_H */
