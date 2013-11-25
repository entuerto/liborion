// LogMacros.h
//
// Copyright 2013 tomas <tomasp@videotron.ca>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.
//

#ifndef ORION_LOGGING_LOGMACROS_H
#define ORION_LOGGING_LOGMACROS_H

#ifndef LOG
#define LOG(lvl) \
if (orion::logging::Logger::get_logger().level() <= orion::logging::Logger::lvl) \
   orion::logging::Logger::get_logger() += LogRecord(orion::logging::Logger::lvl, __FILE__, __LINE__, __FUNCTION__)
#endif

#ifndef LOG_EXCEPTION
#define LOG_EXCEPTION(except) \
orion::logging::Logger::get_logger() += LogExceptionRecord(except, __FILE__, __LINE__, __FUNCTION__)
#endif

#ifndef LOG_FUNCTION
#define LOG_FUNCTION(lvl, func) \
if (orion::logging::Logger::get_logger().level() <= orion::logging::Logger::lvl) \
   orion::logging::LogFunction(orion::logging::Logger::lvl, func, __FILE__, __LINE__);
#endif

#ifndef LOG_IF_FAIL
#define LOG_IF_FAIL(lvl, expr) \
{                                       \
   if (expr) { } else                   \
   {	                                \
      LOG(lvl) << "Condition failed ( " \
               << #expr                 \
               << " )";                 \
   }				        \
}
#endif

#ifndef LOG_WRITE
#define LOG_WRITE() \
orion::logging::Logger::get_logger() += LogRecord()
#endif

#ifndef LOG_START
#define LOG_START(func) \
orion::logging::Logger::get_logger().start(func);
#endif

#ifndef LOG_END
#define LOG_END() \
orion::logging::Logger::get_logger().shutdown();
#endif

#endif /* ORION_LOGGING_LOGMACROS_H */
