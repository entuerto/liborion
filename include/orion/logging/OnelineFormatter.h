// OnelineFormatter.h
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

#ifndef ORION_LOGGING_ONELINEFORMATTER_H
#define ORION_LOGGING_ONELINEFORMATTER_H

#include <string>

#include <orion/NonCopyable.h>
#include <orion/logging/IFormatter.h>

namespace orion
{
namespace logging
{
class LogRecord;

//! One line log formatter
/*!
    Formats the log record on a single line.
 */
class OnelineFormatter :
   public IFormatter,
   NonCopyable
{
public:
   OnelineFormatter();
   virtual ~OnelineFormatter();

   virtual std::string format(const LogRecord& log_record);

   static IFormatter::SharedPtr create();

};

} // namespace logging
} // namespace orion
#endif /* ORION_LOGGING_ONELINEFORMATTER_H */
