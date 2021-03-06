//
// Logger.ipp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_LOG_LOGGER_IPP
#define ORION_LOG_LOGGER_IPP

#include <orion/Utils.h>
#include <orion/log/ExceptionRecord.h>
#include <orion/log/Record.h>

namespace orion
{
namespace log
{

//---------------------------------------------------------------------------------------

template<typename... Args>
void info(Args&&... args)
{
   default_logger().write(Level::Info, args...);
}

template<typename... Args>
void message(Args&&... args)
{
   default_logger().write(Level::Message, args...);
}

template<typename... Args>
void warning(Args&&... args)
{
   default_logger().write(Level::Warning, args...);
}

template<typename... Args>
void error(Args&&... args)
{
   default_logger().write(Level::Error, args...);
}

template<typename... Args>
void error_if(bool expr, Args&&... args)
{
   if (expr)
      default_logger().write(Level::Error, args...);
}

template<typename... Args>
void error_if(const std::error_code& ec, Args&&... args)
{
   if (ec)
      default_logger().write(Level::Error, args...);
}

template<typename... Args>
void debug(Args&&... args)
{
   default_logger().write(Level::Debug, args...);
}

template<typename... Args>
void debug2(Args&&... args)
{
   default_logger().write(Level::Debug2, args...);
}

template<typename... Args>
void debug3(Args&&... args)
{
   default_logger().write(Level::Debug3, args...);
}

template<typename... Args>
void exception(const std::exception& e, Args&&... args)
{
   default_logger().exception(e, args...);
}

template<typename... Args>
void exception(const orion::Exception& e, Args&&... args)
{
   default_logger().exception(e, args...);
}

template<typename... Args>
void fatal(Args&&... args)
{
   default_logger().fatal(args...);
}

template<typename... Args>
void write(Level level, Args&&... args)
{
   default_logger().write(level, args...);
}

template<typename... Args>
void write(Args&&... args)
{
   default_logger().write(Level::NotSet, args...);
}

} // namespace log
} // namespace orion

#endif // ORION_LOG_LOGGER_IPP
