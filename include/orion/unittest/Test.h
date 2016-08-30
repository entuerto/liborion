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

#include <memory>
#include <string>
#include <vector>

#include <orion/Orion-Stddefs.h>
#include <orion/unittest/TestResult.h>
#include <orion/unittest/TestOutput.h>

namespace orion
{
namespace unittest
{
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

   TestResult* execute_test();

   static std::vector<std::unique_ptr<Test>>& tests();

protected:
   virtual void execute_test_impl(TestResult* test_result) const;

private:
   std::string _name;
   std::string _suite_name;
   std::unique_ptr<TestResult> _test_result;
};

//!
/*!
 */
class API_EXPORT TestAddHelper
{
public:
   TestAddHelper(std::vector<std::unique_ptr<Test>>& tests, Test* test)
   {
      tests.push_back(std::unique_ptr<Test>(test));
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
