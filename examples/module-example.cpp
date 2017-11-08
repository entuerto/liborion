//
// module-example.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include "module-example-lib.h"

#include <orion/Module.h>
#include <orion/ModuleException.h>

#include <functional>
#include <iostream>
#include <memory>

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

   std::function<Hello*()> create_instance;

   try
   {
      create_instance = module->import<Hello*()>("create_instance");
   }
   catch (const ModuleSymbolNotFoundException& snf)
   {
      std::cout << snf.what() << "\n";
      return 1;
   }

   std::unique_ptr<Hello> hello(create_instance());

   hello->print();
}
