/*
 * IPv6.cpp
 *
 * Copyright 2013 tomas <tomasp@videotron.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include <orion/net/IPv6.h>

#include <algorithm>

#include <orion/StringUtils.h>

namespace orion
{
namespace net
{

//---------------------------------------------------------------------------------------
// IPv6
IPv6 IPv6::zero                      = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
IPv6 IPv6::unspecified               = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
IPv6 IPv6::loopback                  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
IPv6 IPv6::interface_local_all_nodes = {0xff, 0x01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x01};
IPv6 IPv6::link_local_all_nodes      = {0xff, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x01};
IPv6 IPv6::link_local_all_routers    = {0xff, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x02};

IPv6::IPv6():
   _a(IPv6::zero._a)
{

}

IPv6::IPv6(const std::array<uint8_t, 16>& b):
   _a()
{
   std::copy(std::begin(b), std::end(b), std::begin(_a.b));
}

IPv6::IPv6(std::initializer_list<uint8_t> l):
   _a()
{
   std::copy(l.begin(), l.end(), std::begin(_a.b));
}

IPv6::IPv6(const IPv6& Other):
   _a(Other._a) 
{

}

IPv6::IPv6(IPv6&& Other):
   _a(std::move(Other._a)) 
{

}

IPv6::~IPv6() 
{

}

IPv6& IPv6::operator=(const IPv6& Rhs) 
{
   if (this == &Rhs)
      return *this;

   std::copy(std::begin(Rhs._a.b), std::end(Rhs._a.b), std::begin(_a.b));

   return *this;
}

IPv6& IPv6::operator=(IPv6&& Rhs) 
{
   _a = std::move(Rhs._a);
   return *this;
}

IPv6& IPv6::operator=(std::initializer_list<uint8_t> l)
{
   std::copy(l.begin(), l.end(), std::begin(_a.b));
   return *this;
}

bool IPv6::operator==(const IPv6& Rhs) const
{
   return std::equal(std::begin(_a.b), std::end(_a.b), std::begin(Rhs._a.b));
}

bool IPv6::is_loopback() const 
{
   return std::equal(std::begin(_a.b), std::end(_a.b), std::begin(loopback._a.b));
}

bool IPv6::is_multicast() const 
{
   return _a.b[0] == 0xff;
}

bool IPv6::is_unspecified() const 
{
   return std::equal(std::begin(_a.b), _a.b + 16, std::begin(unspecified._a.b));
}

uint8_t* IPv6::to_bytes() const
{
   return const_cast<uint8_t*>(&(_a.b[0]));
}

std::string IPv6::to_string() const 
{
   std::string s;
   std::for_each(_a.b, _a.b + 16, [&](auto &n) { 
      s += std::to_string(n);
      s += ":"; 
   });

   if (not s.empty())
      s.pop_back();

   return s;
}

} // net
} // orion
