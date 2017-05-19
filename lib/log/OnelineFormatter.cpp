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

#include <orion/log/OnelineFormatter.h>

#include <orion/log/Logger.h>
#include <orion/log/Record.h>

#include <sstream>

namespace orion
{
namespace log
{

//--------------------------------------------------------------------------
// Class OnelineFormatter

OnelineFormatter::OnelineFormatter()
{
}

OnelineFormatter::~OnelineFormatter()
{
}

std::string OnelineFormatter::format(const Record& record)
{
   if (record.level() == Level::NotSet)
      return record.message();

   std::string scope;

   auto& l = default_logger();

   for (uint32_t i = 0; i < l.scope_depth(); i++)
   {
      scope += " |";
   }

   std::ostringstream stream;

   stream << "|" << to_string(record.level()) << "|" << record.time_stamp() << "|" << scope
          << record.message();

   return stream.str();
}

} // namespace log
} // namespace orion
