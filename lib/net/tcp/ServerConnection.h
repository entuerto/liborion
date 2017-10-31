//
// ServerConnection.h
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#ifndef ORION_NET_TCP_SERVERCONNECTION_H
#define ORION_NET_TCP_SERVERCONNECTION_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/tcp/Connection.h>
#include <orion/net/tcp/Utils.h>

#include <memory>

namespace orion
{
namespace net
{
namespace tcp
{

///
class ServerConnection 
   : public tcp::Connection
   //, public std::enable_shared_from_this<ServerConnection>
{
public:
   NO_COPY(ServerConnection)

   ServerConnection(asio::io_service& io_service, const Handler& handler);
   virtual ~ServerConnection();

private:
   /// Perform an asynchronous read operation.
   void do_read() override;

   /// Perform an asynchronous write operation.
   void do_write() override;

   std::error_code on_handler(std::streambuf* in, std::streambuf* out);

   /// Request handler
   const Handler& _handler;

   /// Buffer for incoming data. 
   StreamBuffer _in_streambuf;

   /// Buffer for outgoing data. 
   StreamBuffer _out_streambuf;

   std::size_t _in_buffer_size;
};

} // tcp
} // net
} // orion
#endif // ORION_NET_TCP_SERVERCONNECTION_H
