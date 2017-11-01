//
// EndPoint.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_ENDPOINT_H
#define ORION_NET_ENDPOINT_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/Address.h>
#include <orion/net/AddressV4.h>
#include <orion/net/AddressV6.h>

#include <memory>
#include <string>

namespace orion
{
namespace net
{

/// EndPoint represents the address of an IP end point.
class API_EXPORT EndPoint
{
public:
   EndPoint();
   /// Construct an endpoint using a port number and an IPv4 address.
   EndPoint(const AddressV4& addr, uint16_t port);
   /// Construct an endpoint using a port number and an IPv6 address.
   EndPoint(const AddressV6& addr, uint16_t port);

   EndPoint(const EndPoint& other);
   EndPoint(EndPoint&& other);
   virtual ~EndPoint();

   EndPoint& operator=(const EndPoint& rhs);
   EndPoint& operator=(EndPoint&& rhs);

   virtual Address* address() const;

   virtual uint16_t port() const;

private:
   std::unique_ptr<Address> _addr;
   uint16_t _port;
};

API_EXPORT std::string to_string(const EndPoint& ep);

inline std::ostream& operator<<(std::ostream& o, const EndPoint& ep)
{
   o << to_string(ep);
   return o;
}

} // net
} // orion
#endif // ORION_NET_ENDPOINT_H
