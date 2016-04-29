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

#include <string>
#include <vector>

#include <orion/Orion-Stddefs.h>
#include <orion/MemoryUtils.h>
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
   DECLARE_POINTERS(Test)

   typedef std::vector<SharedPtr> SharedPtrVector;

   Test(const std::string& name, const std::string& suite_name = "DefaultSuite");
   virtual ~Test();

   std::string name() const;
   std::string suite_name() const;

   TestResult::SharedPtr test_result() const;

   TestResult::SharedPtr execute_test();

   static Test::SharedPtrVector& tests();

protected:
   virtual void execute_test_impl(TestResult::SharedPtr test_result) const;

private:
   static Test::SharedPtrVector s_tests;

   std::string _name;
   std::string _suite_name;
   TestResult::SharedPtr _test_result;
};

//!
/*!
 */
class API_EXPORT TestAddHelper
{
public:
   TestAddHelper(Test::SharedPtrVector& tests, Test::SharedPtr test)
   {
      tests.push_back(test);
   }
};

//!
API_EXPORT int run_all_tests(TestOutput::SharedPtr output, const std::string& suite_name = "");

namespace UnitTestSuite
{
   API_EXPORT inline const char* suite_name() { return "DefaultSuite"; }
}

} // namespace orion
} // namespace unittest

#endif /* ORION_UNITTEST_TEST_H */
