//
// Logger.h
//
// Created by Tomas Palazuelos on 2017-03-17.
// Copyright © 2017 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_LOG_LOGGER_H
#define ORION_LOG_LOGGER_H

#include <functional>
#include <vector>

#include <orion/Orion-Stddefs.h>
#include <orion/log/Level.h>
#include <orion/log/OutputHandler.h>

namespace orion
{
namespace log
{

using SystemInfoFunc = std::function<std::string(void)>;

API_EXPORT std::string DefaultInfo();

//---------------------------------------------------------------------------------------
// forward declares
class Formatter;
class Record;

//---------------------------------------------------------------------------------------
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

   //! Gets the output handlers for the logger
   const OutputHandlers& output_handlers() const;

   void add_output_handler(std::unique_ptr<OutputHandler>&& hdl);

   //! Writes an Info message
   template<typename... Args>
   void info(Args... args);

   template<typename... Args>
   void message(Args... args);

   //! Writes a Warning message
   template<typename... Args>
   void warning(Args... args);

   //! Writes an Error message
   template<typename... Args>
   void error(Args... args);

   //! Writes a Debug message
   template<typename... Args>
   void debug(Args... args);

   //! Writes an Exception
   template<typename... Args>
   void exception(const std::exception& e, Args... args);

   //! Writes a message to the logger
   template<typename... Args>
   void write(Args... args);

   //! Writes a message the with a specified logging level
   template<typename... Args>
   void write(Level level, Args... args);

   //! Writes a log record with a specified logging level
   void write(const Record& record);

   //! Starts the logging
   void start(SystemInfoFunc system_info = DefaultInfo);

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
   Logger& operator+=(const Record& record);

protected:
   Logger();

private:
   struct Private;
   const std::unique_ptr<Private> _impl;

};


//---------------------------------------------------------------------------------------

template<typename... Args>
void info(Args... args);

template<typename... Args>
void message(Args... args);

template<typename... Args>
void warning(Args... args);

template<typename... Args>
void error(Args... args);

template<typename... Args>
void debug(Args... args);

template<typename... Args>
void debug2(Args... args);

template<typename... Args>
void debug3(Args... args);

template<typename... Args>
void exception(const std::exception& e, Args... args);

template<typename... Args>
void write(Level level, Args... args);

template<typename... Args>
void write(Args... args);

} // namespace log
} // namespace orion

#include <orion/log/impl/Logger.ipp>

#endif // ORION_LOG_LOGGER_H
