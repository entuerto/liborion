//
// Module-win32.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Module.h>

#include <orion/ErrorMacros.h>
#include <orion/Exception.h>
#include <orion/Log.h>
#include <orion/SourceLocation.h>
#include <orion/Throw.h>

#include <host/win32/String.h>

#include <windows.h>

namespace orion
{

//-------------------------------------------------------------------------------------------------

struct Module::Private
{
   HMODULE handle;
   bool is_open;
   std::string name;
};

//-------------------------------------------------------------------------------------------------

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

///
/// Module name
///
std::string Module::name() const
{
   return _impl->name;
}

///
/// Is module open (loaded)
///
bool Module::is_open() const
{
   return _impl->is_open;
}

///
/// Load library
///
void Module::open(const std::string& file_name)
{
   std::wstring ws_file_name = win32::utf8_to_wstring(file_name);

   _impl->handle = LoadLibraryExW(ws_file_name.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

   if (_impl->handle == nullptr)
   {
      std::string error_message;

      win32::format_error_message(GetLastError(), error_message);
      throw_exception<ModuleException>(error_message, _src_loc);
   }

   _impl->is_open = true;
   _impl->name    = file_name;
}

///
/// Close library
///
void Module::close()
{
   if (not _impl->is_open)
      return;

   FreeLibrary(_impl->handle);
   _impl->is_open = false;
}

///
/// Finds a symbol pointer in the library.
///
void* Module::find_symbol_address(const std::string& symbol_name)
{
   RETURN_VALUE_IF_FAIL(is_open(), nullptr);

   // reinterpret_cast
   void* symbol = reinterpret_cast<void*>(GetProcAddress(_impl->handle, symbol_name.c_str()));

   if (symbol == nullptr)
   {
      std::string error_message;

      win32::format_error_message(GetLastError(), error_message);
      throw_exception<ModuleSymbolNotFoundException>(error_message, _src_loc);
   }

   return symbol;
}

Module& Module::operator=(Module&& rhs) noexcept
{
   _impl = std::move(rhs._impl);

   return *this;
}

} // namespace orion
