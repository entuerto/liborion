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
#include <orion/ModuleException.h>
#include <orion/SourceLocation.h>
#include <orion/StringUtils.h>
#include <orion/ThrowUtils.h>

#include <windows.h>

namespace orion
{

static void get_last_error_message(DWORD last_error_code, std::string& error_message)
{
   wchar_t* buffer;

   FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                     FORMAT_MESSAGE_IGNORE_INSERTS,
                  nullptr,
                  last_error_code,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPWSTR)&buffer,
                  0,
                  nullptr);

   if (buffer)
   {
      error_message = wstring_to_utf8(buffer);

      LocalFree(buffer);
   }
}

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

Module::Module(Module&& rhs)
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
   _impl->handle =
      LoadLibraryExW(utf8_to_wstring(file_name).c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

   if (_impl->handle == nullptr)
   {
      std::string error_message;

      get_last_error_message(GetLastError(), error_message);
      throw_exception<ModuleException>(error_message, _src_loc);
   }

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

   FreeLibrary(_impl->handle);
   _impl->is_open = false;
}

/*!
   Finds a symbol pointer in the library.
 */
void* Module::find_symbol_address(const std::string& symbol_name)
{
   RETURN_VALUE_IF_FAIL(is_open(), nullptr);

   // reinterpret_cast
   void* symbol = reinterpret_cast<void*>(GetProcAddress(_impl->handle, symbol_name.c_str()));

   if (symbol == nullptr)
   {
      std::string error_message;

      get_last_error_message(GetLastError(), error_message);
      throw_exception<ModuleSymbolNotFoundException>(error_message, _src_loc);
   }

   return symbol;
}

Module& Module::operator=(Module&& rhs)
{
   _impl = std::move(rhs._impl);

   return *this;
}

} // namespace orion
