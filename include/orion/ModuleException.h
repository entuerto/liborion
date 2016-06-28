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

#include <cstdint>
#include <string>
#include <orion/Exception.h>

namespace orion
{
//! General module exception
/*!
    Throw this exception for module errors.
 */
class ModuleException :
   public Exception
{
public:
   ModuleException(const std::string& text);
   ModuleException(const std::string& text,
                   const std::string& file_name,
                         int32_t      line_number);
   ModuleException(const std::string& text,
                   const std::string& file_name,
                         int32_t      line_number,
                   const std::string& function);
   ModuleException(const ModuleException& other);
   ModuleException(const ModuleException&& other);
   virtual ~ModuleException() throw();

   ModuleException& operator=(const ModuleException& other);
   ModuleException& operator=(const ModuleException&& other);

};

//! Symbol not found exception
/*!
    This exception is thrown when loading the modules
    and we cannot find the symbol in the module.
 */
class ModuleSymbolNotFoundException :
   public ModuleException
{
public:
   ModuleSymbolNotFoundException(const std::string& text);
   ModuleSymbolNotFoundException(const std::string& text,
                                 const std::string& file_name,
                                       int32_t      line_number);
   ModuleSymbolNotFoundException(const std::string& text,
                                 const std::string& file_name,
                                       int32_t      line_number,
                                 const std::string& function);
   ModuleSymbolNotFoundException(const ModuleSymbolNotFoundException& other);
   ModuleSymbolNotFoundException(const ModuleSymbolNotFoundException&& other);
   virtual ~ModuleSymbolNotFoundException() throw();

   ModuleSymbolNotFoundException& operator=(const ModuleSymbolNotFoundException& other);
   ModuleSymbolNotFoundException& operator=(const ModuleSymbolNotFoundException&& other);
};

} // namespace orion
#endif /* ORION_MODULEEXCEPTION_H */
