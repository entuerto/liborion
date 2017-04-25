
#include <stdexcept>

#include <orion/TestUtils.h>
#include <orion/ErrorMacros.h>
#include <orion/Exception.h>

using namespace orion::unittest;
using namespace orion::unittest::option;

TestSuite(OrionCore)
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

   t.assert<true>(scope_test.test_result().passed(), _src_loc);
}

void check_failure_on_false(Test& t)
{
   Test scope_test("scope test", Suite("testSuite"), [](Test& st) { 
      st.assert<false>(false); 
   });

   t.assert<false>(scope_test.test_result().failed(), _src_loc);
}

void check_function_failure_on_exception(Test& t)
{
   Test scope_test("scope test", Suite("testSuite"), [](Test& st) { 
      st.assert<std::equal_to<>>(calc_throwing_function(), 1); 
   });

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

   t.assert<true>(scope_test.test_result().failed(), _src_loc);
}

void check_expected_exception(Test& t)
{
   Test scope_test("throwing", Suite("testSuite"), [](Test& st) { 
      st.assert_throw<std::logic_error>(throwing_function); 
   });

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

RegisterTestCase(OrionCore, check_success_on_true);
RegisterTestCase(OrionCore, check_failure_on_false);
RegisterTestCase(OrionCore, check_function_failure_on_exception);
RegisterTestCase(OrionCore, check_close_success_on_equal, Disabled{"Not implemented"});
RegisterTestCase(OrionCore, check_close_fails_on_not_equal, Disabled{"Not implemented"});
RegisterTestCase(OrionCore, check_not_expected_exception);
RegisterTestCase(OrionCore, check_expected_exception);
RegisterTestCase(OrionCore, check_test_result_log_success);
RegisterTestCase(OrionCore, check_test_result_log_failure);
} 
