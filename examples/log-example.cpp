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
#include <orion/Logging.h>

using namespace orion;
using namespace orion::logging;

void function_a()
{
   LOG_FUNCTION(Debug, "function_a()");


   LOG(Warning) << "Test warning output from function A";
}

void setup_logger()
{
   auto cout_handler = StreamOutputHandler::create(std::cout);

   //cout_handler->set_formatter(MultilineFormatter::create());
   //cout_handler->set_formatter(OnelineWithSourceInfoFormatter::create());

   Logger& logger = Logger::get_logger();

   logger.level(Level::Debug);
   logger.output_handlers().push_back(std::move(cout_handler));
}

//----------------------------------------------------------
int main()
{
   setup_logger();

   LOG_START();

   LOG(Error) << "Test error output";
   LOG(Warning) << "Test warning output";

   function_a();

   LOG_END();
   return EXIT_SUCCESS;   
}

