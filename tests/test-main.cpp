//  test-main.cpp
//
//  Created by Tomas Palazuelos on 2016-06-29.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <iostream>

#include <orion/Logging.h>
#include <orion/TestUtils.h>

using namespace orion;
using namespace orion::logging;
using namespace orion::unittest;

#ifdef ORION_TEST_LOGGER
extern LogRecord output_record;

extern void setup_logger(LogRecord& record);
#endif

//----------------------------------------------------------------------------
// Main functions
//----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
#ifdef ORION_TEST_LOGGER
   setup_logger(output_record);
#endif

   LOG_START();

   auto ret = Runner::runner().run(argc, argv);

   LOG_END();

   return ret ? EXIT_SUCCESS : EXIT_FAILURE;
}
