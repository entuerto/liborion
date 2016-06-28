// LogExceptionRecord.h
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

#ifndef ORION_LOGGING_LOGEXCEPTIONRECORD_H
#define ORION_LOGGING_LOGEXCEPTIONRECORD_H

#include <cstdint>
#include <string>

#include <orion/Orion-Stddefs.h>
#include <orion/logging/Logger.h>
#include <orion/logging/LogRecord.h>


namespace orion
{
namespace logging
{
//! Log record to record exceptions
/*!
    This Log record is used to record exceptions.
 */
class API_EXPORT LogExceptionRecord: public LogRecord
{
public:
   LogExceptionRecord(const std::exception& except);

   LogExceptionRecord(const std::exception& except,
                      const std::string& file,
                            int32_t line,
                      const std::string& function);

   LogExceptionRecord(const LogExceptionRecord& rhs);
   LogExceptionRecord(LogExceptionRecord&& rhs);

   virtual ~LogExceptionRecord();

   std::string thrown_file_name() const;

   int32_t thrown_line_number() const;

   std::string thrown_function_name() const;

   LogExceptionRecord& operator=(const LogExceptionRecord& rhs);
   LogExceptionRecord& operator=(LogExceptionRecord&& rhs);

private:
   std::string _thrown_file_name;
   int32_t     _thrown_line_number;
   std::string _thrown_function_name;
};

} // namespace logging
} // namespace orion

#endif /* ORION_LOGGING_LOGEXCEPTIONRECORD_H */
