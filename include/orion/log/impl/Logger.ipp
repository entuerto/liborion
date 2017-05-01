//
// Logger.ipp
//
// Created by Tomas Palazuelos on 2017-03-17.
// Copyright © 2017 Tomas Palazuelos. All rights reserved.
//
#ifndef ORION_LOG_LOGGER_IPP
#define ORION_LOG_LOGGER_IPP

#include <orion/Orion-Stddefs.h>

#include <orion/Utils.h>
#include <orion/log/Record.h>
#include <orion/log/ExceptionRecord.h>

namespace orion
{
namespace log
{

//---------------------------------------------------------------------------------------

template<typename... Args>
void info(Args... args)
{
   default_logger().write(Level::Info, args...);
}

template<typename... Args>
void message(Args... args)
{
   default_logger().write(Level::Message, args...);
}

template<typename... Args>
void warning(Args... args)
{
   default_logger().write(Level::Warning, args...);
}

template<typename... Args>
void error(Args... args)
{
   default_logger().write(Level::Error, args...);
}

template<typename... Args>
void debug(Args... args)
{
   default_logger().write(Level::Debug, args...);
}

template<typename... Args>
void debug2(Args... args)
{
   default_logger().write(Level::Debug2, args...);
}

template<typename... Args>
void debug3(Args... args)
{
   default_logger().write(Level::Debug3, args...);
}

template<typename... Args>
void exception(const std::exception& e, Args... args)
{
   default_logger().exception(e, args...);
}

template<typename... Args>
void exception(const orion::Exception& e, Args... args)
{
   default_logger().exception(e, args...);
}

template<typename... Args>
void write(Level level, Args... args)
{
   default_logger().write(level, args...);
}

template<typename... Args>
void write(Args... args)
{
   default_logger().write(Level::NotSet, args...);
}

} // namespace log
} // namespace orion

#endif // ORION_LOG_LOGGER_IPP
