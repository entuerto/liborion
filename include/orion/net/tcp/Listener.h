//
// Listener.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_TCP_LISTENER_H
#define ORION_NET_TCP_LISTENER_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/EndPoint.h>
#include <orion/net/Listener.h>
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
class Listener 
   : public std::enable_shared_from_this<Listener>
   , public net::Listener
{
public:
   NO_COPY(Listener)

   Listener(asio::io_context& io_context, EndPoint ep, Handler handler);
   ~Listener() override;

   /// Endpoint where it will accepts incoming connections. 
   EndPoint endpoint() const override;

   /// Indicates if we are still listening for incoming connections.
   bool is_listening() const override;

   /// Start accepting incoming connections
   std::error_code start() override;

   /// Close closes the listener.
   std::error_code close() override;

protected:
   void do_accept();
   void on_accept(const std::error_code& ec);

private:
   EndPoint _endpoint;

   asio::ip::tcp::acceptor _acceptor;
   asio::ip::tcp::socket _socket;

   Handler _handler;
};

} // tcp
} // net
} // orion

#include <orion/net/tcp/impl/Listener.ipp>

#endif // ORION_NET_TCP_LISTENER_H
