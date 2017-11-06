/*
 * IPv4-darwin.cpp
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

#include <orion/net/AddressV4.h>

#include <orion/StringUtils.h>

namespace orion
{
namespace net
{

AddressV4::AddressV4(uint32_t value)
   : _a()
{
   _a.s_addr = htonl(value);
}

uint32_t AddressV4::to_ulong() const
{
   return ntohl(_a.s_addr);
}

std::string to_string(const AddressV4& addr) 
{
   
   return "";
}

AddressV4 make_address_v4(const std::string& value)
{
   
   return AddressV4();
}

} // net
} // orion
