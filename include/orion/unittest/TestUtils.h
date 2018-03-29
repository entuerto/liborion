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

struct Stats
{
   std::string label;
   
   uint64_t count;
   uint64_t passed_count;
   uint64_t failed_count;
   uint64_t skipped_count;

   uint64_t item_count;
   uint64_t passed_item_count;
   uint64_t failed_item_count;
   uint64_t skipped_item_count;

   std::chrono::nanoseconds time_elapsed;
};

API_EXPORT Stats& operator+=(Stats& lhs, const Stats& rhs);

//-------------------------------------------------------------------------------------------------

struct ItemStats
{
   std::string label;

   uint64_t count;
   uint64_t passed_count;
   uint64_t failed_count;
   uint64_t skipped_count;

   std::chrono::nanoseconds time_elapsed;
};

} // namespace unittest
} // namespace orion

#endif /* ORION_UNITTEST_TESTUTILS_H */
