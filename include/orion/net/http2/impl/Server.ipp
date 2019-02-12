//
// Server.ipp
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP2_SERVER_IPP
#define ORION_NET_HTTP2_SERVER_IPP

namespace orion
{
namespace net
{
namespace http2
{
//-------------------------------------------------------------------------------------------------
// ServerImpl
class ServerImpl : NonCopyable
{
public:
   ServerImpl() = default;
   explicit ServerImpl(http::RequestMux mux)
      : _mux(std::move(mux))
   {
   }
   explicit ServerImpl(int backlog)
      : _backlog(backlog)
   {
   }
   ServerImpl(http::RequestMux mux, int backlog)
      : _backlog(backlog)
      , _mux(std::move(mux))
   {
   }
   virtual ~ServerImpl() = default;

   /// Endpoint where it will accepts incoming connections. 
   constexpr const EndPoint& endpoint() const { return _endpoint; }

   /// Returns the request multiplexer
   constexpr http::RequestMux& request_mux()             { return _mux; }
   constexpr const http::RequestMux& request_mux() const { return _mux; }

   /// Sets the maximum length to which the queue of pending connections.
   constexpr void backlog(int value) { _backlog = value; }
   /// The maximum length of the queue of pending connections.
   constexpr int backlog() const { return _backlog; }

   /// Sets TLS handshake timeout, which defaults to 60 seconds.
   /// A zero value for sec means Read will not time out.
   constexpr void tls_handshake_timeout(std::chrono::seconds t)
   {
      _tls_handshake_timeout = std::move(t);
   }
   /// Get the current value of the tls handshake timeout.
   constexpr std::chrono::seconds tls_handshake_timeout() const { return _tls_handshake_timeout; }

   /// Sets read timeout, which defaults to 60 seconds.
   /// A zero value for sec means Read will not time out.
   constexpr void read_timeout(std::chrono::seconds t) { _read_timeout = std::move(t); }
   /// Get the current value of the read timeout.
   constexpr std::chrono::seconds read_timeout() const { return _read_timeout; }

   /// Indicates if the server is listening
   virtual bool is_running() const = 0;

   virtual void shutdown() = 0;

   std::error_code listen_and_serve(EndPoint endpoint)
   {
      _endpoint = std::move(endpoint);

      return do_listen_and_serve();
   }

   std::error_code listen_and_serve(EndPoint endpoint, http::RequestMux mux)
   {
      _endpoint = std::move(endpoint);
      _mux = std::move(mux);

      return do_listen_and_serve();
   }

protected:
   virtual std::error_code do_listen_and_serve() = 0;

private:
   int _backlog{-1};

   std::chrono::seconds _read_timeout{60};
   std::chrono::seconds _tls_handshake_timeout{60};

   EndPoint _endpoint;

   http::RequestMux _mux;
};

//-------------------------------------------------------------------------------------------------
// Server Implementation

inline Server::Server(std::shared_ptr<ServerImpl> si)
   : _impl(si)
{
}

inline Server::Server(Server&& rhs) noexcept
   : _impl(std::move(rhs._impl))
{
}

inline Server::~Server() = default;

inline constexpr Server& Server::operator=(Server&& rhs)
{
   _impl = std::move(rhs._impl);
   return *this;
}

inline constexpr const EndPoint& Server::endpoint() const
{
   return impl()->endpoint();
}

inline constexpr void Server::shutdown()
{
   impl()->shutdown();
}

inline constexpr bool Server::is_running() const
{
   return impl()->is_running();
}

inline constexpr http::RequestMux& Server::request_mux()
{
   return impl()->request_mux();
}

inline constexpr const http::RequestMux& Server::request_mux() const
{
   return impl()->request_mux();
}

inline constexpr void Server::backlog(int value)
{
   impl()->backlog(value);
}

inline constexpr int Server::backlog() const
{
   return impl()->backlog();
}

inline constexpr void Server::tls_handshake_timeout(std::chrono::seconds t)
{
   return impl()->tls_handshake_timeout(std::move(t));
}

inline constexpr void Server::read_timeout(std::chrono::seconds t)
{
   return impl()->read_timeout(std::move(t));
}

inline std::error_code Server::listen_and_serve(EndPoint endpoint)
{
   return impl()->listen_and_serve(std::move(endpoint));
}

inline std::error_code Server::listen_and_serve(EndPoint endpoint, http::RequestMux mux)
{
   return impl()->listen_and_serve(std::move(endpoint), std::move(mux));
}

} // namespace http2
} // namespace net
} // namespace orion

#endif // ORION_NET_HTTP2_SERVER_IPP
