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

#include <system_error>

namespace orion
{
namespace net
{
namespace tcp
{

/// Controls whether the operating system should delay packet transmission in hopes of
/// sending fewer packets (Nagle's algorithm). The default is true (no delay), meaning
/// that data is sent as soon as possible after a Write.
inline std::error_code set_option(Connection<TcpSocket>& conn, const NoDelay& value);

} // tcp
} // net
} // orion

#include <orion/net/tcp/impl/Connection.ipp>

#endif // ORION_NET_TCP_CONNECTION_H
