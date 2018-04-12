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
   auto tr = TestResult();

   check_true(tr.passed());
   check_false(tr.failed());
   check_eq(uint64_t{0}, tr.counters().failed);
   check_eq(uint64_t{0}, tr.counters().passed);
}

TestCase("Number of failed items")
{
   auto tr = TestResult();

   tr.log_failure("1");
   tr.log_failure("2");
   tr.log_failure("3");

   check_eq(uint64_t{3}, tr.counters().failed);
}

TestCase("Number of passed items")
{
   auto tr = TestResult();

   tr.log_success("1");
   tr.log_success("2");
   tr.log_success("3");

   check_eq(uint64_t{3}, tr.counters().passed);
}

} 
