//
//  test-url.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/Url.h>
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
void url_basic(Test& t)
{
   Url u("http://foo.com/path");

   t.assert<eq>(u.protocol(), "http"s, _src_loc);
   t.assert<eq>(u.hostname(), "foo.com"s, _src_loc);
   t.assert<eq>(u.pathname(), "/path"s, _src_loc);
   t.assert<eq>(u.port(), 80, _src_loc);
}

void url_simple_with_port(Test& t)
{
   Url u("http://foo.com:22/path");

   t.assert<eq>(u.protocol(), "http"s, _src_loc);
   t.assert<eq>(u.hostname(), "foo.com"s, _src_loc);
   t.assert<eq>(u.pathname(), "/path"s, _src_loc);
   t.assert<eq>(u.port(), 22, _src_loc);
}

void url_simple_username(Test& t)
{
   Url u("http://toto@foo.com/path");

   t.assert<eq>(u.protocol(), "http"s, _src_loc);
   t.assert<eq>(u.hostname(), "foo.com"s, _src_loc);
   t.assert<eq>(u.pathname(), "/path"s, _src_loc);
   t.assert<eq>(u.userinfo().username, "toto"s, _src_loc);
   t.assert<eq>(u.userinfo().password, ""s, _src_loc);
   t.assert<eq>(u.port(), 80, _src_loc);
}

void url_simple_user_info(Test& t)
{
   Url u("http://toto:tata@foo.com/path");

   t.assert<eq>(u.protocol(), "http"s, _src_loc);
   t.assert<eq>(u.hostname(), "foo.com"s, _src_loc);
   t.assert<eq>(u.pathname(), "/path"s, _src_loc);
   t.assert<eq>(u.userinfo().username, "toto"s, _src_loc);
   t.assert<eq>(u.userinfo().password, "tata"s, _src_loc);
   t.assert<eq>(u.port(), 80, _src_loc);
}

void url_simple_with_query(Test& t)
{
   Url u("http://www.google.com/?q=go+language");

   t.assert<eq>(u.protocol(), "http"s, _src_loc);
   t.assert<eq>(u.hostname(), "www.google.com"s, _src_loc);
   t.assert<eq>(u.pathname(), "/"s, _src_loc);
   t.assert<eq>(u.query("q"), "go language"s, _src_loc);
   t.assert<eq>(u.port(), 80, _src_loc);
}

void url_simple_with_query_no_equals(Test& t)
{
   Url u("http://www.google.com/?q");

   t.assert<eq>(u.protocol(), "http"s, _src_loc);
   t.assert<eq>(u.hostname(), "www.google.com"s, _src_loc);
   t.assert<eq>(u.pathname(), "/"s, _src_loc);
   t.assert<eq>(u.query("q"), ""s, _src_loc);
   t.assert<eq>(u.port(), 80, _src_loc);
}

void url_simple_with_multiple_query(Test& t)
{
   Url u("http://www.google.com/?q=go+language&foo=bar");

   t.assert<eq>(u.protocol(), "http"s, _src_loc);
   t.assert<eq>(u.hostname(), "www.google.com"s, _src_loc);
   t.assert<eq>(u.pathname(), "/"s, _src_loc);
   t.assert<eq>(u.query("q"), "go language"s, _src_loc);
   t.assert<eq>(u.query("foo"), "bar"s, _src_loc);
   t.assert<eq>(u.port(), 80, _src_loc);
}

void url_simple_with_fragment(Test& t)
{
   Url u("https://www.google.com/#foo");

   t.assert<eq>(u.protocol(), "https"s, _src_loc);
   t.assert<eq>(u.hostname(), "www.google.com"s, _src_loc);
   t.assert<eq>(u.pathname(), "/"s, _src_loc);
   t.assert<eq>(u.hash(), "#foo"s, _src_loc);
   t.assert<eq>(u.port(), 443, _src_loc);
}

