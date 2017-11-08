//
// System.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
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
