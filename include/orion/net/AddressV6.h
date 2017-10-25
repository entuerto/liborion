//
// AddressV6.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_ADDRESSV6_H
#define ORION_NET_ADDRESSV6_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/Address.h>
#include <orion/net/Types.h>

#include <array>
#include <string>

namespace orion
{
namespace net
{

class API_EXPORT AddressV6 : public Address
{
public:
   AddressV6();
   AddressV6(const std::array<uint8_t, 16>& b, uint32_t scope_id = 0);
   AddressV6(std::initializer_list<uint8_t> l);
   AddressV6(const AddressV6& other);
   AddressV6(AddressV6&& other);
   virtual ~AddressV6();

   AddressV6& operator=(const AddressV6& rhs);
   AddressV6& operator=(AddressV6&& rhs);

   AddressV6& operator=(std::initializer_list<uint8_t> l);

   /// The scope ID of the address.
   uint32_t scope_id() const;
   void scope_id(uint32_t value);

   /// Reports whether ip is a loopback address.
   virtual bool is_loopback() const override;
   /// Reports whether ip is a multicast address.
   virtual bool is_multicast() const override;
   /// Reports whether ip is an unspecified address.
   virtual bool is_unspecified() const override;

   /// Returns the string form of the IP address
   virtual std::string to_string() const override;

   /// Get the address in bytes, in network byte order.
   std::array<uint8_t, 16> to_bytes() const;

   static AddressV6 parse(const std::string& s);

public:
   // Friends
   friend API_EXPORT bool operator==(const AddressV6& a1, const AddressV6& a2);
   friend API_EXPORT bool operator!=(const AddressV6& a1, const AddressV6& a2);
   friend API_EXPORT bool operator<(const AddressV6& a1, const AddressV6& a2);
   friend API_EXPORT bool operator>(const AddressV6& a1, const AddressV6& a2);
   friend API_EXPORT bool operator<=(const AddressV6& a1, const AddressV6& a2);
   friend API_EXPORT bool operator>=(const AddressV6& a1, const AddressV6& a2);

public:
   static AddressV6 zero;
   static AddressV6 unspecified;
   static AddressV6 loopback;
   static AddressV6 interface_local_all_nodes;
   static AddressV6 link_local_all_nodes;
   static AddressV6 link_local_all_routers;

private:
   in6_addr_type _a;
   uint32_t _scope_id;
};

/// Compare two addresses for equality.
API_EXPORT bool operator==(const AddressV6& a1, const AddressV6& a2);
/// Compare two addresses for inequality.
API_EXPORT bool operator!=(const AddressV6& a1, const AddressV6& a2);
/// Compare addresses for ordering.
API_EXPORT bool operator<(const AddressV6& a1, const AddressV6& a2);
/// Compare addresses for ordering.
API_EXPORT bool operator>(const AddressV6& a1, const AddressV6& a2);
/// Compare addresses for ordering.
API_EXPORT bool operator<=(const AddressV6& a1, const AddressV6& a2);
/// Compare addresses for ordering.
API_EXPORT bool operator>=(const AddressV6& a1, const AddressV6& a2);
} // net
} // orion
#endif // ORION_NET_ADDRESSV6_H
