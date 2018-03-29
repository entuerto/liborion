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

TestSuite(OrionCore_Units, Label{"Units"})
{
//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
void units_SI_kilo_bytes(Test& t)
{
   Value<si::Kilobyte> k = 12345;

   t.assert<true>(12345 != k, _src_loc, Disabled{"No time to fix"});
}

void units_SI_kilo_mega(Test& t)
{
   Value<si::Kilobyte> k = 12345;
   Value<si::Megabyte> m = 12345;

   t.assert<true>(k != m, _src_loc);

   Value<si::Megabyte> cm = k;

   // m  = 12345 MB
   // cm = 12 MB
   t.assert<true>(cm != m, _src_loc);
}

void units_SI_mega_literal(Test& t)
{
   Value<si::Megabyte> m = 12345;

   t.assert<true>(m == 12345_mb, _src_loc);
}

void units_SI_int_double(Test& t)
{
   Value<si::Megabyte> m = 12345;
   Value<si::Megabyte, double> dm = 12345;

   t.assert<true>(m == dm, _src_loc);
}

RegisterTestCase(OrionCore_Units, units_SI_kilo_bytes, Label{"SI KB neq Bytes"});
RegisterTestCase(OrionCore_Units, units_SI_kilo_mega, Label{"SI KB MB"});
RegisterTestCase(OrionCore_Units, units_SI_mega_literal, Label{"SI MB literal"});
RegisterTestCase(OrionCore_Units, units_SI_int_double, Label{"SI MB as double"});
} // TEST_SUITE(OrionCore)
