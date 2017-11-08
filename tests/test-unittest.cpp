//
// test-unittest.cpp
// 
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
// 
#include <orion/Test.h>
#include <iostream>

int main()
{
   // Help with uncaught exceptions in g++
   //std::set_terminate(__gnu_cxx::__verbose_terminate_handler);

   using namespace orion::unittest;

   return run_all_tests(TestStdOutput::create(std::cout));
}
