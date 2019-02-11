//
// EndPoint.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_ENDPOINT_H
#define ORION_NET_ENDPOINT_H

#include <orion/Config.h>

#include <orion/net/Address.h>

#include <memory>
#include <string>

namespace orion
{
namespace net
{

/// EndPoint represents the address of an IP end point.
class EndPoint
{
public:
   EndPoint() = default;
   /// Construct an endpoint using a port number and an IPv4 address.
   EndPoint(const AddressV4& addr, uint16_t port);
   /// Construct an endpoint using a port number and an IPv6 address.
   EndPoint(const AddressV6& addr, uint16_t port);

   EndPoint(const EndPoint& other);
   EndPoint(EndPoint&& other) noexcept;
   ~EndPoint() = default;

   constexpr EndPoint& operator=(const EndPoint& rhs);
   constexpr EndPoint& operator=(EndPoint&& rhs) noexcept;

   constexpr Address* address() const;

   constexpr uint16_t port() const;

private:
   std::unique_ptr<Address> _addr;
   uint16_t _port{0};
};

std::string to_string(const EndPoint& ep);

std::ostream& operator<<(std::ostream& o, const EndPoint& ep);

} // namespace net
} // namespace orion

#include <orion/net/impl/EndPoint.ipp>

#endif // ORION_NET_ENDPOINT_H
