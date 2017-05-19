// ModuleException.h
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

#ifndef ORION_MODULEEXCEPTION_H
#define ORION_MODULEEXCEPTION_H

#include <orion/Orion-Stddefs.h>
#include <orion/Exception.h>
#include <string>

namespace orion
{
//! General module exception
/*!
    Throw this exception for module errors.
 */
class API_EXPORT ModuleException : public Exception
{
public:
   ModuleException(const std::string& text, const SourceLocation& src_loc = SourceLocation{});

   ModuleException(const ModuleException& other);
   ModuleException(ModuleException&& other);

   virtual ~ModuleException() = default;

   ModuleException& operator=(const ModuleException& other);
   ModuleException& operator=(ModuleException&& other);
};

//! Symbol not found exception
/*!
    This exception is thrown when loading the modules
    and we cannot find the symbol in the module.
 */
class API_EXPORT ModuleSymbolNotFoundException : public ModuleException
{
public:
   ModuleSymbolNotFoundException(const std::string& text,
                                 const SourceLocation& src_loc = SourceLocation{});

   ModuleSymbolNotFoundException(const ModuleSymbolNotFoundException& other);
   ModuleSymbolNotFoundException(ModuleSymbolNotFoundException&& other);

   virtual ~ModuleSymbolNotFoundException() = default;

   ModuleSymbolNotFoundException& operator=(const ModuleSymbolNotFoundException& other);
   ModuleSymbolNotFoundException& operator=(ModuleSymbolNotFoundException&& other);
};

} // namespace orion
#endif /* ORION_MODULEEXCEPTION_H */
