// PlugInException.cpp
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

#include <orion/plugin/PlugInException.h>

namespace orion
{
namespace plugin
{

// ------------------------------------------------------------------------------------------------
// ---- PlugInException ----

PlugInException::PlugInException(const std::string& text, const SourceLocation& src_loc) :
   Exception(text, src_loc)
{
}

PlugInException::PlugInException(const PlugInException& other) :
   Exception(other)
{
}

PlugInException::PlugInException(PlugInException&& other) :
   Exception(std::move(other))
{
}

PlugInException& PlugInException::operator=(const PlugInException& other)
{
   if (this == &other) 
   {
      return *this;
   }

   Exception::operator=(other);

   return *this ;
}

PlugInException& PlugInException::operator=(PlugInException&& other)
{
   Exception::operator=(std::move(other));

   return *this ;
}

// ------------------------------------------------------------------------------------------------
// ---- PlugInSymbolNotFoundException ----

PlugInSymbolNotFoundException::PlugInSymbolNotFoundException(const std::string& text, const SourceLocation& src_loc) :
   PlugInException(text, src_loc)
{
}

PlugInSymbolNotFoundException::PlugInSymbolNotFoundException(const PlugInSymbolNotFoundException& other) :
   PlugInException(other)
{
}

PlugInSymbolNotFoundException::PlugInSymbolNotFoundException(PlugInSymbolNotFoundException&& other) :
   PlugInException(std::move(other))
{
}

PlugInSymbolNotFoundException& PlugInSymbolNotFoundException::operator=(const PlugInSymbolNotFoundException& other)
{
   if (this == &other) 
   {
      return *this;
   }

   PlugInException::operator=(other);

   return *this ;
}

PlugInSymbolNotFoundException& PlugInSymbolNotFoundException::operator=(PlugInSymbolNotFoundException&& other)
{
   PlugInException::operator=(std::move(other));

   return *this ;
}

} // plugin
} // orion
