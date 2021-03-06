//
// LoggerImpl.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_LOG_LOGGERIMPL_H
#define ORION_LOG_LOGGERIMPL_H

#include <orion/Common.h>

#include <orion/log/OutputHandler.h>
#include <orion/log/Record.h>

#include <functional>
#include <memory>
#include <string>

namespace orion
{
namespace log
{
using SystemInfoFunc = std::function<std::string(void)>;

/// Service implementation for the logger.
class API_EXPORT LoggerImpl : public std::enable_shared_from_this<LoggerImpl>
{
public:
   NO_COPY(LoggerImpl)
   NO_MOVE(LoggerImpl)

   explicit LoggerImpl(Level level);

   ~LoggerImpl() = default;

   /// Get the logging level
   Level level() const;

   /// Set the logging level
   void level(Level value);

   bool is_enabled(Level level) const;

   /// Indicates if the service is suspended
   bool is_running() const;

   void is_running(bool value);

   void add_output_handler(std::unique_ptr<OutputHandler>&& hdl);

   /// Write a record to the output handlers
   void write(const Record& record);

   /// Starts the logging
   void start(const SystemInfoFunc& system_info);

   /// Shuts down the logger
   void shutdown();

   /// Suspend logging
   void suspend();

   /// Resume logging
   void resume();

   void push_scope();
   void pop_scope();

   uint32_t scope_depth() const;

private:
   OutputHandlers _output_handlers;

   Level _level;
   bool _is_running;
   uint32_t _scope_depth;
};

} // namespace log
} // namespace orion

#endif /* ORION_LOG_LoggerImpl_H */