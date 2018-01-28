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

#include <orion/net/http/Utils.h>

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

   ServerImpl();
   ~ServerImpl();

   int port() const;

   bool is_running() const;

   void shutdown();

   void add_handler(const std::string& p, HandlerFunc h);

   std::error_code listen_and_serve(const std::string& addr, int port);

   void do_accept(asio::ip::tcp::acceptor& acceptor);
   void do_close();

private:
   void setup_signals();

   int _port;
   bool _is_running;
   Handlers _handlers;

   // The io_context used to perform asynchronous operations.
   asio::io_context _io_context;

   // The signal_set is used to register for process termination notifications.
   asio::signal_set _signals;

   // Acceptor used to listen for incoming connections.
   std::vector<asio::ip::tcp::acceptor> _acceptors;
};

} // http
} // net
} // orion

#endif // ORION_NET_HTTP_SERVERIMPL_H
