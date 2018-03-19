//
// Listener.ipp
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_TCP_LISTENER_IPP
#define ORION_NET_TCP_LISTENER_IPP

#include <orion/Orion-Stddefs.h>

#include <orion/Log.h>
#include <orion/net/tcp/Connection.h>

#include <functional>

namespace orion
{
namespace net
{
namespace tcp
{

Listener::Listener(asio::io_context& io_context, EndPoint ep, Handler handler) 
   : _endpoint(std::move(ep))
   , _acceptor(io_context)
   , _socket(io_context)
   , _handler(std::move(handler))
{
   std::error_code ec;

   auto addr = asio::ip::make_address(to_string(_endpoint.address()));

   asio::ip::tcp::endpoint endpoint{addr, _endpoint.port()};

   // Open the acceptor
   _acceptor.open(endpoint.protocol(), ec);
   if (ec)
   {
      log::error("Openning the acceptor. ", ec, _src_loc);
      return;
   }

   _acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));

   // Bind to the endpoint
   _acceptor.bind(endpoint, ec);
   if (ec)
   {
      log::error("Binding the endpoint. ", ec, _src_loc);
      return;
   }

   // Start listening for connections
   _acceptor.listen(asio::socket_base::max_listen_connections, ec);
   if (ec)
   {
      log::error("Listening for connections. ", ec, _src_loc);
      return;
   }

}

Listener::~Listener() = default;

EndPoint Listener::endpoint() const
{
   return _endpoint;
}

bool Listener::is_listening() const
{
   return _acceptor.is_open();
}

/// Start accepting incoming connections
std::error_code Listener::start()
{
   if (not _acceptor.is_open())
      return std::error_code();

   do_accept();

   return std::error_code();
}

/// Close closes the listener.
std::error_code Listener::close()
{
   std::error_code ec;

   _acceptor.close(ec);

   return ec;
}

void Listener::do_accept()
{
   _acceptor.async_accept(
      _socket, std::bind(&Listener::on_accept, shared_from_this(), std::placeholders::_1));
}

void Listener::on_accept(const std::error_code& ec)
{
   // Check whether the server was stopped by a signal before this
   // completion handler had a chance to run.
   if (not _acceptor.is_open())
      return;

   if (ec)
   {
      log::error("Listener::on_accept() ", ec, _src_loc);
      return;
   }

   std::make_shared<Connection>(std::move(_socket), _handler)->accept();

   do_accept();
}

} // tcp
} // net
} // orion
#endif // ORION_NET_TCP_LISTENER_IPP