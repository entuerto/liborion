//
// Listener.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_TCP_LISTENER_H
#define ORION_NET_TCP_LISTENER_H

#include <orion/Common.h>

#include <orion/net/EndPoint.h>
#include <orion/net/tcp/Utils.h>

#include <asio.hpp>

#include <system_error>

namespace orion
{
namespace net
{
namespace tcp
{
///
/// Accepts incoming connections 
///
template<typename ConnectionT, typename HandlerT>
class Listener 
   : public std::enable_shared_from_this<Listener<ConnectionT, HandlerT>>
   , NonCopyable
{
public:
   Listener(asio::io_context& io_context, EndPoint ep, HandlerT handler);
   Listener(asio::io_context& io_context, EndPoint ep, HandlerT handler, int backlog);
   ~Listener();

   /// Endpoint where it will accepts incoming connections. 
   constexpr const EndPoint& endpoint() const;

   /// Indicates if we are still listening for incoming connections.
   constexpr bool is_listening() const;

   /// Start accepting incoming connections
   std::error_code start();

   /// Close closes the listener.
   std::error_code close();

   constexpr int backlog() const; 

   constexpr void backlog(int value);

   /// Sets the timeout for future Read calls.
   /// A zero value for sec means Read will not time out.
   constexpr void read_timeout(std::chrono::seconds sec);

   /// Get the current value of the read timeout.
   constexpr std::chrono::seconds read_timeout() const;

   /// Sets the timeout for tls handshakes.
   /// A zero value for sec means tls handshakes will not time out.
   constexpr void tls_handshake_timeout(std::chrono::seconds sec);

   /// Get the current value of the tls handshake timeout.
   constexpr std::chrono::seconds tls_handshake_timeout() const;

protected:
   void init();
   void do_accept();

private:
   EndPoint _endpoint;

   int _backlog{asio::socket_base::max_listen_connections};

   std::chrono::seconds _read_timeout;
   std::chrono::seconds _tls_handshake_timeout;

   asio::ip::tcp::acceptor _acceptor;

   HandlerT _handler;
};

} // tcp
} // net
} // orion

#include <orion/net/tcp/impl/Listener.ipp>

#endif // ORION_NET_TCP_LISTENER_H
