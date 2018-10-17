//
// log-example.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Exception.h>
#include <orion/Log.h>
#include <orion/Throw.h>

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

using namespace orion;
using namespace orion::log;

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
   throw_exception<InvalidArgumentError>("No arguments in function c", _src_loc);
}

void setup_logger()
{
   auto cout_handler = make_stream_output_handler(std::cout);

   // cout_handler->set_formatter(make_multiline_formatter());
   // cout_handler->set_formatter(make_one_line_formatter(true));

   auto& logger = default_logger();

   logger.level(Level::Debug);
   logger.add_output_handler(std::move(cout_handler));
}

//----------------------------------------------------------
int main()
{
   setup_logger();

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
      log::exception(e, _src_loc);
   }

   try
   {
      function_c();
   }
   catch (const Exception& e)
   {
      log::exception(e, _src_loc);
   }

   log::shutdown();

   // std::this_thread::sleep_for(2s);

   return EXIT_SUCCESS;
}
