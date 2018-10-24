//
// Listener.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP_LISTENER_H
#define ORION_NET_HTTP_LISTENER_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/EndPoint.h>
#include <orion/net/Listener.h>
#include <orion/net/http/RequestMux.h>

#include <net/http/ServerConnection.h>

#include <asio.hpp>

#include <chrono>
#include <system_error>

using namespace std::literals::chrono_literals;

namespace orion
{
namespace net
{
namespace http
{
///
/// Accepts incoming connections
///
class Listener
   : public std::enable_shared_from_this<Listener>
   , public net::Listener
{
public:
   NO_COPY(Listener)

   // DEFAULT_MOVE(Listener)

   Listener(asio::io_context& io_context, EndPoint ep, RequestMux& mux)
      : _endpoint(std::move(ep))
      , _read_timeout(60s)
      , _tls_handshake_timeout(60s)
      , _acceptor(io_context)
      , _socket(io_context)
      , _mux(mux)
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

   ~Listener() override = default;

   EndPoint endpoint() const override
   {
      return _endpoint;
   }

   /// Sets the timeout for future Read calls.
   /// A zero value for sec means Read will not time out.
   std::error_code read_timeout(const std::chrono::seconds& sec)
   {
      _read_timeout = sec;
      return std::error_code();
   }

   /// Get the current value of the read timeout.
   std::chrono::seconds read_timeout() const
   {
      return _read_timeout;
   }

   /// Sets the timeout for tls handshakes.
   /// A zero value for sec means tls handshakes will not time out.
   std::error_code tls_handshake_timeout(const std::chrono::seconds& sec)
   {
      _tls_handshake_timeout = sec;
      return std::error_code();
   }

   /// Get the current value of the tls handshake timeout.
   std::chrono::seconds tls_handshake_timeout() const
   {
      return _read_timeout;
   }

   bool is_listening() const override
   {
      return _acceptor.is_open();
   }

   /// Start accepting incoming connections
   std::error_code start() override
   {
      if (not _acceptor.is_open())
         return std::error_code();

      do_accept();

      return std::error_code();
   }

   /// Close closes the listener.
   std::error_code close() override
   {
      std::error_code ec;

      _acceptor.close(ec);

      return ec;
   }

protected:
   void do_accept()
   {
      _acceptor.async_accept(
         _socket, std::bind(&Listener::on_accept, shared_from_this(), std::placeholders::_1));
   }

   void on_accept(const std::error_code& ec)
   {
      // Check whether the server was stopped by a signal before this
      // completion handler had a chance to run.
      if (not _acceptor.is_open())
         return;

      if (ec)
      {
         log::error("Listener::on_accept() ", ec);
         return;
      }

      _connection = std::make_shared<ServerConnection>(std::move(_socket), _mux);

      set_option(*_connection, KeepAlive{true});

      _connection->read_timeout(_read_timeout);
      //_connection->tls_handshake_timeout(_read_timeout);

      _connection->accept();

      do_accept();
   }

private:
   EndPoint _endpoint;

   std::chrono::seconds _read_timeout;
   std::chrono::seconds _tls_handshake_timeout;

   // Acceptor used to listen for incoming connections.
   asio::ip::tcp::acceptor _acceptor;

   asio::ip::tcp::socket _socket;

   RequestMux& _mux;

   std::shared_ptr<ServerConnection> _connection;
};

} // http
} // net
} // orion

#endif // ORION_NET_HTTP_LISTENER_H
