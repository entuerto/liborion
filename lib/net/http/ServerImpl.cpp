//
// ServerImpl.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <net/http/ServerImpl.h>

#include <orion/Log.h>
#include <orion/net/EndPoint.h>
#include <orion/net/Utils.h>

#include <future>

namespace orion
{
namespace net
{
namespace http
{

ServerImpl::ServerImpl()
   : _port(0)
   , _mux()
   , _io_context()
   , _signals(_io_context)
   , _listener()
{
}

ServerImpl::~ServerImpl() = default;

uint16_t ServerImpl::port() const
{
   return _port;
}

RequestMux& ServerImpl::request_mux()
{
   return _mux;
}

bool ServerImpl::is_running() const
{
   return true; //_acceptor.is_open();
}

void ServerImpl::shutdown()
{
   if (_listener == nullptr)
      return;

   std::error_code ec = _listener->close();

   if (ec)
      log::error(ec);
}

std::error_code ServerImpl::listen_and_serve(EndPoint endpoint)
{
   setup_signals();

   _listener = std::make_shared<http::Listener>(_io_context, std::move(endpoint), _mux);

   _listener->start();

   _io_context.run();

   return std::error_code();
}

std::error_code ServerImpl::listen_and_serve(EndPoint endpoint, RequestMux mux)
{
   _mux = std::move(mux);

   return listen_and_serve(std::move(endpoint));
}

void ServerImpl::do_await_close()
{
   _signals.async_wait([this](std::error_code ec, int /*signo*/) {
      if (ec)
         log::error(ec);

      // The server is stopped by cancelling all outstanding asynchronous
      // operations. Once all operations have finished the io_context::run()
      // call will exit.
      if (_listener != nullptr)
         _listener->close();
   });
}

void ServerImpl::setup_signals()
{
   // Register to handle the signals that indicate when the server should exit.
   _signals.add(SIGINT);
   _signals.add(SIGTERM);

#if defined(SIGQUIT)   
   _signals.add(SIGQUIT);
#endif

   do_await_close();
}

} // http
} // net
} // orion
