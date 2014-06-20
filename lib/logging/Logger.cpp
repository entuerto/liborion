/*
 * Logger.cpp
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

#include <orion/logging/Logger.h>

#include <iostream>
#include <sstream>

#include <orion/SystemInfo.h>
#include <orion/logging/LogRecord.h>
#include <orion/logging/LogExceptionRecord.h>
#include <logging/LogEndRecord.h>
#include <logging/LogStartRecord.h>
#include <logging/LogSystemInfoRecord.h>

namespace orion
{
namespace logging
{
#if WIN32
const std::string consolePath = "CON";
#else
const std::string consolePath = "/dev/tty";
#endif


//--------------------------------------------------------------------------
// Private Impl structure
struct Logger::Private
{
   Private() :
      output_handlers(), level(Logger::Warning), is_running(false), scope_depth(0)
      {}

   void write(const LogRecord& log_record);

   OutputHandlers output_handlers;
   Logger::Level level;
   bool is_running;
   uint32_t scope_depth;
};

void Logger::Private::write(const LogRecord& log_record)
{
   if (not is_running)
      return;

   OutputHandlers::iterator iter = output_handlers.begin();
   OutputHandlers::iterator end  = output_handlers.end();

   for (; iter != end; ++iter) {
      (*iter)->write(log_record);
   }
}

//--------------------------------------------------------------------------
// Logger

Logger::Logger() :
   _impl(new Private)
{
}

Logger::~Logger()
{
}

/*!
   \return the current level of the logger
 */
Logger::Level Logger::level() const
{
   return _impl->level;
}

/*!
   \param level level to set
 */
void Logger::level(Logger::Level level)
{
   _impl->level = level;
}

/*! 
   Indicates if the level is enabled for logging
*/
bool Logger::is_enabled(Level level) const
{
   if (not _impl->is_running)
      return false;

   return level >= _impl->level;
}

/*!
   \param handler the output handler to set
 */
Logger::OutputHandlers& Logger::output_handlers()
{
   return _impl->output_handlers;
}

/*!
   \param msg message to write
 */
void Logger::info(const std::string& msg)
{
   if (Info <= _impl->level)
      return;
   _impl->write(LogRecord(Info, msg));
}

/*!
   \param msg message to write
 */
void Logger::warning(const std::string& msg)
{
   if (Warning <= _impl->level)
      return;
   _impl->write(LogRecord(Warning, msg));
}

/*!
   \param msg message to write
 */
void Logger::error(const std::string& msg)
{
   if (Error <= _impl->level)
      return;
   _impl->write(LogRecord(Error, msg));
}

/*!
   \param msg message to write
 */
void Logger::debug(const std::string& msg)
{
   if (Debug <= _impl->level)
      return;
   _impl->write(LogRecord(Debug, msg));
}

/*!
   \param except exception to write
 */
void Logger::exception(const std::exception& except)
{
   if (Exception <= _impl->level)
      return;
   _impl->write(LogExceptionRecord(except));
}

/*!
   \param msg message to write
 */
void Logger::write(const std::string& msg)
{
   _impl->write(LogRecord(Logger::NotSet, msg));
}

/*!
   \param level level of the log message
   \param msg message to write
 */
void Logger::write(Logger::Level level, const std::string& msg)
{
   if (level <= _impl->level)
      return;
   _impl->write(LogRecord(level, msg));
}

/*!
   \param level level of the log message
   \param msg message to write
   \param file file name from where the function was called
   \param line line number from where the function was called
   \param function function name from where the function was called
 */
void Logger::write(Logger::Level level, const std::string& msg, const std::string& file, int line, const std::string& function)
{
   if (level <= _impl->level)
      return;
   _impl->write(LogRecord(level, msg, file, line, function));
}

/*!
   \param except exception to write
   \param msg message to write
   \param file file name from where the function was called
   \param line line number from where the function was called
   \param function function name from where the function was called
 */
void Logger::write(const std::exception& except, const std::string& file, int line, const std::string& function)
{
   if (Exception <= _impl->level)
      return;
   _impl->write(LogExceptionRecord(except, file, line, function));
}

/*!
   Starts up the logging process. A function can be passed to
   incorporate system information into the output of the
   logger.

   Example:
   \code
   logger.start(sigc::mem_fun(someobj, & SomeClass::somemethod));
   or
   logger.start(sigc::ptr_fun(&somefunction));
   \endcode

   \param system_info function that returns a string to output.
 */
void Logger::start(std::function<std::string(void)> system_info)
{
   _impl->is_running = true;
   _impl->write(LogStartRecord());
   _impl->write(LogSystemInfoRecord(system_info()));
}

/*!
   Perform cleanup actions in the logging system (e.g. flushing
    buffers, writting end record.).
 */
void Logger::shutdown()
{
   _impl->write(LogEndRecord());
   _impl->is_running = false;
}

/*! 
   Stop logging
*/
void Logger::stop()
{
   _impl->is_running = false;
}

/*! 
   Resume logging
*/
void Logger::resume()
{
   _impl->is_running = true;
}

/*!
 */
void Logger::push_scope()
{
   _impl->scope_depth++;
}

/*!
 */
void Logger::pop_scope()
{
   if (_impl->scope_depth == 0)
      return;

   _impl->scope_depth--;
}

/*!
 */
uint32_t Logger::scope_depth() const
{
   return _impl->scope_depth;
}

/*!
   Return the default logger.
 */
Logger& Logger::get_logger()
{
   static Logger logger;

   return logger;
}

/*!
   Log a character
 */
Logger& Logger::operator+=(LogRecord log_record)
{
   _impl->write(log_record);
   return *this;
}

//--------------------------------------------------------------------------
//
std::string level_as_text(Logger::Level level)
{
   switch (level)
   {
      case Logger::Debug3:
         return "Debug3    ";
      case Logger::Debug2:
         return "Debug2    ";
      case Logger::Debug:
         return "Debug     ";
      case Logger::Info:
         return "Info      ";
      case Logger::Message:
         return "Message   ";
      case Logger::Warning:
         return "Warning   ";
      case Logger::Error:
         return "Error     ";
      case Logger::Exception:
         return "Exception ";
      default:
         break;
   }
   return "";
}

std::string DefaultInfo()
{
   std::ostringstream stream_info;

   stream_info << "System Information\n"
               << "  Name:       " << systeminfo::get_user_name() << "\n"
               << "  Host name:  " << systeminfo::get_host_name() << "\n"
               << "  Process ID: " << systeminfo::get_process_id() << "\n"
               << "  Sytem:      " << systeminfo::get_os_version() << "\n"
               << "  CPU:        " << systeminfo::get_cpu_model() << "\n"
               << "\n"
               << "Program Information\n"
               << "  " << systeminfo::get_program_name() << "\n";
               

   return stream_info.str();
}

} // namespace logging
} // namespace orion
