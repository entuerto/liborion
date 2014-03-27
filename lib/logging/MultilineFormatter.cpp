// MultilineFormatter.cpp
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

#include <orion/logging/MultilineFormatter.h>

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
// Class MultilineFormatter

MultilineFormatter::MultilineFormatter()
{
}

MultilineFormatter::~MultilineFormatter()
{
}

std::string MultilineFormatter::format(const LogRecord& log_record)
{
   if (log_record.level() == Logger::NotSet)
      return log_record.message();

   std::string scope;

   for (uint32_t i = 0; i < Logger::get_logger().scope_count(); i++)
   {
      scope += "++";
   }
   if (not scope.empty())
      scope = scope + " ";

   std::string source_info_prefix("\t- " + scope);
   std::string except_info;

   if (log_record.level() == Logger::Exception)
   {
      try
      {
         const LogExceptionRecord& except_record = dynamic_cast<const LogExceptionRecord&>(log_record);

         except_info = format_source_info("\t- Thrown: ",
                                                   except_record.thrown_file_name(),
                                                   except_record.thrown_line_number(),
                                                   except_record.thrown_function_name());
         source_info_prefix = "\t- Caught: ";
      }
      catch(std::bad_cast&)
      {
      }
   }

   std::string source_info;
   if (log_record.line() >= 0)
   {
      source_info = format_source_info(source_info_prefix,
                                       log_record.file_name(),
                                       log_record.line(),
                                       log_record.function_name());
   }

   stream.clear();
   stream.str("");

   stream << "|" 
          << level_as_text(log_record.level())
          << "|"
          << log_record.time_stamp()
          << "\n\t- "
          << scope
          << log_record.message()
          << "\n"
          << except_info
          << source_info;

   return stream.str();
}

IFormatter::SharedPtr MultilineFormatter::create()
{
   return IFormatter::SharedPtr(new MultilineFormatter);
}

std::string MultilineFormatter::format_source_info(const std::string& prefix,
                                                     const std::string& file,
                                                           int32_t line,
                                                     const std::string& function)
{
   stream.clear();
   stream.str("");

   stream << prefix
          << file
          << ":"
          << line
          << ":"
          << function
          << "\n";

   return stream.str();
}

} // namespace logging
} // namespace orion
