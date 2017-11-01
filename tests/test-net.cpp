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
   t.assert<false>(AddressV4({127, 0, 0, 1}).is_unspecified(), _src_loc);
}

void AddressV6_is_unspecified(Test& t)
{
   t.assert<true>(AddressV6::unspecified.is_unspecified(), _src_loc);
   t.assert<false>(AddressV6::interface_local_all_nodes.is_unspecified(), _src_loc);
}

void AddressV4_is_loopback(Test& t)
{
   t.assert<true>(AddressV4({127, 0, 0, 1}).is_loopback(), _src_loc);
   t.assert<true>(AddressV4({127, 255, 255, 254}).is_loopback(), _src_loc);
   t.assert<false>(AddressV4({128, 1, 2, 3}).is_loopback(), _src_loc);
}

void AddressV6_is_loopback(Test& t)
{
   t.assert<true>(AddressV6::loopback.is_loopback(), _src_loc);
   t.assert<false>(AddressV6::link_local_all_routers.is_loopback(), _src_loc);
}

void AddressV4_is_multicast(Test& t)
{
   t.assert<true>(AddressV4({224, 0, 0, 0}).is_multicast(), _src_loc);
   t.assert<true>(AddressV4({239, 0, 0, 0}).is_multicast(), _src_loc);
   t.assert<false>(AddressV4({240, 0, 0, 0}).is_multicast(), _src_loc);
}

void AddressV6_is_multicast(Test& t)
{
   t.assert<true>(AddressV6::link_local_all_nodes.is_multicast(), _src_loc);
   t.assert<true>(AddressV6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}).is_multicast(), _src_loc);
   t.assert<false>(AddressV6(std::array<uint8_t, 16>{{0xfe, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}).is_multicast(), _src_loc);
}

void AddressV4_to_string(Test& t)
{
   t.assert<std::equal_to<>>(to_string(AddressV4({127, 0, 0, 1})), "127.0.0.1"s, _src_loc);

   Address* inv_addr = nullptr;

   t.assert_throw<std::system_error>([&]() { to_string(inv_addr); }, _src_loc);

   AddressV4* inv_addr_v4 = nullptr;

   t.assert_throw<std::system_error>([&]() { to_string(inv_addr_v4); }, _src_loc);

   AddressV6* inv_addr_v6 = nullptr;

   t.assert_throw<std::system_error>([&]() { to_string(inv_addr_v6); }, _src_loc);
}

void AddressV6_to_string(Test& t)
{
   t.assert<std::equal_to<>>(to_string(AddressV6::loopback), "::1"s, _src_loc);
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
   AddressV4 addr(AddressV4({255, 255, 255, 255}));

   t.assert<std::equal_to<>>(addr, AddressV4::broadcast, _src_loc);

   addr = AddressV4({224, 0, 0, 1});

   t.assert<std::equal_to<>>(addr, AddressV4::all_systems, _src_loc);
}

void AddressV6_move(Test& t)
{
   AddressV6 addr(AddressV6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}));

   t.assert<std::equal_to<>>(addr, AddressV6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}), _src_loc);

   addr = AddressV6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});

   t.assert<std::equal_to<>>(addr, AddressV6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}), _src_loc);
}

void AddressV4_ipv4_literal(Test& t)
{
   t.assert<std::equal_to<>>(AddressV4({224, 0, 0, 1}), "224.0.0.1"_ipv4, _src_loc);
}

void AddressV6_ipv6_literal(Test& t)
{
   auto addr = AddressV6{0x20, 0x1, 0xd, 0xb8, 0, 0, 0, 0, 0, 0, 0x1, 0x23, 0, 0x12, 0, 0x1};

   t.assert<std::equal_to<>>(addr, "2001:db8::123:12:1"_ipv6, _src_loc);
}

void EndPoint_from_AddressV4(Test& t)
{
   EndPoint end_point(make_address_v4("10.1.2.173"), 22);

   auto addr = AddressV4{10, 1, 2, 173};

   t.fail_if(end_point.address() == nullptr, _src_loc);

   if (end_point.address() != nullptr)
   {
      auto addr4 = dynamic_cast<AddressV4*>(end_point.address());
      t.assert<std::equal_to<>>(addr, *addr4, _src_loc);
   }
}

void EndPoint_from_AddressV6(Test& t)
{
   EndPoint end_point(make_address_v6("2001:db8::123:12:1"), 22);

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
RegisterTestCase(OrionNet, AddressV4_to_string);
RegisterTestCase(OrionNet, AddressV6_to_string);
RegisterTestCase(OrionNet, AddressV4_copy);
RegisterTestCase(OrionNet, AddressV6_copy);
RegisterTestCase(OrionNet, AddressV4_move);
RegisterTestCase(OrionNet, AddressV6_move);
RegisterTestCase(OrionNet, AddressV4_ipv4_literal);
RegisterTestCase(OrionNet, AddressV6_ipv6_literal);
RegisterTestCase(OrionNet, EndPoint_from_AddressV4);
RegisterTestCase(OrionNet, EndPoint_from_AddressV6);
} // TestSuite(OrionNet)
