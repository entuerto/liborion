//
// Macros.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_LOG_MACROS_H
#define ORION_LOG_MACROS_H

#ifndef LOG
#define LOG(lvl)                                                    \
   \
if(orion::log::default_logger().is_enabled(orion::log::Level::lvl)) \
      orion::log::default_logger() += orion::log::Record(orion::log::Level::lvl, "", _src_loc)
#endif

#ifndef LOG_FUNCTION
#define LOG_FUNCTION(lvl, func_name)                  \
   \
orion::log::Function log_func(orion::log::Level::lvl, \
                              orion::SourceLocation{__FILE__, __LINE__, func_name});
#endif

#ifndef LOG_IF_FAIL
#define LOG_IF_FAIL(lvl, expr)                               \
   \
{                                                      \
      if (expr)                                              \
      {                                                      \
      }                                                      \
      else                                                   \
      {                                                      \
         LOG(lvl) << "Condition failed ( " << #expr << " )"; \
      }                                                      \
   \
}
#endif

#endif /* ORION_LOG_MACROS_H */
