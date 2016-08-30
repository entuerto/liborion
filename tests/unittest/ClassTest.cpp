
#include <orion/TestUtils.h>

using namespace orion::unittest;

namespace
{

TEST(PassingTestHasNoFailures)
{
   class PassingTest : public Test
   {
   public:
      PassingTest() : Test("passing") {}
      virtual void execute_test_impl(TestResult* test_result) const
      {
         EXPECT(true);
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
         EXPECT(false);
      }
   };

   FailingTest failing_test;

   TestResult* scope_test_result = failing_test.execute_test();

   EXPECT(scope_test_result->failed());
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

   EXPECT(scope_test_result->failed());
}


TEST(TestWithUnspecifiedSuiteGetsDefaultSuite)
{
    Test test("test");
    EXPECT(test.suite_name() != "");
    EXPECT("DefaultSuite" == test.suite_name());
}


TEST(TestReflectsSpecifiedSuiteName)
{
    Test test("test", "testSuite");
    EXPECT(test.suite_name() != "");
    EXPECT("testSuite" == test.suite_name());
}

TEST(TestHasSameNameAndSuiteNameAsTestResult)
{
   EXPECT(name() == test_result->name());
   EXPECT(suite_name() == test_result->suite_name());
}

}
