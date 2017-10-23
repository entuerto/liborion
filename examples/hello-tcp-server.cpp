// main.cpp
//
// Copyright 2009 tomas <tomasp@videotron.ca>
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

#include <orion/Log.h>
#include <orion/net/tcp/Server.h>

#include <cstdio>
#include <iostream>

using namespace orion;
using namespace orion::log;
using namespace orion::net;

void setup_logger()
{
   auto cout_handler = std::make_unique<StreamOutputHandler>(std::cout);

   Logger& logger = default_logger();

   logger.level(Level::Debug);
   logger.add_output_handler(std::move(cout_handler));
}

int main()
{
   setup_logger();

   log::start();

   auto server = std::make_unique<tcp::Server>();

   server->register_handler([&](std::streambuf* in, std::streambuf* out) {
      std::cout << "Received :\n";

      std::cout << in << "\n";

      std::ostream o(out);

      o << "Welcome" << "\n";

      return std::error_code();
   });

   std::cout << "Server listening on port: 9090\n";

   try
   {
      std::error_code ec = server->listen_and_serve("", 9090);

      if (ec)
         log::error(ec);
   }
   catch (const std::system_error& e)
   {
      log::exception(e);
   }
   catch (const std::exception& e)
   {
      log::exception(e);
   }

   log::shutdown();
   return EXIT_SUCCESS;
}
