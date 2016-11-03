//  test-main.cpp
//
//  Created by Tomas Palazuelos on 2016-06-29.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <iostream>

#include <orion/Logging.h>
#include <orion/TestUtils.h>

#include <boost/program_options.hpp>

using namespace orion;
using namespace orion::logging;
using namespace orion::unittest;

namespace opts = boost::program_options;

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
   ReportLevel level = ReportLevel::Error;

   opts::options_description desc("Test options");
   desc.add_options()
      ("help,h", "Help message")
      ("level,l", opts::value<ReportLevel>(&level)->default_value(ReportLevel::Error), "Set report output level");

   opts::variables_map vm;
   opts::store(opts::parse_command_line(argc, argv, desc), vm);
   opts::notify(vm);

   if (vm.count("help")) 
   {
      std::cout << desc 
                << "\n";
      return EXIT_FAILURE;
   }

   LOG_START();

   int ret = run_all_tests(TestStdOutput::create(std::cout, level));

   LOG_END();

   return ret;
}
