// PlugInManager.cpp
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

#include <orion/plugin/PlugInManager.h>

#include <algorithm>
#include <functional>
#include <orion/ErrorMacros.h>
#include <orion/Logging.h>
#include <orion/Module.h>
#include <orion/ModuleException.h>
#include <orion/plugin/PlugInException.h>

namespace orion
{
namespace plugin
{
using namespace orion::logging;

//-----------------------------------------------------------------------------------
// Predicate class for find

class ByName :
   public std::unary_function<const PlugIn::SharedPtr&, bool>
{
public:
   ByName(const std::string& name) : _name(name) {}

   bool operator() (const PlugIn::SharedPtr& plugin)
   {
      return plugin->name() == _name;
   }

private:
   std::string _name;
};

//-----------------------------------------------------------------------------------
// PlugInManager: implementation
/*!
   Default constructor
 */
PlugInManager::PlugInManager() :
   _plugins(),
   _paths()
{
}

/*!
 */
PlugInManager::~PlugInManager()
{
}

/*!
 */
PlugInManager::iterator PlugInManager::begin()
{
   return _plugins.begin();
}

/*!
 */
PlugInManager::const_iterator PlugInManager::begin() const
{
   return _plugins.begin();
}

/*!
 */
PlugInManager::iterator PlugInManager::end()
{
   return _plugins.end();
}

/*!
 */
PlugInManager::const_iterator PlugInManager::end() const
{
   return _plugins.end();
}

/*!
 */
PlugInManager::reverse_iterator PlugInManager::rbegin()
{
   return _plugins.rbegin();
}

/*!
 */
PlugInManager::const_reverse_iterator PlugInManager::rbegin() const
{
   return _plugins.rbegin();
}

/*!
 */
PlugInManager::reverse_iterator PlugInManager::rend()
{
   return _plugins.rend();
}

/*!
 */
PlugInManager::const_reverse_iterator PlugInManager::rend() const
{
   return _plugins.rend();
}

/*!
 */
PlugIn::SharedPtr PlugInManager::find(const std::string& name)
{
   PlugInManager::iterator iter = std::find_if(begin(), end(), ByName(name));

   if (iter != end())
      return *iter;

   return PlugIn::SharedPtr();
}

/*!
    \param dir directory where we are going to load
    the modules
 */
void PlugInManager::add_path(const std::string& dir)
{
   _paths.push_back(dir);
}

/*!

 */
void PlugInManager::load_modules()
{
   PathList::iterator begin = _paths.begin();
   PathList::iterator end   = _paths.end();
   PathList::iterator iter  = begin;

   for (; iter != end; ++iter)
   {
      try
      {
         load_modules_in_directory(*iter);
      }
      catch (Exception& error) // TODO: Change exception type
      {
         LOG_EXCEPTION(error);
      }
   }
}

/*!
    \param dir_name directory where modules are loaded
 */
void PlugInManager::load_modules_in_directory(const std::string& dir_name)
{
/*
   if (not Glib::file_test(dir_name, Glib::FILE_TEST_EXISTS | Glib::FILE_TEST_IS_DIR))
   {
      return;
   }

   Glib::Dir plugin_files(dir_name);

   Glib::Dir::iterator dir_begin = plugin_files.begin();
   Glib::Dir::iterator dir_end   = plugin_files.end();
   Glib::Dir::iterator dir_iter  = dir_begin;

   for (; dir_iter != dir_end; ++dir_iter)
   {
      if (not Glib::str _has_suffix(*dir_iter, G_MODULE_SUFFIX))
         continue;

      try
      {
         load_module(Glib::build_filename(dir_name, *dir_iter));
      }
      catch (PlugInSymbolNotFoundException& symbol_not_found)
      {
         LOG(Warning) << "Module " << *dir_iter << " not loaded. Entry point not found.";
      }
      catch (PlugInException& e)
      {
         LOG_EXCEPTION(e);
      }
   }
*/   
}

/*!
   \param name module name to load
   \exception PlugInException not able to load module
   \exception PlugInSymbolNotFoundException function create_plugin_instance was not
   found in module
 */
void PlugInManager::load_module(const std::string& name)
{
   Module::SharedPtr module;
   
   try 
   {
      module = Module::create_and_open(name);
   }
   catch (ModuleException& me)
   {
      THROW_EXCEPTION(PlugInException, me.what());
   }

   bool (*func)(PlugIn*& plugin);

   try
   {
      module->get_symbol("create_plugin_instance", reinterpret_cast<void*&>(func));
   }
   catch (ModuleSymbolNotFoundException& snf)
   {
      THROW_EXCEPTION(PlugInSymbolNotFoundException,
                      "Module: " + name + " Symbol not found -> create_plugin_instance");
   }

   PlugIn* plugin = NULL;
   (*func)(plugin);

   _plugins.push_back(PlugIn::SharedPtr(plugin));

   _signal_on_loaded_plugin.emit(_plugins.back());

}

/*!
 */
PlugInManager::OnLoadedPlugIn PlugInManager::signal_on_loaded_plugin()
{
   return _signal_on_loaded_plugin;
}

} // namespace plugin
} // namespace end