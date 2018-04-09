//
//  EvalMacros.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <stdexcept>

#include <orion/Test.h>
#include <orion/ErrorMacros.h>
#include <orion/Exception.h>

using namespace orion;
using namespace orion::unittest;
using namespace orion::unittest::option;

Section(OrionCore_Test, Label{"Test"})
{

int calc_throwing_function()
{
   throw std::logic_error("Oh boy");
}

void throwing_function()
{
   throw std::logic_error("Oh boy");
}

TestCase("Success on true")
{
   Test scope_test("scope test", TestSuite("testSuite"), [](Test& st) { 
      st.xassert<true>(true); 
   });
   
   scope_test.execute_test();

   check_true(scope_test.test_result().passed());
}

TestCase("Failure on false")
{
   Test scope_test("scope test", TestSuite("testSuite"), [](Test& st) { 
      st.xassert<false>(false); 
   });
   
   scope_test.execute_test();

   check_false(scope_test.test_result().failed());
}

TestCase("Function failure with exception")
{
   Test scope_test("scope test", TestSuite("testSuite"), [](Test& st) { 
      st.xassert<eq>(calc_throwing_function(), 1); 
   });

   scope_test.execute_test();
   
   check_true(scope_test.test_result().failed());
}

TestCase("Close success on equal", Disabled{"Not implemented"})
{
/*
   bool failure = true;
   {
      auto r = TestResult::create("scope test result");
      TestResult* test_result = r.get();

      EXPECT_CLOSE(1.0f, 1.001f, 0.01f);

      failure = test_result->failed();
   }

   EXPECT_TRUE(not failure);
*/
   t.fail("Not implemented");
}

TestCase("Close fails on not equal", Disabled{"Not implemented"})
{
/*
   bool failure = false;
   {
      auto r = TestResult::create("scope test result");
      TestResult* test_result = r.get();

      EXPECT_CLOSE(1.0f, 1.1f, 0.01f);

      failure = test_result->failed();
   }

   EXPECT_TRUE(failure);
*/
   t.fail("Not implemented");
}

TestCase("Not expected exception")
{
   Test scope_test("throwing", TestSuite("testSuite"), [](Test& st) { 
      st.xassert_throw<std::domain_error>(throwing_function); 
   });

   scope_test.execute_test();
   
   check_true(scope_test.test_result().failed());
}

TestCase("Expected exception")
{
   Test scope_test("throwing", TestSuite("testSuite"), [](Test& st) { 
      st.xassert_throw<std::logic_error>(throwing_function); 
   });
   
   scope_test.execute_test();

   check_true(scope_test.test_result().passed());
}

TestCase("TestResult::log_success()")
{
   auto test_result = TestResult("scope test result", "suite");

   test_result.log_success();

   check_false(test_result.failed());
}

TestCase("TestResult::log_failure()")
{
   auto test_result = TestResult("scope test result", "suite");

   test_result.log_failure();

   check_true(test_result.failed());
}

} 
