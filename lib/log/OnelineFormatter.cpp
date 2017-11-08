//
// OnelineFormatter.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
// 
#include <orion/log/OnelineFormatter.h>

#include <orion/Chrono.h>
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

   stream << "|" << to_string(record.level()) << "|" << orion::to_string(record.time_stamp(), "%X") << "|" << scope
          << record.message();

   return stream.str();
}

} // namespace log
} // namespace orion
