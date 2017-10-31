//
// ServerImpl.cpp
//
//  Created by Tomas Palazuelos on 2017-10-13.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#include <net/tcp/ServerImpl.h>

#include <orion/Log.h>
#include <orion/net/tcp/Utils.h>

#include <net/tcp/ServerConnection.h>

#include <future>

namespace orion
{
namespace net
{
namespace tcp
{

ServerImpl::ServerImpl()
   : _port(-1)
   , _is_running(false)
   , _handler()
   , _io_service()
   , _signals(_io_service)
   , _acceptors()
{
}

ServerImpl::ServerImpl(Handler h)
   : _port(-1)
   , _is_running(false)
   , _handler(std::move(h))
   , _io_service()
   , _signals(_io_service)
   , _acceptors()
{
}

ServerImpl::~ServerImpl()
{
}

int ServerImpl::port() const
{
   return _port;
}

void ServerImpl::register_handler(Handler h)
{
   _handler = std::move(h);
}

bool ServerImpl::is_running() const
{
   return _is_running;
}

void ServerImpl::shutdown()
{
   asio::error_code ec;

   for (auto& acceptor : _acceptors)
   {
      acceptor.close(ec);

      if (ec)
         log::error(ec);
   }
}

std::error_code ServerImpl::listen_and_serve(const std::string& addr, int port)
{
   setup_signals();

   // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
   asio::ip::tcp::resolver resolver(_io_service);
   asio::ip::tcp::resolver::query query(addr, std::to_string(port));

   std::error_code ec;

   auto it = resolver.resolve(query, ec);
   if (ec)
      return ec;

   for (; it != asio::ip::tcp::resolver::iterator(); ++it)
   {
      asio::ip::tcp::endpoint endpoint = *it;

      auto acceptor = asio::ip::tcp::acceptor(_io_service);

      if (acceptor.open(endpoint.protocol(), ec))
         continue;

      acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));

      if (acceptor.bind(endpoint, ec))
         continue;

      if (acceptor.listen(asio::socket_base::max_connections, ec))
         continue;

      _acceptors.push_back(std::move(acceptor));
   }

   if (_acceptors.empty())
      return ec;

   for (auto& acceptor : _acceptors)
   {
      do_accept(acceptor);
   }

   _io_service.run();

   return std::error_code();
}

void ServerImpl::do_accept(asio::ip::tcp::acceptor& acceptor)
{
   auto conn = std::make_shared<ServerConnection>(_io_service, _handler);

   acceptor.async_accept(conn->socket(), [this, &acceptor, conn](std::error_code ec) {
      // Check whether the server was stopped by a signal before this
      // completion handler had a chance to run.
      if (not acceptor.is_open())
         return;

      if (not ec)
      {
         conn->set_option(KeepAlive{true});
         conn->set_option(NoDelay{true});
         conn->accept();
      }

      do_accept(acceptor);
   });
}

void ServerImpl::do_close()
{
   _signals.async_wait([this](std::error_code ec, int /*signo*/) {
      if (ec)
         log::error(ec);

      // The server is stopped by cancelling all outstanding asynchronous
      // operations. Once all operations have finished the io_service::run()
      // call will exit.
      for (auto& acceptor : _acceptors)
         acceptor.close();
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

   do_close();
}

} // tcp
} // net
} // orion
