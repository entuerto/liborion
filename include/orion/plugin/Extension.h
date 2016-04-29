// Extension.h
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

#ifndef ORION_PLUGIN_EXTENSION_H
#define ORION_PLUGIN_EXTENSION_H

#include <string>

#include <orion/Orion-Stddefs.h>
#include <orion/NonCopyable.h>

namespace orion
{
namespace plugin
{
//! Extension interface
/*!
   Base class for all extensions in a plugin.
 */
struct IExtension
{
   virtual ~IExtension() {}
};

//! Error extension class
/*!
    Use this class to return and error message to the caller
    of the extension.
 */
class API_EXPORT ExtensionError :
   private NonCopyable,
   public IExtension
{
public:
   ExtensionError(const std::string& message);

   virtual ~ExtensionError();

   virtual std::string message() const;

private:
   std::string _message;

};

} // namespace plugin
} // namespace orion

#endif // ORION_PLUGIN_EXTENSION_H
