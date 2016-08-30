/*
 * Connection.h
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

#ifndef ORION_NET_CONNECTION_H
#define ORION_NET_CONNECTION_H

#include <orion/Orion-Stddefs.h>
#include <orion/MemoryUtils.h>
#include <orion/net/IPAddress.h>

namespace orion
{
namespace net
{

//! This class provides a generic network connection
/*!
   Connection is a generic stream-oriented network connection.
 */
class API_EXPORT Connection {
public:
   virtual ~Connection() = default;

   // Close closes the connection.
   // Any blocked Read or Write operations will be unblocked and return errors.
   virtual void close() =0;

   // LocalAddr returns the local network address.
   virtual IPAddress* local_addr() const =0;

   // RemoteAddr returns the remote network address.
   virtual IPAddress* remote_addr() const =0;
};

//!  
class API_EXPORT TcpConnection : public Connection {
public:
   NO_COPY(TcpConnection)
   NO_MOVE(TcpConnection)
   
   TcpConnection();
   ~TcpConnection();

   virtual void close() override;

   // LocalAddr returns the local network address.
   virtual IPAddress* local_addr() const override;

   // RemoteAddr returns the remote network address.
   virtual IPAddress* remote_addr() const override;

private:
   std::unique_ptr<IPAddress> _local_addr;
   std::unique_ptr<IPAddress> _remote_addr;
};

} // net
} // orion
#endif 
