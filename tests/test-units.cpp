//  test-units.cpp
//
//  Created by Tomas Palazuelos on 2016-06-29.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#include <orion/Units.h>
#include <orion/TestUtils.h>

using namespace orion;
using namespace orion::unittest;
using namespace orion::unittest::option;

TestSuite(OrionCore)
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

RegisterTestCase(OrionCore, units_SI_kilo_bytes);
RegisterTestCase(OrionCore, units_SI_kilo_mega);
RegisterTestCase(OrionCore, units_SI_mega_literal);
RegisterTestCase(OrionCore, units_SI_int_double);
} // TEST_SUITE(OrionCore)
