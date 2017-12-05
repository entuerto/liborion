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
class API_EXPORT Runner
{
public:
   Runner();
   virtual ~Runner() = default;

   uint32_t test_suite_count() const;
   uint32_t test_case_count() const;

   Suite& add_test_suite(Suite&& suite);

   virtual bool run(int argc, char* argv[]);

   virtual bool run_tests(Output& output, const std::string& suite_name = "");

   static Runner& runner();

private:
   bool parse(int argc, char* argv[]);

private:
   std::vector<Suite> _test_suites;

   ReportLevel _level;
};

//-------------------------------------------------------------------------------------------------

///
struct API_EXPORT RegisterTestSuiteHelper
{
   template<typename... Args>
   RegisterTestSuiteHelper(const std::string& name, Args... args)
      : suite(Runner::runner().add_test_suite(Suite{name}))
   {
      set_options(suite, args...);
   }

   Suite& suite;
};

//-------------------------------------------------------------------------------------------------

struct API_EXPORT RegisterTestHelper
{
   template<typename... Args>
   RegisterTestHelper(Suite& suite, const std::string& name, TestCaseFunc f, Args... args)
   {
      auto& t = suite.add_test(name, f);
      set_options(t, args...);
   }
};
} // namespace orion
} // namespace unittest

#endif /* ORION_UNITTEST_TESTRUNNER_H */
