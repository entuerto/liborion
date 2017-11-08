//
// module-example-lib.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include "module-example-lib.h"

#include <iostream>

struct HelloWorld : public Hello
{
   virtual void print() { std::cout << "Hello world\n"; }
};

extern "C" {

Hello* create_instance()
{
   return new HelloWorld;
}
}
