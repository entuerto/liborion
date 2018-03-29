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

TestSuite(OrionCore_Test, Label{"Test"})
{

int calc_throwing_function()
{
   throw std::logic_error("Oh boy");
}

void throwing_function()
{
   throw std::logic_error("Oh boy");
}

void check_success_on_true(Test& t)
{
   Test scope_test("scope test", Suite("testSuite"), [](Test& st) { 
      st.assert<true>(true); 
   });
   
   scope_test.execute_test();

   t.assert<true>(scope_test.test_result().passed(), _src_loc);
}

void check_failure_on_false(Test& t)
{
   Test scope_test("scope test", Suite("testSuite"), [](Test& st) { 
      st.assert<false>(false); 
   });
   
   scope_test.execute_test();

   t.assert<false>(scope_test.test_result().failed(), _src_loc);
}

void check_function_failure_on_exception(Test& t)
{
   Test scope_test("scope test", Suite("testSuite"), [](Test& st) { 
      st.assert<eq>(calc_throwing_function(), 1); 
   });

   scope_test.execute_test();
   
   t.assert<true>(scope_test.test_result().failed(), _src_loc);
}

void check_close_success_on_equal(Test& t)
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

void check_close_fails_on_not_equal(Test& t)
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

void check_not_expected_exception(Test& t)
{
   Test scope_test("throwing", Suite("testSuite"), [](Test& st) { 
      st.assert_throw<std::domain_error>(throwing_function); 
   });

   scope_test.execute_test();
   
   t.assert<true>(scope_test.test_result().failed(), _src_loc);
}

void check_expected_exception(Test& t)
{
   Test scope_test("throwing", Suite("testSuite"), [](Test& st) { 
      st.assert_throw<std::logic_error>(throwing_function); 
   });
   
   scope_test.execute_test();

   t.assert<true>(scope_test.test_result().passed(), _src_loc);
}

void check_test_result_log_success(Test& t)
{
   auto test_result = TestResult("scope test result", "suite");

   test_result.log_success();

   t.assert<false>(test_result.failed(), _src_loc);
}

void check_test_result_log_failure(Test& t)
{
   auto test_result = TestResult("scope test result", "suite");

   test_result.log_failure();

   t.assert<true>(test_result.failed(), _src_loc);
}

RegisterTestCase(OrionCore_Test, check_success_on_true, Label{"Success on true"});
RegisterTestCase(OrionCore_Test, check_failure_on_false, Label{"Failure on false"});
RegisterTestCase(OrionCore_Test, check_function_failure_on_exception, Label{"Function failure with exception"});
RegisterTestCase(OrionCore_Test, check_close_success_on_equal, Label{"Close success on equal"}, Disabled{"Not implemented"});
RegisterTestCase(OrionCore_Test, check_close_fails_on_not_equal, Label{"Close fails on not equal"}, Disabled{"Not implemented"});
RegisterTestCase(OrionCore_Test, check_not_expected_exception, Label{"Not expected exception"});
RegisterTestCase(OrionCore_Test, check_expected_exception, Label{"Expected exception"});
RegisterTestCase(OrionCore_Test, check_test_result_log_success, Label{"TestResult::log_success()"});
RegisterTestCase(OrionCore_Test, check_test_result_log_failure, Label{"TestResult::log_failure()"});
} 
