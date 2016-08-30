/*
 * Request.cpp
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

#include <orion/ws/Request.h>

#include <cstdlib>
#include <orion/StringUtils.h>

namespace orion
{
namespace ws
{

Request::Request() :
   _remote_address(),
   _host_address(),
   _http_header(),
   _data()
{
}

Request::~Request()
{
}

//! Client's IP address
const net::IPAddress* Request::remote_address() const
{
   return _remote_address.get();
}

//! Host IP address
const net::IPAddress* Request::host_address() const
{
   return _host_address.get();
}

std::string Request::header(const std::string& name) const
{
   auto it = _http_header.find(name);

   if (it != _http_header.end())
      return it->second;

   return "";
}

std::string Request::content() const
{
   return _data;
}

} // ws
} // orion

