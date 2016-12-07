//
// Server.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#include <net/http/AsioServer.h>

#include <future>

#include <orion/Logging.h>
#include <net/http/AsioServerConnection.h>

using namespace orion::logging;

namespace orion
{
namespace net
{
namespace http
{

AsioServer::AsioServer() :
   Server(),
   _io_service(),
   _signals(_io_service),
   _acceptors()
{
}

AsioServer::~AsioServer()
{
}

void AsioServer::shutdown()
{
   asio::error_code ec;
   
   for (auto& acceptor : _acceptors)
   {
      acceptor.close(ec);

      if (ec)
         LOG(Error) << ec;
   }
}

std::error_code AsioServer::listen_and_serve(const std::string& addr, int port)
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

void AsioServer::do_accept(asio::ip::tcp::acceptor& acceptor)
{
   auto conn = std::make_shared<AsioServerConnection>(_io_service, _RequestHandlers);

   acceptor.async_accept(conn->socket(),
      [this, &acceptor, conn](std::error_code ec)
      {
         // Check whether the server was stopped by a signal before this
         // completion handler had a chance to run.
         if (not acceptor.is_open())
            return;

         if (not ec)
         {
            conn->socket().set_option(asio::ip::tcp::socket::keep_alive(true));
            conn->socket().set_option(asio::ip::tcp::no_delay(true));
            conn->accept(); 
         }

         do_accept(acceptor);
      });
}

void AsioServer::do_close()
{
   _signals.async_wait(
      [this](std::error_code ec, int /*signo*/)
      {
         if (ec)
            LOG(Error) << ec;

         // The server is stopped by cancelling all outstanding asynchronous
         // operations. Once all operations have finished the io_service::run()
         // call will exit.
         for (auto& acceptor : _acceptors)
            acceptor.close();
      });
}

void AsioServer::setup_signals()
{
   // Register to handle the signals that indicate when the server should exit.
   _signals.add(SIGINT);
   _signals.add(SIGTERM);
   //_impl->signals.add(SIGQUIT);

   do_close();
}

} // http
} // net
} // orion
   