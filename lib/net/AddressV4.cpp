//
// AddressV4.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/AddressV4.h>

#include <orion/Log.h>
#include <orion/String.h>
#include <orion/Utils.h>

#include <algorithm>
#include <cstring>

namespace orion
{
namespace net
{
//--------------------------------------------------------------------------------------------------
// Static global AddressV4 adresses

AddressV4& AddressV4::broadcast()
{  
   try
   { 
      static AddressV4 addr{255, 255, 255, 255};
      return addr;
   }
   catch (...)
   {
      log::fatal(type_name(std::current_exception()), 
         "An unexpected, unknown exception was thrown: ", _src_loc);
   }
}

AddressV4& AddressV4::all_systems()
{  
   try
   { 
      static AddressV4 addr{224, 0, 0, 1};
      return addr;
   }
   catch (...)
   {
      log::fatal(type_name(std::current_exception()), 
         "An unexpected, unknown exception was thrown: ", _src_loc);
   }
}

AddressV4& AddressV4::all_routers()
{  
   try
   { 
      static AddressV4 addr{224, 0, 0, 2};
      return addr;
   }
   catch (...)
   {
      log::fatal(type_name(std::current_exception()), 
         "An unexpected, unknown exception was thrown: ", _src_loc);
   }
}

AddressV4& AddressV4::zero()
{  
   try
   { 
      static AddressV4 addr{0, 0, 0, 0};
      return addr;
   }
   catch (...)
   {
      log::fatal(type_name(std::current_exception()), 
         "An unexpected, unknown exception was thrown: ", _src_loc);
   }
}

//--------------------------------------------------------------------------------------------------
// AddressV4

AddressV4::AddressV4()
   : _a()
{
   _a.s_addr = 0;
}

AddressV4::AddressV4(const std::array<uint8_t, 4>& b)
   : _a()
{
   std::memcpy(&_a.s_addr, b.data(), 4);
}

AddressV4::AddressV4(std::initializer_list<uint8_t> l)
   : _a()
{
   std::memcpy(&_a.s_addr, l.begin(), 4);
}

AddressV4::AddressV4(const AddressV4& other)
   : _a(other._a)
{
}

AddressV4::AddressV4(AddressV4&& other) noexcept
   : _a(other._a)
{
}

AddressV4::~AddressV4() = default;

AddressV4& AddressV4::operator=(const AddressV4& rhs)
{
   if (this == &rhs)
   {
      return *this;
   }

   _a = rhs._a;

   return *this;
}

AddressV4& AddressV4::operator=(AddressV4&& rhs) noexcept
{
   _a = rhs._a;
   return *this;
}

AddressV4& AddressV4::operator=(std::initializer_list<uint8_t> l)
{
   std::memcpy(&_a.s_addr, l.begin(), 4);
   return *this;
}

/// Determine whether the address is a class A address.
bool AddressV4::is_class_a() const
{
   return (to_ulong() & 0x80000000) == 0;
}

/// Determine whether the address is a class B address.
bool AddressV4::is_class_b() const
{
   return (to_ulong() & 0xC0000000) == 0x80000000;
}

/// Determine whether the address is a class C address.
bool AddressV4::is_class_c() const
{
   return (to_ulong() & 0xE0000000) == 0xC0000000;
}

bool AddressV4::is_loopback() const
{
   return (to_ulong() & 0xFF000000) == 0x7F000000;
}

bool AddressV4::is_multicast() const
{
   return (to_ulong() & 0xF0000000) == 0xE0000000;
}

bool AddressV4::is_unspecified() const
{
   return to_ulong() == 0;
}

std::array<uint8_t, 4> AddressV4::to_bytes() const
{
   std::array<uint8_t, 4> bytes{};

   std::memcpy(bytes.data(), &_a.s_addr, 4);

   return bytes;
}

//--------------------------------------------------------------------------------------------------

AddressV4 make_address_v4(uint32_t value)
{
   return AddressV4(value);
}

AddressV4 make_address_v4(const std::array<uint8_t, 4>& value)
{
   return AddressV4(value);
}

//--------------------------------------------------------------------------------------------------

bool operator==(const AddressV4& a1, const AddressV4& a2)
{
   return a1._a.s_addr == a2._a.s_addr;
}

bool operator!=(const AddressV4& a1, const AddressV4& a2)
{
   return a1._a.s_addr != a2._a.s_addr;
}

bool operator<(const AddressV4& a1, const AddressV4& a2)
{
   return a1.to_ulong() < a2.to_ulong();
}

bool operator>(const AddressV4& a1, const AddressV4& a2)
{
   return a1.to_ulong() > a2.to_ulong();
}

bool operator<=(const AddressV4& a1, const AddressV4& a2)
{
   return a1.to_ulong() <= a2.to_ulong();
}

bool operator>=(const AddressV4& a1, const AddressV4& a2)
{
   return a1.to_ulong() >= a2.to_ulong();
}

} // namespace net
} // namespace orion
