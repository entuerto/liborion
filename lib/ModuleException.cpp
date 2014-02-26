// ModuleException.cpp
//
// Copyright 2013 Tomas Palazuelos <tomasp@videotron.ca>
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

#include <orion/ModuleException.h>

namespace orion
{
// ---------------------------------------------------------------------------
// ---- ModuleException ----

ModuleException::ModuleException(const std::string& text) :
   Exception(text)
{
}

ModuleException::ModuleException(const std::string& text,
                                 const std::string& file_name,
                                       int32_t      line_number) :
   Exception(text, file_name, line_number)
{
}

ModuleException::ModuleException(const std::string& text,
                                 const std::string& file_name,
                                       int32_t      line_number,
                                 const std::string& function) :
   Exception(text, file_name, line_number, function)
{
}

ModuleException::ModuleException(const ModuleException& other) :
   Exception(other)
{
}

ModuleException::~ModuleException() throw()
{
}

ModuleException& ModuleException::operator=(const ModuleException& other)
{
   if (this == &other) 
   {
      return *this;
   }

   Exception::operator=(other);

   return *this ;
}

// ---------------------------------------------------------------------------
// ---- ModuleSymbolNotFoundException ----

ModuleSymbolNotFoundException::ModuleSymbolNotFoundException(const std::string& text) :
   ModuleException(text)
{
}

ModuleSymbolNotFoundException::ModuleSymbolNotFoundException(const std::string& text,
                                                             const std::string& file_name,
                                                                   int32_t      line_number) :
   ModuleException(text, file_name, line_number)
{
}

ModuleSymbolNotFoundException::ModuleSymbolNotFoundException(const std::string& text,
                                                             const std::string& file_name,
                                                                   int32_t      line_number,
                                                             const std::string& function) :
   ModuleException(text, file_name, line_number, function)
{
}

ModuleSymbolNotFoundException::ModuleSymbolNotFoundException(const ModuleSymbolNotFoundException& other) :
   ModuleException(other)
{
}

ModuleSymbolNotFoundException::~ModuleSymbolNotFoundException() throw()
{
}

ModuleSymbolNotFoundException& ModuleSymbolNotFoundException::operator=(const ModuleSymbolNotFoundException& other)
{
   if (this == &other) 
   {
      return *this;
   }

   ModuleException::operator=(other);

   return *this ;
}

} // orion
