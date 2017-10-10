//  test-main.cpp
//
//  Created by Tomas Palazuelos on 2016-06-29.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#include <orion/Test.h>

using namespace orion;
using namespace orion::unittest;

//----------------------------------------------------------------------------
// Main functions
//----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
   auto ret = Runner::runner().run(argc, argv);

   return ret ? EXIT_SUCCESS : EXIT_FAILURE;
}
