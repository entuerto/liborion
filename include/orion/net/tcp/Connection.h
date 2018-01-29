//
// Connection.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_TCP_CONNECTION_H
#define ORION_NET_TCP_CONNECTION_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/Connection.h>
#include <orion/net/tcp/Utils.h>

#include <asio.hpp>

#include <system_error>

namespace orion
{
namespace net
{
namespace tcp
{

class Connection : public net::Connection<asio::ip::tcp::socket>
{
public:
   Connection(asio::ip::tcp::socket socket, const Handler& handler);
   virtual ~Connection();

protected:
   /// Perform an asynchronous read operation.
   void do_read() override;

   /// Perform an asynchronous write operation.
   void do_write() override;

private:
   std::error_code on_handler(std::streambuf* in, std::streambuf* out);

   /// Request handler
   const Handler& _handler;

   /// Buffer for incoming data.
   asio::streambuf _in_streambuf;

   /// Buffer for outgoing data.
   asio::streambuf _out_streambuf;

   std::size_t _in_buffer_size;
};

/// Controls whether the operating system should delay packet transmission in hopes of
/// sending fewer packets (Nagle's algorithm). The default is true (no delay), meaning
/// that data is sent as soon as possible after a Write.
inline std::error_code set_option(Connection& conn, const NoDelay& value);

} // tcp
} // net
} // orion

#include <orion/net/tcp/impl/Connection.ipp>

#endif // ORION_NET_TCP_CONNECTION_H
