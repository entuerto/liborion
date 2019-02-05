//
// Session.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP_SESSION_H
#define ORION_NET_HTTP_SESSION_H

#include <orion/Common.h>

#include <orion/net/EndPoint.h>
#include <orion/net/Options.h>
#include <orion/net/Url.h>
#include <orion/net/Utils.h>
#include <orion/net/http/Request.h>
#include <orion/net/http/Response.h>
#include <orion/net/http/Utils.h>

#include <asio.hpp>

#include <string>

namespace orion
{
namespace net
{
namespace http
{

///
/// Base session class
///
class API_EXPORT BaseSession 
{
public:
   NO_COPY(BaseSession);

   BaseSession(asio::io_context& io_context);

   // Move constructor
   BaseSession(BaseSession&& rhs) noexcept;

   /// Default destructor
   virtual ~BaseSession();

   /// Move assignment
   BaseSession& operator=(BaseSession&& rhs) noexcept;

   void set_option(const Parameters& parameters);
   void set_option(Parameters&& parameters);

   void set_option(const Timeout& timeout);
    
   bool connected() const;
   void connected(bool value);

   std::error_code close();

   asio::io_context& io_context();

   asio::ip::tcp::socket& socket();

private:
   /// The io_context used to perform asynchronous operations.
   asio::io_context& _io_context;

   /// Socket for the connection.
   asio::ip::tcp::socket _socket;

   Parameters _params;
   Timeout _timeout;

   bool _connected;
};

///
/// Synchronous session
///
class API_EXPORT SyncSession
   : public BaseSession
   , public std::enable_shared_from_this<SyncSession>
{
public:
   NO_COPY(SyncSession);

   SyncSession(asio::io_context& io_context);

   template<typename... Ts>
   SyncSession(asio::io_context& io_context, Ts&&... ts)
      : SyncSession(io_context)
   {
      set_option(std::forward<Ts>(ts)...);
   }

   // Move constructor
   SyncSession(SyncSession&& rhs) noexcept;

   /// Default destructor
   ~SyncSession() override;

   /// Move assignment
   SyncSession& operator=(SyncSession&& rhs) noexcept;

   template<typename T, typename... Ts>
   void set_option(SyncSession& session, T&& t, Ts&&... ts)
   {
      set_option(std::forward<T>(t));
      set_option(std::forward<Ts>(ts)...);
   }

   Response submit(Request&& req);

   Response submit(const Method& m, const Url& url);

private:
   bool connect(const std::string& host, int port, std::error_code& ec);

};

///
/// Asynchronous session
///
class API_EXPORT AsyncSession 
   : public BaseSession
   , public std::enable_shared_from_this<AsyncSession>
{
public:
   NO_COPY(AsyncSession);

   AsyncSession(asio::io_context& io_context);

   template<typename... Ts>
   AsyncSession(asio::io_context& io_context, Ts&&... ts)
      : AsyncSession(io_context)
   {
      set_option(std::forward<Ts>(ts)...);
   }

   // Move constructor
   AsyncSession(AsyncSession&& rhs) noexcept;

   /// Default destructor
   ~AsyncSession() override;

   /// Move assignment
   AsyncSession& operator=(AsyncSession&& rhs) noexcept;
    
   template <typename T, typename... Ts>
   void set_option(AsyncSession& session, T&& t, Ts&&... ts)
   {
      set_option(std::forward<T>(t));
      set_option(std::forward<Ts>(ts)...);
   }

   void on_close(CloseHandler h);
   void on_error(ErrorHandler h);
   void on_response(ResponseHandler h);

   void submit(Request&& req);

   void submit(const Method& m, const Url& url);

private:
   void connect(const std::string& host, int port);

   void do_read();
   void do_write();

   void do_on_close();
   void do_on_error(const std::error_code& ec);
   void do_on_response(const Response& res);

   CloseHandler _close_handler;
   ErrorHandler _error_handler;
   ResponseHandler _response_handler;

   /// Buffer for incoming data. 
   asio::streambuf _in_buffer;

   Request _request;
   Response _response;
};


template <typename T>
void set_option(BaseSession& session, T&& t)
{
   session.set_option(std::forward<T>(t));
}
    
template <typename T, typename... Ts>
void set_option(BaseSession& session, T&& t, Ts&&... ts)
{
   set_option(session, std::forward<T>(t));
   set_option(session, std::forward<Ts>(ts)...);
}
    
} // namespace http
} // namespace net
} // namespace orion
#endif // ORION_NET_HTTP_SESSION_H
