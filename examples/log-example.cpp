//
// log-example.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Log.h>

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

using namespace orion;
using namespace std::chrono_literals;

void function_a()
{
   LOG_FUNCTION(Debug, "function_a()");

   LOG(Warning) << "MACRO: Test warning output from function A";

   log::warning("FUNC: Test warning output from function A");
}

void function_b()
{
   throw_exception<std::invalid_argument>("No arguments in function b");
}

void function_c()
{
   throw_exception<InvalidArgumentError>("No arguments in function c", DbgSrcLoc);
}

//----------------------------------------------------------
int main()
{
   log::setup_logger(log::Level::Debug);

   log::start();

   LOG(Error) << "MACRO: Test error output";
   log::error("Test error output");

   LOG(Warning) << "MACRO: Test warning output";
   log::warning("Test warning output");

   const std::string text = "Text from a variable";

   log::write(text);
   log::write(text, " v2");

   function_a();

   try
   {
      function_b();
   }
   catch (const std::exception& e)
   {
      log::exception(e, DbgSrcLoc);
   }

   try
   {
      function_c();
   }
   catch (const Exception& e)
   {
      log::exception(e, DbgSrcLoc);
   }

   log::shutdown();

   // std::this_thread::sleep_for(2s);

   return EXIT_SUCCESS;
}
