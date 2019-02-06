//
//  test-units.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Log.h>
#include <orion/Units.h>
#include <orion/Test.h>

using namespace orion;
using namespace orion::unittest;

Section(OrionCore_Units, Label{"Units"})
{

TestCase("SI KB neq Bytes")
{
   Size<si::Kilobyte> k = size_cast<Size<si::Kilobyte>>(12345_b);

   check_true(12345 != k);
}

TestCase("SI KB MB")
{
   Size<si::Kilobyte> k = 12345_kb;
   Size<si::Megabyte> m = 12345_mb;

   check_true(k != m);

   Size<si::Megabyte> cm = size_cast<Size<si::Megabyte>>(k);

   // m  = 12345 MB
   // cm = 12 MB
   check_true(cm != m);
}

TestCase("SI MB literal")
{
   Size<si::Megabyte> m = 12345_mb;

   check_true(m == 12345_mb);
}

TestCase("SI MB as double")
{
   Size<si::Megabyte> m = 12345_mb;
   Size<si::Megabyte, double> dm = size_cast<Size<si::Megabyte, double>>(12345_mb);

   check_true(m == dm);
}

} // TEST_SUITE(OrionCore)
