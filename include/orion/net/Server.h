//
// Server.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_SERVER_H
#define ORION_NET_SERVER_H

#include <orion/Orion-Stddefs.h>

#include <string>
#include <system_error>

namespace orion
{
namespace net
{
/// Server interface declaration
class Server
{
public:
   virtual ~Server() = default;

   virtual void shutdown() = 0;

   virtual bool is_running() const = 0;

   virtual std::error_code listen_and_serve(const std::string& addr, int port) = 0;
};

} // net
} // orion
#endif
