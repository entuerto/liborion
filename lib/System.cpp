// System.cpp
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

#include <orion/System.h>

#include <iomanip>
#include <sstream>

namespace orion
{
namespace sys
{

std::string human_readable(uint64_t value, uint64_t base /* = 1024 */)
{
   uint32_t i = -1;

   // base 1000
   const char* units_si[] = {" kB", " MB", " GB", " TB", "PB", "EB", "ZB", "YB"};
   // base 1024
   const char* units_iec[] = {" kiB", " MiB", " GiB", " TiB", "PiB", "EiB", "ZiB", "YiB"};

   double bytes = value;
   do
   {
      bytes = bytes / base;
      i++;
   } while (bytes > base);

   std::ostringstream oss;

   oss << std::setprecision(2) << bytes << (base == 1000 ? units_si[i] : units_iec[i]);

   return oss.str();
}

} // namespace sys
} // namespace orion
