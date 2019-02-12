//
// Listener.ipp
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_TCP_LISTENER_IPP
#define ORION_NET_TCP_LISTENER_IPP

#include <orion/Log.h>

#include <functional>

namespace orion
{
namespace net
{
namespace tcp
{

template<typename ConnectionT, typename HandlerT>
Listener<ConnectionT, HandlerT>::Listener(asio::io_context& io_context,
                                          EndPoint ep,
                                          HandlerT handler)
   : _endpoint(std::move(ep))
   , _acceptor(io_context)
   , _handler(std::move(handler))
{
   init();
}

template<typename ConnectionT, typename HandlerT>
Listener<ConnectionT, HandlerT>::Listener(asio::io_context& io_context,
                                          EndPoint ep,
                                          HandlerT handler,
                                          int backlog)
   : _endpoint(std::move(ep))
   , _acceptor(io_context)
   , _handler(std::move(handler))
   , _backlog(backlog)
{
   init();
}

template<typename ConnectionT, typename HandlerT>
Listener<ConnectionT, HandlerT>::~Listener() = default;

template<typename ConnectionT, typename HandlerT>
inline constexpr const EndPoint& Listener<ConnectionT, HandlerT>::endpoint() const
{
   return _endpoint;
}

template<typename ConnectionT, typename HandlerT>
inline constexpr bool Listener<ConnectionT, HandlerT>::is_listening() const
{
   return _acceptor.is_open();
}

template<typename ConnectionT, typename HandlerT>
void Listener<ConnectionT, HandlerT>::init()
{
   std::error_code ec;

   auto addr = asio::ip::make_address(to_string(_endpoint.address()));

   asio::ip::tcp::endpoint endpoint{addr, _endpoint.port()};

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

/// Start accepting incoming connections
template<typename ConnectionT, typename HandlerT>
std::error_code Listener<ConnectionT, HandlerT>::start()
{
   std::error_code ec;

   // Start listening for connections
   _acceptor.listen(_backlog, ec);
   if (ec)
      return ec;

   if (not _acceptor.is_open())
      return {};

   do_accept();

   return {};
}

/// Close closes the listener.
template<typename ConnectionT, typename HandlerT>
std::error_code Listener<ConnectionT, HandlerT>::close()
{
   std::error_code ec;

   _acceptor.close(ec);

   return ec;
}

template<typename ConnectionT, typename HandlerT>
inline constexpr int Listener<ConnectionT, HandlerT>::backlog() const
{
   return _backlog;
}

template<typename ConnectionT, typename HandlerT>
inline constexpr void Listener<ConnectionT, HandlerT>::backlog(int value)
{
   _backlog = value;
}

template<typename ConnectionT, typename HandlerT>
inline constexpr void Listener<ConnectionT, HandlerT>::read_timeout(std::chrono::seconds sec)
{
   _read_timeout = std::move(sec);
}

template<typename ConnectionT, typename HandlerT>
inline constexpr std::chrono::seconds Listener<ConnectionT, HandlerT>::read_timeout() const
{
   return _read_timeout;
}

template<typename ConnectionT, typename HandlerT>
inline constexpr void Listener<ConnectionT, HandlerT>::tls_handshake_timeout(
   std::chrono::seconds sec)
{
   _tls_handshake_timeout = std::move(sec);
}

template<typename ConnectionT, typename HandlerT>
inline constexpr std::chrono::seconds Listener<ConnectionT, HandlerT>::tls_handshake_timeout() const
{
   return _read_timeout;
}

template<typename ConnectionT, typename HandlerT>
void Listener<ConnectionT, HandlerT>::do_accept()
{
   auto self = this->shared_from_this();

   _acceptor.async_accept([self, this](const std::error_code& ec, asio::ip::tcp::socket socket) {
      // Check whether the server was stopped by a signal before this
      // completion handler had a chance to run.
      if (not _acceptor.is_open())
         return;

      if (ec)
      {
         log::error("Listener::on_accept() ", ec, DbgSrcLoc);
         return;
      }

      auto connection = std::make_shared<ConnectionT>(std::move(socket), _handler);

      connection->read_timeout(_read_timeout);
      // connection->tls_handshake_timeout(_read_timeout);

      connection->accept();

      do_accept();
   });
}

} // namespace tcp
} // namespace net
} // namespace orion
#endif // ORION_NET_TCP_LISTENER_IPP