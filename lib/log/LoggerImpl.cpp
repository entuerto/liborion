// LoggerImpl.cpp
//
// Copyright 2017 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <orion/log/LoggerImpl.h>

#include <functional>
#include <iostream>

#include <log/EndRecord.h>
#include <log/StartRecord.h>
#include <log/SystemInfoRecord.h>

namespace orion
{
namespace log
{

LoggerImpl::LoggerImpl(Level level) : 
   _output_handlers(),
   _level(level), 
   _is_running(false), 
   _scope_depth(0)
{
   std::cout << "Constructor: LoggerImpl::LoggerImpl()\n";
}
   
LoggerImpl::~LoggerImpl()
{
   std::cout << "Destructor: LoggerImpl::~LoggerImpl()\n";
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
void LoggerImpl::start(SystemInfoFunc system_info)
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
