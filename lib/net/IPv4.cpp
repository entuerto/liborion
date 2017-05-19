/*
 * IPv4.cpp
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

#include <orion/net/IPv4.h>

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

IPv4::IPv4()
   : _a(IPv4::zero._a)
{
}

IPv4::IPv4(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
   : _a({{a, b, c, d}})
{
}

IPv4::IPv4(const std::array<uint8_t, 4>& b)
   : _a()
{
   std::copy(std::begin(b), std::end(b), std::begin(_a.b));
}

IPv4::IPv4(std::initializer_list<uint8_t> l)
   : _a()
{
   std::copy(l.begin(), l.end(), std::begin(_a.b));
}

IPv4::IPv4(const IPv4& Other)
   : _a(Other._a)
{
}

IPv4::IPv4(IPv4&& Other)
   : _a(std::move(Other._a))
{
}

IPv4::~IPv4()
{
}

IPv4& IPv4::operator=(const IPv4& Rhs)
{
   if (this == &Rhs)
      return *this;

   _a = Rhs._a;

   return *this;
}

IPv4& IPv4::operator=(IPv4&& Rhs)
{
   _a = std::move(Rhs._a);
   return *this;
}

IPv4& IPv4::operator=(std::initializer_list<uint8_t> l)
{
   std::copy(l.begin(), l.end(), std::begin(_a.b));
   return *this;
}

bool IPv4::operator==(const IPv4& Rhs) const
{
   return _a.l == Rhs._a.l;
}

/// Determine whether the address is a class A address.
bool IPv4::is_class_a() const
{
   return (_a.l & 0x80000000) == 0;
}

/// Determine whether the address is a class B address.
bool IPv4::is_class_b() const
{
   return (_a.l & 0xC0000000) == 0x80000000;
}

/// Determine whether the address is a class C address.
bool IPv4::is_class_c() const
{
   return (_a.l & 0xE0000000) == 0xC0000000;
}

bool IPv4::is_loopback() const
{
   return _a.b[0] == 127;
}

bool IPv4::is_multicast() const
{
   return (_a.b[0] & 0xf0) == 0xe0;
}

bool IPv4::is_unspecified() const
{
   return _a.l == 0;
}

uint8_t* IPv4::to_bytes() const
{
   return const_cast<uint8_t*>(&(_a.b[0]));
}

std::string IPv4::to_string() const
{
   std::string s;

   s = std::to_string(_a.b[0]);
   s += ".";
   s += std::to_string(_a.b[1]);
   s += ".";
   s += std::to_string(_a.b[2]);
   s += ".";
   s += std::to_string(_a.b[3]);

   return s;
}

} // net
} // orion
