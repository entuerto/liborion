//
// ServerImpl.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_TCP_SERVERIMPL_H
#define ORION_NET_TCP_SERVERIMPL_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/tcp/Utils.h>

#include <asio.hpp>

#include <string>
#include <vector>

namespace orion
{
namespace net
{
namespace tcp
{
/// Implement a tcp server with lib asio
class ServerImpl
{
public:
   NO_COPY(ServerImpl)

   ServerImpl();
   ServerImpl(Handler h);
   ~ServerImpl();

   int port() const;

   bool is_running() const;

   void shutdown();

   void register_handler(Handler h);

   std::error_code listen_and_serve(const std::string& addr, int port);

protected:
   void do_accept(asio::ip::tcp::acceptor& acceptor);
   void do_close();

private:
   void setup_signals();

   int _port;
   bool _is_running;
   Handler _handler;

   // The io_context used to perform asynchronous operations.
   asio::io_context _io_context;

   // The signal_set is used to register for process termination notifications.
   asio::signal_set _signals;

   // Acceptor used to listen for incoming connections.
   std::vector<asio::ip::tcp::acceptor> _acceptors;
};

} // tcp
} // net
} // orion

#endif // ORION_NET_TCP_SERVERIMPL_H
