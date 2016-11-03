// Test.h
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

#ifndef ORION_UNITTEST_TEST_H
#define ORION_UNITTEST_TEST_H

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include <orion/Orion-Stddefs.h>
#include <orion/unittest/TestResult.h>
#include <orion/unittest/TestOutput.h>

namespace orion
{
namespace unittest
{
// Forward declaration
class Test;

using Tests = std::unordered_multimap<std::string, std::unique_ptr<Test>>;

class SetUpTestCase
{
public:
   SetUpTestCase();
   SetUpTestCase(const std::function<void (Test* test)>& f);

   virtual ~SetUpTestCase() = default;

   virtual void operator()(Test* test);

private:
   std::function<void(Test* test)> _func;
};

class TearDownTestCase
{
public:
   TearDownTestCase();
   TearDownTestCase(const std::function<void (Test* test)>& f);

   virtual ~TearDownTestCase() = default;

   virtual void operator()(Test* test);

private:
   std::function<void(Test* test)> _func;
};

//!
/*!
 */
class API_EXPORT Test 
{
public:
   Test(const std::string& name, const std::string& suite_name = "DefaultSuite");
   virtual ~Test();

   std::string name() const;
   std::string suite_name() const;

   TestResult* test_result() const;

   // Sets up the test fixture.
   virtual void SetUp();

   // Tears down the test fixture.
   virtual void TearDown();

   void set_option(SetUpTestCase&& f);
   void set_option(TearDownTestCase&& f);

   TestResult* execute_test();

   static Tests& tests();

protected:
   virtual void execute_test_impl(TestResult* test_result) const;

private:
   std::string _name;
   std::string _suite_name;
   std::unique_ptr<TestResult> _test_result;

   SetUpTestCase _setup;
   TearDownTestCase _teardown;
};

inline void set_options(Test* /* test */)
{
}

template <typename T>
void set_options(Test* test, T&& t) 
{
   test->set_option(std::forward<decltype(t)>(t));
}

template <typename T, typename... Ts>
void set_options(Test* test, T&& t, Ts&&... ts)
{
   set_options(test, std::forward<decltype(t)>(t));
   set_options(test, std::forward<decltype(ts)>(ts)...);
}

//!
/*!
 */
class API_EXPORT TestAddHelper
{
public:
   TestAddHelper(Tests& tests, Test* test)
   {
      tests.emplace(std::make_pair(test->suite_name(), std::unique_ptr<Test>(test)));
   }
};

//!
API_EXPORT int run_all_tests(const std::unique_ptr<TestOutput>& output, const std::string& suite_name = "");

namespace UnitTestSuite
{
   API_EXPORT inline const char* suite_name() { return "DefaultSuite"; }
}

} // namespace orion
} // namespace unittest

#endif /* ORION_UNITTEST_TEST_H */
