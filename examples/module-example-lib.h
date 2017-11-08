//
// module-example-lib.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef MODULE_EXAMPLE_H
#define MODULE_EXAMPLE_H

#include <orion/Orion-Stddefs.h>

struct Hello
{
   virtual void print() = 0;
};

extern "C" {
API_EXPORT Hello* create_instance();
}

#endif
