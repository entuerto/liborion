//
// Logger.cpp
//
// Created by Tomas Palazuelos on 2017-03-17.
// Copyright © 2017 Tomas Palazuelos. All rights reserved.
//

#include <orion/log/Logger.h>

#include <sstream>

#include <orion/SystemInfo.h>
#include <orion/log/Record.h>

#include <log/EndRecord.h>
#include <log/StartRecord.h>
#include <log/SystemInfoRecord.h>

namespace orion
{
namespace log
{

//--------------------------------------------------------------------------
// Private Impl structure
struct Logger::Private
{
   Private() :
      output_handlers(), level(Level::Warning), is_running(false), scope_depth(0)
      {}

   void write(const Record& record);

   OutputHandlers output_handlers;
   Level level;
   bool is_running;
   uint32_t scope_depth;
};

void Logger::Private::write(const Record& record)
{
   if (not is_running)
      return;

   for (auto& output : output_handlers)
   {
      output->write(record);
   }
}

//--------------------------------------------------------------------------
// Logger

Logger::Logger() :
   _impl(std::make_unique<Private>())
{
}

Logger::~Logger()
{
}

/*!
   \return the current level of the logger
 */
Level Logger::level() const
{
   return _impl->level;
}

/*!
   \param level level to set
 */
void Logger::level(Level level)
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
const Logger::OutputHandlers& Logger::output_handlers() const
{
   return _impl->output_handlers;
}

void Logger::add_output_handler(std::unique_ptr<OutputHandler>&& hdl)
{
   _impl->output_handlers.push_back(std::move(hdl));
}

/*!
   \param level level of the log message
   \param log_record log record to write
 */
void Logger::write(const Record& record)
{
   if (record.level() <= _impl->level)
      return;
   _impl->write(record);
}

/*!
   Starts up the logging process. A function can be passed to
   incorporate system information into the output of the
   logger.

   \param system_info function that returns a string to output.
 */
void Logger::start(SystemInfoFunc system_info)
{
   _impl->is_running = true;
   _impl->write(StartRecord());
   _impl->write(SystemInfoRecord(system_info()));
}

/*!
   Perform cleanup actions in the logging system (e.g. flushing
    buffers, writting end record.).
 */
void Logger::shutdown()
{
   _impl->write(EndRecord());
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
Logger& Logger::operator+=(const Record& record)
{
   _impl->write(record);
   return *this;
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

} // namespace log
} // namespace orion
