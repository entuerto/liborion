// Test.h
//
// Copyright 2017 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
#ifndef ORION_UNITTEST_TESTSUITE_H
#define ORION_UNITTEST_TESTSUITE_H

#include <functional>
#include <string>

#include <orion/Orion-Stddefs.h>
#include <orion/unittest/Test.h>
#include <orion/unittest/TestOutput.h>

namespace orion
{
namespace unittest
{
//---------------------------------------------------------------------------------------

///
/// A test suite is a set of tests that all share the same fixture.
///
class API_EXPORT Suite
{
public:
   Suite(const std::string& name);
   virtual ~Suite() = default;

   /// Name of the test suite
   std::string name() const;

   const OutputStats& stats() const;

   const std::vector<Test>& test_cases() const;

   int test_count() const;

   /// Sets up the test suite.
   void setup();

   /// Tears down the test suite.
   void teardown();

   /// Executes the tests and logs then to output.
   const OutputStats& run_tests(Output& output);

   Test& add_test(Test&& test);

   Test& add_test(const std::string& name, TestCaseFunc f);

   void add_tests(std::initializer_list<Test> l);

protected:
   std::function<void()> _setup;
   std::function<void()> _teardown;

private:
   std::string _name;
   std::vector<Test> _test_cases;

   OutputStats _stats;
};


struct API_EXPORT RegisterTestHelper
{
   //RegisterHelper(Suite& suite, std::initializer_list<Test> l)
   
   template<typename... Args>
   RegisterTestHelper(Suite& suite, const std::string& name, TestCaseFunc f, Args... args)
   {
      auto& t = suite.add_test(name, f);
      set_options(t, args...);
   }
};

} // namespace orion
} // namespace unittest

#endif /* ORION_UNITTEST_TESTSUITE_H */
