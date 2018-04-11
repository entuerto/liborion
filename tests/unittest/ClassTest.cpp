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

Section(OrionCore_Test, Label{"Test"})
{

TestCase("Passing with no failure")
{
   Test passing_test("passing", TestSuite("testSuite"), [](Test& ft) { 
      ft.xassert<true>(true); 
   });

   auto scope_test_result = passing_test.invoke();

   check_true(scope_test_result.passed());
}

TestCase("Failing with no failure")
{
   Test failing_test("failing", TestSuite("testSuite"), [](Test& ft) { 
      ft.xassert<false>(false); 
   });

   auto scope_test_result = failing_test.invoke();

   check_false(scope_test_result.failed());
}

TestCase("Throwing reported as failure")
{
   Test crashing_test("throwing", TestSuite("testSuite"), [](Test&) { 
      throw std::logic_error("Oh boy"); 
   });

   auto scope_test_result = crashing_test.invoke();

   check_true(scope_test_result.failed());
}

TestCase("Same Test name and Suite name as TestResult")
{
   auto test_result = t.test_result();

   check_eq(t.name(), test_result.name());
}

} // TestSuite(OrionCore_Test)
