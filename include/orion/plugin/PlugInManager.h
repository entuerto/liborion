// PlugInManager.h
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

#ifndef ORION_PLUGIN_PLUGINMANAGER_H
#define ORION_PLUGIN_PLUGINMANAGER_H

#include <string>

#include <orion/Orion-Stddefs.h>
#include <orion/MemoryUtils.h>
#include <orion/NonCopyable.h>
#include <orion/Signal.h>
#include <orion/StringUtils.h>
#include <orion/plugin/PlugIn.h>

namespace orion
{
namespace plugin
{
//! Manages the applications plugins
/*!
    The PlugInManager class loads PlugIns at run-time.
 */
class API_EXPORT PlugInManager :
   NonCopyable
{
public:
   DECLARE_POINTERS(PlugInManager)

   typedef std::vector<PlugIn::SharedPtr> PlugInVector;

   typedef PlugInVector::iterator iterator;
   typedef PlugInVector::const_iterator const_iterator;
   typedef PlugInVector::reverse_iterator reverse_iterator;
   typedef PlugInVector::const_reverse_iterator const_reverse_iterator;

   typedef Signal< void(PlugIn::SharedPtr) > OnLoadedPlugIn;

public:
   PlugInManager();
   virtual ~PlugInManager();

   iterator begin();
   const_iterator begin() const;

   iterator end();
   const_iterator end() const;

   reverse_iterator rbegin();
   const_reverse_iterator rbegin() const;

   reverse_iterator rend();
   const_reverse_iterator rend() const;

   //! Finds a plug-in by name
   PlugIn::SharedPtr find(const std::string& name);

   //! Paths where the modules will be loaded
   void add_path(const std::string& dir);

   //! Loads all the modules in the paths specified by add_path()
   void load_modules();

   //! Loads a single module
   void load_module(const std::string& name);

   virtual OnLoadedPlugIn signal_on_loaded_plugin();

protected:
   OnLoadedPlugIn _signal_on_loaded_plugin;

private:
   void load_modules_in_directory(const std::string& dir_name);

private:
   typedef std::vector<std::string> PathList;

   PlugInVector _plugins;
   PathList _paths;

};

} // namespace plugin
} // namespace orion
#endif // ORION_PLUGIN_PLUGINMANAGER_H
