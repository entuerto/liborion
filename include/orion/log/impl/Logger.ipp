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

namespace detail
{
struct StringConcat
{
   std::string& text;

   template<typename T>
   void operator()(T) {  }

   void operator()(const char* v)        { text += v; }
   void operator()(const std::string& v) { text += v; }
};
}

template<typename... Args>
void Logger::info(Args... args)
{
   write(Level::Info, args...);
}

template<typename... Args>
void Logger::message(Args... args)
{
   write(Level::Message, args...);
}

template<typename... Args>
void Logger::warning(Args... args)
{
   write(Level::Warning, args...);
}

template<typename... Args>
void Logger::error(Args... args)
{
   write(Level::Error, args...);
}

template<typename... Args>
void Logger::debug(Args... args)
{
   write(Level::Debug, args...);
}

template<typename... Args>
void Logger::exception(const std::exception& e, Args... args)
{
   auto t = std::make_tuple(args...);

   std::string msg = e.what(); 
   
   get_all_values(detail::StringConcat{msg}, t);

   auto sl = get_value<SourceLocation>(t, SourceLocation{});

   write(ExceptionRecord{msg, SourceLocation{}, sl});
}

template<typename... Args>
void Logger::exception(const orion::Exception& e, Args... args)
{
   auto t = std::make_tuple(args...);

   std::string msg = e.what(); 
   
   get_all_values(detail::StringConcat{msg}, t);

   auto sl = get_value<SourceLocation>(t, SourceLocation{});

   write(ExceptionRecord{msg, e.source_location(), sl});
}

template<typename... Args>
void Logger::write(Level level, Args... args)
{
   auto t = std::make_tuple(args...);

   std::string msg;

   get_all_values(detail::StringConcat{msg}, t);

   auto sl = get_value<SourceLocation>(t, SourceLocation{});

   write(Record{level, msg, sl});
}

//---------------------------------------------------------------------------------------

API_EXPORT inline void start(SystemInfoFunc system_info = DefaultInfo)
{
   Logger::get_logger().start(system_info);
}

API_EXPORT inline void shutdown()
{
   Logger::get_logger().shutdown();
}

template<typename... Args>
void info(Args... args)
{
   Logger::get_logger().write(Level::Info, args...);
}

template<typename... Args>
void message(Args... args)
{
   Logger::get_logger().write(Level::Message, args...);
}

template<typename... Args>
void warning(Args... args)
{
   Logger::get_logger().write(Level::Warning, args...);
}

template<typename... Args>
void error(Args... args)
{
   Logger::get_logger().write(Level::Error, args...);
}

template<typename... Args>
void debug(Args... args)
{
   Logger::get_logger().write(Level::Debug, args...);
}

template<typename... Args>
void debug2(Args... args)
{
   Logger::get_logger().write(Level::Debug2, args...);
}

template<typename... Args>
void debug3(Args... args)
{
   Logger::get_logger().write(Level::Debug3, args...);
}

template<typename... Args>
void exception(const std::exception& e, Args... args)
{
   Logger::get_logger().exception(e, args...);
}

template<typename... Args>
void exception(const orion::Exception& e, Args... args)
{
   Logger::get_logger().exception(e, args...);
}

template<typename... Args>
void write(Level level, Args... args)
{
   Logger::get_logger().write(level, args...);
}

template<typename... Args>
void write(Args... args)
{
   Logger::get_logger().write(Level::NotSet, args...);
}

} // namespace log
} // namespace orion

#endif // ORION_LOG_LOGGER_IPP
