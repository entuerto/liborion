//
// TestUtils.ipp
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TESTUTILS_IPP
#define ORION_UNITTEST_TESTUTILS_IPP

#include <orion/String.h>

#include <iostream>
#include <stdexcept>

namespace orion
{
namespace unittest
{

using namespace std::string_literals;

inline std::string to_string(ReportLevel rl)
{
   switch (rl)
   {
      case ReportLevel::Error:
         return "Error";
      case ReportLevel::Short:
         return "Short";
      case ReportLevel::Detailed:
         return "Detailed";
      default:
         AssertUnreachable("Invalid ReportLevel");
         break;
   }
   return "Error";
}

inline std::istream& operator>>(std::istream& in, ReportLevel& report_level)
{
   std::string token;
   in >> token;

   if (iequals(token, "Error"s))
   {
      report_level = ReportLevel::Error;
   }
   else if (iequals(token, "Short"s))
   {
      report_level = ReportLevel::Short;
   }
   else if (iequals(token, "Detailed"s))
   {
      report_level = ReportLevel::Detailed;
   }
   else
   {
      throw std::runtime_error("Invalid Report level");
   }

   return in;
}

inline std::ostream& operator<<(std::ostream& out, ReportLevel report_level)
{
   out << to_string(report_level);
   return out;
}

inline constexpr Counters& operator+=(Counters& lhs, const Counters& rhs)
{
   lhs.passed  += rhs.passed;
   lhs.failed  += rhs.failed;
   lhs.skipped += rhs.skipped;

   return lhs;
}

inline constexpr TestSuiteStats& operator+=(TestSuiteStats& lhs, const TestSuiteStats& rhs)
{
   lhs.assertions += rhs.assertions;
   lhs.tests      += rhs.tests;

   lhs.time_elapsed += rhs.time_elapsed;

   return lhs;
}

inline constexpr TestStats& operator+=(TestStats& lhs, const TestStats& rhs)
{
   lhs.assertions += rhs.assertions;

   lhs.time_elapsed += rhs.time_elapsed;

   return lhs;
}

} // namespace unittest
} // namespace orion

#endif // ORION_UNITTEST_TESTUTILS_IPP
