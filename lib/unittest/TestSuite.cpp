//
// TestSuite.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/unittest/TestSuite.h>

using namespace orion::unittest::option;

namespace orion
{
namespace unittest
{

//---------------------------------------------------------------------------------------

/// Constructor
TestSuite::TestSuite(const std::string& name)
   : _name(name)
   , _label(name)
   , _is_enabled(true)
   , _stats()
{
   _stats.label = name;
}

/// Name of the test suite
std::string TestSuite::name() const
{
   return _name;
}

std::string TestSuite::label() const
{
   return _label;
}

std::string TestSuite::description() const
{
   return _description;
}

bool TestSuite::enabled() const
{
   return _is_enabled;
}

std::string TestSuite::disabled_reason() const
{
   return _disabled_reason;
}

const Stats& TestSuite::stats() const
{
   return _stats;
}

const std::vector<Test>& TestSuite::test_cases() const
{
   return _test_cases;
}

uint64_t TestSuite::test_count() const
{
   return _test_cases.size();
}

void TestSuite::set_option(Label opt)
{
   _label = opt.text;
   _stats.label = opt.text;
}

void TestSuite::set_option(Description opt)
{
   _description = opt.text;
}

void TestSuite::set_option(SetupFunc opt)
{
   _setup_func = std::move(opt.func);
}

void TestSuite::set_option(TeardownFunc opt)
{
   _teardown_func = std::move(opt.func);
}

void TestSuite::set_option(Enabled /*opt*/)
{
   _is_enabled = true;
}

void TestSuite::set_option(Disabled opt)
{
   _is_enabled      = false;
   _disabled_reason = std::move(opt.reason);
}

/// Sets up the test suite.
void TestSuite::setup()
{
   if (_setup_func)
      _setup_func();
}

/// Tears down the test suite.
void TestSuite::teardown()
{
   if (_teardown_func)
      _teardown_func();
}

Test& TestSuite::add_test(const std::string& name, TestCaseFunc f)
{
   Test t(name, *this, std::move(f));

   return add_test(std::move(t));
}

Test& TestSuite::add_test(Test&& test)
{
   _test_cases.push_back(std::move(test));
   return _test_cases.back();
}

void TestSuite::add_tests(std::initializer_list<Test> l)
{
   _test_cases.insert(_test_cases.end(), l);
}

/// Executes the tests and logs then to output.
const Stats& TestSuite::run_tests(Output& output)
{
   _stats.count = _test_cases.size();

   output.suite_start(*this);

   setup();

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

      _stats.item_count += test_result.item_count();
      _stats.passed_item_count += test_result.passed_item_count();
      _stats.failed_item_count += test_result.failed_item_count();
      _stats.skipped_item_count += test_result.skipped_item_count();
      _stats.time_elapsed += test_result.time_elapsed();

      output.write(test_result);
   }

   teardown();

   output.suite_end(*this);

   return _stats;
}

} // namespace unittest
} // namespace orion
