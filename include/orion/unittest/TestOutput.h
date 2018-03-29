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

#include <cstdint>

namespace orion
{
namespace unittest
{
struct Stats;
class Suite;
class TestResult;

///
/// Default interface for unit test output.
///
class Output
{
public:
   virtual ~Output() = default;

   /// Global test report header
   virtual void write_header(uint64_t test_count) = 0;

   /// New test suite section
   virtual void suite_start(const Suite& suite) = 0;

   /// Test results information
   virtual void write(const TestResult& test_result) = 0;

   /// End of test suite section
   virtual void suite_end(const Suite& suite) = 0;

   /// Footer of the output report
   virtual void write_footer(const Stats& stats) = 0;
};

} // namespace unittest
} // namespace orion

#endif /* TESTOUTPUT_H */
