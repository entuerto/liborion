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

namespace
{
   bool is_test_in_suite(const Test* test , const std::string& suite_name)
   {
	   return suite_name.empty() or test->suite_name() == suite_name;
   }
}

/*!
 */
Test::Test(const std::string& name, const std::string& suite_name) :
   _name(name),
   _suite_name(suite_name),
   _test_result()
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
      execute_test_impl(_test_result.get());
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
std::vector<std::unique_ptr<Test>>& Test::tests()
{
   static std::vector<std::unique_ptr<Test>> s_tests;

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
   int failure_count = 0;
   int failed_item_count = 0;

   auto& all_tests = Test::tests();

   std::stable_sort(all_tests.begin(), all_tests.end(), less_suite_name);

   int test_count = 0;

   if (suite_name.empty())
   {
      test_count = all_tests.size();
   }
   else
   {
      test_count = std::count_if(all_tests.begin(),
                                 all_tests.end(), 
                                 [&](const std::unique_ptr<Test>& t) {
                                    return is_test_in_suite(t.get(), suite_name);
                                 });
   }
   output->write_header(suite_name, test_count);

   Timer timer;

   timer.start();

   for (auto&& test : all_tests)
   {
      if (not is_test_in_suite(test.get(), suite_name))
         continue;
      
      auto test_result = test->execute_test();

      if (test_result->failed())
      {
         failure_count++;
         failed_item_count += test_result->failed_item_count();
      }
      output->write(test_result);
   }

   timer.stop();

   output->write_summary(failure_count, failed_item_count, all_tests.size(), timer.elapsed());

   return failure_count;
}

} // namespace orion
} // namespace unittest
