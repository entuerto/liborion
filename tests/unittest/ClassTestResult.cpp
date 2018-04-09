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

Section(OrionCore_Test, Label{"Test"})
{

TestCase("Initial values")
{
   auto tr = TestResult("toto", "toto_suite");

   check_eq("toto"s, tr.name());
   check_eq("toto_suite"s, tr.suite_name());
   check_true(tr.passed());
   check_false(tr.failed());
   check_eq(uint64_t{0}, tr.failed_item_count());
   check_eq(uint64_t{0}, tr.passed_item_count());
}

TestCase("Number of failed items")
{
   auto tr = TestResult("toto", "toto_suite");

   tr.log_failure("1");
   tr.log_failure("2");
   tr.log_failure("3");

   check_eq(uint64_t{3}, tr.failed_item_count());
}

TestCase("Number of passed items")
{
   auto tr = TestResult("toto", "toto_suite");

   tr.log_success("1");
   tr.log_success("2");
   tr.log_success("3");

   check_eq(uint64_t{3}, tr.passed_item_count());
}

} 
