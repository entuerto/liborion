// Module.h
//
// Copyright 2013 tomas <tomasp@videotron.ca>
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

#ifndef ORION_MODULE_H
#define ORION_MODULE_H

#include <memory>
#include <string>

#include <orion/Orion-Stddefs.h>
#include <orion/MemoryUtils.h>
#include <orion/NonCopyable.h>

#ifdef __cplusplus
extern "C" {
#endif 

namespace orion
{
/*!
   Dynamic Loading of Modules These functions provide a portable way to 
   dynamically load object files. 
 */
class API_EXPORT Module :
   private NonCopyable
{
public:
   DECLARE_POINTERS(Module)

   Module();
   ~Module();

   //! Module name
   std::string name() const;

   //! Is module open (loaded)
   bool is_open() const;

   //! Load library
   void open(const std::string& file_name);

   //! Close library
   void close();

   //! Gets a symbol pointer from the module.
   void get_symbol(const std::string& symbol_name, void*& symbol);

   
   static Module::SharedPtr create();
   
   static Module::SharedPtr create_and_open(const std::string& file_name);

private:
   struct Private;
   std::unique_ptr<Private> _impl;
};

} // namespace orion

#ifdef __cplusplus
}
#endif

#endif

