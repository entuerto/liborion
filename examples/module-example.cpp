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

#include <module-example-lib.h>

#include <iostream>
#include <orion/Module.h>
#include <orion/ModuleException.h>

using namespace orion;

//----------------------------------------------------------
int main()
{
   Module::SharedPtr module;
   
   try 
   {
      module = Module::create_and_open("libmodule-example.dll");
   }
   catch (ModuleException& me)
   {
      std::cout << me.what() << "\n";
      return 1;
   }

   bool (*func)(Hello*& hello);

   try
   {
      module->get_symbol("create_instance", reinterpret_cast<void*&>(func));
   }
   catch (ModuleSymbolNotFoundException& snf)
   {
      std::cout << snf.what() << "\n";
      return 1;
   }

   Hello* hello = nullptr;
   (*func)(hello);
   
   hello->print();

   delete hello;
}

