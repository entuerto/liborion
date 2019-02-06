//
//  test-net.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/Address.h>
#include <orion/net/AddressV4.h>
#include <orion/net/AddressV6.h>
#include <orion/net/EndPoint.h>
#include <orion/Log.h>
#include <orion/Test.h>

using namespace orion;
using namespace orion::net;
using namespace orion::unittest;

using namespace std::string_literals;

Section(OrionNet_Address, Label{"Address"})
{

TestCase("AddressV4 is unspecified")
{
   check_true(AddressV4::zero().is_unspecified());

   AddressV4 addr{127, 0, 0, 1};

   check_false(addr.is_unspecified());
}

TestCase("AddressV6 is unspecified")
{
   check_true(AddressV6::unspecified().is_unspecified());
   check_false(AddressV6::interface_local_all_nodes().is_unspecified());
}

TestCase("AddressV4 is loopback")
{
   AddressV4 addr1{127, 0, 0, 1};
   check_true(addr1.is_loopback());

   AddressV4 addr2{127, 255, 255, 254};
   check_true(addr2.is_loopback());

   AddressV4 addr3{128, 1, 2, 3};
   check_false(addr3.is_loopback());
}

TestCase("AddressV6 is loopback")
{
   check_true(AddressV6::loopback().is_loopback());
   check_false(AddressV6::link_local_all_routers().is_loopback());
}

TestCase("AddressV4 is multicast")
{
   AddressV4 addr1{224, 0, 0, 0};
   check_true(addr1.is_multicast());

   AddressV4 addr2{239, 0, 0, 0};
   check_true(addr2.is_multicast());

   AddressV4 addr3{240, 0, 0, 0};
   check_false(addr3.is_multicast());
}

TestCase("AddressV6 is multicast")
{
   check_true(AddressV6::link_local_all_nodes().is_multicast());

   AddressV6 addr{std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}};
   check_true(addr.is_multicast());

   AddressV6 addr2{std::array<uint8_t, 16>{{0xfe, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}};
   check_false(addr2.is_multicast());
}

TestCase("AddressV4 to_string")
{
   check_eq(to_string(AddressV4{127, 0, 0, 1}), "127.0.0.1"s);

   check_throws_as([]() { 
      Address* inv_addr = nullptr; 

      to_string(inv_addr); 
   }(), std::system_error);

   
   check_throws_as([]() { 
      AddressV4* inv_addr_v4 = nullptr; 

      to_string(inv_addr_v4); 
   }(), std::system_error);
}

TestCase("AddressV6 to_string")
{
   check_eq(to_string(AddressV6::loopback()), "::1"s);

   check_throws_as([]() { 
      AddressV6* inv_addr_v6 = nullptr; 

      to_string(inv_addr_v6); 
   }(), std::system_error);
}

TestCase("AddressV4 copy")
{
   AddressV4 addr(AddressV4::broadcast());

   check_eq(addr, AddressV4::broadcast());

   addr = AddressV4::all_systems();

   check_eq(addr, AddressV4::all_systems());
}

TestCase("AddressV6 copy")
{
   AddressV6 addr(AddressV6::link_local_all_routers());

   check_eq(addr, AddressV6::link_local_all_routers());

   addr = AddressV6::interface_local_all_nodes();

   check_eq(addr, AddressV6::interface_local_all_nodes());
}

TestCase("AddressV4 move")
{
   AddressV4 addr(AddressV4{255, 255, 255, 255});

   check_eq(addr, AddressV4::broadcast());

   addr = AddressV4{224, 0, 0, 1};

   check_eq(addr, AddressV4::all_systems());
}

TestCase("AddressV6 move")
{
   AddressV6 addr(AddressV6{std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}});

   AddressV6 addr1{std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}};
   check_eq(addr, addr1);

   addr = AddressV6{std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}};

   check_eq(addr, addr1);
}

TestCase("AddressV4 ipv4 literal")
{
   AddressV4 addr{224, 0, 0, 1};

   check_eq(addr, "224.0.0.1"_ipv4);
}

TestCase("AddressV6 ipv6 literal")
{
   auto addr = AddressV6{0x20, 0x1, 0xd, 0xb8, 0, 0, 0, 0, 0, 0, 0x1, 0x23, 0, 0x12, 0, 0x1};

   check_eq(addr, "2001:db8::123:12:1"_ipv6);
}

TestCase("EndPoint from AddressV4")
{
   EndPoint end_point(make_address_v4("10.1.2.173"), 22);

   auto addr = AddressV4{10, 1, 2, 173};

   t.fail_if(end_point.address() == nullptr);

   if (end_point.address() != nullptr)
   {
      auto addr4 = dynamic_cast<AddressV4*>(end_point.address());
      check_eq(addr, *addr4);
   }
}

TestCase("EndPoint from AddressV6")
{
   EndPoint end_point(make_address_v6("2001:db8::123:12:1"), 22);

   auto addr = AddressV6{0x20, 0x1, 0xd, 0xb8, 0, 0, 0, 0, 0, 0, 0x1, 0x23, 0, 0x12, 0, 0x1};

   t.fail_if(end_point.address() == nullptr);

   if (end_point.address() != nullptr)
   {
      auto addr6 = dynamic_cast<AddressV6*>(end_point.address());
      check_eq(addr, *addr6);
   }
}

} // TestSuite(OrionNet)
