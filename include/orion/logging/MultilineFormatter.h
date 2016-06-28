// MultilineFormatter.h
//
// Copyright 2009 tomas <tomasp@videotron.ca>
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

#ifndef ORION_LOGGING_MULTILINEFORMATTER_H
#define ORION_LOGGING_MULTILINEFORMATTER_H

#include <cstdint>
#include <string>

#include <orion/Orion-Stddefs.h>
#include <orion/logging/IFormatter.h>

namespace orion
{
namespace logging
{
class LogRecord;

//! Multi line formatter
/*!
    Formats the information on more than one line.
 */
class API_EXPORT MultilineFormatter : public IFormatter
{
public:
   NO_COPY(MultilineFormatter)
   NO_MOVE(MultilineFormatter)
   
   MultilineFormatter();
   virtual ~MultilineFormatter();

   virtual std::string format(const LogRecord& log_record);

   static IFormatter::SharedPtr create();

private:
   std::string format_source_info(const std::string& prefix,
                                  const std::string& file,
                                             int32_t line,
                                  const std::string& function);

};

} // namespace logging
} // namespace orion
#endif /* ORION_LOGGING_MULTILINEFORMATTER_H */
