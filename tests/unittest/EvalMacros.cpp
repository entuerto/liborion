
#include <stdexcept>

#include <orion/TestUtils.h>
#include <orion/ErrorMacros.h>
#include <orion/Exception.h>

using namespace orion::unittest;

TEST_SUITE(OrionCore)
{

TEST(CheckSuccessOnTrue)
{
   bool failure = true;
   {
      auto r = TestResult::create("scope test result");
      TestResult* test_result = r.get();

      EXPECT(true);

      failure = test_result->failed();
   }

   EXPECT(not failure);
}

TEST(CheckFailureOnFalse)
{
   bool failure = true;
   {
      auto r = TestResult::create("scope test result");
      TestResult* test_result = r.get();

      EXPECT(false);

      failure = test_result->failed();
   }

   EXPECT(failure);
}

int throwing_function()
{
   THROW_EXCEPTION(orion::Exception, "Oh boy")
}

TEST(CheckFailureOnException)
{
   bool failure = false;
   {
      auto r = TestResult::create("scope test result");
      TestResult* test_result = r.get();

      EXPECT(throwing_function() == 1);

      failure = test_result->failed();
   }

   EXPECT(failure);
}

int side_effect = 0;
int function_with_side_effects()
{
   ++side_effect;
   return 1;
}

TEST(CheckEqualDoesNotHaveSideEffectsWhenPassing)
{
   side_effect = 0;
   {
      auto r = TestResult::create("scope test result");
      TestResult* test_result = r.get();

      EXPECT(1 == function_with_side_effects());
   }
   EXPECT(1 == side_effect);
}

TEST(CheckCloseSuccessOnEqual)
{
   bool failure = true;
   {
      auto r = TestResult::create("scope test result");
      TestResult* test_result = r.get();

      EXPECT_CLOSE(1.0f, 1.001f, 0.01f);

      failure = test_result->failed();
   }

   EXPECT(not failure);
}

TEST(CheckCloseFailsOnNotEqual)
{
   bool failure = false;
   {
      auto r = TestResult::create("scope test result");
      TestResult* test_result = r.get();

      EXPECT_CLOSE(1.0f, 1.1f, 0.01f);

      failure = test_result->failed();
   }

   EXPECT(failure);
}

TEST(CheckCloseFailsOnException)
{
   bool failure = false;
   {
      auto r = TestResult::create("scope test result");
      TestResult* test_result = r.get();

      EXPECT_CLOSE((float)throwing_function(), 1.0001f, 0.1f);

      failure = test_result->failed();
   }

   EXPECT(failure);
}

TEST(CheckExpectThrowOnSuccess)
{
   bool failure = true;
   {
      auto r = TestResult::create("scope test result");
      TestResult* test_result = r.get();

      EXPECT_THROW(throwing_function(), orion::Exception);

      failure = test_result->failed();
   }

   EXPECT(not failure);
}

TEST(CheckExpectThrowOnFailure)
{
   bool failure = false;
   {
      auto r = TestResult::create("scope test result");
      TestResult* test_result = r.get();

      EXPECT_THROW(throwing_function(), std::logic_error);

      failure = test_result->failed();
   }

   EXPECT(failure);
}

TEST(CheckFailIfOnTrue)
{
   bool failure = false;
   {
      auto r = TestResult::create("scope test result");
      TestResult* test_result = r.get();

      FAIL_IF(true);

      failure = test_result->failed();
   }

   EXPECT(failure);
}

TEST(CheckFailIfOnFalse)
{
   bool failure = false;
   {
      auto r = TestResult::create("scope test result");
      TestResult* test_result = r.get();

      FAIL_IF(false);

      failure = test_result->failed();
   }

   EXPECT(not failure);
}

} // TEST_SUITE(OrionCore)
