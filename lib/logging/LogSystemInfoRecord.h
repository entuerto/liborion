// LogSystemInfoRecord.h
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

#ifndef ORION_LOGGING_LOGSYSTEMINFORECORD_H
#define ORION_LOGGING_LOGSYSTEMINFORECORD_H

#include <cstdint>
#include <string>

#include <orion/logging/Logger.h>
#include <orion/logging/LogRecord.h>

namespace orion
{
namespace logging
{
//! LogRecord that contains the system information to print
/*!
    LogRecord that contains the system information to print.
    The record is sent when the logger is started.
 */
class LogSystemInfoRecord : public LogRecord
{
public:
   LogSystemInfoRecord();

   LogSystemInfoRecord(const std::string& msg);

   LogSystemInfoRecord(const LogSystemInfoRecord& rhs);
   LogSystemInfoRecord(LogSystemInfoRecord&& rhs);

   virtual ~LogSystemInfoRecord();

   virtual Level level() const override;

   virtual void level(Level level)  override;

   virtual std::string time_stamp() const override;

   virtual std::string message() const override;

   virtual void message(const std::string& msg) override;

   virtual std::string file_name() const override;

   virtual void file_name(const std::string& file) override;

   virtual int32_t line() const override;

   virtual void line(int32_t l) override;

   virtual std::string function_name() const override;

   virtual void function_name(const std::string& function) override;

   LogSystemInfoRecord& operator=(const LogSystemInfoRecord& rhs);
   LogSystemInfoRecord& operator=(LogSystemInfoRecord&& rhs);

private:
   std::string _message;

};

} // namespace logging
} // namespace orion

#endif /* ORION_LOGGING_LOGSYSTEMINFORECORD_H */
