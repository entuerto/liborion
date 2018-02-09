//
// Utils.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP_UTILS_H
#define ORION_NET_HTTP_UTILS_H

#include <orion/Orion-Stddefs.h>

#include <orion/String.h>
#include <orion/net/http/Error.h>

#include <fmt/format.h>

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace orion
{
namespace net
{
namespace http
{
//-------------------------------------------------------------------------------------------------
class Request;
class Response;

//-------------------------------------------------------------------------------------------------

using Header = std::map<std::string, std::string>;

using HandlerFunc = std::function<std::error_code(const Request&, Response&)>;

using Handlers = std::map<std::string, HandlerFunc>;

//-------------------------------------------------------------------------------------------------

struct Version
{
   int major;
   int minor;
};

//-------------------------------------------------------------------------------------------------

#if defined(DELETE) && defined(_WIN32)
#undef DELETE
#endif
///
/// HTTP defines methods (sometimes referred to as verbs) to indicate the desired action to be
/// performed on the identified resource.
///
/// Summary table
/// +--------------+----------+----------+----------+------+------------+-----------+
/// | HTTP Method  | RFC      | Request  | Response | Safe | Idempotent | Cacheable |
/// |              |          | Has Body | Has Body |      |            |           |
/// +--------------+----------+----------+----------+------+------------+-----------+
/// | GET          | RFC 7231 |    No    |   Yes    | Yes  |    Yes     |    Yes    |
/// | HEAD         | RFC 7231 |    No    |   No     | Yes  |    Yes     |    Yes    |
/// | POST         | RFC 7231 |    Yes   |   Yes    | No   |    No      |    Yes    |
/// | PUT          | RFC 7231 |    Yes   |   Yes    | No   |    Yes     |    No     |
/// | DELETE       | RFC 7231 |    No    |   Yes    | No   |    Yes     |    No     |
/// | CONNECT      | RFC 7231 |    Yes   |   Yes    | No   |    No      |    No     |
/// | OPTIONS      | RFC 7231 | Optional |   Yes    | Yes  |    Yes     |    No     |
/// | TRACE        | RFC 7231 |    No    |   Yes    | Yes  |    Yes     |    No     |
/// | PATCH        | RFC 5789 |    Yes   |   Yes    | No   |    No      |    Yes    |
/// +--------------+----------+----------+----------+------+------------+-----------+
///
enum class Method
{
   DELETE = 0,
   GET    = 1,
   HEAD   = 2,
   POST   = 3,
   PUT    = 4,
   // Pathological
   CONNECT = 5,
   OPTIONS = 6,
   TRACE   = 7,
   // WebDAV
   COPY      = 8,
   LOCK      = 9,
   MKCOL     = 10,
   MOVE      = 11,
   PROPFIND  = 12,
   PROPPATCH = 13,
   SEARCH    = 14,
   UNLOCK    = 15,
   BIND      = 16,
   REBIND    = 17,
   UNBIND    = 18,
   ACL       = 19,
   // Subversion
   REPORT     = 20,
   MKACTIVITY = 21,
   CHECKOUT   = 22,
   MERGE      = 23,
   // upnp
   MSEARCH     = 24,
   NOTIFY      = 25,
   SUBSCRIBE   = 26,
   UNSUBSCRIBE = 27,
   // RFC-5789
   PATCH = 28,
   PURGE = 29,
   // CalDAV
   MKCALENDAR = 30,
   // RFC-2068, section 19.6.1.2
   LINK   = 31,
   UNLINK = 32
};

//-------------------------------------------------------------------------------------------------

void format_arg(fmt::BasicFormatter<char>& f, const char*& fmt_str, Method m);

std::ostream& operator<<(std::ostream& o, Method m);

bool operator==(Method m, const std::string& text);

bool operator==(const std::string& text, Method m);

std::string to_string(Method m);

Method as_method(const std::string& text);

//-------------------------------------------------------------------------------------------------

const char crlf[] = {'\r', '\n', '\0'};

} // namespace http
} // namespace net
} // namespace orion

#include <orion/net/http/impl/Utils.ipp>

#endif // ORION_NET_HTTP_UTILS_H
