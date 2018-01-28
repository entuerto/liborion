//
// Connection.ipp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_TCP_CONNECTION_IPP
#define ORION_TCP_CONNECTION_IPP

#include <orion/Orion-Stddefs.h>

namespace orion
{
namespace net
{
namespace tcp
{

inline std::error_code set_option(Connection<asio::ip::tcp::socket>& conn, const NoDelay& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::ip::tcp::no_delay(value), ec);
   return ec;
}

} // tcp
} // net
} // orion
#endif // ORION_TCP_CONNECTION_IPP