// PlugIn.h
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

#ifndef ORION_PLUGIN_PLUGIN_H
#define ORION_PLUGIN_PLUGIN_H

#include <string>

#include <orion/Orion-Stddefs.h>
#include <orion/MemoryUtils.h>

namespace orion
{
namespace plugin
{
class IExtension;

//! Base PlugIn interface
/*!
    To create a pluggable module for an application the program
    must sub class this class.
 */
class API_EXPORT PlugIn 
{
public:
   NO_COPY(PlugIn)
   NO_MOVE(PlugIn)

   PlugIn(const std::string& name,
          const std::string& description,
          const std::string& version,
          const std::string& author,
          const std::string& copyright);

   virtual ~PlugIn();

   //! The name of the plugin.
   virtual std::string name() const;

   //! The author of the plugin.
   virtual std::string author() const;

   //! The copyright notice of the plugin
   virtual std::string copyright() const;

   //! The description of the plugin.
   virtual std::string description() const;

   //! The version of the plugin.
   virtual std::string version() const;

   //! Enables the PlugIn.
   virtual void enable();

   //! Disables the PlugIn.
   virtual void disable();

   //! Indicates if the PlugIn is enabled.
   virtual bool is_enabled() const;

   virtual bool query_extension(const std::string& name, orion::plugin::IExtension*& extension) =0;

protected:
   virtual bool do_enable() =0;
   virtual bool do_disable() =0;

private:
   std::string _name;
   std::string _description;
   std::string _version;
   std::string _author;
   std::string _copyright;
   bool _is_enabled;
};

} // namespace plugin
} // namespace orion

extern "C"
{
API_EXPORT bool create_plugin_instance(orion::plugin::PlugIn*& plugin);
}

#endif // ORION_PLUGIN_PLUGIN_H
