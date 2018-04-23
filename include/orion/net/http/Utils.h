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

template<typename T>
T from_string(const std::string& text) {}

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
   Unknown,

   Delete,
   Get,
   Head,
   Post,
   Put,

   // Pathological
   Connect,
   Options,
   Trace,

   // WebDAV
   Copy,
   Lock,
   MkCol,
   Move,
   PropFind,
   PropPatch,
   Search,
   Unlock,
   Bind,
   Rebind,
   Unbind,
   Acl,

   // Subversion
   Report,
   MkActivity,
   Checkout,
   Merge,

   // upnp
   MSearch,
   Notify,
   Subscribe,
   Unsubscribe,

   // RFC-5789
   Patch,
   Purge,

   // CalDAV
   MkCalendar,

   // RFC-2068, section 19.6.1.2
   Link,
   Unlink
};

//-------------------------------------------------------------------------------------------------

void format_arg(fmt::BasicFormatter<char>& f, const char*& fmt_str, Method m);

std::ostream& operator<<(std::ostream& o, Method m);

bool operator==(Method m, const std::string& text);

bool operator==(const std::string& text, Method m);

std::string to_string(Method m);

template<>
Method from_string<Method>(const std::string& text);

//-------------------------------------------------------------------------------------------------

enum class Field
{
   Accept,                    // Accept
   AcceptCharset,             // Accept-Charset
   AcceptEncoding,            // Accept-Encoding
   AcceptLanguage,            // Accept-Language
   AcceptPost,                // Accept-Post
   AcceptRanges,              // Accept-Ranges
   Age,                       // Age
   Allow,                     // Allow
   ALPN,                      // ALPN
   AltSvc,                    // Alt-Svc
   AltUsed,                   // Alt-Used
   AuthenticationInfo,        // Authentication-Info
   Authorization,             // Authorization
   CacheControl,              // Cache-Control
   CalDAVTimezones,           // CalDAV-Timezones
   Connection,                // Connection
   ContentDisposition,        // Content-Disposition
   ContentEncoding,           // Content-Encoding
   ContentLanguage,           // Content-Language
   ContentLength,             // Content-Length
   ContentLocation,           // Content-Location
   ContentRange,              // Content-Range
   ContentType,               // Content-Type
   Cookie,                    // Cookie
   DASL,                      // DASL
   DAV,                       // DAV
   Date,                      // Date
   Depth,                     // Depth
   Destination,               // Destination
   ETag,                      // ETag
   Expect,                    // Expect
   Expires,                   // Expires
   Forwarded,                 // Forwarded
   From,                      // From
   Host,                      // Host
   HTTP2Settings,             // HTTP2-Settings
   If,                        // If
   IfMatch,                   // If-Match
   IfModifiedSince,           // If-Modified-Since
   IfNoneMatch,               // If-None-Match
   IfRange,                   // If-Range                       
   IfScheduleTagMatch,        // If-Schedule-Tag-Match
   IfUnmodifiedSince,         // If-Unmodified-Since
   LastModified,              // Last-Modified                  
   Link,                      // Link
   Location,                  // Location
   LockToken,                 // Lock-Token
   MaxForwards,               // Max-Forwards
   MIMEVersion,               // MIME-Version
   OrderingType,              // Ordering-Type
   Origin,                    // Origin
   Overwrite,                 // Overwrite
   Position,                  // Position
   Pragma,                    // Pragma
   Prefer,                    // Prefer
   PreferenceApplied,         // Preference-Applied
   ProxyAuthenticate,         // Proxy-Authenticate
   ProxyAuthenticationInfo,   // Proxy-Authentication-Info
   ProxyAuthorization,        // Proxy-Authorization
   PublicKeyPins,             // Public-Key-Pins
   PublicKeyPinsReportOnly,   // Public-Key-Pins-Report-Only
   Range,                     // Range
   Referer,                   // Referer
   RetryAfter,                // Retry-After
   ScheduleReply,             // Schedule-Reply
   ScheduleTag,               // Schedule-Tag
   SecWebSocketAccept,        // Sec-WebSocket-Accept
   SecWebSocketExtensions,    // Sec-WebSocket-Extensions
   SecWebSocketKey,           // Sec-WebSocket-Key
   SecWebSocketProtocol,      // Sec-WebSocket-Protocol
   SecWebSocketVersion,       // Sec-WebSocket-Version
   Server,                    // Server
   SetCookie,                 // Set-Cookie
   SLUG,                      // SLUG
   StrictTransportSecurity,   // Strict-Transport-Security
   TE,                        // TE
   Timeout,                   // Timeout
   Topic,                     // Topic
   Trailer,                   // Trailer
   TransferEncoding,          // Transfer-Encoding
   TTL,                       // TTL
   Urgency,                   // Urgency
   Upgrade,                   // Upgrade
   UserAgent,                 // User-Agent
   Vary,                      // Vary
   Via,                       // Via
   WWWAuthenticate,           // WWW-Authenticate
   Warning,                   // Warning
   XContentTypeOptions        // X-Content-Type-Options
};

//-------------------------------------------------------------------------------------------------

void format_arg(fmt::BasicFormatter<char>& f, const char*& fmt_str, Field field);

std::ostream& operator<<(std::ostream& o, Field f);

bool operator==(Field f, const std::string& text);

bool operator==(const std::string& text, Field f);

std::string to_string(Field f);

template<>
Field from_string<Field>(const std::string& text);


//-------------------------------------------------------------------------------------------------

const char crlf[] = {'\r', '\n', '\0'};

} // namespace http
} // namespace net
} // namespace orion

#include <orion/net/http/impl/Utils.ipp>

#endif // ORION_NET_HTTP_UTILS_H
