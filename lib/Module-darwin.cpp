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
#include <orion/Log.h>

namespace orion
{
using namespace orion::log;

struct Module::Private
{
    bool is_open;
    std::string name;
};

Module::Module() :
   _impl(new Private{false, ""})
{
}

Module::Module(const std::string& file_name) :
   _impl(new Private{false, file_name})
{
}
   
Module::Module(Module&& rhs) :
   _impl(std::move(rhs._impl))
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
    
   _impl->is_open = false;
}

/*!
 Gets a symbol pointer from the module.
 */
void* Module::find_symbol_address(const std::string& symbol_name)
{
   RETURN_VALUE_IF_FAIL(is_open(), nullptr);
   
   return nullptr;
}

Module& Module::operator=(Module&& rhs)
{
   _impl = std::move(rhs._impl);
        
   return *this;
}
    
} // namespace orion

