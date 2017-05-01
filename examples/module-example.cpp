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

#include <functional>
#include <iostream>
#include <memory>

#include <orion/Module.h>
#include <orion/ModuleException.h>

using namespace orion;

//----------------------------------------------------------
int main()
{
   std::unique_ptr<Module> module = std::make_unique<Module>();
   
   try 
   {
      module->open("mod-example-0.1-d.dll");
   }
   catch (const ModuleException& me)
   {
      std::cout << me.what() << "\n";
      return 1;
   }

   std::function<Hello* ()> create_instance;

   try
   {
      create_instance = module->import<Hello* ()>("create_instance");
   }
   catch (const ModuleSymbolNotFoundException& snf)
   {
      std::cout << snf.what() << "\n";
      return 1;
   }

   std::unique_ptr<Hello> hello(create_instance());
   
   hello->print();
}

