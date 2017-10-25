//
// Address.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_ADDRESS_H
#define ORION_NET_ADDRESS_H

#include <orion/Orion-Stddefs.h>

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

   /// Returns the string form of the Address address
   virtual std::string to_string() const = 0;
};

inline std::ostream& operator<<(std::ostream& o, const Address& Address)
{
   o << Address.to_string();
   return o;
}

inline std::ostream& operator<<(std::ostream& o, const Address* Address)
{
   o << (Address == nullptr ? "null" : Address->to_string());
   return o;
}

} // net
} // orion
#endif // ORION_NET_ADDRESS_H
