//
// TestRunner.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_UNITTEST_TESTRUNNER_H
#define ORION_UNITTEST_TESTRUNNER_H

#include <orion/Orion-Stddefs.h>
#include <orion/unittest/Test.h>
#include <orion/unittest/TestSuite.h>

#include <string>
#include <vector>

namespace orion
{
namespace unittest
{
// Forward declaration
//class Test;
class Output;

//-------------------------------------------------------------------------------------------------

///
/// Runs the defined tests.
///
class API_EXPORT TestRunner
{
public:
   TestRunner() = default;

   constexpr uint32_t test_suite_count() const;
   uint32_t test_case_count() const;

   TestSuite& add_test_suite(TestSuite&& suite);

   bool run(int argc, char* argv[]);

   bool run_tests(Output& output, const std::string& suite_name = "");

   static TestRunner& runner();

private:
   bool parse(int argc, char* argv[]);

private:
   std::vector<TestSuite> _test_suites;

   ReportLevel _level{ReportLevel::Detailed};
};

//-------------------------------------------------------------------------------------------------

///
struct API_EXPORT RegisterTestSuiteHelper
{
   template<typename... Args>
   explicit RegisterTestSuiteHelper(const std::string& name, Args... args)
      : suite(TestRunner::runner().add_test_suite(TestSuite{name}))
   {
      set_options(suite, args...);
   }

   TestSuite& suite;
};

//-------------------------------------------------------------------------------------------------

struct API_EXPORT RegisterTestHelper
{
   template<typename... Args>
   RegisterTestHelper(TestSuite& suite, const std::string& name, TestCaseFunc f, Args... args)
   {
      auto& t = suite.add_test(name, std::move(f));
      set_options(t, args...);
   }
};
} // namespace orion
} // namespace unittest

#endif /* ORION_UNITTEST_TESTRUNNER_H */
