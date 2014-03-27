// OnelineWithSourceInfoFormatter.cpp
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

#include <orion/logging/OnelineWithSourceInfoFormatter.h>

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
// Class OnelineWithSourceInfoFormatter

OnelineWithSourceInfoFormatter::OnelineWithSourceInfoFormatter()
{
}

OnelineWithSourceInfoFormatter::~OnelineWithSourceInfoFormatter()
{
}

std::string OnelineWithSourceInfoFormatter::format(const LogRecord& log_record)
{
   if (log_record.level() == Logger::NotSet)
      return log_record.message();

   std::string scope;

   for (uint32_t i = 0; i < Logger::get_logger().scope_count(); i++)
   {
      scope += " |";
   }

   std::string source_info_prefix;
   std::string source_info;
   std::string except_info;

   if (log_record.level() == Logger::Exception)
   {
      try
      {
         stream.clear();
         stream.str("");

         const LogExceptionRecord& except_record = dynamic_cast<const LogExceptionRecord&>(log_record);

         stream << "Thrown: "
                << except_record.thrown_file_name()
                << ":"
                << except_record.thrown_line_number()
                << ":"
                << except_record.thrown_function_name();

         except_info = stream.str();
         source_info_prefix = "|Caught: ";
      }
      catch(std::bad_cast&)
      {
      }
   }

   if (log_record.line() >= 0)
   {
      stream.clear();
      stream.str("");

      stream << source_info_prefix
             << log_record.file_name()
             << ":"
             << log_record.line()
             << ":"
             << log_record.function_name();

      source_info = stream.str();
   }

   stream.clear();
   stream.str("");

   stream << "|" 
          << level_as_text(log_record.level())
          << "|"
          << log_record.time_stamp()
          << "|"
          << scope
          << log_record.message()
          << "|"
          << except_info
          << source_info;

   return stream.str();
}

IFormatter::SharedPtr OnelineWithSourceInfoFormatter::create()
{
   return IFormatter::SharedPtr(new OnelineWithSourceInfoFormatter);
}

} // namespace logging
} // namespace orion
