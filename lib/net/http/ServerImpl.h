//
// ServerImpl.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP_SERVERIMPL_H
#define ORION_NET_HTTP_SERVERIMPL_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/http/RequestMux.h>
#include <orion/net/http/Utils.h>
#include <orion/net/tcp/Listener.h>

#include <net/http/ServerConnection.h>

#include <asio.hpp>

#include <string>
#include <vector>

namespace orion
{
namespace net
{
namespace http
{
/// Implement a tcp server with lib asio
class ServerImpl 
{
public:
   NO_COPY(ServerImpl)
   NO_MOVE(ServerImpl)

   using ListenerType = tcp::Listener<ServerConnection, RequestMux>;

   ServerImpl();
   ~ServerImpl();

   EndPoint endpoint() const;

   bool is_running() const;

   void shutdown();

   RequestMux& request_mux();

   // Sets TLS handshake timeout, which defaults to 60 seconds.
   void tls_handshake_timeout(const std::chrono::seconds& t);

   // Sets read timeout, which defaults to 60 seconds.
   void read_timeout(const std::chrono::seconds& t);

   std::error_code listen_and_serve(EndPoint endpoint);
   std::error_code listen_and_serve(EndPoint endpoint, RequestMux mux);

   void do_await_close();

private:
   void setup_signals();

   EndPoint _endpoint;
   RequestMux _mux;

   std::chrono::seconds _read_timeout;
   std::chrono::seconds _tls_handshake_timeout;

   // The io_context used to perform asynchronous operations.
   asio::io_context _io_context;

   // The signal_set is used to register for process termination notifications.
   asio::signal_set _signals;

   std::shared_ptr<ListenerType> _listener;
};

} // http
} // net
} // orion

#endif // ORION_NET_HTTP_SERVERIMPL_H
