//
// Formatter.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/log/Formatter.h>

#include <orion/Chrono.h>
#include <orion/log/Logger.h>
#include <orion/log/Record.h>

#include <sstream>

namespace orion
{
namespace log
{

//--------------------------------------------------------------------------------------------------
// Class OnelineFormatter

OnelineFormatter::OnelineFormatter(bool with_src_loc /*= false*/)
   : _with_src_loc(with_src_loc)
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

   stream << "|" << to_string(record.level()) << "|" << orion::to_string(record.time_stamp(), "%X")
          << "|" << scope << record.message();

   // Do not print source location
   if (not _with_src_loc)
      return stream.str();

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

   if (sl.line_number() != 0)
      stream << "|" << sl;

   return stream.str();
}

std::unique_ptr<OnelineFormatter> make_one_line_formatter(bool with_src_loc /*= false*/)
{
   return std::make_unique<OnelineFormatter>(with_src_loc);
}

//--------------------------------------------------------------------------------------------------
// Class MultilineFormatter

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

   stream << "|" << to_string(record.level()) << "|" << orion::to_string(record.time_stamp(), "%X")
          << "\n\t- " << scope << record.message() << "\n";

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

   if (src_loc.line_number() == 0)
      return stream.str();

   stream << prefix << src_loc << "\n";

   return stream.str();
}

std::unique_ptr<MultilineFormatter> make_multiline_formatter()
{
   return std::make_unique<MultilineFormatter>();
}

} // namespace log
} // namespace orion
