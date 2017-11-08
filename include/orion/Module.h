//
// Module.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_MODULE_H
#define ORION_MODULE_H

#include <orion/Orion-Stddefs.h>

#include <functional>
#include <memory>
#include <string>

namespace orion
{
/*!
   Dynamic Loading of Modules These functions provide a portable way to
   dynamically load object files.
 */
class API_EXPORT Module
{
public:
   NO_COPY(Module)

   Module();
   Module(const std::string& file_name);

   Module(Module&& rhs);

   ~Module();

   //! Module name
   std::string name() const;

   //! Is module open (loaded)
   bool is_open() const;

   //! Load library
   void open(const std::string& file_name);

   //! Close library
   void close();

   //! Finds a symbol pointer in the library.
   void* find_symbol_address(const std::string& symbol_name);

   template<class T>
   std::function<T> import(const std::string& name);

   Module& operator=(Module&& rhs);

private:
   struct Private;
   std::unique_ptr<Private> _impl;
};

//-------------------------------------------------------------------------------------------------

template<class T>
std::function<T> Module::import(const std::string& name)
{
   auto f = reinterpret_cast<T*>(find_symbol_address(name));
   return std::function<T>(f);
}

} // namespace orion

#endif
