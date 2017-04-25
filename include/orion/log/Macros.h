// Macros.h
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

#ifndef ORION_LOG_MACROS_H
#define ORION_LOG_MACROS_H

#ifndef LOG
#define LOG(lvl) \
if (orion::log::Logger::get_logger().is_enabled(orion::log::Level::lvl)) \
   orion::log::Logger::get_logger() += orion::log::Record(orion::log::Level::lvl, "", _src_loc)
#endif

#ifndef LOG_FUNCTION
#define LOG_FUNCTION(lvl, func_name) \
orion::log::Function log_func(orion::log::Level::lvl, orion::SourceLocation{__FILE__, __LINE__, func_name});
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

#endif /* ORION_LOG_MACROS_H */
