//  test-units.cpp
//
//  Created by Tomas Palazuelos on 2016-06-29.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#include <iostream>
#include <orion/Units.h>
#include <orion/TestUtils.h>

using namespace orion;
using namespace orion::unittest;

//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
TEST(TestSI_kilobytes)
{
   Value<si::Kilobyte> k = 12345;

   EXPECT(k != 12345);
}

TEST(TestSI_kiloMega)
{
   Value<si::Kilobyte> k = 12345;
   Value<si::Megabyte> m = 12345;

   EXPECT(k != m);

   Value<si::Megabyte> cm = k;

   // m  = 12345 MB
   // cm = 12 MB
   EXPECT(cm != m);
}

TEST(TestSI_MegaLiteral)
{
   Value<si::Megabyte> m = 12345;

   EXPECT(m == 12345_mb);
}

TEST(TestSI_IntDouble)
{
   Value<si::Megabyte> m = 12345;
   Value<si::Megabyte, double> dm = 12345;

   EXPECT(m == dm);
}

//----------------------------------------------------------------------------
// Main functions
//----------------------------------------------------------------------------

int main()
{
   int ret = run_all_tests(TestStdOutput::create(std::cout));

   return ret;
}
