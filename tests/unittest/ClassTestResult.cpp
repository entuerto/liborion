
#include <orion/TestUtils.h>

using namespace orion::unittest;

namespace
{

TEST(InitialValues)
{
   TestResult::SharedPtr tr = TestResult::create("toto", "toto_suite");

   EXPECT(tr->name() == "toto");
   EXPECT(tr->suite_name() == "toto_suite");
   EXPECT(tr->passed());
   FAIL_IF(tr->failed());
   EXPECT(tr->failed_item_count() == 0);
   EXPECT(tr->passed_item_count() == 0);
}

TEST(NumberOfFailedItems)
{
   TestResult::SharedPtr tr = TestResult::create("toto", "toto_suite");

   tr->on_failure(TestResultItem::create_failure("1"));
   tr->on_failure(TestResultItem::create_failure("2"));
   tr->on_failure(TestResultItem::create_failure("3"));

   EXPECT(tr->failed_item_count() == 3);
}

TEST(NumberOfPassedItems)
{
   TestResult::SharedPtr tr = TestResult::create("toto", "toto_suite");

   tr->on_success(TestResultItem::create_success("1"));
   tr->on_success(TestResultItem::create_success("2"));
   tr->on_success(TestResultItem::create_success("3"));

   EXPECT(tr->passed_item_count() == 3);
}

TEST(TestResultItemSuccessCreation)
{
   TestResultItem::SharedPtr tri = TestResultItem::create_success("toto", "toto.txt", 99);

   EXPECT(tri->result() == TestResultItem::Passed);
   EXPECT(tri->message() == "toto");
   EXPECT(tri->file_name() == "toto.txt");
   EXPECT(tri->line_number() == 99);
}

TEST(TestResultItemFailureCreation)
{
   TestResultItem::SharedPtr tri = TestResultItem::create_failure("tata", "tata.txt", 88);

   EXPECT(tri->result() == TestResultItem::Failed);
   EXPECT(tri->message() == "tata");
   EXPECT(tri->file_name() == "tata.txt");
   EXPECT(tri->line_number() == 88);
}

}
