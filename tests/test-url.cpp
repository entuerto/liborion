//
//  test-url.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Log.h>
#include <orion/net/Url.h>
#include <orion/Test.h>

using namespace orion;
using namespace orion::net;
using namespace orion::unittest;

using namespace std::string_literals;

Section(OrionNet_URL, Label{"URL"})
{

TestCase("Basic")
{
   Url u("http://foo.com/path");

   check_eq(u.protocol(), "http"s);
   check_eq(u.hostname(), "foo.com"s);
   check_eq(u.pathname(), "/path"s);
   check_eq(u.port(), 80);
}

TestCase("Simple with port")
{
   Url u("http://foo.com:22/path");

   check_eq(u.protocol(), "http"s);
   check_eq(u.hostname(), "foo.com"s);
   check_eq(u.pathname(), "/path"s);
   check_eq(u.port(), 22);
}

TestCase("Simple username")
{
   Url u("http://toto@foo.com/path");

   check_eq(u.protocol(), "http"s);
   check_eq(u.hostname(), "foo.com"s);
   check_eq(u.pathname(), "/path"s);
   check_eq(u.userinfo().username, "toto"s);
   check_eq(u.userinfo().password, ""s);
   check_eq(u.port(), 80);
}

TestCase("Simple user information")
{
   Url u("http://toto:tata@foo.com/path");

   check_eq(u.protocol(), "http"s);
   check_eq(u.hostname(), "foo.com"s);
   check_eq(u.pathname(), "/path"s);
   check_eq(u.userinfo().username, "toto"s);
   check_eq(u.userinfo().password, "tata"s);
   check_eq(u.port(), 80);
}

TestCase("Simple with query")
{
   Url u("http://www.google.com/?q=go+language");

   check_eq(u.protocol(), "http"s);
   check_eq(u.hostname(), "www.google.com"s);
   check_eq(u.pathname(), "/"s);
   check_eq(u.query("q"), "go language"s);
   check_eq(u.port(), 80);
}

TestCase("Simple with query no equals")
{
   Url u("http://www.google.com/?q");

   check_eq(u.protocol(), "http"s);
   check_eq(u.hostname(), "www.google.com"s);
   check_eq(u.pathname(), "/"s);
   check_eq(u.query("q"), ""s);
   check_eq(u.port(), 80);
}

TestCase("Simple with multiple query")
{
   Url u("http://www.google.com/?q=go+language&foo=bar");

   check_eq(u.protocol(), "http"s);
   check_eq(u.hostname(), "www.google.com"s);
   check_eq(u.pathname(), "/"s);
   check_eq(u.query("q"), "go language"s);
   check_eq(u.query("foo"), "bar"s);
   check_eq(u.port(), 80);
}

TestCase("Simple with fragment")
{
   Url u("https://www.google.com/#foo");

   check_eq(u.protocol(), "https"s);
   check_eq(u.hostname(), "www.google.com"s);
   check_eq(u.pathname(), "/"s);
   check_eq(u.hash(), "#foo"s);
   check_eq(u.port(), 443);
}

TestCase("Simple with query and fragment")
{
   Url u("http://www.google.com/?q=go+language#foo");

   check_eq(u.protocol(), "http"s);
   check_eq(u.hostname(), "www.google.com"s);
   check_eq(u.pathname(), "/"s);
   check_eq(u.query("q"), "go language"s);
   check_eq(u.hash(), "#foo"s);
}

TestCase("Path with hex escaping")
{
   Url u("http://www.google.com/file%20one%26two");

   check_eq(u.protocol(), "http"s);
   check_eq(u.hostname(), "www.google.com"s);
   check_eq(u.pathname(), "/file one&two"s);
}

TestCase("Escape sequence in username")
{
   Url u("ftp://john%20doe@www.google.com/");

   check_eq(u.protocol(), "ftp"s);
   check_eq(u.hostname(), "www.google.com"s);
   check_eq(u.userinfo().username, "john doe"s);
   check_eq(u.pathname(), "/"s);
}

TestCase("Host ipv4 address in rfc3986")
{
   Url u("http://192.168.0.1/");

   check_eq(u.protocol(), "http"s);
   check_eq(u.hostname(), "192.168.0.1"s);
   check_eq(u.pathname(), "/"s);
}

TestCase("Host and port ipv4 address in rfc3986")
{
   Url u("http://192.168.0.1:8080/");

   check_eq(u.protocol(), "http"s);
   check_eq(u.hostname(), "192.168.0.1"s);
   check_eq(u.pathname(), "/"s);
   check_eq(u.port(), 8080);
}

TestCase("Host ipv6 address in rfc3986")
{
   Url u("http://[fe80::1]/");

   check_eq(u.protocol(), "http"s);
   check_eq(u.hostname(), "fe80::1"s);
   check_eq(u.pathname(), "/"s);
}

TestCase("Host and port ipv6 address in rfc3986")
{
   Url u("http://[fe80::1]:8080/");

   check_eq(u.protocol(), "http"s);
   check_eq(u.hostname(), "fe80::1"s);
   check_eq(u.pathname(), "/"s);
   check_eq(u.port(), 8080);
}

TestCase("Host ipv6 address with zone identifier in rfc6874")
{
   Url u("http://[fe80::1%25en0]/");

   check_eq(u.protocol(), "http"s);
   check_eq(u.hostname(), "fe80::1%en0"s);
   check_eq(u.pathname(), "/"s);
}

TestCase("Host and port ipv6 address with zone identifier in rfc6874")
{
   Url u("http://[fe80::1%25en0]:8080/");

   check_eq(u.protocol(), "http"s);
   check_eq(u.hostname(), "fe80::1%en0"s);
   check_eq(u.pathname(), "/"s);
   check_eq(u.port(), 8080);
}

TestCase("Host ipv6 address with zone identifier in rfc6874 percent encoded")
{
   Url u("http://[fe80::1%25%65%6e%301-._~]/");

   check_eq(u.protocol(), "http"s);
   check_eq(u.hostname(), "fe80::1%en01-._~"s);
   check_eq(u.pathname(), "/"s);
}

TestCase("Host and port ipv6 address with zone identifier in rfc6874 percent encoded")
{
   Url u("http://[fe80::1%25%65%6e%301-._~]:8080/");

   check_eq(u.protocol(), "http"s);
   check_eq(u.hostname(), "fe80::1%en01-._~"s);
   check_eq(u.pathname(), "/"s);
   check_eq(u.port(), 8080);
}

TestCase("href simple")
{
   Url u("http://www.google.com/");

   check_eq(u.href(), "http://www.google.com/"s);  
}

TestCase("href simple with port")
{
   Url u("http://www.google.com:80/");

   check_eq(u.href(), "http://www.google.com:80/"s);  
}

TestCase("href simple username")
{
   Url u("http://user:pass@www.google.com/");

   check_eq(u.href(), "http://user:pass@www.google.com/"s);  
}

TestCase("href simple with query")
{
   Url u("http://www.google.com/?q=go+language");

   check_eq(u.href(), "http://www.google.com/?q=go+language"s);
}

TestCase("href path with hex escaping")
{
   Url u("http://www.google.com/file%20one&two");

   check_eq(u.href(), "http://www.google.com/file%20one&two"s);
}

TestCase("href escape sequence in username")
{
   Url u("ftp://john%20doe@www.google.com/");

   check_eq(u.href(), "ftp://john%20doe@www.google.com/"s);
   check_eq(u.port(), 21);
}

TestCase("href escape sequence in argument1")
{
   Url u("http://www.google.com/?q=go+language#foo%26bar");

   check_eq(u.hash(), "#foo&bar"s);
   check_eq(u.href(), "http://www.google.com/?q=go+language#foo&bar"s);
}

}
