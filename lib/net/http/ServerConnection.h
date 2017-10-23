//
// ServerConnection.h
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#ifndef ORION_NET_HTTP_SERVERCONNECTION_H
#define ORION_NET_HTTP_SERVERCONNECTION_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/http/Request.h>
#include <orion/net/http/Response.h>
#include <orion/net/http/Utils.h>
#include <orion/net/tcp/Connection.h>

#include <net/http/Parser.h>

#include <asio.hpp>

#include <array>
#include <memory>

namespace orion
{
namespace net
{
namespace http
{

///
class ServerConnection 
   : public tcp::Connection
   , public std::enable_shared_from_this<ServerConnection>
{
public:
   NO_COPY(ServerConnection)

   ServerConnection(asio::io_service& io_service, const Handlers& handlers);
   virtual ~ServerConnection();

   virtual void close() override;

   virtual std::error_code keep_alive(bool value) override;
   virtual bool keep_alive() const override;
   virtual std::error_code no_delay(bool value) override;
   virtual bool no_delay() const override;

   asio::ip::tcp::socket& socket();

   void accept();

   /// Handle timeout
   void on_read_timeout(const asio::error_code& e);
   void on_write_timeout(const asio::error_code& e);

private:
   virtual std::error_code on_handler(std::streambuf* in, std::streambuf* out) override;
   
private:
   void dump_socket_options();

   void start_read_timer();
   void start_write_timer();

   /// Perform an asynchronous read operation.
   void do_read();

   /// Perform an asynchronous write operation.
   void do_write();

   /// Process the request.
   void do_handler();

   /// Socket for the connection.
   asio::ip::tcp::socket _socket;

   /// Request handlers
   const Handlers& _handlers;

   Request _request;
   Response _response;

   /// Buffer for incoming data. 8k
   std::array<char, 8192> _in_buffer;

   Parser _parser;

   asio::steady_timer _read_deadline;
   asio::steady_timer _write_deadline;
};

} // http
} // net
} // orion
#endif // ORION_NET_HTTP_SERVERCONNECTION_H
