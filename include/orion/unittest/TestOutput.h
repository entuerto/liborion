//
// TestOutput.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TESTOUTPUT_H
#define ORION_UNITTEST_TESTOUTPUT_H

#include <orion/Orion-Stddefs.h>

#include <chrono>
#include <istream>
#include <string>

namespace orion
{
namespace unittest
{
class Suite;
class TestResult;

//---------------------------------------------------------------------------------------

enum class ReportLevel
{
   Error    = 0,
   Short    = 1,
   Detailed = 2
};

std::string to_string(ReportLevel rl);

std::istream& operator>>(std::istream& in, ReportLevel& report_level);
std::ostream& operator<<(std::ostream& out, ReportLevel report_level);

//---------------------------------------------------------------------------------------

struct OutputStats
{
   std::size_t count;
   std::size_t passed_count;
   std::size_t failed_count;
   std::size_t skipped_count;

   std::size_t item_count;
   std::size_t passed_item_count;
   std::size_t failed_item_count;
   std::size_t skipped_item_count; 
   
   std::chrono::milliseconds time_elapsed;
};

OutputStats& operator+=(OutputStats& lhs, const OutputStats& rhs);

//!
/*!
 */
class Output
{
public:
   virtual ~Output() = default;

   /// Global test report header
   virtual void write_header(int test_count) = 0;

   /// Test suite header
   virtual void write_suite_header(const Suite& suite) = 0;

   /// Test results information
   virtual void write(const TestResult& test_result) = 0;

   /// Summary of the test suite
   virtual void write_suite_summary(const Suite& suite) = 0;

   /// Summary of all tests
   virtual void write_summary(const OutputStats& stats) = 0;
};

} // namespace orion
} // namespace unittest

#endif /* TESTOUTPUT_H */
