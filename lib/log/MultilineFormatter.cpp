// MultilineFormatter.cpp
//
// Copyright 2013 tomas <tomasp@videotron.ca>
//
// Copyright 2016 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <orion/log/MultilineFormatter.h>

#include <orion/Chrono.h>
#include <orion/log/Logger.h>
#include <orion/log/Record.h>
#include <sstream>

namespace orion
{
namespace log
{

//--------------------------------------------------------------------------
// Class MultilineFormatter

MultilineFormatter::MultilineFormatter()
{
}

MultilineFormatter::~MultilineFormatter()
{
}

std::string MultilineFormatter::format(const Record& record)
{
   if (record.level() == Level::NotSet)
      return record.message();

   std::string scope;

   auto& l = default_logger();

   for (uint32_t i = 0; i < l.scope_depth(); i++)
   {
      scope += "++";
   }
   if (not scope.empty())
      scope = scope + " ";

   std::ostringstream stream;

   stream << "|" << to_string(record.level()) << "|" << orion::to_string(record.time_stamp(), "%X") << "\n\t- " << scope
          << record.message() << "\n";

   if (record.level() == Level::Exception)
   {
      try
      {
         auto& except_record = dynamic_cast<const ExceptionRecord&>(record);

         stream << format_source_info("\t- Thrown from: ", except_record.thrown_source_location())
                << format_source_info("\t- Caught at:   ", except_record.caught_source_location());

         return stream.str();
      }
      catch (const std::bad_cast&)
      {
         // Continue with the record values
      }
   }

   stream << format_source_info("\t- " + scope, record.source_location());

   return stream.str();
}

std::string MultilineFormatter::format_source_info(const std::string& prefix,
                                                   const SourceLocation& src_loc)
{
   std::ostringstream stream;

   if (src_loc.line_number == 0)
      return stream.str();

   stream << prefix << src_loc << "\n";

   return stream.str();
}

} // namespace log
} // namespace orion
