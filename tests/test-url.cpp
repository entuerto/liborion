//  test-encoding.cpp
//
//  Created by Tomas Palazuelos on 2016-06-29.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <orion/net/Url.h>
#include <orion/TestUtils.h>

using namespace orion;
using namespace orion::net;
using namespace orion::unittest;

TEST_SUITE(OrionNet)
{
//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
TEST(Url, Basic)
{
   Url u("http://foo.com/path");

   EXPECT_EQ(u.protocol(), "http");
   EXPECT_EQ(u.hostname(), "foo.com");
   EXPECT_EQ(u.pathname(), "/path");
   EXPECT_EQ(u.port(), 80);
}

TEST(Url, SimpleWithPort)
{
   Url u("http://foo.com:22/path");

   EXPECT_EQ(u.protocol(), "http");
   EXPECT_EQ(u.hostname(), "foo.com");
   EXPECT_EQ(u.pathname(), "/path");
   EXPECT_EQ(u.port(), 22);
}

TEST(Url, SimpleUsername)
{
   Url u("http://toto@foo.com/path");

   EXPECT_EQ(u.protocol(), "http");
   EXPECT_EQ(u.hostname(), "foo.com");
   EXPECT_EQ(u.pathname(), "/path");
   EXPECT_EQ(u.userinfo().username, "toto");
   EXPECT_EQ(u.userinfo().password, "");
   EXPECT_EQ(u.port(), 80);
}

TEST(Url, SimpleUserInfo)
{
   Url u("http://toto:tata@foo.com/path");

   EXPECT_EQ(u.protocol(), "http");
   EXPECT_EQ(u.hostname(), "foo.com");
   EXPECT_EQ(u.pathname(), "/path");
   EXPECT_EQ(u.userinfo().username, "toto");
   EXPECT_EQ(u.userinfo().password, "tata");
   EXPECT_EQ(u.port(), 80);
}

TEST(Url, SimpleWithQuery)
{
   Url u("http://www.google.com/?q=go+language");

   EXPECT_EQ(u.protocol(), "http");
   EXPECT_EQ(u.hostname(), "www.google.com");
   EXPECT_EQ(u.pathname(), "/");
   EXPECT_EQ(u.query("q"), "go language");
   EXPECT_EQ(u.port(), 80);
}

TEST(Url, SimpleWithQueryNoEquals)
{
   Url u("http://www.google.com/?q");

   EXPECT_EQ(u.protocol(), "http");
   EXPECT_EQ(u.hostname(), "www.google.com");
   EXPECT_EQ(u.pathname(), "/");
   EXPECT_EQ(u.query("q"), "");
   EXPECT_EQ(u.port(), 80);
}

TEST(Url, SimpleWithMultipleQuery)
{
   Url u("http://www.google.com/?q=go+language&foo=bar");

   EXPECT_EQ(u.protocol(), "http");
   EXPECT_EQ(u.hostname(), "www.google.com");
   EXPECT_EQ(u.pathname(), "/");
   EXPECT_EQ(u.query("q"), "go language");
   EXPECT_EQ(u.query("foo"), "bar");
   EXPECT_EQ(u.port(), 80);
}

TEST(Url, SimpleWithFragment)
{
   Url u("https://www.google.com/#foo");

   EXPECT_EQ(u.protocol(), "https");
   EXPECT_EQ(u.hostname(), "www.google.com");
   EXPECT_EQ(u.pathname(), "/");
   EXPECT_EQ(u.hash(), "#foo");
   EXPECT_EQ(u.port(), 443);
}

TEST(Url, SimpleWithQueryAndFragment)
{
   Url u("http://www.google.com/?q=go+language#foo");

   EXPECT_EQ(u.protocol(), "http");
   EXPECT_EQ(u.hostname(), "www.google.com");
   EXPECT_EQ(u.pathname(), "/");
   EXPECT_EQ(u.query("q"), "go language");
   EXPECT_EQ(u.hash(), "#foo");
}

TEST(Url, PathWithHexEscaping)
{
   Url u("http://www.google.com/file%20one%26two");

   EXPECT_EQ(u.protocol(), "http");
   EXPECT_EQ(u.hostname(), "www.google.com");
   EXPECT_EQ(u.pathname(), "/file one&two");
}

TEST(Url, EscapeSequenceInUsername)
{
   Url u("ftp://john%20doe@www.google.com/");

   EXPECT_EQ(u.protocol(), "ftp");
   EXPECT_EQ(u.hostname(), "www.google.com");
   EXPECT_EQ(u.userinfo().username, "john doe");
   EXPECT_EQ(u.pathname(), "/");
}

TEST(Url, HostIPv4AddressInRFC3986)
{
   Url u("http://192.168.0.1/");

   EXPECT_EQ(u.protocol(), "http");
   EXPECT_EQ(u.hostname(), "192.168.0.1");
   EXPECT_EQ(u.pathname(), "/");
}

TEST(Url, HostAndPortIPv4AddressInRFC3986)
{
   Url u("http://192.168.0.1:8080/");

   EXPECT_EQ(u.protocol(), "http");
   EXPECT_EQ(u.hostname(), "192.168.0.1");
   EXPECT_EQ(u.pathname(), "/");
   EXPECT_EQ(u.port(), 8080);
}

TEST(Url, HostIPv6AddressInRFC3986)
{
   Url u("http://[fe80::1]/");

   EXPECT_EQ(u.protocol(), "http");
   EXPECT_EQ(u.hostname(), "fe80::1");
   EXPECT_EQ(u.pathname(), "/");
}

TEST(Url, HostAndPortIPv6AddressInRFC3986)
{
   Url u("http://[fe80::1]:8080/");

   EXPECT_EQ(u.protocol(), "http");
   EXPECT_EQ(u.hostname(), "fe80::1");
   EXPECT_EQ(u.pathname(), "/");
   EXPECT_EQ(u.port(), 8080);
}

TEST(Url, HostIPv6AddressWithZoneIdentifierInRFC6874)
{
   Url u("http://[fe80::1%25en0]/");

   EXPECT_EQ(u.protocol(), "http");
   EXPECT_EQ(u.hostname(), "fe80::1%en0");
   EXPECT_EQ(u.pathname(), "/");
}

TEST(Url, HostAndPortIPv6AddressWithZoneIdentifierInRFC6874)
{
   Url u("http://[fe80::1%25en0]:8080/");

   EXPECT_EQ(u.protocol(), "http");
   EXPECT_EQ(u.hostname(), "fe80::1%en0");
   EXPECT_EQ(u.pathname(), "/");
   EXPECT_EQ(u.port(), 8080);
}

TEST(Url, HostIPv6AddressWithZoneIdentifierInRFC6874PercentEncoded)
{
   Url u("http://[fe80::1%25%65%6e%301-._~]/");

   EXPECT_EQ(u.protocol(), "http");
   EXPECT_EQ(u.hostname(), "fe80::1%en01-._~");
   EXPECT_EQ(u.pathname(), "/");
}

TEST(Url, HostAndPortIPv6AddressWithZoneIdentifierInRFC6874ercentEncoded)
{
   Url u("http://[fe80::1%25%65%6e%301-._~]:8080/");

   EXPECT_EQ(u.protocol(), "http");
   EXPECT_EQ(u.hostname(), "fe80::1%en01-._~");
   EXPECT_EQ(u.pathname(), "/");
   EXPECT_EQ(u.port(), 8080);
}

TEST(Url, HrefSimple)
{
   Url u("http://www.google.com/");

   EXPECT_EQ(u.href(), "http://www.google.com/");  
}

TEST(Url, HrefSimpleWithPort)
{
   Url u("http://www.google.com:80/");

   EXPECT_EQ(u.href(), "http://www.google.com:80/");  
}

TEST(Url, HrefSimpleUsername)
{
   Url u("http://user:pass@www.google.com/");

   EXPECT_EQ(u.href(), "http://user:pass@www.google.com/");  
}

TEST(Url, HrefSimpleWithQuery)
{
   Url u("http://www.google.com/?q=go+language");

   EXPECT_EQ(u.href(), "http://www.google.com/?q=go+language");
}

TEST(Url, HrefPathWithHexEscaping)
{
   Url u("http://www.google.com/file%20one&two");

   EXPECT_EQ(u.href(), "http://www.google.com/file%20one&two");
}

TEST(Url, HrefEscapeSequenceInUsername)
{
   Url u("ftp://john%20doe@www.google.com/");

   EXPECT_EQ(u.href(), "ftp://john%20doe@www.google.com/");
   EXPECT_EQ(u.port(), 21);
}

TEST(Url, HrefEscapeSequenceIngragment1)
{
   Url u("http://www.google.com/?q=go+language#foo%26bar");

   EXPECT_EQ(u.hash(), "#foo&bar");
   EXPECT_EQ(u.href(), "http://www.google.com/?q=go+language#foo&bar");
}

}
