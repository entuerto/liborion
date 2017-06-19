//
// AsioServer.h
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_NET_HTTP_ASIOSERVER_H
#define ORION_NET_HTTP_ASIOSERVER_H

#include <orion/Orion-Stddefs.h>
#include <orion/net/http/Server.h>

#include <asio.hpp>

#include <string>
#include <vector>

namespace orion
{
namespace net
{
namespace http
{
/// Implement a http server with lib asio
class AsioServer : public Server
{
public:
   NO_COPY(AsioServer)

   AsioServer();
   virtual ~AsioServer();

   void shutdown() override;

   std::error_code listen_and_serve(const std::string& addr, int port) override;

protected:
   void do_accept(asio::ip::tcp::acceptor& acceptor);
   void do_close();

private:
   void setup_signals();

   // The io_service used to perform asynchronous operations.
   asio::io_service _io_service;

   // The signal_set is used to register for process termination notifications.
   asio::signal_set _signals;

   // Acceptor used to listen for incoming connections.
   std::vector<asio::ip::tcp::acceptor> _acceptors;
};

} // http
} // net
} // orion
#endif
