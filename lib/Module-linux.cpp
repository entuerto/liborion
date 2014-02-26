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

#include <dlfcn.h>
#include <orion/ErrorMacros.h>
#include <orion/ModuleException.h>
#include <orion/Logging.h>

namespace orion
{
using namespace orion::logging;

struct Module::Private
{
   void* handle;
   bool is_open;
   std::string name;
};

Module::Module() :
   _impl(new Private)
{
   _impl->handle = nullptr;
   _impl->is_open = false;
}

Module::~Module()
{
}

/*! 
   Module name
*/
std::string Module::name() const
{
   return _impl->name;
}

/*! 
   Is module open (loaded)
*/
bool Module::is_open() const
{
   return _impl->is_open;
}

/*! 
   Load library
*/
void Module::open(const std::string& file_name)
{
   dlerror(); /* Reset error status. */

   _impl->handle = dlopen(file_name, RTLD_LAZY);

   if (_impl->handle == nullptr)
      THROW_EXCEPTION(ModuleException, dlerror());

   _impl->is_open = true;
   _impl->name = file_name;
}

/*! 
   Close library
*/
void Module::close()
{
   if (not _impl->is_open)
      return;

   dlclose(_impl->handle);
   _impl->is_open = false;
}

/*! 
   Gets a symbol pointer from the module.
 */
void Module::get_symbol(const std::string& symbol_name, void*& symbol)
{
   RETURN_IF_FAIL(is_open());

   dlerror(); /* Reset error status. */

   symbol = dlsym(_impl->handle, symbol_name);

   if (symbol == nullptr)
      THROW_EXCEPTION(ModuleSymbolNotFoundException, dlerror());

   return symbol != nullptr;
}

} // namespace orion

