//
// Address.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_ADDRESS_H
#define ORION_NET_ADDRESS_H

#include <orion/Config.h>

#include <array>
#include <string>

namespace orion
{
namespace net
{

/// AddressAddress represents the address of an Address end point.
class Address
{
public:
   virtual ~Address() = default;

   /// Reports whether Address is a loopback address.
   virtual bool is_loopback() const = 0;
   /// Reports whether Address is a multicast address.
   virtual bool is_multicast() const = 0;
   /// Reports whether Address is an unspecified address.
   virtual bool is_unspecified() const = 0;
};

/// Returns the string form of the address
API_EXPORT std::string to_string(const Address& addr);

/// Returns the string form of the address
API_EXPORT std::string to_string(const Address* addr);

API_EXPORT std::ostream& operator<<(std::ostream& o, const Address& addr);

API_EXPORT std::ostream& operator<<(std::ostream& o, const Address* addr);

} // namespace net
} // namespace orion
#endif // ORION_NET_ADDRESS_H
