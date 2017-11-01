//
// AddressV6.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/AddressV6.h>

#include <orion/StringUtils.h>

#include <algorithm>
#include <cstring>

namespace orion
{
namespace net
{

//---------------------------------------------------------------------------------------
// AddressV6
AddressV6 AddressV6::zero                      = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
AddressV6 AddressV6::unspecified               = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
AddressV6 AddressV6::loopback                  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
AddressV6 AddressV6::interface_local_all_nodes = {0xff, 0x01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x01};
AddressV6 AddressV6::link_local_all_nodes      = {0xff, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x01};
AddressV6 AddressV6::link_local_all_routers    = {0xff, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x02};

AddressV6::AddressV6()
   : _a()
   , _scope_id(0)
{
}

AddressV6::AddressV6(const std::array<uint8_t, 16>& b, uint32_t scope_id /* = 0 */)
   : _a()
   , _scope_id(scope_id)
{
   std::memcpy(_a.s6_addr, b.data(), 16);
}

AddressV6::AddressV6(std::initializer_list<uint8_t> l)
   : _a()
   , _scope_id(0)
{
   std::memcpy(_a.s6_addr, l.begin(), 16);
}

AddressV6::AddressV6(const AddressV6& other)
   : _a(other._a)
   , _scope_id(other._scope_id)
{
}

AddressV6::AddressV6(AddressV6&& other)
   : _a(std::move(other._a))
   , _scope_id(std::move(other._scope_id))
{
}

AddressV6::~AddressV6()
{
}

AddressV6& AddressV6::operator=(const AddressV6& rhs)
{
   if (this == &rhs)
      return *this;

   std::memcpy(_a.s6_addr, rhs._a.s6_addr, 16);

   _scope_id = rhs._scope_id;

   return *this;
}

AddressV6& AddressV6::operator=(AddressV6&& rhs)
{
   _a = std::move(rhs._a);
   _scope_id = std::move(rhs._scope_id);
   return *this;
}

AddressV6& AddressV6::operator=(std::initializer_list<uint8_t> l)
{
   std::memcpy(_a.s6_addr, l.begin(), 16);
   return *this;
}

uint32_t AddressV6::scope_id() const
{
   return _scope_id;
}

void AddressV6::scope_id(uint32_t value)
{
   _scope_id = value;
}

bool AddressV6::is_loopback() const
{
   return std::equal(std::begin(_a.s6_addr), std::end(_a.s6_addr), std::begin(loopback._a.s6_addr));
}

bool AddressV6::is_multicast() const
{
   return _a.s6_addr[0] == 0xff;
}

bool AddressV6::is_unspecified() const
{
   return std::equal(std::begin(_a.s6_addr), std::end(_a.s6_addr), std::begin(unspecified._a.s6_addr));
}

std::array<uint8_t, 16> AddressV6::to_bytes() const
{
   std::array<uint8_t, 16> bytes;

   std::memcpy(bytes.data(), _a.s6_addr, 16);

   return bytes;
}

//--------------------------------------------------------------------------------------------------

AddressV6 make_address_v6(const std::array<uint8_t, 16>& b, uint32_t scope_id /* = 0 */)
{
   return AddressV6(b, scope_id);
}

//--------------------------------------------------------------------------------------------------

bool operator==(const AddressV6& a1, const AddressV6& a2)
{
   return a1._scope_id == a2._scope_id and
          std::equal(std::begin(a1._a.s6_addr), std::end(a1._a.s6_addr), std::begin(a2._a.s6_addr));
}

bool operator!=(const AddressV6& a1, const AddressV6& a2)
{
   return not (a1 == a2);
}

bool operator<(const AddressV6& a1, const AddressV6& a2)
{
   return std::lexicographical_compare(std::begin(a1._a.s6_addr), std::end(a1._a.s6_addr), 
                                       std::begin(a2._a.s6_addr), std::end(a2._a.s6_addr));
}

bool operator>(const AddressV6& a1, const AddressV6& a2)
{
   return not std::lexicographical_compare(std::begin(a1._a.s6_addr), std::end(a1._a.s6_addr), 
                                           std::begin(a2._a.s6_addr), std::end(a2._a.s6_addr));
}

bool operator<=(const AddressV6& a1, const AddressV6& a2)
{
   return a1 == a2 or a1 < a2;
}

bool operator>=(const AddressV6& a1, const AddressV6& a2)
{
   return a1 == a2 or a1 > a2;
}

} // net
} // orion
