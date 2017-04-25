// MultilineFormatter.cpp
//
// Copyright 2013 tomas <tomasp@videotron.ca>
//
// Copyright 2016 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <orion/log/MultilineFormatter.h>

#include <sstream> 

#include <orion/log/Logger.h>
#include <orion/log/Record.h>

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

   auto& l = Logger::get_logger();

   for (uint32_t i = 0; i < l.scope_depth(); i++)
   {
      scope += "++";
   }
   if (not scope.empty())
      scope = scope + " ";

   std::string source_info_prefix("\t- " + scope);
   std::string source_info;

   source_info = (record.level() == Level::Exception) ?
                    format_source_info("\t- Caught: ", record.source_location()) :
                    format_source_info(source_info_prefix, record.source_location());

   std::ostringstream stream;

   stream << "|" 
          << to_string(record.level())
          << "|"
          << record.time_stamp()
          << "\n\t- "
          << scope
          << record.message()
          << "\n"
          << source_info;

   return stream.str();
}

std::string MultilineFormatter::format_source_info(const std::string& prefix, const SourceLocation& src_loc)
{
   std::ostringstream stream;

   if (src_loc.line_number == 0)
      return stream.str();

   stream << prefix
          << src_loc
          << "\n";

   return stream.str();
}

} // namespace log
} // namespace orion