void url_simple_with_query_and_fragment(Test& t)
{
   Url u("http://www.google.com/?q=go+language#foo");

   t.assert<eq>(u.protocol(), "http"s, _src_loc);
   t.assert<eq>(u.hostname(), "www.google.com"s, _src_loc);
   t.assert<eq>(u.pathname(), "/"s, _src_loc);
   t.assert<eq>(u.query("q"), "go language"s, _src_loc);
   t.assert<eq>(u.hash(), "#foo"s, _src_loc);
}

void url_path_with_hex_escaping(Test& t)
{
   Url u("http://www.google.com/file%20one%26two");

   t.assert<eq>(u.protocol(), "http"s, _src_loc);
   t.assert<eq>(u.hostname(), "www.google.com"s, _src_loc);
   t.assert<eq>(u.pathname(), "/file one&two"s, _src_loc);
}

void url_escape_sequence_in_username(Test& t)
{
   Url u("ftp://john%20doe@www.google.com/");

   t.assert<eq>(u.protocol(), "ftp"s, _src_loc);
   t.assert<eq>(u.hostname(), "www.google.com"s, _src_loc);
   t.assert<eq>(u.userinfo().username, "john doe"s, _src_loc);
   t.assert<eq>(u.pathname(), "/"s, _src_loc);
}

void url_host_ipv4_address_in_rfc3986(Test& t)
{
   Url u("http://192.168.0.1/");

   t.assert<eq>(u.protocol(), "http"s, _src_loc);
   t.assert<eq>(u.hostname(), "192.168.0.1"s, _src_loc);
   t.assert<eq>(u.pathname(), "/"s, _src_loc);
}

void url_host_and_port_ipv4_address_in_rfc3986(Test& t)
{
   Url u("http://192.168.0.1:8080/");

   t.assert<eq>(u.protocol(), "http"s, _src_loc);
   t.assert<eq>(u.hostname(), "192.168.0.1"s, _src_loc);
   t.assert<eq>(u.pathname(), "/"s, _src_loc);
   t.assert<eq>(u.port(), 8080, _src_loc);
}

void url_host_ipv6_address_in_rfc3986(Test& t)
{
   Url u("http://[fe80::1]/");

   t.assert<eq>(u.protocol(), "http"s, _src_loc);
   t.assert<eq>(u.hostname(), "fe80::1"s, _src_loc);
   t.assert<eq>(u.pathname(), "/"s, _src_loc);
}

void url_host_and_port_ipv6_address_in_rfc3986(Test& t)
{
   Url u("http://[fe80::1]:8080/");

   t.assert<eq>(u.protocol(), "http"s, _src_loc);
   t.assert<eq>(u.hostname(), "fe80::1"s, _src_loc);
   t.assert<eq>(u.pathname(), "/"s, _src_loc);
   t.assert<eq>(u.port(), 8080, _src_loc);
}

void url_host_ipv6_address_with_zone_identifier_in_rfc6874(Test& t)
{
   Url u("http://[fe80::1%25en0]/");

   t.assert<eq>(u.protocol(), "http"s, _src_loc);
   t.assert<eq>(u.hostname(), "fe80::1%en0"s, _src_loc);
   t.assert<eq>(u.pathname(), "/"s, _src_loc);
}

void url_host_and_port_ipv6_address_with_zone_identifier_in_rfc6874(Test& t)
{
   Url u("http://[fe80::1%25en0]:8080/");

   t.assert<eq>(u.protocol(), "http"s, _src_loc);
   t.assert<eq>(u.hostname(), "fe80::1%en0"s, _src_loc);
   t.assert<eq>(u.pathname(), "/"s, _src_loc);
   t.assert<eq>(u.port(), 8080, _src_loc);
}

void url_host_ipv6_address_with_zone_identifier_in_rfc6874_percent_encoded(Test& t)
{
   Url u("http://[fe80::1%25%65%6e%301-._~]/");

   t.assert<eq>(u.protocol(), "http"s, _src_loc);
   t.assert<eq>(u.hostname(), "fe80::1%en01-._~"s, _src_loc);
   t.assert<eq>(u.pathname(), "/"s, _src_loc);
}

