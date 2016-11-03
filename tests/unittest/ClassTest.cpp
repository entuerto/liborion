
#include <orion/TestUtils.h>

using namespace orion::unittest;

TEST_SUITE(OrionCore)
{

TEST(PassingTestHasNoFailures)
{
   class PassingTest : public Test
   {
   public:
      PassingTest() : Test("passing") {}
      virtual void execute_test_impl(TestResult* test_result) const
      {
         EXPECT_TRUE(true);
      }
   };

   PassingTest passing_test;

   TestResult* scope_test_result = passing_test.execute_test();

   FAIL_IF(scope_test_result->failed());
}


TEST(FailingTestHasFailures)
{
   class FailingTest : public Test
   {
   public:
      FailingTest() : Test("failing") {}
      virtual void execute_test_impl(TestResult* test_result) const
      {
         EXPECT_FALSE(false);
      }
   };

   FailingTest failing_test;

   TestResult* scope_test_result = failing_test.execute_test();

   EXPECT_FALSE(scope_test_result->failed());
}


TEST(ThrowingTestsAreReportedAsFailures)
{
   class CrashingTest : public Test
   {
   public:
      CrashingTest() : Test("throwing") {}
      virtual void execute_test_impl(TestResult* /* test_result */) const
      {
         throw "Blah";
      }
   };

   CrashingTest crashing_test;

   TestResult* scope_test_result = crashing_test.execute_test();

   EXPECT_TRUE(scope_test_result->failed());
}


TEST(TestWithUnspecifiedSuiteGetsDefaultSuite)
{
    Test test("test");
    EXPECT_NE(test.suite_name(), "");
    EXPECT_EQ("DefaultSuite", test.suite_name());
}


TEST(TestReflectsSpecifiedSuiteName)
{
    Test test("test", "testSuite");
    EXPECT_NE(test.suite_name(), "");
    EXPECT_EQ("testSuite", test.suite_name());
}

TEST(TestHasSameNameAndSuiteNameAsTestResult)
{
   EXPECT_EQ(name(), test_result->name());
   EXPECT_EQ(suite_name(), test_result->suite_name());
}

} // TEST_SUITE(OrionCore)
