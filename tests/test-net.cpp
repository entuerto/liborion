//  test-net.cpp
//
//  Created by Tomas Palazuelos on 2016-06-29.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <orion/net/IP.h>
#include <orion/net/IPv4.h>
#include <orion/net/IPv6.h>
#include <orion/net/IPAddress.h>
#include <orion/TestUtils.h>

using namespace orion;
using namespace orion::net;
using namespace orion::unittest;

TEST_SUITE(OrionNet)
{
//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
TEST(IPv4, IsUnspecified)
{
   EXPECT_TRUE(IPv4::zero.is_unspecified());
   EXPECT_FALSE(IPv4(127, 0, 0, 1).is_unspecified());
}

TEST(IPv6, IsUnspecified)
{
   EXPECT_TRUE(IPv6::unspecified.is_unspecified());
   EXPECT_FALSE(IPv6::interface_local_all_nodes.is_unspecified());
}

TEST(IPv4, IsLoopback)
{
   EXPECT_TRUE(IPv4(127, 0, 0, 1).is_loopback());
   EXPECT_TRUE(IPv4(127, 255, 255, 254).is_loopback());
   EXPECT_FALSE(IPv4(128, 1, 2, 3).is_loopback());
}

TEST(IPv6, IsLoopback)
{
   EXPECT_TRUE(IPv6::loopback.is_loopback());
   EXPECT_FALSE(IPv6::link_local_all_routers.is_loopback());
}

TEST(IPv4, IsMulticast)
{
   EXPECT_TRUE(IPv4(224, 0, 0, 0).is_multicast());
   EXPECT_TRUE(IPv4(239, 0, 0, 0).is_multicast());
   EXPECT_FALSE(IPv4(240, 0, 0, 0).is_multicast());
}

TEST(IPv6, IsMulticast)
{
   EXPECT_TRUE(IPv6::link_local_all_nodes.is_multicast());
   EXPECT_TRUE(IPv6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}).is_multicast());
   EXPECT_FALSE(IPv6(std::array<uint8_t, 16>{{0xfe, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}).is_multicast());
}

TEST(IPv4, ToString)
{
   EXPECT_EQ(IPv4(127, 0, 0, 1).to_string(), "127.0.0.1");
}

TEST(IPv6, ToString)
{
   EXPECT_EQ(IPv6::loopback.to_string(), "0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:1");
}

TEST(IPv4, Copy)
{
   IPv4 ip(IPv4::broadcast);

   EXPECT_EQ(ip, IPv4::broadcast);

   ip = IPv4::all_systems;

   EXPECT_EQ(ip, IPv4::all_systems);
}

TEST(IPv6, Copy)
{
   IPv6 ip(IPv6::link_local_all_routers);

   EXPECT_EQ(ip, IPv6::link_local_all_routers);

   ip = IPv6::interface_local_all_nodes;

   EXPECT_EQ(ip, IPv6::interface_local_all_nodes);
}

TEST(IPv4, Move)
{
   IPv4 ip(std::move(IPv4(255, 255, 255, 255)));

   EXPECT_EQ(ip, IPv4::broadcast);

   ip = std::move(IPv4(224, 0, 0, 1));

   EXPECT_EQ(ip, IPv4::all_systems);
}

TEST(IPv6, Move)
{
   IPv6 ip(std::move(IPv6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}})));

   EXPECT_EQ(ip, IPv6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}));

   ip = std::move(IPv6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}));

   EXPECT_EQ(ip, IPv6(std::array<uint8_t, 16>{{0xff, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}));
}

TEST(TcpAddress, IPv6)
{
   IPAddress addr(IPv6::parse("2001:db8::123:12:1"), 22);

   auto ip = IPv6{0x20, 0x1, 0xd, 0xb8, 0, 0, 0, 0, 0, 0, 0x1, 0x23, 0, 0x12, 0, 0x1};

   FAIL_IF(addr.ip() == nullptr)

   if (addr.ip() != nullptr)
   {
      auto ip6 = dynamic_cast<IPv6*>(addr.ip());
      EXPECT_EQ(ip, *ip6);
   }
}

} // TEST_SUITE(OrionNet)
