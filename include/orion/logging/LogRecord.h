// LogRecord.h
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

#ifndef ORION_LOGGING_LOGRECORD_H
#define ORION_LOGGING_LOGRECORD_H

#include <cstdint>

#include <orion/Orion-Stddefs.h>
#include <orion/MemoryUtils.h>
#include <orion/logging/Logger.h>

namespace orion
{
namespace logging
{
//! A LogRecord represents an event being logged.
/*!
    A LogRecord instance is created every time something is logged. They
    contain all the information pertinent to the event being logged.

    All usefull information is passed to the LogRecord to log. It is
    the jog of th Formatter to format the LogRecords information that
    will be saved by the OutputHandler.
 */
class API_EXPORT LogRecord 
{
public:
   DECLARE_POINTERS(LogRecord)

   LogRecord();

   LogRecord(Logger::Level level,
             const std::string& msg);

   LogRecord(Logger::Level level,
             const std::string& file,
                   int32_t line,
             const std::string& function);

   LogRecord(Logger::Level level,
             const std::string& msg,
             const std::string& file,
                   int32_t line,
             const std::string& function);

   LogRecord(const LogRecord& rhs);

   virtual ~LogRecord();

   //! Returns the level of the log record
   virtual Logger::Level level() const;

   //! Records the level of the log record
   virtual void level(Logger::Level level);

   //! Returns the time stamp of the log record
   virtual std::string time_stamp() const;

   //! Returns the message recorded
   virtual std::string message() const;

   //! Records a message
   virtual void message(const std::string& msg);

   //! Returns the file name recorded
   virtual std::string file_name() const;

   //! Records a file name
   virtual void file_name(const std::string& file);

   //! Returns the line number recorded
   virtual int32_t line() const;

   //! Record a line number
   virtual void line(int32_t l);

   //! Returns the function name recorded
   virtual std::string function_name() const;

   //! Records a function name
   virtual void function_name(const std::string& function);

   LogRecord& operator=(const LogRecord& rhs);

   //! Log zero terminated strings
   LogRecord& operator<<(const std::string& value);

   //! Log an integer
   LogRecord& operator<<(int value);

   //! Log a double
   LogRecord& operator<<(double value);

   //! Log a character
   LogRecord& operator<<(char value);

   static LogRecord::SharedPtr create();

   static LogRecord::SharedPtr create(Logger::Level level, const std::string& msg);

   static LogRecord::SharedPtr create(Logger::Level level,
                                      const std::string& file,
                                            int32_t line,
                                      const std::string& function);

   static LogRecord::SharedPtr create(Logger::Level level,
                                      const std::string& msg,
                                      const std::string& file,
                                            int32_t line,
                                      const std::string& function);
private:
   Logger::Level _level;
   std::string   _time_stamp;
   std::string   _message;
   std::string   _file_name;
   int32_t       _line;
   std::string   _function_name;

};

} // namespace logging
} // namespace orion

#endif /* ORION_LOGGING_LOGRECORD_H */
