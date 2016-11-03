
#include <orion/TestUtils.h>

using namespace orion::unittest;

TEST_SUITE(OrionCore)
{

TEST(Unittest, InitialValues)
{
   auto tr = TestResult::create("toto", "toto_suite");

   EXPECT_EQ(tr->name(), "toto");
   EXPECT_EQ(tr->suite_name(), "toto_suite");
   EXPECT_TRUE(tr->passed());
   FAIL_IF(tr->failed());
   EXPECT_EQ(tr->failed_item_count(), 0);
   EXPECT_EQ(tr->passed_item_count(), 0);
}

TEST(Unittest, NumberOfFailedItems)
{
   auto tr = TestResult::create("toto", "toto_suite");

   tr->on_failure(TestResultItem::create_failure("1"));
   tr->on_failure(TestResultItem::create_failure("2"));
   tr->on_failure(TestResultItem::create_failure("3"));

   EXPECT_EQ(tr->failed_item_count(), 3);
}

TEST(Unittest, NumberOfPassedItems)
{
   auto tr = TestResult::create("toto", "toto_suite");

   tr->on_success(TestResultItem::create_success("1"));
   tr->on_success(TestResultItem::create_success("2"));
   tr->on_success(TestResultItem::create_success("3"));

   EXPECT_EQ(tr->passed_item_count(), 3);
}

TEST(Unittest, TestResultItemSuccessCreation)
{
   auto tri = TestResultItem::create_success("toto", "toto.txt", 99);

   EXPECT_EQ(tri->result(), Result::Passed);
   EXPECT_EQ(tri->message(), "toto");
   EXPECT_EQ(tri->file_name(), "toto.txt");
   EXPECT_EQ(tri->line_number(), 99);
}

TEST(Unittest, TestResultItemFailureCreation)
{
   auto tri = TestResultItem::create_failure("tata", "tata.txt", 88);

   EXPECT_EQ(tri->result(), Result::Failed);
   EXPECT_EQ(tri->message(), "tata");
   EXPECT_EQ(tri->file_name(), "tata.txt");
   EXPECT_EQ(tri->line_number(), 88);
}

} // TEST_SUITE(OrionCore)
