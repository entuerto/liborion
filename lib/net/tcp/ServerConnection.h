//
// ServerConnection.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_TCP_SERVERCONNECTION_H
#define ORION_NET_TCP_SERVERCONNECTION_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/Connection.h>
#include <orion/net/tcp/Utils.h>

#include <memory>

namespace orion
{
namespace net
{
namespace tcp
{

///
class ServerConnection : public Connection<asio::ip::tcp::socket>
{
public:
   ServerConnection(asio::io_context& io_context, const Handler& handler);
   virtual ~ServerConnection();

protected:
   /// Perform an asynchronous read operation.
   void do_read() override;

   /// Perform an asynchronous write operation.
   void do_write() override;

private:
   std::error_code on_handler(std::streambuf* in, std::streambuf* out);

   /// Request handler
   const Handler& _handler;

   /// Buffer for incoming data.
   asio::streambuf _in_streambuf;

   /// Buffer for outgoing data.
   asio::streambuf _out_streambuf;

   std::size_t _in_buffer_size;
};

} // tcp
} // net
} // orion
#endif // ORION_NET_TCP_SERVERCONNECTION_H
