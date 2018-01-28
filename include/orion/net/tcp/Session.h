//
// Session.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_TCP_SESSION_H
#define ORION_NET_TCP_SESSION_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/Utils.h>
#include <orion/net/tcp/Utils.h>

#include <asio.hpp>

#include <string>

namespace orion
{
namespace net
{
namespace tcp
{
class SessionImpl;
///
///
///
class API_EXPORT Session
{
public:
   NO_COPY(Session);

   Session(asio::io_context& io_context);

   template<typename... Ts>
   Session(asio::io_context& io_context, Ts&&... ts)
      : Session(io_context)
   {
      set_option(std::forward<Ts>(ts)...);
   }

   Session(Session&& rhs) noexcept;
   ~Session();

   Session& operator=(Session&& rhs) noexcept;

   void set_option(const Parameters& parameters);
   void set_option(Parameters&& parameters);

   void set_option(const Timeout& timeout);
    
   template <typename T, typename... Ts>
   void set_option(Session& session, T&& t, Ts&&... ts)
   {
      set_option(std::forward<T>(t));
      set_option(std::forward<Ts>(ts)...);
   }

   bool connected() const;
   
   void on_connect(ConnectHandler h);
   void on_read(ReadHandler h);
   void on_write(WriteHandler h);

   void connect(const std::string& addr, int port);

   void write(std::streambuf* streambuf);

   void write(const uint8_t* data, std::size_t len);

   template<std::size_t N>
   void write(const std::array<uint8_t, N>& data, std::size_t len)
   {
      write(data.data(), len);
   }

   std::error_code close();

private:
   std::shared_ptr<SessionImpl> _impl; 
};

    
template <typename T>
void set_option(Session& session, T&& t)
{
   session.set_option(std::forward<T>(t));
}
    
template <typename T, typename... Ts>
void set_option(Session& session, T&& t, Ts&&... ts)
{
   set_option(session, std::forward<T>(t));
   set_option(session, std::forward<Ts>(ts)...);
}
    
} // namespace tcp
} // namespace net
} // namespace orion
#endif // ORION_NET_TCP_SESSION_H
