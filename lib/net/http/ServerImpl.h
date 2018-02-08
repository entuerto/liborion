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

#include <net/http/Listener.h>

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

   ServerImpl();
   ~ServerImpl();

   EndPoint endpoint() const;

   bool is_running() const;

   void shutdown();

   RequestMux& request_mux();

   std::error_code listen_and_serve(EndPoint endpoint);
   std::error_code listen_and_serve(EndPoint endpoint, RequestMux mux);

   void do_await_close();

private:
   void setup_signals();

   EndPoint _endpoint;
   RequestMux _mux;

   // The io_context used to perform asynchronous operations.
   asio::io_context _io_context;

   // The signal_set is used to register for process termination notifications.
   asio::signal_set _signals;

   std::shared_ptr<Listener> _listener;
};

} // http
} // net
} // orion

#endif // ORION_NET_HTTP_SERVERIMPL_H
