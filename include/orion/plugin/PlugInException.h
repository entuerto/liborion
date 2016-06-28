// PlugInException.h
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

#ifndef ORION_PLUGIN_PLUGINEXCEPTION_H
#define ORION_PLUGIN_PLUGINEXCEPTION_H

#include <cstdint>
#include <string>

#include <orion/Orion-Stddefs.h>
#include <orion/Exception.h>

namespace orion
{
namespace plugin
{
//! General plugin exception
/*!
    Throw this exception for plugin errors.
 */
class API_EXPORT PlugInException :
   public Exception
{
public:
   PlugInException(const std::string& text);
   PlugInException(const std::string& text,
                   const std::string& file_name,
                         int32_t      line_number);
   PlugInException(const std::string& text,
                   const std::string& file_name,
                         int32_t      line_number,
                   const std::string& function);
   PlugInException(const PlugInException& other);
   PlugInException(PlugInException&& other);
   virtual ~PlugInException() throw();

   PlugInException& operator=(const PlugInException& other);
   PlugInException& operator=(PlugInException&& other);

};

//! Symbol not found exception
/*!
    This exception is thrown when loading the modules
    and we cannot find the entry point in the module.
 */
class API_EXPORT PlugInSymbolNotFoundException :
   public PlugInException
{
public:
   PlugInSymbolNotFoundException(const std::string& text);
   PlugInSymbolNotFoundException(const std::string& text,
                                 const std::string& file_name,
                                       int32_t      line_number);
   PlugInSymbolNotFoundException(const std::string& text,
                                 const std::string& file_name,
                                       int32_t      line_number,
                                 const std::string& function);
   PlugInSymbolNotFoundException(const PlugInSymbolNotFoundException& other);
   PlugInSymbolNotFoundException(PlugInSymbolNotFoundException&& other);
   virtual ~PlugInSymbolNotFoundException() throw();

   PlugInSymbolNotFoundException& operator=(const PlugInSymbolNotFoundException& other);
   PlugInSymbolNotFoundException& operator=(PlugInSymbolNotFoundException&& other);
};

} // namespace plugin
} // namespace orion
#endif /* ORION_PLUGIN_PLUGINEXCEPTION_H */
