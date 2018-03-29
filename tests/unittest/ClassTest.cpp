//
//  ClassTest.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Test.h>

using namespace std::string_literals;

using namespace orion;
using namespace orion::unittest;
using namespace orion::unittest::option;

TestSuite(OrionCore_Test, Label{"Test"})
{

void passing_test_has_no_failures(Test& t)
{
   Test passing_test("passing", Suite("testSuite"), [](Test& ft) { 
      ft.assert<true>(true); 
   });

   auto scope_test_result = passing_test.execute_test();

   t.assert<true>(scope_test_result.passed(), _src_loc);
}


void failing_test_has_failures(Test& t)
{
   Test failing_test("failing", Suite("testSuite"), [](Test& ft) { 
      ft.assert<false>(false); 
   });

   auto scope_test_result = failing_test.execute_test();

   t.assert<false>(scope_test_result.failed(), _src_loc);
}


void throwing_tests_are_reported_as_failures(Test& t)
{
   Test crashing_test("throwing", Suite("testSuite"), [](Test&) { 
      throw std::logic_error("Oh boy"); 
   });

   auto scope_test_result = crashing_test.execute_test();

   t.assert<true>(scope_test_result.failed(), _src_loc);
}

void has_same_name_and_suite_name_as_test_result(Test& t)
{
   auto test_result = t.test_result();

   t.assert<eq>(t.name(), test_result.name(), _src_loc);
}

RegisterTestCase(OrionCore_Test, passing_test_has_no_failures, Label{"Passing with no failure"});
RegisterTestCase(OrionCore_Test, failing_test_has_failures, Label{"Failing with no failure"});
RegisterTestCase(OrionCore_Test, throwing_tests_are_reported_as_failures, Label{"Throwing  reported as failure"});
RegisterTestCase(OrionCore_Test, has_same_name_and_suite_name_as_test_result, Label{"Same Test name and Suite name as TestResult"});
} // TestSuite(OrionCore_Test)
