// LogEndRecord.h
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

#ifndef ORION_LOGGING_LOGENDRECORD_H
#define ORION_LOGGING_LOGENDRECORD_H

#include <cstdint>
#include <string>

#include <orion/logging/LogRecord.h>
#include <orion/logging/Logger.h>

namespace orion
{
namespace logging
{
//! Informs the end of the log
/*!
    The last record sent when the logger is shutdown. If this
    log record is not written to the output, the application
    has probably crashed.
 */
class LogEndRecord : public LogRecord
{
public:
   LogEndRecord();

   LogEndRecord(const LogEndRecord& rhs);
   LogEndRecord(LogEndRecord&& rhs);

   virtual ~LogEndRecord();

   virtual Level level() const override;

   virtual void level(Level level) override;

   virtual std::string time_stamp() const override;

   virtual std::string message() const override;

   virtual void message(const std::string& msg) override;

   virtual std::string file_name() const override;

   virtual void file_name(const std::string& file) override;

   virtual int32_t line() const override;

   virtual void line(int32_t l) override;

   virtual std::string function_name() const override;

   virtual void function_name(const std::string& function) override;

   LogEndRecord& operator=(const LogEndRecord& rhs);
   LogEndRecord& operator=(LogEndRecord&& rhs);

private:

};

} // namespace logging
} // namespace orion

#endif /* ORION_LOGGING_LOGENDRECORD_H */
