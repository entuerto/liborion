/*
 * Logger.h
 *
 * Copyright 2013 tomas <tomasp@videotron.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef ORION_LOGGING_LOGGER_H
#define ORION_LOGGING_LOGGER_H

#include <functional>
#include <memory>
#include <vector>

#include <orion/Orion-Stddefs.h>
#include <orion/logging/OutputHandler.h>

namespace orion
{
namespace logging
{
API_EXPORT std::string DefaultInfo();
//--------------------------------------------------------------------------
// forward declares
class Formatter;
class LogRecord;

enum class Level
{
   NotSet    = 0,
   Debug3    = 10,
   Debug2    = 20,
   Debug     = 30,
   Info      = 40,
   Message   = 50,
   Warning   = 60,
   Error     = 70,
   Exception = 80
};

//--------------------------------------------------------------------------
// Class Logger

//! Logging class
/*!
   A simple logging class. The written information to the logger is sent
   to the output handlers of the logger and is formatted by the specified
   Formatter class.
 */
class API_EXPORT Logger
{
public:
   typedef std::vector<std::unique_ptr<OutputHandler>> OutputHandlers;

public:
   virtual ~Logger();

   //! Get the logging level
   Level level() const;

   //! Set the logging level
   void level(Level level);

   //! Indicates if the level is enabled for logging
   bool is_enabled(Level level) const;

   //! Sets an output handler for the logger
   OutputHandlers& output_handlers();

   //! Writes an Info message
   void info(const std::string& msg);

   //! Writes a Warning message
   void warning(const std::string& msg);

   //! Writes an Error message
   void error(const std::string& msg);

   //! Writes a Debug message
   void debug(const std::string& msg);

   //! Writes an Exception
   void exception(const std::exception& except);

   //! Writes a message to the logger
   void write(const std::string& msg);

   //! Writes a message the with a specified logging level
   void write(Level level, const std::string& msg);

   //! Writes a message the with a specified logging level
   void write(Level level, const std::string& msg, const std::string& file, int line, const std::string& function);

   //! Writes an exception
   void write(const std::exception& except, const std::string& file, int line, const std::string& function);

   //! Starts the logging
   void start(std::function<std::string(void)> system_info = DefaultInfo);

   //! Shuts down the logger
   void shutdown();

   //! Stop logging
   void stop();

   //! Resume logging
   void resume();

   void push_scope();
   void pop_scope();
   uint32_t scope_depth() const;

   //! Retreive the logger instance
   static Logger& get_logger();

   //! Log a character
   Logger& operator+=(LogRecord log_record);

protected:
   Logger();

private:
   struct Private;
   const std::unique_ptr<Private> _impl;

};

API_EXPORT std::string level_as_text(Level level);

} // namespace logging
} // namespace orion
#endif // ORION_LOGGING_LOGGER_H
