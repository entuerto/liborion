//
//  test-unittests.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Log.h>
#include <orion/Test.h>

using namespace std::string_literals;

using namespace orion;
using namespace orion::unittest;

Section(OrionCore_Test, Label{"Test"})
{

class ScopeTest : public Test
{
public:
   ScopeTest(std::string name, std::function<void(const Test&)> f)
      : Test(std::move(name))
      , func(std::move(f))
   {
   }
   ~ScopeTest() override = default;

protected:
   void do_invoke() const override
   {
      func(*this);
   }

   std::function<void(const Test&)> func;
};
                                                              

int calc_function()
{
   return 0;
}

int calc_throwing_function()
{
   throw std::logic_error("Oh boy");
}

void throwing_function()
{
   throw std::logic_error("Oh boy");
}

TestCase("Passing with no failure")
{
   ScopeTest passing_test("passing", [](const Test& ft) { 
      ft.asserter(eval_true(true, "true")); 
   });

   auto scope_test_result = passing_test.invoke();

   check_true(scope_test_result.passed());
}

TestCase("Failing with no failure")
{
   ScopeTest failing_test("failing", [](const Test& ft) { 
      ft.asserter(eval_false(false, "false")); 
   });

   auto scope_test_result = failing_test.invoke();

   check_false(scope_test_result.failed());
}

TestCase("Throwing reported as failure")
{
   ScopeTest crashing_test("throwing", [](const Test&) { 
      throw std::logic_error("Oh boy"); 
   });

   auto scope_test_result = crashing_test.invoke();

   check_true(scope_test_result.failed());
}

TestCase("Initial values")
{
   auto tr = TestResult{};

   check_true(tr.passed());
   check_false(tr.failed());
   check_eq(uint64_t{0}, tr.counters().failed);
   check_eq(uint64_t{0}, tr.counters().passed);
}

TestCase("Number of failed items")
{
   auto tr = TestResult{};

   tr.log(AssertionFailed{});
   tr.log(AssertionFailed{});
   tr.log(AssertionFailed{});

   check_eq(uint64_t{3}, tr.counters().failed);
}

TestCase("Number of passed items")
{
   auto tr = TestResult{};

   tr.log(AssertionPassed{});
   tr.log(AssertionPassed{});
   tr.log(AssertionPassed{});

   check(uint64_t{3} == tr.counters().passed);
}

TestCase("Success on true")
{
   ScopeTest scope_test("scope test", [](const Test& st) { 
      st.asserter(eval_true(true, "true")); 
   });
   
   scope_test.invoke();

   check_true(scope_test.result().passed());
}

TestCase("Failure on false")
{
   ScopeTest scope_test("scope test", [](const Test& st) { 
      st.asserter(eval_false(false, "false")); 
   });
   
   scope_test.invoke();

   check_false(scope_test.result().failed());
}

TestCase("Function failure with exception")
{
   ScopeTest scope_test("scope test", [](const Test& st) { 
      st.asserter(eval_true(calc_throwing_function() == 1, "calc_throwing_function() == 1")); 
   });

   scope_test.invoke();
   
   check_true(scope_test.result().failed());
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
   fail("Not implemented");
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
   fail("Not implemented");
}

TestCase("Not expected exception")
{
   check_no_throw(calc_function());
}

TestCase("Expected exception")
{
   check_throws(throwing_function());
}

TestCase("Expected specific exception")
{
   check_throws_as(throwing_function(), std::logic_error);
}

TestCase("TestResult::log_success()")
{
   auto test_result = TestResult{};

   test_result.log(AssertionPassed{});

   check_false(test_result.failed());
}

TestCase("TestResult::log_failure()")
{
   auto test_result = TestResult{};

   test_result.log(AssertionFailed{});

   check_true(test_result.failed());
}

} // TestSuite(OrionCore_Test)
