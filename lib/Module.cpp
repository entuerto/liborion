// Module.cpp
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

#include <orion/Module.h>

#include <orion/ErrorMacros.h>
#include <orion/Logging.h>

namespace orion
{
using namespace orion::logging;

/*!
*/
std::unique_ptr<Module> Module::create()
{
   return std::make_unique<Module>();
}
   
/*!
*/
std::unique_ptr<Module> Module::create_and_open(const std::string& file_name)
{
   std::unique_ptr<Module> module = Module::create();

   module->open(file_name);

   return module;
}

} // namespace orion

