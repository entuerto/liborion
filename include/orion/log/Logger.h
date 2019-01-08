//
// Logger.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_LOG_LOGGER_H
#define ORION_LOG_LOGGER_H

#include <orion/Orion-Stddefs.h>

#include <orion/Exception.h>
#include <orion/log/BasicLogger.h>
#include <orion/log/Level.h>
#include <orion/log/LoggerImpl.h>
#include <orion/log/LoggerService.h>

#include <functional>
#include <iosfwd>

namespace orion
{
namespace log
{

API_EXPORT std::string DefaultInfo();

//---------------------------------------------------------------------------------------
// Logger
using Logger = BasicLogger<LoggerService<LoggerImpl>>;

//---------------------------------------------------------------------------------------

template<typename... Args>
void info(Args&&... args);

template<typename... Args>
void message(Args&&... args);

template<typename... Args>
void warning(Args&&... args);

template<typename... Args>
void error(Args&&... args);

template<typename... Args>
void error_if(bool expr, Args&&... args);

template<typename... Args>
void error_if(const std::error_code& ec, Args&&... args);

template<typename... Args>
void debug(Args&&... args);

template<typename... Args>
void debug2(Args&&... args);

template<typename... Args>
void debug3(Args&&... args);

template<typename... Args>
void exception(const std::exception& e, Args&&... args);

template<typename... Args>
void exception(const orion::Exception& e, Args&&... args);

template<typename... Args>
[[noreturn]] void fatal(Args&&... args);

template<typename... Args>
void write(Level level, Args&&... args);

template<typename... Args>
void write(Args&&... args);

/// Retreive the logger instance
API_EXPORT Logger& default_logger();

API_EXPORT void setup_logger(Level level = Level::Info);

API_EXPORT void setup_logger(Level level, std::ostream& stream);

API_EXPORT void start(SystemInfoFunc system_info = DefaultInfo);

API_EXPORT void shutdown();

} // namespace log
} // namespace orion

#include <orion/log/impl/Logger.ipp>

#endif // ORION_LOG_LOGGER_H