void url_host_and_port_ipv6_address_with_zone_identifier_in_rfc6874_percent_encoded(Test& t)
{
   Url u("http://[fe80::1%25%65%6e%301-._~]:8080/");

   t.assert<eq>(u.protocol(), "http"s, _src_loc);
   t.assert<eq>(u.hostname(), "fe80::1%en01-._~"s, _src_loc);
   t.assert<eq>(u.pathname(), "/"s, _src_loc);
   t.assert<eq>(u.port(), 8080, _src_loc);
}

void url_href_simple(Test& t)
{
   Url u("http://www.google.com/");

   t.assert<eq>(u.href(), "http://www.google.com/"s, _src_loc);  
}

void url_href_simple_with_port(Test& t)
{
   Url u("http://www.google.com:80/");

   t.assert<eq>(u.href(), "http://www.google.com:80/"s, _src_loc);  
}

void url_href_simple_username(Test& t)
{
   Url u("http://user:pass@www.google.com/");

   t.assert<eq>(u.href(), "http://user:pass@www.google.com/"s, _src_loc);  
}

void url_href_simple_with_query(Test& t)
{
   Url u("http://www.google.com/?q=go+language");

   t.assert<eq>(u.href(), "http://www.google.com/?q=go+language"s, _src_loc);
}

void url_href_path_with_hex_escaping(Test& t)
{
   Url u("http://www.google.com/file%20one&two");

   t.assert<eq>(u.href(), "http://www.google.com/file%20one&two"s, _src_loc);
}

void url_href_escape_sequence_in_username(Test& t)
{
   Url u("ftp://john%20doe@www.google.com/");

   t.assert<eq>(u.href(), "ftp://john%20doe@www.google.com/"s, _src_loc);
   t.assert<eq>(u.port(), 21, _src_loc);
}

void url_href_escape_sequence_in_argument1(Test& t)
{
   Url u("http://www.google.com/?q=go+language#foo%26bar");

   t.assert<eq>(u.hash(), "#foo&bar"s, _src_loc);
   t.assert<eq>(u.href(), "http://www.google.com/?q=go+language#foo&bar"s, _src_loc);
}

RegisterTestCase(OrionNet, url_basic);
RegisterTestCase(OrionNet, url_simple_with_port);
RegisterTestCase(OrionNet, url_simple_username);
RegisterTestCase(OrionNet, url_simple_user_info);
RegisterTestCase(OrionNet, url_simple_with_query);
RegisterTestCase(OrionNet, url_simple_with_query_no_equals);
RegisterTestCase(OrionNet, url_simple_with_multiple_query);
RegisterTestCase(OrionNet, url_simple_with_fragment);
RegisterTestCase(OrionNet, url_simple_with_query_and_fragment);
RegisterTestCase(OrionNet, url_path_with_hex_escaping);
RegisterTestCase(OrionNet, url_escape_sequence_in_username);
RegisterTestCase(OrionNet, url_host_ipv4_address_in_rfc3986);
RegisterTestCase(OrionNet, url_host_and_port_ipv4_address_in_rfc3986);
RegisterTestCase(OrionNet, url_host_ipv6_address_in_rfc3986);
RegisterTestCase(OrionNet, url_host_and_port_ipv6_address_in_rfc3986);
RegisterTestCase(OrionNet, url_host_ipv6_address_with_zone_identifier_in_rfc6874);
RegisterTestCase(OrionNet, url_host_and_port_ipv6_address_with_zone_identifier_in_rfc6874);
RegisterTestCase(OrionNet, url_host_ipv6_address_with_zone_identifier_in_rfc6874_percent_encoded);
RegisterTestCase(OrionNet, url_host_and_port_ipv6_address_with_zone_identifier_in_rfc6874_percent_encoded);
RegisterTestCase(OrionNet, url_href_simple);
RegisterTestCase(OrionNet, url_href_simple_with_port);
RegisterTestCase(OrionNet, url_href_simple_username);
RegisterTestCase(OrionNet, url_href_simple_with_query);
RegisterTestCase(OrionNet, url_href_path_with_hex_escaping);
RegisterTestCase(OrionNet, url_href_escape_sequence_in_username);
RegisterTestCase(OrionNet, url_href_escape_sequence_in_argument1);
}
