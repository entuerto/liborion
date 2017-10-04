//  test-main.cpp
//
//  Created by Tomas Palazuelos on 2016-06-29.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#include <orion/TestUtils.h>

using namespace orion;
using namespace orion::unittest;

#include <crtdbg.h>
class LeakDetector {
public:
   LeakDetector() {
      int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
      flag |= _CRTDBG_LEAK_CHECK_DF;
      flag |= _CRTDBG_ALLOC_MEM_DF;
      _CrtSetDbgFlag(flag);
      _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
      _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
      // Change this to leaking allocation's number to break there
      _CrtSetBreakAlloc(-1);
   }
};

LeakDetector ld;
//----------------------------------------------------------------------------
// Main functions
//----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
   auto ret = Runner::runner().run(argc, argv);

   return ret ? EXIT_SUCCESS : EXIT_FAILURE;
}
