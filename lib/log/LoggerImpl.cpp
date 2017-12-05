//
// LoggerImpl.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
// 
#include <orion/log/LoggerImpl.h>

#include <log/EndRecord.h>
#include <log/StartRecord.h>
#include <log/SystemInfoRecord.h>

#include <functional>

namespace orion
{
namespace log
{

LoggerImpl::LoggerImpl(Level level)
   : _level(level)
   , _is_running(false)
   , _scope_depth(0)
{
}

/// Get the logging level
Level LoggerImpl::level() const
{
   return _level;
}

/// Set the logging level
void LoggerImpl::level(Level value)
{
   _level = value;
}

bool LoggerImpl::is_enabled(Level level) const
{
   if (not _is_running)
      return false;

   return level >= _level;
}

bool LoggerImpl::is_running() const
{
   return _is_running;
}

void LoggerImpl::is_running(bool value)
{
   _is_running = value;
}

void LoggerImpl::add_output_handler(std::unique_ptr<OutputHandler>&& hdl)
{
   _output_handlers.push_back(std::move(hdl));
}

void LoggerImpl::write(const Record& record)
{
   // Pass the work of writing to the background thread.
   for (auto& output : _output_handlers)
   {
      output->write(record);
   }
}

/// Starts the logging
void LoggerImpl::start(const SystemInfoFunc& system_info)
{
   resume();
   write(StartRecord());
   write(SystemInfoRecord(system_info()));
}

/// Shuts down the logger
void LoggerImpl::shutdown()
{
   write(EndRecord());
   suspend();
}

/// Suspend logging
void LoggerImpl::suspend()
{
   _is_running = false;
}

/// Resume logging
void LoggerImpl::resume()
{
   _is_running = true;
}

void LoggerImpl::push_scope()
{
   _scope_depth++;
}

/*!
 */
void LoggerImpl::pop_scope()
{
   if (_scope_depth == 0)
      return;

   _scope_depth--;
}

/*!
 */
uint32_t LoggerImpl::scope_depth() const
{
   return _scope_depth;
}

} // namespace log
} // namespace orion
