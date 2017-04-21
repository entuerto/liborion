// Test.cpp
//
// Copyright 2017 The liborion Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
#include <orion/unittest/TestSuite.h>

namespace orion
{
namespace unittest
{

//---------------------------------------------------------------------------------------

/// Constructor
Suite::Suite(const std::string& name) :
   _setup(),
   _teardown(),
   _name(name),
   _test_cases(),
   _stats()
{
}

/// Name of the test suite
std::string Suite::name() const
{
   return _name;
}

const OutputStats& Suite::stats() const
{
   return _stats;
}

const std::vector<Test>& Suite::test_cases() const
{
   return _test_cases;
}

int Suite::test_count() const
{
   return _test_cases.size();
}

/// Sets up the test suite.
void Suite::setup()
{
   if (_setup)
      _setup();
}

/// Tears down the test suite.
void Suite::teardown()
{
   if (_teardown)
      _teardown();
}

///
Test& Suite::add_test(const std::string& name, TestCaseFunc f)
{
   Test t(name, *this, std::move(f));

   return add_test(std::move(t));
}

///
Test& Suite::add_test(Test&& test)
{
   _test_cases.push_back(std::move(test));
   return _test_cases.back();
}

///
void Suite::add_tests(std::initializer_list<Test> l)
{
   _test_cases.insert(_test_cases.end(), l);
}

/// Executes the tests and logs then to output.
const OutputStats& Suite::run_tests(Output& output)
{
   _stats.count = _test_cases.size();

   output.write_suite_header(*this);

   for (auto& test : _test_cases)
   {
      auto& test_result = test.execute_test();

      if (test_result.item_count() == test_result.skipped_item_count())
      {
         _stats.skipped_count++;
      }
      else if (test_result.failed())
      {
         _stats.failed_count++;
      } 
      else 
      {
         _stats.passed_count++;
      }

      _stats.item_count         += test_result.item_count();
      _stats.passed_item_count  += test_result.passed_item_count();
      _stats.failed_item_count  += test_result.failed_item_count();
      _stats.skipped_item_count += test_result.skipped_item_count();
      _stats.time_elapsed       += test_result.time_elapsed();

      output.write(test_result);
   }

   output.write_suite_summary(*this);

   return _stats;
}

} // namespace orion
} // namespace unittest
