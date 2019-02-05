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

using namespace std::literals::chrono_literals;

namespace orion
{
namespace net
{
namespace http
{

ServerImpl::ServerImpl()
   : _endpoint()
   , _mux()
   , _read_timeout(60s)
   , _tls_handshake_timeout(60s)
   , _io_context()
   , _signals(_io_context)
   , _listener()
{
}

ServerImpl::~ServerImpl() = default;

EndPoint ServerImpl::endpoint() const
{
   return _listener->endpoint();
}

RequestMux& ServerImpl::request_mux()
{
   return _mux;
}

bool ServerImpl::is_running() const
{
   return _listener->is_listening();
}

void ServerImpl::shutdown()
{
   if (_listener == nullptr)
      return;

   std::error_code ec = _listener->close();

   if (ec)
      log::error(ec, DbgSrcLoc);
}

void ServerImpl::tls_handshake_timeout(const std::chrono::seconds& t)
{
   _tls_handshake_timeout = t;
}

void ServerImpl::read_timeout(const std::chrono::seconds& t)
{
   _read_timeout = t;
}

std::error_code ServerImpl::listen_and_serve(EndPoint endpoint)
{
   setup_signals();

   _listener = std::make_shared<ListenerType>(_io_context, std::move(endpoint), _mux);

   _listener->read_timeout(_read_timeout);
   //_listener->tls_handshake_timeout(_tls_handshake_timeout);

   auto ec = _listener->start();
   if (ec)
      return ec;

   _io_context.run();

   return ec;
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
         log::error(ec, DbgSrcLoc);

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
