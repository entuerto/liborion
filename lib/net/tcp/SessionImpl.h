//
//  SessionImpl.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_TCP_SESSIONIMPL_H
#define ORION_NET_TCP_SESSIONIMPL_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/Utils.h>
#include <orion/net/tcp/Utils.h>

#include <asio.hpp>

#include <string>
#include <thread>

namespace orion
{
namespace net
{
namespace tcp
{
///
///
///
class SessionImpl : public std::enable_shared_from_this<SessionImpl>
{
public:
   NO_COPY(SessionImpl);

   SessionImpl(asio::io_service& io_service);
   ~SessionImpl();

   void set_option(const Parameters& parameters);
   void set_option(Parameters&& parameters);

   void set_option(const Timeout& timeout);

   bool connected() const;

   void on_connect(ConnectHandler h);
   void on_read(ReadHandler h);
   void on_write(WriteHandler h);
    
   void connect(const std::string& addr, int port);

   void write(std::streambuf* streambuf);

   void write(const uint8_t* data, std::size_t len);

   std::error_code close();

private:
   void do_read();
   void do_write();

private:
   Parameters _params;
   Timeout _timeout;

   bool _connected;

   ConnectHandler _connect_handler;
   ReadHandler    _read_handler;
   WriteHandler   _write_handler;

   /// The io_service used to perform asynchronous operations.
   asio::io_service& _io_service;

   /// Socket for the connection.
   asio::ip::tcp::socket _socket;

   /// Buffer for incoming data. 
   asio::streambuf _in_streambuf;

   /// Buffer for outgoing data. 
   asio::streambuf _out_streambuf;

};
    
} // namespace tcp
} // namespace net
} // namespace orion
#endif // ORION_NET_TCP_SESSIONIMPL_H
