/*
 * Listener.h
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

#ifndef ORION_NET_LISTENER_H
#define ORION_NET_LISTENER_H

#include <orion/Orion-Stddefs.h>
#include <orion/MemoryUtils.h>
#include <orion/net/Connection.h> 
#include <orion/net/IPAddress.h> 

namespace orion
{
namespace net
{

//! This class provides a generic network listener
/*!
   A Listener is a generic network listener for stream-oriented protocols.
 */
class API_EXPORT Listener {
public:
	DECLARE_POINTERS(Listener)

   virtual ~Listener() {}

   // Accept waits for and returns the next connection to the listener.
   virtual Connection::SharedPtr accept() =0;

   // Close closes the listener.
   // Any blocked Accept operations will be unblocked and return errors.
   virtual void close() =0;

   // Addr returns the listener's network address.
   virtual IPAddress::WeakPtr addr() const  =0;
};

//! TCPListener is a TCP network listener. 
class API_EXPORT TcpListener : public Listener {
public:
   ~TcpListener();

   virtual Connection::SharedPtr accept() override;

   virtual void close() override;

   virtual IPAddress::WeakPtr addr() const override;

   static Listener::SharedPtr open_tcp4(const TcpAddress& addr);
   static Listener::SharedPtr open_tcp6(const TcpAddress& addr);

private:
	struct Impl;
   std::unique_ptr<Impl> _impl;

   TcpListener(Impl* impl);
};

} // net
} // orion
#endif 