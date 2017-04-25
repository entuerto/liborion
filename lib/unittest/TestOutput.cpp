// TestOutput.cpp
//
// Copyright 2017 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
// 
#include <orion/unittest/TestOutput.h>

#include <stdexcept>

#include <orion/StringUtils.h>

namespace orion
{
namespace unittest
{

std::string to_string(ReportLevel rl)
{
   switch(rl)
   {
      case ReportLevel::Error:
         return "Error";
      case ReportLevel::Short:
         return "Short";
      case ReportLevel::Detailed:
         return "Detailed";
      default:
         break;
   }
   return "Error";
}

std::istream& operator>> (std::istream &in, ReportLevel& report_level)
{
   std::string token;
   in >> token;

   if (equals_no_case(token, "Error"))
   {
      report_level = ReportLevel::Error;
   }
   else if (equals_no_case(token, "Short"))
   {
      report_level = ReportLevel::Short;
   }
   else if (equals_no_case(token, "Detailed"))
   {
      report_level = ReportLevel::Detailed;
   }
   else
   {
      throw std::runtime_error("Invalid Report level");
   }

   return in;
}

std::ostream& operator<< (std::ostream& out, ReportLevel report_level)
{
   out << to_string(report_level);
   return out;
}

OutputStats& operator+= (OutputStats& lhs, const OutputStats& rhs)
{
   lhs.count += rhs.count; 
   lhs.passed_count  += rhs.passed_count; 
   lhs.failed_count  += rhs.failed_count;
   lhs.skipped_count += rhs.skipped_count;

   lhs.item_count += rhs.item_count;
   lhs.passed_item_count += rhs.passed_item_count; 
   lhs.failed_item_count += rhs.failed_item_count; 
   lhs.skipped_item_count += rhs.skipped_item_count; 
   
   lhs.time_elapsed += rhs.time_elapsed;

   return lhs;
}

} // namespace orion
} // namespace unittest
