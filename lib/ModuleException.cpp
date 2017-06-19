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

ModuleException::ModuleException(const std::string& text, const SourceLocation& src_loc)
   : Exception(text, src_loc)
{
}

ModuleException::ModuleException(const ModuleException& other)
   : Exception(other)
{
}

ModuleException::ModuleException(ModuleException&& other)
   : Exception(std::move(other))
{
}

ModuleException& ModuleException::operator=(const ModuleException& other)
{
   if (this == &other)
   {
      return *this;
   }

   Exception::operator=(other);

   return *this;
}

ModuleException& ModuleException::operator=(ModuleException&& other)
{
   Exception::operator=(std::move(other));

   return *this;
}

// ---------------------------------------------------------------------------
// ---- ModuleSymbolNotFoundException ----

ModuleSymbolNotFoundException::ModuleSymbolNotFoundException(const std::string& text,
                                                             const SourceLocation& src_loc)
   : ModuleException(text, src_loc)
{
}

ModuleSymbolNotFoundException::ModuleSymbolNotFoundException(
   const ModuleSymbolNotFoundException& other)
   : ModuleException(other)
{
}

ModuleSymbolNotFoundException::ModuleSymbolNotFoundException(ModuleSymbolNotFoundException&& other)
   : ModuleException(std::move(other))
{
}

ModuleSymbolNotFoundException& ModuleSymbolNotFoundException::operator=(
   const ModuleSymbolNotFoundException& other)
{
   if (this == &other)
   {
      return *this;
   }

   ModuleException::operator=(other);

   return *this;
}

ModuleSymbolNotFoundException& ModuleSymbolNotFoundException::operator=(
   ModuleSymbolNotFoundException&& other)
{
   ModuleException::operator=(std::move(other));

   return *this;
}

} // orion
