//
// TestUtils.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TESTUTILS_H
#define ORION_UNITTEST_TESTUTILS_H

#include <orion/Common.h>

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
   uint64_t passed{0u};
   uint64_t failed{0u};
   uint64_t skipped{0u};

   constexpr uint64_t total() const { return passed + failed + skipped; }
};

API_EXPORT Counters& operator+=(Counters& lhs, const Counters& rhs);

//-------------------------------------------------------------------------------------------------

struct Totals
{
   Counters assertions;
   Counters tests;

   std::chrono::nanoseconds time_elapsed{0};
};

struct TestSuiteStats
{
   std::string label;

   Counters assertions;
   Counters tests;

   std::chrono::nanoseconds time_elapsed{0};
};

API_EXPORT TestSuiteStats& operator+=(TestSuiteStats& lhs, const TestSuiteStats& rhs);

struct TestStats
{
   std::string label;

   Counters assertions;

   std::chrono::nanoseconds time_elapsed{0};
};

API_EXPORT TestStats& operator+=(TestStats& lhs, const TestStats& rhs);

} // namespace unittest
} // namespace orion

#endif /* ORION_UNITTEST_TESTUTILS_H */
