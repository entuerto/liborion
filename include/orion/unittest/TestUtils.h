//
// TestUtils.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TESTUTILS_H
#define ORION_UNITTEST_TESTUTILS_H

#include <orion/Orion-Stddefs.h>

#include <chrono>
#include <iosfwd>
#include <string>

namespace orion
{
namespace unittest
{

//-------------------------------------------------------------------------------------------------

enum class ReportLevel
{
   Error    = 0,
   Short    = 1,
   Detailed = 2
};

API_EXPORT std::string to_string(ReportLevel rl);

API_EXPORT std::istream& operator>>(std::istream& in, ReportLevel& report_level);
API_EXPORT std::ostream& operator<<(std::ostream& out, ReportLevel report_level);

//-------------------------------------------------------------------------------------------------

struct Counters
{
   uint64_t passed;
   uint64_t failed;
   uint64_t skipped;

   uint64_t total() const { return passed + failed + skipped; }
};

API_EXPORT Counters& operator+=(Counters& lhs, const Counters& rhs);

//-------------------------------------------------------------------------------------------------

struct Totals
{
   Counters assertions;
   Counters tests;

   std::chrono::nanoseconds time_elapsed;
};

struct TestSuiteStats
{
   std::string label;

   Counters assertions;
   Counters tests;

   std::chrono::nanoseconds time_elapsed;
};

API_EXPORT TestSuiteStats& operator+=(TestSuiteStats& lhs, const TestSuiteStats& rhs);

struct TestStats
{
   std::string label;

   Counters assertions;

   std::chrono::nanoseconds time_elapsed;
};

API_EXPORT TestStats& operator+=(TestStats& lhs, const TestStats& rhs);

} // namespace unittest
} // namespace orion

#endif /* ORION_UNITTEST_TESTUTILS_H */
