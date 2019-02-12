//
// Server.h
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP2_SERVER_H
#define ORION_NET_HTTP2_SERVER_H

#include <orion/Common.h>

#include <orion/net/EndPoint.h>
#include <orion/net/http/RequestMux.h>
#include <orion/net/http/Utils.h>

#include <chrono>
#include <memory>
#include <string>

namespace orion
{
namespace net
{
namespace http2
{
//-------------------------------------------------------------------------------------------------
// Forward declarations
class ServerImpl;

//-------------------------------------------------------------------------------------------------
/// This class provides an embedded HTTP/2 Server
///
/// This class provides an embedded HTTP/2 Server to handle incoming Requests
/// and send HTTP/2 valid responses.
///
class API_EXPORT Server : NonCopyable
{
public:
   /// Default constructor
   Server(std::shared_ptr<ServerImpl> si);

   /// Default move constructor
   Server(Server&& rhs) noexcept;

   /// Default destructor
   ~Server();

   /// Default move assignment
   constexpr Server& operator=(Server&& rhs);

   /// Endpoint where it will accepts incoming connections. 
   constexpr const EndPoint& endpoint() const;

   /// Shhutdown the server
   constexpr void shutdown();

   /// Indicates if the server is listening
   constexpr bool is_running() const;

   /// Sets the maximum length to which the queue of pending connections.
   constexpr void backlog(int value);

   /// The maximum length of the queue of pending connections.
   constexpr int backlog() const;

   /// Returns the request multiplexer
   constexpr http::RequestMux& request_mux();

   /// Returns the request multiplexer
   constexpr const http::RequestMux& request_mux() const;

   /// Sets TLS handshake timeout, which defaults to 60 seconds.
   constexpr void tls_handshake_timeout(std::chrono::seconds t);

   /// Sets read timeout, which defaults to 60 seconds.
   constexpr void read_timeout(std::chrono::seconds t);

   std::error_code listen_and_serve(EndPoint endpoint);

   std::error_code listen_and_serve(EndPoint endpoint, http::RequestMux mux);

private:
   constexpr const ServerImpl* impl() const { return _impl.get(); }
   constexpr ServerImpl* impl() { return _impl.get(); }
   
   std::shared_ptr<ServerImpl> _impl;
};

API_EXPORT Server make_server();
API_EXPORT Server make_server(int backlog);
API_EXPORT Server make_server(http::RequestMux&& mux);
API_EXPORT Server make_server(http::RequestMux&& mux, int backlog);

} // namespace http2
} // namespace net
} // namespace orion

#include <orion/net/http2/impl/Server.ipp>

#endif // ORION_NET_HTTP2_SERVER_H
