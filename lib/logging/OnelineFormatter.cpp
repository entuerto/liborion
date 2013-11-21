// OnelineFormatter.cpp
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

#include <orion/logging/OnelineFormatter.h>

#include <sstream> 

#include <orion/logging/Logger.h>
#include <orion/logging/LogRecord.h>
#include <orion/logging/LogExceptionRecord.h>

namespace orion
{
namespace logging
{
namespace 
{
static std::ostringstream stream;
}
//--------------------------------------------------------------------------
// Class OnelineFormatter

OnelineFormatter::OnelineFormatter()
{
}

OnelineFormatter::~OnelineFormatter()
{
}

std::string OnelineFormatter::format(const LogRecord& log_record)
{
   if (log_record.level() == Logger::NotSet)
      return log_record.message();

   stream.clear();
   stream.str("");

   stream << "|" 
          << level_as_text(log_record.level())
          << "|"
          << log_record.time_stamp()
          << "|"
          << log_record.message();

   return stream.str();
}

IFormatter::SharedPtr OnelineFormatter::create()
{
   return IFormatter::SharedPtr(new OnelineFormatter);
}

} // namespace logging
} // namespace orion
