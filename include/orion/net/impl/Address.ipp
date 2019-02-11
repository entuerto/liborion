//
// Address.ipp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_ADDRESS_IPP
#define ORION_NET_ADDRESS_IPP

#include <orion/Log.h>
#include <orion/String.h>
#include <orion/Utils.h>
#include <orion/net/Error.h>

#include <algorithm>
#include <cstring>
#include <ostream>

namespace orion
{
namespace net
{

//-------------------------------------------------------------------------------------------------

inline std::string to_string(const Address& addr)
{
   return to_string(&addr);
}

inline std::string to_string(const Address* addr)
{
   auto ip_v4 = dynamic_cast<const AddressV4*>(addr);

   if (ip_v4 != nullptr)
      return to_string(*ip_v4);

   auto ip_v6 = dynamic_cast<const AddressV6*>(addr);

   if (ip_v6 != nullptr)
      return to_string(*ip_v6);

   throw std::system_error{net::make_error_code(ErrorCode::InvalidAddress)};
}

//-------------------------------------------------------------------------------------------------

inline std::ostream& operator<<(std::ostream& o, const Address& addr)
{
   o << to_string(addr);
   return o;
}

inline std::ostream& operator<<(std::ostream& o, const Address* addr)
{
   o << (addr == nullptr ? "null" : to_string(addr));
   return o;
}

//-------------------------------------------------------------------------------------------------
// Class AddressV4 Implementation

inline AddressV4::AddressV4()
{
   _a.s_addr = 0;
}

inline AddressV4::AddressV4(const std::array<uint8_t, 4>& b)
{
   std::memcpy(&_a.s_addr, b.data(), 4);
}

inline AddressV4::AddressV4(const AddressV4& other)
   : _a(other._a)
{
}

inline AddressV4::AddressV4(AddressV4&& other) noexcept
   : _a(std::move(other._a))
{
}

inline AddressV4& AddressV4::operator=(const AddressV4& rhs)
{
   if (this == &rhs)
   {
      return *this;
   }

   _a = rhs._a;

   return *this;
}

inline AddressV4& AddressV4::operator=(AddressV4&& rhs) noexcept
{
   _a = std::move(rhs._a);
   return *this;
}

inline AddressV4& AddressV4::operator=(std::initializer_list<uint8_t> l)
{
   std::memcpy(&_a.s_addr, l.begin(), 4);
   return *this;
}

/// Determine whether the address is a class A address.
inline constexpr bool AddressV4::is_class_a() const
{
   return (to_ulong() & 0x80000000) == 0;
}

/// Determine whether the address is a class B address.
inline constexpr bool AddressV4::is_class_b() const
{
   return (to_ulong() & 0xC0000000) == 0x80000000;
}

/// Determine whether the address is a class C address.
inline constexpr bool AddressV4::is_class_c() const
{
   return (to_ulong() & 0xE0000000) == 0xC0000000;
}

inline bool AddressV4::is_loopback() const
{
   return (to_ulong() & 0xFF000000) == 0x7F000000;
}

inline bool AddressV4::is_multicast() const
{
   return (to_ulong() & 0xF0000000) == 0xE0000000;
}

inline bool AddressV4::is_unspecified() const
{
   return to_ulong() == 0;
}

inline std::array<uint8_t, 4> AddressV4::to_bytes() const
{
   std::array<uint8_t, 4> bytes{};

   std::memcpy(bytes.data(), &_a.s_addr, 4);

   return bytes;
}

inline AddressV4 make_address_v4(uint32_t value)
{
   return AddressV4{value};
}

inline AddressV4 make_address_v4(const std::array<uint8_t, 4>& value)
{
   return AddressV4{value};
}

inline constexpr bool operator==(const AddressV4& a1, const AddressV4& a2)
{
   return a1._a.s_addr == a2._a.s_addr;
}

inline constexpr bool operator!=(const AddressV4& a1, const AddressV4& a2)
{
   return not (a1 == a2);
}

inline constexpr bool operator<(const AddressV4& a1, const AddressV4& a2)
{
   return a1.to_ulong() < a2.to_ulong();
}

inline constexpr bool operator>(const AddressV4& a1, const AddressV4& a2)
{
   return a1.to_ulong() > a2.to_ulong();
}

inline constexpr bool operator<=(const AddressV4& a1, const AddressV4& a2)
{
   return a1.to_ulong() <= a2.to_ulong();
}

inline constexpr bool operator>=(const AddressV4& a1, const AddressV4& a2)
{
   return a1.to_ulong() >= a2.to_ulong();
}

//--------------------------------------------------------------------------------------------------
// Static global AddressV4 adresses

inline AddressV4& AddressV4::broadcast()
{  
   try
   { 
      static AddressV4 addr{{255, 255, 255, 255}};
      return addr;
   }
   catch (...)
   {
      log::fatal(type_name(std::current_exception()), 
         "An unexpected, unknown exception was thrown: ", DbgSrcLoc);
   }
}

inline AddressV4& AddressV4::all_systems()
{  
   try
   { 
      static AddressV4 addr{{224, 0, 0, 1}};
      return addr;
   }
   catch (...)
   {
      log::fatal(type_name(std::current_exception()), 
         "An unexpected, unknown exception was thrown: ", DbgSrcLoc);
   }
}

inline AddressV4& AddressV4::all_routers()
{  
   try
   { 
      static AddressV4 addr{{224, 0, 0, 2}};
      return addr;
   }
   catch (...)
   {
      log::fatal(type_name(std::current_exception()), 
         "An unexpected, unknown exception was thrown: ", DbgSrcLoc);
   }
}

inline AddressV4& AddressV4::zero()
{  
   try
   { 
      static AddressV4 addr{{0, 0, 0, 0}};
      return addr;
   }
   catch (...)
   {
      log::fatal(type_name(std::current_exception()), 
         "An unexpected, unknown exception was thrown: ", DbgSrcLoc);
   }
}

//-------------------------------------------------------------------------------------------------
// Class AddressV6 Implementation

inline AddressV6::AddressV6(const std::array<uint8_t, 16>& b, uint32_t scope_id /* = 0 */)
   : _scope_id(scope_id)
{
   std::memcpy(_a.s6_addr, b.data(), 16);
}

inline AddressV6::AddressV6(const AddressV6& other)
   : _a(other._a)
   , _scope_id(other._scope_id)
{
}

inline AddressV6::AddressV6(AddressV6&& other) noexcept
   : _a(other._a)
   , _scope_id(other._scope_id)
{
}

inline AddressV6& AddressV6::operator=(const AddressV6& rhs)
{
   if (this == &rhs)
   {
      return *this;
   }

   std::memcpy(_a.s6_addr, rhs._a.s6_addr, 16);

   _scope_id = rhs._scope_id;

   return *this;
}

inline AddressV6& AddressV6::operator=(AddressV6&& rhs) noexcept
{
   std::memcpy(_a.s6_addr, rhs._a.s6_addr, 16);
   _scope_id = rhs._scope_id;
   return *this;
}

inline AddressV6& AddressV6::operator=(std::initializer_list<uint8_t> l)
{
   std::memcpy(_a.s6_addr, l.begin(), 16);
   return *this;
}

inline constexpr uint32_t AddressV6::scope_id() const
{
   return _scope_id;
}

inline constexpr void AddressV6::scope_id(uint32_t value)
{
   _scope_id = value;
}

inline bool AddressV6::is_loopback() const
{
   return std::equal(std::begin(_a.s6_addr), std::end(_a.s6_addr), std::begin(loopback()._a.s6_addr));
}

inline bool AddressV6::is_multicast() const
{
   return _a.s6_addr[0] == 0xff;
}

inline bool AddressV6::is_unspecified() const
{
   return std::equal(
      std::begin(_a.s6_addr), std::end(_a.s6_addr), std::begin(unspecified()._a.s6_addr));
}

inline std::array<uint8_t, 16> AddressV6::to_bytes() const
{
   std::array<uint8_t, 16> bytes{};

   std::memcpy(bytes.data(), _a.s6_addr, 16);

   return bytes;
}

inline AddressV6 make_address_v6(const std::array<uint8_t, 16>& b, uint32_t scope_id /* = 0 */)
{
   return AddressV6{b, scope_id};
}

inline constexpr bool operator==(const AddressV6& a1, const AddressV6& a2)
{
   return a1._scope_id == a2._scope_id and
          std::equal(std::begin(a1._a.s6_addr), std::end(a1._a.s6_addr), std::begin(a2._a.s6_addr));
}

inline constexpr bool operator!=(const AddressV6& a1, const AddressV6& a2)
{
   return not(a1 == a2);
}

inline bool operator<(const AddressV6& a1, const AddressV6& a2)
{
   return std::lexicographical_compare(std::begin(a1._a.s6_addr),
                                       std::end(a1._a.s6_addr),
                                       std::begin(a2._a.s6_addr),
                                       std::end(a2._a.s6_addr));
}

inline bool operator>(const AddressV6& a1, const AddressV6& a2)
{
   return not std::lexicographical_compare(std::begin(a1._a.s6_addr),
                                           std::end(a1._a.s6_addr),
                                           std::begin(a2._a.s6_addr),
                                           std::end(a2._a.s6_addr));
}

inline constexpr bool operator<=(const AddressV6& a1, const AddressV6& a2)
{
   return a1 == a2 or a1 < a2;
}

inline constexpr bool operator>=(const AddressV6& a1, const AddressV6& a2)
{
   return a1 == a2 or a1 > a2;
}

//--------------------------------------------------------------------------------------------------
// Static global AddressV6 adresses

inline AddressV6& AddressV6::zero()
{  
   try
   { 
      static AddressV6 addr{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
      return addr;
   }
   catch (...)
   {
      log::fatal(type_name(std::current_exception()), 
         "An unexpected, unknown exception was thrown: ", DbgSrcLoc);
   }
}

inline AddressV6& AddressV6::unspecified()
{  
   try
   { 
      static AddressV6 addr{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
      return addr;
   }
   catch (...)
   {
      log::fatal(type_name(std::current_exception()), 
         "An unexpected, unknown exception was thrown: ", DbgSrcLoc);
   }
}

inline AddressV6& AddressV6::loopback()
{  
   try
   { 
      static AddressV6 addr{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}};
      return addr;
   }
   catch (...)
   {
      log::fatal(type_name(std::current_exception()), 
         "An unexpected, unknown exception was thrown: ", DbgSrcLoc);
   }
}

inline AddressV6& AddressV6::interface_local_all_nodes()
{  
   try
   { 
      static AddressV6 addr{{0xff, 0x01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x01}};
      return addr;
   }
   catch (...)
   {
      log::fatal(type_name(std::current_exception()), 
         "An unexpected, unknown exception was thrown: ", DbgSrcLoc);
   }
}

inline AddressV6& AddressV6::link_local_all_nodes()
{  
   try
   { 
      static AddressV6 addr{{0xff, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x01}};
      return addr;
   }
   catch (...)
   {
      log::fatal(type_name(std::current_exception()), 
         "An unexpected, unknown exception was thrown: ", DbgSrcLoc);
   }
}

inline AddressV6& AddressV6::link_local_all_routers()
{  
   try
   { 
      static AddressV6 addr{{0xff, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x02}};
      return addr;
   }
   catch (...)
   {
      log::fatal(type_name(std::current_exception()), 
         "An unexpected, unknown exception was thrown: ", DbgSrcLoc);
   }
}

} // net
} // orion

#endif // ORION_NET_ADDRESS_IPP
