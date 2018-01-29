//
// ServerImpl.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <net/http/ServerImpl.h>

#include <orion/Log.h>
#include <orion/net/EndPoint.h>
#include <orion/net/Utils.h>
#include <orion/net/tcp/Connection.h>
#include <orion/net/tcp/Utils.h>

#include <future>

namespace orion
{
namespace net
{
namespace http
{

ServerImpl::ServerImpl()
   : _port(-1)
   , _handlers()
   , _io_context()
   , _signals(_io_context)
   , _acceptor(_io_context)
{
}

ServerImpl::~ServerImpl() = default;

int ServerImpl::port() const
{
   return _port;
}

void ServerImpl::add_handler(const std::string& p, HandlerFunc h)
{
   _handlers.insert(std::make_pair(p, std::move(h)));
}

bool ServerImpl::is_running() const
{
   return _acceptor.is_open();
}

void ServerImpl::shutdown()
{
   asio::error_code ec;

   _acceptor.close(ec);

   if (ec)
      log::error(ec);
}

std::error_code ServerImpl::listen_and_serve(asio::ip::tcp::endpoint endpoint)
{
   setup_signals();

   std::error_code ec;

   // Open the acceptor
   _acceptor.open(endpoint.protocol(), ec);
   if (ec)
      return ec;

   _acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));

   // Bind to the server address
   _acceptor.bind(endpoint, ec);
   if (ec)
      return ec;

   // Start listening for connections
   _acceptor.listen(asio::socket_base::max_listen_connections, ec);
   if (ec)
      return ec;

   do_accept();

   _io_context.run();

   return std::error_code();
}

void ServerImpl::do_accept()
{
   _new_connection = std::make_shared<ServerConnection>(_io_context, _handlers);

   _acceptor.async_accept(_new_connection->socket(), [this](std::error_code ec) {
      // Check whether the server was stopped by a signal before this
      // completion handler had a chance to run.
      if (not _acceptor.is_open())
         return;

      if (not ec)
      {
         //set_option(*_new_connection, KeepAlive{true});
         //tcp::set_option(*_new_connection, tcp::NoDelay{true});
         _new_connection->accept();
      }

      do_accept();
   });
}

void ServerImpl::do_close()
{
   _signals.async_wait([this](std::error_code ec, int /*signo*/) {
      if (ec)
         log::error(ec);

      // The server is stopped by cancelling all outstanding asynchronous
      // operations. Once all operations have finished the io_context::run()
      // call will exit.
      _acceptor.close();
   });
}

void ServerImpl::setup_signals()
{
   // Register to handle the signals that indicate when the server should exit.
   _signals.add(SIGINT);
   _signals.add(SIGTERM);

#if defined(SIGQUIT)   
   _signals.add(SIGQUIT);
#endif

   do_close();
}

} // http
} // net
} // orion
