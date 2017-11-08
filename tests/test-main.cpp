//
//  test-main.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
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
