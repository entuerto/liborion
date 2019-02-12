//
// ServerConnection.h
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP2_SERVERCONNECTION_H
#define ORION_NET_HTTP2_SERVERCONNECTION_H

#include <orion/Common.h>

#include <orion/net/Connection.h>
#include <orion/net/http/RequestMux.h>

#include <asio.hpp>

#include <array>
#include <memory>

namespace orion
{
namespace net
{
namespace http2
{
//-------------------------------------------------------------------------------------------------
// Forward declarations
class Handler;

//-------------------------------------------------------------------------------------------------
/// Handle an accepted connection
///
class ServerConnection : public Connection<asio::ip::tcp::socket>
{
public:
   ServerConnection(asio::ip::tcp::socket socket, http::RequestMux& mux);
   ~ServerConnection() override = default;

protected:
   /// Perform extra accept operations.
   void do_accept() override;

   /// Perform an asynchronous read operation.
   void do_read() override;

   /// Perform an asynchronous write operation.
   void do_write() override;

private:
   http::RequestMux& _mux;
   /// Handler for parsing http2 messages
   std::shared_ptr<Handler> _handler;

   /// Buffer for incoming data. 8k
   std::array<uint8_t, 8192> _in_buffer;
   /// Buffer for outgoing data. 64k
   std::array<uint8_t, 65536> _out_buffer;

   bool _writing{false};
};

} // namespace http2
} // namespace net
} // namespace orion
#endif // ORION_NET_HTTP2_SERVERCONNECTION_H
