//  test-net.cpp
//
//  Created by Tomas Palazuelos on 2016-06-29.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <orion/net/IP.h>
#include <orion/net/IPv4.h>
#include <orion/net/IPv6.h>
#include <orion/net/IPAddress.h>
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
void ipv4_is_unspecified(Test& t)
{
   t.assert<true>(IPv4::zero.is_unspecified(), _src_loc);
   t.assert<false>(IPv4(127, 0, 0, 1).is_unspecified(), _src_loc);
}

void ipv6_is_unspecified(Test& t)
{
   t.assert<true>(IPv6::unspecified.is_unspecified(), _src_loc);
   t.assert<false>(IPv6::interface_local_all_nodes.is_unspecified(), _src_loc);
}

void ipv4_is_loopback(Test& t)
{
   t.assert<true>(IPv4(127, 0, 0, 1).is_loopback(), _src_loc);
   t.assert<true>(IPv4(127, 255, 255, 254).is_loopback(), _src_loc);
   t.assert<false>(IPv4(128, 1, 2, 3).is_loopback(), _src_loc);
}

void ipv6_is_loopback(Test& t)
{
   t.assert<true>(IPv6::loopback.is_loopback(), _src_loc);
   t.assert<false>(IPv6::link_local_all_routers.is_loopback(), _src_loc);
}

void ipv4_is_multicast(Test& t)
{
   t.assert<true>(IPv4(224, 0, 0, 0).is_multicast(), _src_loc);
   t.assert<true>(IPv4(239, 0, 0, 0).is_multicast(), _src_loc);
   t.assert<false>(IPv4(240, 0, 0, 0).is_multicast(), _src_loc);
}

void ipv6_is_multicast(Test& t)
{
   t.assert<true>(IPv6::link_local_all_nodes.is_multicast(), _src_loc);
   t.assert<true>(IPv6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}).is_multicast(), _src_loc);
   t.assert<false>(IPv6(std::array<uint8_t, 16>{{0xfe, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}).is_multicast(), _src_loc);
}

void ipv4_tostring(Test& t)
{
   t.assert<std::equal_to<>>(IPv4(127, 0, 0, 1).to_string(), "127.0.0.1"s, _src_loc);
}

void ipv6_tostring(Test& t)
{
   t.assert<std::equal_to<>>(IPv6::loopback.to_string(), "0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:1"s, _src_loc);
}

void ipv4_copy(Test& t)
{
   IPv4 ip(IPv4::broadcast);

   t.assert<std::equal_to<>>(ip, IPv4::broadcast, _src_loc);

   ip = IPv4::all_systems;

   t.assert<std::equal_to<>>(ip, IPv4::all_systems, _src_loc);
}

void ipv6_copy(Test& t)
{
   IPv6 ip(IPv6::link_local_all_routers);

   t.assert<std::equal_to<>>(ip, IPv6::link_local_all_routers, _src_loc);

   ip = IPv6::interface_local_all_nodes;

   t.assert<std::equal_to<>>(ip, IPv6::interface_local_all_nodes, _src_loc);
}

void ipv4_move(Test& t)
{
   IPv4 ip(std::move(IPv4(255, 255, 255, 255)));

   t.assert<std::equal_to<>>(ip, IPv4::broadcast, _src_loc);

   ip = std::move(IPv4(224, 0, 0, 1));

   t.assert<std::equal_to<>>(ip, IPv4::all_systems, _src_loc);
}

void ipv6_move(Test& t)
{
   IPv6 ip(std::move(IPv6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}})));

   t.assert<std::equal_to<>>(ip, IPv6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}), _src_loc);

   ip = std::move(IPv6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}));

   t.assert<std::equal_to<>>(ip, IPv6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}), _src_loc);
}

void tcpaddress_ipv6(Test& t)
{
   IPAddress addr(IPv6::parse("2001:db8::123:12:1"), 22);

   auto ip = IPv6{0x20, 0x1, 0xd, 0xb8, 0, 0, 0, 0, 0, 0, 0x1, 0x23, 0, 0x12, 0, 0x1};

   t.fail_if(addr.ip() == nullptr, _src_loc);

   if (addr.ip() != nullptr)
   {
      auto ip6 = dynamic_cast<IPv6*>(addr.ip());
      t.assert<std::equal_to<>>(ip, *ip6, _src_loc);
   }
}

RegisterTestCase(OrionNet, ipv4_is_unspecified);
RegisterTestCase(OrionNet, ipv6_is_unspecified);
RegisterTestCase(OrionNet, ipv4_is_loopback);
RegisterTestCase(OrionNet, ipv6_is_loopback);
RegisterTestCase(OrionNet, ipv4_is_multicast);
RegisterTestCase(OrionNet, ipv6_is_multicast);
RegisterTestCase(OrionNet, ipv4_tostring);
RegisterTestCase(OrionNet, ipv6_tostring);
RegisterTestCase(OrionNet, ipv4_copy);
RegisterTestCase(OrionNet, ipv6_copy);
RegisterTestCase(OrionNet, ipv4_move);
RegisterTestCase(OrionNet, ipv6_move);
RegisterTestCase(OrionNet, tcpaddress_ipv6);
} // TestSuite(OrionNet)
