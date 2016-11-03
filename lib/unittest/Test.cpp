// Test.cpp
//
// Copyright 2010 tomas <tomasp@videotron.ca>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.
//

#include <orion/Timer.h>
#include <orion/unittest/Test.h>
#include <orion/unittest/TestResultItem.h>

#include <algorithm>

namespace orion
{
namespace unittest
{
//---------------------------------------------------------------------------------------

SetUpTestCase::SetUpTestCase() : 
   _func() 
{
}

SetUpTestCase::SetUpTestCase(const std::function<void (Test* test)>& f) :
   _func(f) 
{
}

void SetUpTestCase::operator()(Test* test)
{
  if (_func)
     _func(test);
}

//---------------------------------------------------------------------------------------

TearDownTestCase::TearDownTestCase() : 
   _func() 
{
}

TearDownTestCase::TearDownTestCase(const std::function<void (Test* test)>& f) :
   _func(f) 
{
}

void TearDownTestCase::operator()(Test* test)
{
   if (_func)
      _func(test);
}

//---------------------------------------------------------------------------------------

/*!
 */
Test::Test(const std::string& name, const std::string& suite_name) :
   _name(name),
   _suite_name(suite_name),
   _test_result(),
   _setup(),
   _teardown()
{
}

/*!
 */
Test::~Test()
{
}

/*!
 */
std::string Test::name() const
{
   return _name;
}

/*!
 */
std::string Test::suite_name() const
{
   return _suite_name;
}

TestResult* Test::test_result() const
{
   return _test_result.get();
}

void Test::set_option(SetUpTestCase&& f)
{
   _setup = std::move(f);
}

void Test::set_option(TearDownTestCase&& f)
{
   _teardown = std::move(f);
}

void Test::SetUp()
{
   _setup(this);
}

void Test::TearDown()
{
   _teardown(this);
}

/*!
 */
void Test::execute_test_impl(TestResult* /* test_result */) const
{
}

/*!
 */
TestResult* Test::execute_test()
{
   _test_result = TestResult::create(name(), suite_name());

   _test_result->on_start();
   try
   {
      SetUp();
      execute_test_impl(_test_result.get());
      TearDown();
   }
   catch (const std::exception& e)
   {
      std::string msg("Unhandled exception: ");

      msg += e.what();

      auto item = TestResultItem::create_failure(msg, __FILE__, __LINE__); 
      _test_result->on_failure(std::move(item));
   }
   catch (...)
   {
      auto item = TestResultItem::create_failure("Unhandled exception: Crash!",__FILE__, __LINE__);
      _test_result->on_failure(std::move(item));
   }
   _test_result->on_end();

   return _test_result.get();
}

/*!
 */
Tests& Test::tests()
{
   static Tests s_tests;

   return s_tests;
}

//---------------------------------------------------------------------------------------

bool less_suite_name(const std::unique_ptr<Test>& t1, const std::unique_ptr<Test>& t2)
{
   return t1->suite_name() < t2->suite_name();
}

//---------------------------------------------------------------------------------------

/*!
 */
int run_all_tests(const std::unique_ptr<TestOutput>& output, const std::string& suite_name)
{
   TestOutputStats stats{};

   auto& all_tests = Test::tests();

   stats.count = suite_name.empty() ? all_tests.size() : 
                                      all_tests.count(suite_name);

   output->write_header(suite_name, stats.count);

   auto range = suite_name.empty() ? std::make_pair(all_tests.begin(), all_tests.end()) :
                                     all_tests.equal_range(suite_name);

   Timer timer;

   timer.start();

   std::for_each(range.first, range.second, [&](const auto& test) 
   { 
      auto test_result = test.second->execute_test();

      if (test_result->failed())
      {
         stats.failed_count++;
         stats.failed_item_count += test_result->failed_item_count();
      } 
      else if (test_result->passed())
      {
         stats.passed_count++;
         stats.passed_item_count += test_result->passed_item_count();
      }
      stats.item_count += test_result->result_items().size();

      output->write(test_result);
   });

   timer.stop();

   stats.time_elapsed = timer.elapsed();

   output->write_summary(stats);

   return stats.failed_item_count;
}

} // namespace orion
} // namespace unittest
