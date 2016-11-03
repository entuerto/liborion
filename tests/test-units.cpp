//  test-units.cpp
//
//  Created by Tomas Palazuelos on 2016-06-29.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#include <orion/Units.h>
#include <orion/TestUtils.h>

using namespace orion;
using namespace orion::unittest;

TEST_SUITE(OrionCore)
{
//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
TEST(UnitsSI, kilobytes)
{
   Value<si::Kilobyte> k = 12345;

   EXPECT_NE(k, 12345);
}

TEST(UnitsSI, kiloMega)
{
   Value<si::Kilobyte> k = 12345;
   Value<si::Megabyte> m = 12345;

   EXPECT_NE(k, m);

   Value<si::Megabyte> cm = k;

   // m  = 12345 MB
   // cm = 12 MB
   EXPECT_NE(cm, m);
}

TEST(UnitsSI, MegaLiteral)
{
   Value<si::Megabyte> m = 12345;

   EXPECT_EQ(m, 12345_mb);
}

TEST(UnitsSI, IntDouble)
{
   Value<si::Megabyte> m = 12345;
   Value<si::Megabyte, double> dm = 12345;

   EXPECT_EQ(m, dm);
}

} // TEST_SUITE(OrionCore)
