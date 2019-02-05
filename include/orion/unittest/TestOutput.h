//
// TestOutput.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TESTOUTPUT_H
#define ORION_UNITTEST_TESTOUTPUT_H

#include <orion/Config.h>

namespace orion
{
namespace unittest
{
struct Totals;
struct TestSuiteStats;

class Test;
class TestSuite;
class TestResult;
class TestRunner;

///
/// Default interface for unit test output.
///
class Output
{
public:
   virtual ~Output() = default;

   /// The test runner is starting
   virtual void runner_start(const TestRunner& runner) = 0;

   /// New test suite section
   virtual void suite_start(const TestSuite& suite) = 0;

   /// Test case is starting
   virtual void test_start(const Test& test) = 0;

   /// Test case ended
   virtual void test_end(const TestResult& test_result) = 0;

   /// End of test suite section
   virtual void suite_end(const TestSuiteStats& stats) = 0;

   /// The test runner ended
   virtual void runner_end(const Totals& totals) = 0;
};

} // namespace unittest
} // namespace orion

#endif /* TESTOUTPUT_H */
