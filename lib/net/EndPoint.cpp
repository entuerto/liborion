//
// EndPoint.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/EndPoint.h>

#include <orion/String.h>
#include <cstdio>

namespace orion
{
namespace net
{

namespace priv
{

Address* clone(const Address* addr)
{
   const AddressV4* addr_v4 = dynamic_cast<const AddressV4*>(addr);

   if (addr_v4 != nullptr)
      return new AddressV4(*addr_v4);

   const AddressV6* addr_v6 = dynamic_cast<const AddressV6*>(addr);

   if (addr_v6 != nullptr)
      return new AddressV6(*addr_v6);

   return nullptr;
}

} // namespace priv

//---------------------------------------------------------------------------------------
// EndPoint

EndPoint::EndPoint()
   : _addr()
   , _port(0)
{
}

EndPoint::EndPoint(const AddressV4& addr, uint16_t port)
   : _addr(std::make_unique<AddressV4>(addr))
   , _port(port)
{
}

EndPoint::EndPoint(const AddressV6& addr, uint16_t port)
   : _addr(std::make_unique<AddressV6>(addr))
   , _port(port)
{
}

EndPoint::EndPoint(const EndPoint& other)
   : _addr(priv::clone(other._addr.get()))
   , _port(other._port)
{
}

EndPoint::EndPoint(EndPoint&& other) noexcept
   : _addr(std::move(other._addr))
   , _port(std::move(other._port))
{
}

EndPoint::~EndPoint() = default;

EndPoint& EndPoint::operator=(const EndPoint& rhs)
{
   if (this == &rhs)
      return *this;

   _addr = std::unique_ptr<Address>(priv::clone(rhs._addr.get()));
   _port = rhs._port;

   return *this;
}

EndPoint& EndPoint::operator=(EndPoint&& rhs) noexcept
{
   _addr = std::move(rhs._addr);
   _port = std::move(rhs._port);
   return *this;
}

Address* EndPoint::address() const
{
   return _addr.get();
}

uint16_t EndPoint::port() const
{
   return _port;
}

std::string to_string(const EndPoint& ep)
{
   return to_string(ep.address()) + ":" + std::to_string(ep.port());
}

std::ostream& operator<<(std::ostream& o, const EndPoint& ep)
{
   o << to_string(ep);
   return o;
}

} // net
} // orion
