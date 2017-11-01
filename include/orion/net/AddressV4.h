//
// AddressV4.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_ADDRESSV4_H
#define ORION_NET_ADDRESSV4_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/Address.h>
#include <orion/net/Types.h>

#include <array>
#include <string>

namespace orion
{
namespace net
{

class API_EXPORT AddressV4 : public Address
{
public:
   /// Default constructor 
   AddressV4();
   /// Construct an address from an unsigned integer in host byte order.
   AddressV4(uint32_t value);
   /// Construct an address from raw bytes.
   AddressV4(const std::array<uint8_t, 4>& b);
   /// Construct an address from raw bytes.
   AddressV4(std::initializer_list<uint8_t> l);

   /// Copy constructor
   AddressV4(const AddressV4& other);
   /// Move constructor
   AddressV4(AddressV4&& other);

   virtual ~AddressV4();

   AddressV4& operator=(const AddressV4& rhs);
   AddressV4& operator=(AddressV4&& rhs);

   AddressV4& operator=(std::initializer_list<uint8_t> l);

   /// Determine whether the address is a class A address.
   bool is_class_a() const;
   /// Determine whether the address is a class B address.
   bool is_class_b() const;
   /// Determine whether the address is a class C address.
   bool is_class_c() const;

   /// Reports whether ip is a loopback address.
   virtual bool is_loopback() const override;
   /// Reports whether ip is a multicast address.
   virtual bool is_multicast() const override;
   /// Reports whether ip is an unspecified address.
   virtual bool is_unspecified() const override;
   
   /// Get the address in bytes, in network byte order.
   std::array<uint8_t, 4> to_bytes() const;
   /// Get the address as an unsigned long in host byte order.
   uint32_t to_ulong() const;

public:
   //
   // Friends:
   // 
   friend API_EXPORT std::string to_string(const AddressV4& addr);

   friend API_EXPORT bool operator==(const AddressV4& a1, const AddressV4& a2);
   friend API_EXPORT bool operator!=(const AddressV4& a1, const AddressV4& a2);
   friend API_EXPORT bool operator<(const AddressV4& a1, const AddressV4& a2);
   friend API_EXPORT bool operator>(const AddressV4& a1, const AddressV4& a2);
   friend API_EXPORT bool operator<=(const AddressV4& a1, const AddressV4& a2);
   friend API_EXPORT bool operator>=(const AddressV4& a1, const AddressV4& a2);

public:
   static AddressV4 broadcast;
   static AddressV4 all_systems;
   static AddressV4 all_routers;
   static AddressV4 zero;

private:
   in4_addr_type _a;
};

/// Convert an IPv4 address to a string
API_EXPORT std::string to_string(const AddressV4& addr);

/// Create an IPv4 address from an unsigned integer in host byte order.
API_EXPORT AddressV4 make_address_v4(uint32_t value);

/// Create an IPv4 address from raw bytes in network order.
API_EXPORT AddressV4 make_address_v4(const std::array<uint8_t, 4>& value);

/// Create an IPv4 address from an IP address string in dotted decimal form.
API_EXPORT AddressV4 make_address_v4(const std::string& value);

/// Convert an IPv4 address to a string
inline AddressV4 operator"" _ipv4(const char* str, std::size_t)
{
   return make_address_v4(str);
}

/// Compare two addresses for equality.
API_EXPORT bool operator==(const AddressV4& a1, const AddressV4& a2);
/// Compare two addresses for inequality.
API_EXPORT bool operator!=(const AddressV4& a1, const AddressV4& a2);
/// Compare addresses for ordering.
API_EXPORT bool operator<(const AddressV4& a1, const AddressV4& a2);
/// Compare addresses for ordering.
API_EXPORT bool operator>(const AddressV4& a1, const AddressV4& a2);
/// Compare addresses for ordering.
API_EXPORT bool operator<=(const AddressV4& a1, const AddressV4& a2);
/// Compare addresses for ordering.
API_EXPORT bool operator>=(const AddressV4& a1, const AddressV4& a2);

} // net
} // orion
#endif // ORION_NET_ADDRESSV4_H
