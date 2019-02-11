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

#include <orion/net/Types.h>

#include <array>
#include <string>

namespace orion
{
namespace net
{
//-------------------------------------------------------------------------------------------------
// Class Address

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
std::string to_string(const Address& addr);

/// Returns the string form of the address
std::string to_string(const Address* addr);

/// Outputs to a stream
std::ostream& operator<<(std::ostream& o, const Address& addr);

/// Outputs to a stream
std::ostream& operator<<(std::ostream& o, const Address* addr);

//-------------------------------------------------------------------------------------------------
// Class AddressV4

class API_EXPORT AddressV4 : public Address
{
public:
   /// Default constructor
   AddressV4();
   /// Construct an address from an unsigned integer in host byte order.
   explicit AddressV4(uint32_t value);
   /// Construct an address from raw bytes.
   explicit AddressV4(const std::array<uint8_t, 4>& b);

   /// Copy constructor
   AddressV4(const AddressV4& other);
   /// Move constructor
   AddressV4(AddressV4&& other) noexcept;

   ~AddressV4() override = default;

   AddressV4& operator=(const AddressV4& rhs);
   AddressV4& operator=(AddressV4&& rhs) noexcept;

   AddressV4& operator=(std::initializer_list<uint8_t> l);

   /// Determine whether the address is a class A address.
   constexpr bool is_class_a() const;
   /// Determine whether the address is a class B address.
   constexpr bool is_class_b() const;
   /// Determine whether the address is a class C address.
   constexpr bool is_class_c() const;

   /// Reports whether ip is a loopback address.
   bool is_loopback() const override;
   /// Reports whether ip is a multicast address.
   bool is_multicast() const override;
   /// Reports whether ip is an unspecified address.
   bool is_unspecified() const override;

   /// Get the address in bytes, in network byte order.
   std::array<uint8_t, 4> to_bytes() const;
   /// Get the address as an unsigned long in host byte order.
   uint32_t to_ulong() const;

public:
   //
   // Friends:
   //
   friend API_EXPORT std::string to_string(const AddressV4& addr);

   friend constexpr bool operator==(const AddressV4& a1, const AddressV4& a2);
   friend constexpr bool operator!=(const AddressV4& a1, const AddressV4& a2);
   friend constexpr bool operator<(const AddressV4& a1, const AddressV4& a2);
   friend constexpr bool operator>(const AddressV4& a1, const AddressV4& a2);
   friend constexpr bool operator<=(const AddressV4& a1, const AddressV4& a2);
   friend constexpr bool operator>=(const AddressV4& a1, const AddressV4& a2);

public:
   static AddressV4& broadcast();
   static AddressV4& all_systems();
   static AddressV4& all_routers();
   static AddressV4& zero();

private:
   in4_addr_type _a{};
};

/// Convert an IPv4 address to a string
API_EXPORT std::string to_string(const AddressV4& addr);

/// Create an IPv4 address from an unsigned integer in host byte order.
AddressV4 make_address_v4(uint32_t value);

/// Create an IPv4 address from raw bytes in network order.
AddressV4 make_address_v4(const std::array<uint8_t, 4>& value);

/// Create an IPv4 address from an IP address string in dotted decimal form.
API_EXPORT AddressV4 make_address_v4(const std::string& value);

/// Convert an IPv4 address to a string
inline AddressV4 operator"" _ipv4(const char* str, std::size_t)
{
   return make_address_v4(str);
}

//-------------------------------------------------------------------------------------------------
// Class AddressV6

class API_EXPORT AddressV6 : public Address
{
public:
   /// Default constructor
   AddressV6() = default;
   /// Construct an address from raw bytes and scope ID.
   AddressV6(const std::array<uint8_t, 16>& b, uint32_t scope_id = 0);

   /// Copy constructor
   AddressV6(const AddressV6& other);
   /// Move constructor
   AddressV6(AddressV6&& other) noexcept;

   ~AddressV6() override = default;

   AddressV6& operator=(const AddressV6& rhs);
   AddressV6& operator=(AddressV6&& rhs) noexcept;

   AddressV6& operator=(std::initializer_list<uint8_t> l);

   /// The scope ID of the address.
   constexpr uint32_t scope_id() const;
   constexpr void scope_id(uint32_t value);

   /// Reports whether ip is a loopback address.
   bool is_loopback() const override;
   /// Reports whether ip is a multicast address.
   bool is_multicast() const override;
   /// Reports whether ip is an unspecified address.
   bool is_unspecified() const override;

   /// Get the address in bytes, in network byte order.
   std::array<uint8_t, 16> to_bytes() const;

public:
   //
   // Friends:
   // 
   friend API_EXPORT std::string to_string(const AddressV6& addr);

   friend constexpr bool operator==(const AddressV6& a1, const AddressV6& a2);
   friend constexpr bool operator!=(const AddressV6& a1, const AddressV6& a2);
   friend bool operator<(const AddressV6& a1, const AddressV6& a2);
   friend bool operator>(const AddressV6& a1, const AddressV6& a2);
   friend constexpr bool operator<=(const AddressV6& a1, const AddressV6& a2);
   friend constexpr bool operator>=(const AddressV6& a1, const AddressV6& a2);

public:
   static AddressV6& zero();
   static AddressV6& unspecified();
   static AddressV6& loopback();
   static AddressV6& interface_local_all_nodes();
   static AddressV6& link_local_all_nodes();
   static AddressV6& link_local_all_routers();

private:
   in6_addr_type _a{};
   uint32_t _scope_id{0};
};

/// Returns the string form of the IP address
API_EXPORT std::string to_string(const AddressV6& addr);

/// Create an IPv6 address from raw bytes and scope ID.
AddressV6 make_address_v6(const std::array<uint8_t, 16>& b, uint32_t scope_id = 0);

/// Create an IPv6 address from an IP address string.
API_EXPORT AddressV6 make_address_v6(const std::string& s);

/// Convert an IPv6 address to a string
inline AddressV6 operator"" _ipv6(const char* str, std::size_t)
{
   return make_address_v6(str);
}

} // namespace net
} // namespace orion

#include <orion/net/impl/Address.ipp>

#endif // ORION_NET_ADDRESS_H
