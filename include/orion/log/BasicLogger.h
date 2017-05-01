//
// BasicLogger.h
//
// Created by Tomas Palazuelos on 2017-03-17.
// Copyright © 2017 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_LOG_BASICLOGGER_H
#define ORION_LOG_BASICLOGGER_H

#include <functional>

#include <orion/Orion-Stddefs.h>

#include <orion/Exception.h>
#include <orion/log/Level.h>
#include <orion/log/OutputHandler.h>

#include <asio.hpp>

namespace orion
{
namespace log
{
using SystemInfoFunc = std::function<std::string(void)>;

//---------------------------------------------------------------------------------------
// forward declares
class Formatter;
class Record;

//---------------------------------------------------------------------------------------
// Class BasicLogger

/// Basic Logging class
///
/// A simple logging class. The written information to the logger is sent
/// to the output handlers of the logger and is formatted by the specified
/// Formatter class.
///
template <typename Service>
class BasicLogger : public asio::basic_io_object<Service>
{
public:
   explicit BasicLogger(asio::io_service& io_service) :
      asio::basic_io_object<Service>(io_service) {}
   
   ~BasicLogger() = default;

   /// Get the logging level
   Level level() const;

   /// Set the logging level
   void level(Level value);

   /// Indicates if the level is enabled for logging
   bool is_enabled(Level level) const;

   void add_output_handler(std::unique_ptr<OutputHandler>&& hdl);

   /// Writes an Info message
   template<typename... Args>
   void info(Args... args);

   template<typename... Args>
   void message(Args... args);

   /// Writes a Warning message
   template<typename... Args>
   void warning(Args... args);

   /// Writes an Error message
   template<typename... Args>
   void error(Args... args);

   /// Writes a Debug message
   template<typename... Args>
   void debug(Args... args);

   /// Writes an Exception
   template<typename... Args>
   void exception(const std::exception& e, Args... args);

   /// Writes an Exception
   template<typename... Args>
   void exception(const orion::Exception& e, Args... args);

   /// Writes a message the with a specified logging level
   template<typename... Args>
   void write(Level level, Args... args);

   /// Writes a log record with a specified logging level
   void write(const Record& record);

   /// Starts the logging
   void start(SystemInfoFunc system_info);

   /// Shuts down the logger
   void shutdown();

   /// Suspend logging
   void suspend();

   /// Resume logging
   void resume();

   void push_scope();
   void pop_scope();

   uint32_t scope_depth() const;

   /// Log a character
   BasicLogger& operator+=(const Record& record);
};

} // namespace log
} // namespace orion

#include <orion/log/impl/BasicLogger.ipp>

#endif // ORION_LOG_BASICLOGGER_H
