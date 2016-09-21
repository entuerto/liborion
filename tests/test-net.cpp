//  test-net.cpp
//
//  Created by Tomas Palazuelos on 2016-06-29.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <orion/net/IP.h>
#include <orion/TestUtils.h>

using namespace orion;
using namespace orion::net;
using namespace orion::unittest;

//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
TEST(TestIPv4IsUnspecified)
{
   EXPECT(IPv4::zero.is_unspecified());
   EXPECT(IPv4(127, 0, 0, 1).is_unspecified() == false);
}

TEST(TestIPv6IsUnspecified)
{
   EXPECT(IPv6::unspecified.is_unspecified());
   EXPECT(IPv6::interface_local_all_nodes.is_unspecified() == false);
}

TEST(TestIPv4IsLoopback)
{
   EXPECT(IPv4(127, 0, 0, 1).is_loopback());
   EXPECT(IPv4(127, 255, 255, 254).is_loopback());
   EXPECT(IPv4(128, 1, 2, 3).is_loopback() == false);
}

TEST(TestIPv6IsLoopback)
{
   EXPECT(IPv6::loopback.is_loopback());
   EXPECT(IPv6::link_local_all_routers.is_loopback() == false);
}

TEST(TestIPv4IsMulticast)
{
   EXPECT(IPv4(224, 0, 0, 0).is_multicast());
   EXPECT(IPv4(239, 0, 0, 0).is_multicast());
   EXPECT(IPv4(240, 0, 0, 0).is_multicast() == false);
}

TEST(TestIPv6IsMulticast)
{
   EXPECT(IPv6::link_local_all_nodes.is_multicast());
   EXPECT(IPv6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}).is_multicast());
   EXPECT(IPv6(std::array<uint8_t, 16>{{0xfe, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}).is_multicast() == false);
}

TEST(TestIPv4ToString)
{
   EXPECT(IPv4(127, 0, 0, 1).to_string() == "127.0.0.1");
}

TEST(TestIPv6ToString)
{
   EXPECT(IPv6::loopback.to_string() == "0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:1");
}

TEST(TestIPv4Copy)
{
   IPv4 ip(IPv4::broadcast);

   EXPECT(ip == IPv4::broadcast);

   ip = IPv4::all_systems;

   EXPECT(ip == IPv4::all_systems);
}

TEST(TestIPv6Copy)
{
   IPv6 ip(IPv6::link_local_all_routers);

   EXPECT(ip == IPv6::link_local_all_routers);

   ip = IPv6::interface_local_all_nodes;

   EXPECT(ip == IPv6::interface_local_all_nodes);
}

TEST(TestIPv4Move)
{
   IPv4 ip(std::move(IPv4(255, 255, 255, 255)));

   EXPECT(ip == IPv4::broadcast);

   ip = std::move(IPv4(224, 0, 0, 1));

   EXPECT(ip == IPv4::all_systems);
}

TEST(TestIPv6Move)
{
   IPv6 ip(std::move(IPv6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}})));

   EXPECT(ip == IPv6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}));

   ip = std::move(IPv6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}));

   EXPECT(ip == IPv6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}));
}
