/*
 * IP.cpp
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

#include <orion/net/IP.h>
#include <orion/StringUtils.h>

#include <algorithm>

namespace orion
{
namespace net
{
//---------------------------------------------------------------------------------------
// IPv4
IPv4 IPv4::broadcast   = IPv4(255, 255, 255, 255);
IPv4 IPv4::all_systems = IPv4(224, 0, 0, 1);
IPv4 IPv4::all_routers = IPv4(224, 0, 0, 2);
IPv4 IPv4::zero        = IPv4(0, 0, 0, 0);

IPv4::IPv4():
   _a(IPv4::zero._a)
{

}

IPv4::IPv4(uint8_t a, uint8_t b, uint8_t c, uint8_t d):
   _a({ {a, b, c, d} })
{

}

IPv4::IPv4(const IPv4& Other):
   _a(Other._a) 
{

}

IPv4::IPv4(IPv4&& Other):
   _a(std::move(Other._a)) 
{

}

IPv4::~IPv4() 
{

}

IPv4& IPv4::operator=(const IPv4& Rhs) 
{
   if (this == &Rhs)
      return *this;

   std::copy(Rhs._a.begin(), Rhs._a.end(), _a.begin());
   return *this;
}

IPv4& IPv4::operator=(IPv4&& Rhs) 
{
   _a = std::move(Rhs._a);
   return *this;
}

bool IPv4::operator==(const IPv4& Rhs) const
{
   return std::equal(_a.begin(), _a.end(), Rhs._a.begin());
}

bool IPv4::is_loopback() const 
{
   return _a[0] == 127;
}

bool IPv4::is_multicast() const 
{
   return (_a[0] & 0xf0) == 0xe0;
}

bool IPv4::is_unspecified() const 
{
   return std::equal(_a.begin(), _a.end(), zero._a.begin());
}

std::string IPv4::to_string() const 
{
   std::string s;
   std::for_each(_a.begin(), _a.end(), [&](auto &n) { 
      s += std::to_string(n);
      s += "."; 
   });

   if (not s.empty())
      s.pop_back();

   return s;
}

//---------------------------------------------------------------------------------------
// IPv6
IPv6 IPv6::zero                      = IPv6(std::array<uint8_t, 16>{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
IPv6 IPv6::unspecified               = IPv6(std::array<uint8_t, 16>{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
IPv6 IPv6::loopback                  = IPv6(std::array<uint8_t, 16>{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}});
IPv6 IPv6::interface_local_all_nodes = IPv6(std::array<uint8_t, 16>{{0xff, 0x01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x01}});
IPv6 IPv6::link_local_all_nodes      = IPv6(std::array<uint8_t, 16>{{0xff, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x01}});
IPv6 IPv6::link_local_all_routers    = IPv6(std::array<uint8_t, 16>{{0xff, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x02}});

IPv6::IPv6():
   _a(IPv6::zero._a)
{

}

IPv6::IPv6(const std::array<uint8_t, 16>& a):
   _a(a)
{

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

   std::copy(Rhs._a.begin(), Rhs._a.end(), _a.begin());
   return *this;
}

IPv6& IPv6::operator=(IPv6&& Rhs) 
{
   _a = std::move(Rhs._a);
   return *this;
}

bool IPv6::operator==(const IPv6& Rhs) const
{
   return std::equal(_a.begin(), _a.end(), Rhs._a.begin());
}

bool IPv6::is_loopback() const 
{
   return std::equal(_a.begin(), _a.end(), loopback._a.begin());
}

bool IPv6::is_multicast() const 
{
   return _a[0] == 0xff;
}

bool IPv6::is_unspecified() const 
{
   return std::equal(_a.begin(), _a.end(), unspecified._a.begin());
}

std::string IPv6::to_string() const 
{
   std::string s;
   std::for_each(_a.begin(), _a.end(), [&](auto &n) { 
      s += std::to_string(n);
      s += ":"; 
   });

   if (not s.empty())
      s.pop_back();

   return s;
}

} // net
} // orion
