// TestOutput.cpp
//
// Copyright 2016 tomas <tomasp@videotron.ca>
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
//
#include <orion/unittest/TestOutput.h>

#include <stdexcept>

#include <orion/ErrorMacros.h>
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
      THROW_STDEXCEPTION(std::runtime_error, "Invalid Report level");
   }

   return in;
}

std::ostream& operator<< (std::ostream& out, ReportLevel report_level)
{
   out << to_string(report_level);
   return out;
}

} // namespace orion
} // namespace unittest
