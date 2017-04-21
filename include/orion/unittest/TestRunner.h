// TestRunner.h
//
// Copyright 2017 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
#ifndef ORION_UNITTEST_TESTRUNNER_H
#define ORION_UNITTEST_TESTRUNNER_H

#include <string>
#include <vector>

#include <orion/Orion-Stddefs.h>
#include <orion/unittest/TestSuite.h>

namespace orion
{
namespace unittest
{
// Forward declaration
class Test;
class Output;

//---------------------------------------------------------------------------------------

///
///
class API_EXPORT Runner
{
public:
   Runner();
   virtual ~Runner() = default;

   virtual uint32_t test_suite_count() const;
   virtual uint32_t test_case_count() const;

   virtual  Suite& add_test_suite(const Suite& suite);

   virtual bool run(int argc, char* argv[]);

   virtual void run_tests(Output& output, const std::string& suite_name = "");

   static Runner& runner();

private:
   bool parse(int argc, char* argv[]);

private:
   std::vector<Suite> _test_suites;

   ReportLevel _level;
};

//---------------------------------------------------------------------------------------

/// 
class API_EXPORT RegisterTestSuiteHelper
{
public:
   RegisterTestSuiteHelper(const std::string& name) :
      suite(Runner::runner().add_test_suite(Suite{name}))
   {
   }

   Suite& suite;
};

} // namespace orion
} // namespace unittest

#endif /* ORION_UNITTEST_TESTRUNNER_H */
