//  test-net.cpp
//
//  Created by Tomas Palazuelos on 2016-06-29.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <orion/net/Address.h>
#include <orion/net/AddressV4.h>
#include <orion/net/AddressV6.h>
#include <orion/net/EndPoint.h>
#include <orion/Test.h>

using namespace orion;
using namespace orion::net;
using namespace orion::unittest;

using namespace std::string_literals;

TestSuite(OrionNet)
{
//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
void AddressV4_is_unspecified(Test& t)
{
   t.assert<true>(AddressV4::zero.is_unspecified(), _src_loc);
   t.assert<false>(AddressV4(127, 0, 0, 1).is_unspecified(), _src_loc);
}

void AddressV6_is_unspecified(Test& t)
{
   t.assert<true>(AddressV6::unspecified.is_unspecified(), _src_loc);
   t.assert<false>(AddressV6::interface_local_all_nodes.is_unspecified(), _src_loc);
}

void AddressV4_is_loopback(Test& t)
{
   t.assert<true>(AddressV4(127, 0, 0, 1).is_loopback(), _src_loc);
   t.assert<true>(AddressV4(127, 255, 255, 254).is_loopback(), _src_loc);
   t.assert<false>(AddressV4(128, 1, 2, 3).is_loopback(), _src_loc);
}

void AddressV6_is_loopback(Test& t)
{
   t.assert<true>(AddressV6::loopback.is_loopback(), _src_loc);
   t.assert<false>(AddressV6::link_local_all_routers.is_loopback(), _src_loc);
}

void AddressV4_is_multicast(Test& t)
{
   t.assert<true>(AddressV4(224, 0, 0, 0).is_multicast(), _src_loc);
   t.assert<true>(AddressV4(239, 0, 0, 0).is_multicast(), _src_loc);
   t.assert<false>(AddressV4(240, 0, 0, 0).is_multicast(), _src_loc);
}

void AddressV6_is_multicast(Test& t)
{
   t.assert<true>(AddressV6::link_local_all_nodes.is_multicast(), _src_loc);
   t.assert<true>(AddressV6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}).is_multicast(), _src_loc);
   t.assert<false>(AddressV6(std::array<uint8_t, 16>{{0xfe, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}).is_multicast(), _src_loc);
}

void AddressV4_tostring(Test& t)
{
   t.assert<std::equal_to<>>(AddressV4(127, 0, 0, 1).to_string(), "127.0.0.1"s, _src_loc);
}

void AddressV6_tostring(Test& t)
{
   t.assert<std::equal_to<>>(AddressV6::loopback.to_string(), "::1"s, _src_loc);
}

void AddressV4_copy(Test& t)
{
   AddressV4 addr(AddressV4::broadcast);

   t.assert<std::equal_to<>>(addr, AddressV4::broadcast, _src_loc);

   addr = AddressV4::all_systems;

   t.assert<std::equal_to<>>(addr, AddressV4::all_systems, _src_loc);
}

void AddressV6_copy(Test& t)
{
   AddressV6 addr(AddressV6::link_local_all_routers);

   t.assert<std::equal_to<>>(addr, AddressV6::link_local_all_routers, _src_loc);

   addr = AddressV6::interface_local_all_nodes;

   t.assert<std::equal_to<>>(addr, AddressV6::interface_local_all_nodes, _src_loc);
}

void AddressV4_move(Test& t)
{
   AddressV4 addr(AddressV4(255, 255, 255, 255));

   t.assert<std::equal_to<>>(addr, AddressV4::broadcast, _src_loc);

   addr = AddressV4(224, 0, 0, 1);

   t.assert<std::equal_to<>>(addr, AddressV4::all_systems, _src_loc);
}

void AddressV6_move(Test& t)
{
   AddressV6 addr(AddressV6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}));

   t.assert<std::equal_to<>>(addr, AddressV6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}), _src_loc);

   addr = AddressV6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});

   t.assert<std::equal_to<>>(addr, AddressV6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}), _src_loc);
}

void tcpaddress_AddressV6(Test& t)
{
   EndPoint end_point(AddressV6::parse("2001:db8::123:12:1"), 22);

   auto addr = AddressV6{0x20, 0x1, 0xd, 0xb8, 0, 0, 0, 0, 0, 0, 0x1, 0x23, 0, 0x12, 0, 0x1};

   t.fail_if(end_point.address() == nullptr, _src_loc);

   if (end_point.address() != nullptr)
   {
      auto addr6 = dynamic_cast<AddressV6*>(end_point.address());
      t.assert<std::equal_to<>>(addr, *addr6, _src_loc);
   }
}

RegisterTestCase(OrionNet, AddressV4_is_unspecified);
RegisterTestCase(OrionNet, AddressV6_is_unspecified);
RegisterTestCase(OrionNet, AddressV4_is_loopback);
RegisterTestCase(OrionNet, AddressV6_is_loopback);
RegisterTestCase(OrionNet, AddressV4_is_multicast);
RegisterTestCase(OrionNet, AddressV6_is_multicast);
RegisterTestCase(OrionNet, AddressV4_tostring);
RegisterTestCase(OrionNet, AddressV6_tostring);
RegisterTestCase(OrionNet, AddressV4_copy);
RegisterTestCase(OrionNet, AddressV6_copy);
RegisterTestCase(OrionNet, AddressV4_move);
RegisterTestCase(OrionNet, AddressV6_move);
RegisterTestCase(OrionNet, tcpaddress_AddressV6);
} // TestSuite(OrionNet)
