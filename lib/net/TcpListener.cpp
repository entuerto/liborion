/*
 * TcpListener.cpp
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

#include <orion/net/Listener.h>

#include <orion/Logging.h>

#include <winsock2.h>
#include <ws2tcpip.h>

namespace orion
{
namespace net
{
using namespace orion::logging;

struct TcpListener::Impl 
{
   std::unique_ptr<IPAddress> addr;

   SOCKET sock;
};

TcpListener::TcpListener(TcpListener::Impl* impl):
   Listener(),
   _impl(impl)
{

}

TcpListener::~TcpListener() 
{

}

std::unique_ptr<Connection> TcpListener::accept() 
{
   return nullptr;
}

void TcpListener::close() 
{

}

IPAddress* TcpListener::addr() const 
{
   return _impl->addr.get();
}

std::unique_ptr<Listener> TcpListener::open_tcp4(const TcpAddress& addr) 
{
   DWORD  flags = 0x01;

   SOCKET sock = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, flags);

   if (sock == INVALID_SOCKET)
      return nullptr;

   auto imp = new TcpListener::Impl;

   imp->sock = sock;
   imp->addr = std::make_unique<TcpAddress>(addr);

   return std::unique_ptr<Listener>(new TcpListener(imp));
}

std::unique_ptr<Listener> TcpListener::open_tcp6(const TcpAddress& /* addr */) 
{
   return nullptr;
}


} // ws
} // orion
