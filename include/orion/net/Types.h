//
// Types.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_TYPES_H
#define ORION_NET_TYPES_H

#include <orion/Orion-Stddefs.h>

#if defined(ORION_WINDOWS)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/un.h>
#endif

namespace orion
{
namespace net
{

#if defined(ORION_WINDOWS) || defined(ORION_CYGWIN)

typedef sockaddr socket_addr_type;

typedef in_addr in4_addr_type;
typedef ip_mreq in4_mreq_type;
typedef sockaddr_in sockaddr_in4_type;

typedef in6_addr in6_addr_type;
typedef ipv6_mreq in6_mreq_type;
typedef sockaddr_in6 sockaddr_in6_type;

typedef sockaddr_storage sockaddr_storage_type;
typedef addrinfo addrinfo_type;

#else

typedef sockaddr socket_addr_type;

typedef in_addr in4_addr_type;
typedef ip_mreq in4_mreq_type;
typedef sockaddr_in sockaddr_in4_type;

typedef in6_addr in6_addr_type;
typedef ipv6_mreq in6_mreq_type;
typedef sockaddr_in6 sockaddr_in6_type;

typedef sockaddr_storage sockaddr_storage_type;
typedef addrinfo addrinfo_type;

#endif

} // namespace net
} // namespace orion
#endif // ORION_NET_TYPES_H
