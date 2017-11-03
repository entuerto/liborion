//
// Server.h
//
//  Created by Tomas Palazuelos on 2017-10-13.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_NET_TCP_SERVER_H
#define ORION_NET_TCP_SERVER_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/Server.h>
#include <orion/net/tcp/Utils.h>

#include <memory>
#include <string>

namespace orion
{
namespace net
{
namespace tcp
{
class ServerImpl;

//! This class provides a tcp protocol Server
/*!
   This class provides a tcp protocol Server to handle incoming requests
   and sends valid responses.
 */
class API_EXPORT Server : public net::Server
{
public:
   NO_COPY(Server)

   Server();
   Server(Handler h);
   virtual ~Server();

   Server(Server&& other) noexcept;
   Server& operator=(Server&& other) noexcept;

   virtual int port() const;

   virtual void register_handler(Handler h);

   virtual void shutdown() override;

   virtual bool is_running() const override;

   virtual std::error_code listen_and_serve(const std::string& addr, int port) override;

private:
   std::unique_ptr<ServerImpl> _impl;  
};

} // tcp
} // net
} // orion
#endif // ORION_NET_TCP_SERVER_H