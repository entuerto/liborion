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
#include <orion/Types.h>
#include <orion/net/http/Error.h>

#include <asio.hpp>
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
// Forward declarations
// 
//class EndPoint;

namespace http
{
//-------------------------------------------------------------------------------------------------
class Request;
class Response;

//-------------------------------------------------------------------------------------------------

using Header = std::map<std::string, std::string>;

using HandlerFunc = std::function<std::error_code(const Request&, Response&)>;

using Handlers = std::map<std::string, HandlerFunc>;

using CloseHandler    = std::function<void()>;
using ConnectHandler  = std::function<void(const asio::ip::tcp::endpoint&)>;
using ErrorHandler    = std::function<void(const std::error_code&)>;
using ResponseHandler = std::function<void(const Response&)>;

//-------------------------------------------------------------------------------------------------

struct Version
{
   int major;
   int minor;
};

//-------------------------------------------------------------------------------------------------

template<typename T>
T from_string(const std::string& text);

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
using Method = BaseType<std::string, struct MethodTag>;

//-------------------------------------------------------------------------------------------------

inline std::string to_string(const Method& m);

inline std::ostream& operator<<(std::ostream& o, const Method& m);

inline bool operator==(const Method& m, const std::string& text);

inline bool operator==(const std::string& text, const Method& m);

inline bool operator==(const Method& m1, const Method& m2);

inline bool operator<(const Method& m1, const Method& m2);

inline Method make_method(const std::string& text);

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

inline std::ostream& operator<<(std::ostream& o, Field f);

inline bool operator==(Field f, const std::string& text);

inline bool operator==(const std::string& text, Field f);

inline std::string to_string(Field f);

template<>
inline Field from_string<Field>(const std::string& text);

//-------------------------------------------------------------------------------------------------

const char crlf[] = {'\r', '\n', '\0'};

} // namespace http
} // namespace net
} // namespace orion

#include <orion/net/http/impl/Utils.ipp>

#endif // ORION_NET_HTTP_UTILS_H
