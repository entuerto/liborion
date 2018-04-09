//
//  test-units.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Units.h>
#include <orion/Test.h>

using namespace orion;
using namespace orion::unittest;
using namespace orion::unittest::option;

Section(OrionCore_Units, Label{"Units"})
{

TestCase("SI KB neq Bytes")
{
   Value<si::Kilobyte> k = 12345;

   check_true(12345 != k, Disabled{"No time to fix"});
}

TestCase("SI KB MB")
{
   Value<si::Kilobyte> k = 12345;
   Value<si::Megabyte> m = 12345;

   check_true(k != m);

   Value<si::Megabyte> cm = k;

   // m  = 12345 MB
   // cm = 12 MB
   check_true(cm != m);
}

TestCase("SI MB literal")
{
   Value<si::Megabyte> m = 12345;

   check_true(m == 12345_mb);
}

TestCase("SI MB as double")
{
   Value<si::Megabyte> m = 12345;
   Value<si::Megabyte, double> dm = 12345;

   check_true(m == dm);
}

} // TEST_SUITE(OrionCore)
