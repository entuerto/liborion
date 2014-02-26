// PlugIn.cpp
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

#include <orion/plugin/PlugIn.h>

namespace orion
{
namespace plugin
{
/*!
 */
PlugIn::PlugIn(const std::string& name,
               const std::string& description,
               const std::string& version,
               const std::string& author,
               const std::string& copyright) :
   _name(name),
   _description(description),
   _version(version),
   _author(author),
   _copyright(copyright),
   _is_enabled(false)
{
}

/*!
 */
PlugIn::~PlugIn()
{
}

/*!
 */
std::string PlugIn::name() const
{
   return _name;
}

/*!
 */
std::string PlugIn::author() const
{
   return _author;
}

/*!
 */
std::string PlugIn::copyright() const
{
   return _copyright;
}

/*!
 */
std::string PlugIn::description() const
{
   return _description;
}

/*!
 */
std::string PlugIn::version() const
{
   return _version;
}

/*!
 */
bool PlugIn::is_enabled() const
{
   return _is_enabled;
}

/*!
 */
void PlugIn::enable()
{
   if (do_enable())
      _is_enabled = true;
}

/*!
 */
void PlugIn::disable()
{
   if (do_disable())
      _is_enabled = false;
}

} // namespace plugin
} // namespace orion
