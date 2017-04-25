
#include <orion/TestUtils.h>

using namespace std::string_literals;

using namespace orion;
using namespace orion::unittest;

TestSuite(OrionCore)
{

void initial_values(Test& t)
{
   auto tr = TestResult("toto", "toto_suite");

   t.assert<std::equal_to<>>("toto"s, tr.name(), _src_loc);
   t.assert<std::equal_to<>>("toto_suite"s, tr.suite_name(), _src_loc);
   t.assert<true>(tr.passed(), _src_loc);
   t.assert<false>(tr.failed(), _src_loc);
   t.assert<std::equal_to<>>(std::size_t(0), tr.failed_item_count(), _src_loc);
   t.assert<std::equal_to<>>(std::size_t(0), tr.passed_item_count(), _src_loc);
}

void number_of_failed_items(Test& t)
{
   auto tr = TestResult("toto", "toto_suite");

   tr.log_failure("1");
   tr.log_failure("2");
   tr.log_failure("3");

   t.assert<std::equal_to<>>(std::size_t(3), tr.failed_item_count(), _src_loc);
}

void number_of_passed_items(Test& t)
{
   auto tr = TestResult("toto", "toto_suite");

   tr.log_success("1");
   tr.log_success("2");
   tr.log_success("3");

   t.assert<std::equal_to<>>(std::size_t(3), tr.passed_item_count(), _src_loc);
}

RegisterTestCase(OrionCore, initial_values);
RegisterTestCase(OrionCore, number_of_failed_items);
RegisterTestCase(OrionCore, number_of_passed_items);
} 
