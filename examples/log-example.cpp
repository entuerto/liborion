// module-example.cpp
//
// Copyright 2014 tomas <tomasp@videotron.ca>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.

#include "module-example-lib.h"

#include <iostream>
#include <orion/Log.h>

using namespace orion;
using namespace orion::log;

void function_a()
{
   LOG_FUNCTION(Debug, "function_a()");


   LOG(Warning) << "MACRO: Test warning output from function A";

   log::warning("FUNC: Test warning output from function A");
}

void setup_logger()
{
   auto cout_handler = std::make_unique<StreamOutputHandler>(std::cout);

   //cout_handler->set_formatter(std::make_unique<MultilineFormatter>());
   cout_handler->set_formatter(std::make_unique<OnelineWithSourceInfoFormatter>());

   Logger& logger = Logger::get_logger();

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

   function_a(); 

   log::shutdown();

   return EXIT_SUCCESS;   
}

