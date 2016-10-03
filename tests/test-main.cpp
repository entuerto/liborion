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

int main()
{
#ifdef ORION_TEST_LOGGER
   setup_logger(output_record);
#endif

   LOG_START();

   int ret = run_all_tests(TestStdOutput::create(std::cout, 1));

   LOG_END();

   return ret;
}
