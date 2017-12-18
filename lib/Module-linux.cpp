//
// Module-linux.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Module.h>

#include <orion/Exception.h>
#include <orion/ErrorMacros.h>
#include <orion/Log.h>
#include <dlfcn.h>

namespace orion
{
using namespace orion::log;

struct Module::Private
{
   void* handle;
   bool is_open;
   std::string name;
};

Module::Module()
   : _impl(new Private{nullptr, false, ""})
{
}

Module::Module(const std::string& file_name)
   : _impl(new Private{nullptr, false, file_name})
{
}

Module::Module(Module&& rhs) noexcept
   : _impl(std::move(rhs._impl))
{
}

Module::~Module()
{
   close();
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
      throw_exception<ModuleException>(dlerror(), _src_loc);

   _impl->is_open = true;
   _impl->name    = file_name;
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
void* Module::find_symbol_address(const std::string& symbol_name)
{
   RETURN_VALUE_IF_FAIL(is_open(), nullptr);

   dlerror(); /* Reset error status. */

   void* symbol = dlsym(_impl->handle, symbol_name);

   if (symbol == nullptr)
      throw_exception<ModuleSymbolNotFoundException>(dlerror(), _src_loc);

   return symbol;
}

Module& Module::operator=(Module&& rhs) noexcept
{
   _impl = std::move(rhs._impl);

   return *this;
}

} // namespace orion
