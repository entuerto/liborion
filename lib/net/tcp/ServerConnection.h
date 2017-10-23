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

#include <asio.hpp>

#include <array>
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
   , public std::enable_shared_from_this<ServerConnection>
{
public:
   NO_COPY(ServerConnection)

   ServerConnection(asio::io_service& io_service, const Handler& handler);
   ~ServerConnection();

   virtual void close() override;

   virtual std::error_code keep_alive(bool value) override;
   virtual bool keep_alive() const override;
   virtual std::error_code no_delay(bool value) override;
   virtual bool no_delay() const override;

   asio::ip::tcp::socket& socket();

   void accept();

   /// Handle timeout
   void on_read_timeout(const asio::error_code& ec);
   void on_write_timeout(const asio::error_code& ec);

   virtual std::error_code on_handler(std::streambuf* in, std::streambuf* out) override;

private:
   void dump_socket_options();

   void start_read_timer();
   void start_write_timer();

   /// Perform an asynchronous read operation.
   void do_read();

   /// Perform an asynchronous write operation.
   void do_write();

   /// Socket for the connection.
   asio::ip::tcp::socket _socket;

   /// Request handler
   const Handler& _handler;

   /// Buffer for incoming data. 
   asio::streambuf _in_streambuf;

   /// Buffer for outgoing data. 
   asio::streambuf _out_streambuf;

   asio::steady_timer _read_deadline;
   asio::steady_timer _write_deadline;

   std::size_t _in_buffer_size;
};

} // tcp
} // net
} // orion
#endif // ORION_NET_TCP_SERVERCONNECTION_H
