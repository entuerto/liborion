//
// OnelineWithSourceInfoFormatter.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
// 
#include <orion/log/OnelineWithSourceInfoFormatter.h>

#include <orion/Chrono.h>
#include <orion/SourceLocation.h>
#include <orion/log/Logger.h>
#include <orion/log/Record.h>

#include <sstream>

namespace orion
{
namespace log
{

//--------------------------------------------------------------------------
// Class OnelineWithSourceInfoFormatter

OnelineWithSourceInfoFormatter::OnelineWithSourceInfoFormatter()
{
}

OnelineWithSourceInfoFormatter::~OnelineWithSourceInfoFormatter()
{
}

std::string OnelineWithSourceInfoFormatter::format(const Record& record)
{
   if (record.level() == Level::NotSet)
      return record.message();

   std::string scope;

   for (uint32_t i = 0; i < default_logger().scope_depth(); i++)
   {
      scope += " |";
   }

   std::ostringstream stream;

   stream << "|" << to_string(record.level()) << "|" << orion::to_string(record.time_stamp(), "%X") << "|" << scope
          << record.message();

   if (record.level() == Level::Exception)
   {
      try
      {
         auto& except_record = dynamic_cast<const ExceptionRecord&>(record);

         stream << "| Thrown from: " << except_record.thrown_source_location()
                << "| Caught at: " << except_record.caught_source_location();

         return stream.str();
      }
      catch (const std::bad_cast&)
      {
         // Continue with the record values
      }
   }

   auto sl = record.source_location();

   if (sl.line_number != 0)
      stream << "|" << sl;

   return stream.str();
}

} // namespace log
} // namespace orion
