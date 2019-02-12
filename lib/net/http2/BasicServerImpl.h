//
// BasicServerImpl.h
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP2_BASICSERVERIMPL_H
#define ORION_NET_HTTP2_BASICSERVERIMPL_H

#include <orion/Common.h>

#include <orion/net/EndPoint.h>
#include <orion/net/http/RequestMux.h>
#include <orion/net/http/Utils.h>
#include <orion/net/http2/Server.h>

#include <asio.hpp>

#include <memory>

namespace orion
{
namespace net
{
namespace http2
{
///
/// Implements a basic non secure tcp server for HTTP v2 request.
///
class BasicServerImpl 
   : public ServerImpl
   , public std::enable_shared_from_this<BasicServerImpl>
{
public:
   /// Default constructor
   BasicServerImpl();
   /// Construct with a maximum queue of pending connections. 
   explicit BasicServerImpl(int backlog);
   /// Construct with a request multiplexer
   explicit BasicServerImpl(http::RequestMux mux);
   /// Construct with a request multiplexer and a maximum queue of pending connections.
   BasicServerImpl(http::RequestMux mux, int backlog);

   /// Default destructor
   ~BasicServerImpl() override = default;

   /// Indicates if we are still listening for incoming connections.
   bool is_running() const override;
   /// Shhutdown the server
   void shutdown() override;

protected:
   void init();
   void do_accept();
   void do_await_close();

   std::error_code do_listen_and_serve() override;

private:
   void setup_signals();

   // The io_context used to perform asynchronous operations.
   asio::io_context _io_context;

   // The signal_set is used to register for process termination notifications.
   asio::signal_set _signals;

   asio::ip::tcp::acceptor _acceptor;
};

} // namespace http2
} // namespace net
} // namespace orion

#endif // ORION_NET_HTTP2_BASICSERVERIMPL_H
