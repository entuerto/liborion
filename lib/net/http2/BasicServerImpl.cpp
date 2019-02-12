//
// BasicServerImpl.cpp
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include "BasicServerImpl.h"

#include <orion/Log.h>

#include <net/http2/ServerConnection.h>

using namespace std::literals::chrono_literals;

namespace orion
{
namespace net
{
namespace http2
{

BasicServerImpl::BasicServerImpl()
   : _signals(_io_context)
   , _acceptor(_io_context)
{
}

BasicServerImpl::BasicServerImpl(int backlog)
   : ServerImpl(backlog)
   , _signals(_io_context)
   , _acceptor(_io_context)
{
}

BasicServerImpl::BasicServerImpl(http::RequestMux mux)
   : ServerImpl(std::move(mux))
   , _signals(_io_context)
   , _acceptor(_io_context)
{
}

BasicServerImpl::BasicServerImpl(http::RequestMux mux, int backlog)
   : ServerImpl(std::move(mux), backlog)
   , _signals(_io_context)
   , _acceptor(_io_context)
{
}

bool BasicServerImpl::is_running() const
{
   return _acceptor.is_open();
}

void BasicServerImpl::shutdown()
{
   std::error_code ec;

   _acceptor.close(ec);

   log::error_if(ec, DbgSrcLoc);
}

std::error_code BasicServerImpl::do_listen_and_serve()
{
   // Open and bind the acceptor
   init();

   setup_signals();

   std::error_code ec;

   // Start listening for connections
   _acceptor.listen(backlog(), ec);
   if (ec)
      return ec;

   if (not _acceptor.is_open())
      return {};

   do_accept();

   _io_context.run();

   return ec;
}

void BasicServerImpl::init()
{
   std::error_code ec;

   auto addr = asio::ip::make_address(to_string(this->endpoint().address()));

   asio::ip::tcp::endpoint endpoint{addr, this->endpoint().port()};

   // Open the acceptor
   _acceptor.open(endpoint.protocol(), ec);
   if (ec)
   {
      log::error("Openning the acceptor. ", ec, DbgSrcLoc);
      return;
   }

   _acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));

   // Bind to the endpoint
   _acceptor.bind(endpoint, ec);
   if (ec)
   {
      log::error("Binding the endpoint. ", ec, DbgSrcLoc);
      return;
   }
}

void BasicServerImpl::do_accept()
{
   auto self = this->shared_from_this();

   _acceptor.async_accept([self, this](const std::error_code& ec, asio::ip::tcp::socket socket) {
      // Check whether the server was stopped by a signal before this
      // completion handler had a chance to run.
      if (not _acceptor.is_open())
         return;

      if (ec)
      {
         log::error("[On accept] ", ec, DbgSrcLoc);
         return;
      }

      auto connection = std::make_shared<ServerConnection>(std::move(socket), request_mux());

      connection->read_timeout(read_timeout());
      // connection->tls_handshake_timeout(_read_timeout);

      connection->accept();

      do_accept();
   });
}

void BasicServerImpl::do_await_close()
{
   _signals.async_wait([this](std::error_code ec, int /*signo*/) {
      log::error_if(ec, DbgSrcLoc);

      // The server is stopped by cancelling all outstanding asynchronous
      // operations. Once all operations have finished the io_context::run()
      // call will exit.
      shutdown();
   });
}

void BasicServerImpl::setup_signals()
{
   // Register to handle the signals that indicate when the server should exit.
   _signals.add(SIGINT);
   _signals.add(SIGTERM);

#if defined(SIGQUIT)   
   _signals.add(SIGQUIT);
#endif

   do_await_close();
}

} // namespace http2
} // namespace net
} // namespace orion
