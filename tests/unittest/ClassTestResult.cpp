//
//  ClassTestResult.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Test.h>

using namespace std::string_literals;

using namespace orion;
using namespace orion::unittest;
using namespace orion::unittest::option;

TestSuite(OrionCore_Test, Label{"Test"})
{

void initial_values(Test& t)
{
   auto tr = TestResult("toto", "toto_suite");

   t.assert<eq>("toto"s, tr.name(), _src_loc);
   t.assert<eq>("toto_suite"s, tr.suite_name(), _src_loc);
   t.assert<true>(tr.passed(), _src_loc);
   t.assert<false>(tr.failed(), _src_loc);
   t.assert<eq>(std::size_t(0), tr.failed_item_count(), _src_loc);
   t.assert<eq>(std::size_t(0), tr.passed_item_count(), _src_loc);
}

void number_of_failed_items(Test& t)
{
   auto tr = TestResult("toto", "toto_suite");

   tr.log_failure("1");
   tr.log_failure("2");
   tr.log_failure("3");

   t.assert<eq>(std::size_t(3), tr.failed_item_count(), _src_loc);
}

void number_of_passed_items(Test& t)
{
   auto tr = TestResult("toto", "toto_suite");

   tr.log_success("1");
   tr.log_success("2");
   tr.log_success("3");

   t.assert<eq>(std::size_t(3), tr.passed_item_count(), _src_loc);
}

RegisterTestCase(OrionCore_Test, initial_values, Label{"Initial values"});
RegisterTestCase(OrionCore_Test, number_of_failed_items, Label{"Number of failed items"});
RegisterTestCase(OrionCore_Test, number_of_passed_items, Label{"Number of passed items"});
} 
