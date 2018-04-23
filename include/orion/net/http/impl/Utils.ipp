//
// Utils.ipp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP_UTILS_IPP
#define ORION_NET_HTTP_UTILS_IPP

#include <orion/Orion-Stddefs.h>

#include <orion/Log.h>
#include <orion/Utils.h>

#include <exception>
#include <map>
#include <string>

#undef DELETE

namespace orion
{
namespace net
{
namespace http
{

//-------------------------------------------------------------------------------------------------

static const std::map<Method, std::string>& method_text()
{
   try
   {
      static const std::map<Method, std::string> MethodText{{Method::Unknown, "UNKNOWN"},
                                                            {Method::Delete, "DELETE"},
                                                            {Method::Get, "GET"},
                                                            {Method::Head, "HEAD"},
                                                            {Method::Post, "POST"},
                                                            {Method::Put, "PUT"},
                                                            {Method::Connect, "CONNECT"},
                                                            {Method::Options, "OPTIONS"},
                                                            {Method::Trace, "TRACE"},
                                                            {Method::Copy, "COPY"},
                                                            {Method::Lock, "LOCK"},
                                                            {Method::MkCol, "MKCOL"},
                                                            {Method::Move, "MOVE"},
                                                            {Method::PropFind, "PROPFIND"},
                                                            {Method::PropPatch, "PROPPATCH"},
                                                            {Method::Search, "SEARCH"},
                                                            {Method::Unlock, "UNLOCK"},
                                                            {Method::Bind, "BIND"},
                                                            {Method::Rebind, "REBIND"},
                                                            {Method::Unbind, "UNBIND"},
                                                            {Method::Acl, "ACL"},
                                                            {Method::Report, "REPORT"},
                                                            {Method::MkActivity, "MKACTIVITY"},
                                                            {Method::Checkout, "CHECKOUT"},
                                                            {Method::Merge, "MERGE"},
                                                            {Method::MSearch, "MSEARCH"},
                                                            {Method::Notify, "NOTIFY"},
                                                            {Method::Subscribe, "SUBSCRIBE"},
                                                            {Method::Unsubscribe, "UNSUBSCRIBE"},
                                                            {Method::Patch, "PATCH"},
                                                            {Method::Purge, "PURGE"},
                                                            {Method::MkCalendar, "MKCALENDAR"},
                                                            {Method::Link, "LINK"},
                                                            {Method::Unlink, "UNLINK"}};

      return MethodText;
   }
   catch (...)
   {
      log::error("An unexpected, unknown exception was thrown: ",
                 type_name(std::current_exception()),
                 _src_loc);
      std::abort();
   }
}

inline void format_arg(fmt::BasicFormatter<char>& f, const char*& /*fmt_str*/, Method m)
{
   f.writer().write("{}", to_string(m));
}

inline std::ostream& operator<<(std::ostream& o, Method m)
{
   auto item = method_text().find(m);

   if (item != method_text().end())
      o << item->second;
   else
      o << "Method: Unkown";

   return o;
}

inline bool operator==(Method m, const std::string& text)
{
   auto item = method_text().find(m);

   if (item != method_text().end())
      return item->second == text;

   return false;
}

inline bool operator==(const std::string& text, Method m)
{
   return operator==(m, text);
}

inline std::string to_string(Method m)
{
   auto item = method_text().find(m);

   if (item != method_text().end())
      return item->second;

   return "Method: Unkown";
}

template<>
inline Method from_string<Method>(const std::string& text)
{
   for (auto& item : method_text())
   {
      if (not iequals(item.second, text))
         continue;

      return item.first;
   }

   throw std::system_error(make_error_code(ErrorCode::MethodTextNotFound));
}

//-------------------------------------------------------------------------------------------------

static const std::map<Field, std::string>& field_text()
{
   try
   {
      static const std::map<Field, std::string> FieldText{
         {Field::Accept, "Accept"},
         {Field::AcceptCharset, "Accept-Charset"},
         {Field::AcceptEncoding, "Accept-Encoding"},
         {Field::AcceptLanguage, "Accept-Language"},
         {Field::AcceptPost, "Accept-Post"},
         {Field::AcceptRanges, "Accept-Ranges"},
         {Field::Age, "Age"},
         {Field::Allow, "Allow"},
         {Field::ALPN, "ALPN"},
         {Field::AltSvc, "Alt-Svc"},
         {Field::AltUsed, "Alt-Used"},
         {Field::AuthenticationInfo, "Authentication-Info"},
         {Field::Authorization, "Authorization"},
         {Field::CacheControl, "Cache-Control"},
         {Field::CalDAVTimezones, "CalDAV-Timezones"},
         {Field::Connection, "Connection"},
         {Field::ContentDisposition, "Content-Disposition"},
         {Field::ContentEncoding, "Content-Encoding"},
         {Field::ContentLanguage, "Content-Language"},
         {Field::ContentLength, "Content-Length"},
         {Field::ContentLocation, "Content-Location"},
         {Field::ContentRange, "Content-Range"},
         {Field::ContentType, "Content-Type"},
         {Field::Cookie, "Cookie"},
         {Field::DASL, "DASL"},
         {Field::DAV, "DAV"},
         {Field::Date, "Date"},
         {Field::Depth, "Depth"},
         {Field::Destination, "Destination"},
         {Field::ETag, "ETag"},
         {Field::Expect, "Expect"},
         {Field::Expires, "Expires"},
         {Field::Forwarded, "Forwarded"},
         {Field::From, "From"},
         {Field::Host, "Host"},
         {Field::HTTP2Settings, "HTTP2-Settings"},
         {Field::If, "If"},
         {Field::IfMatch, "If-Match"},
         {Field::IfModifiedSince, "If-Modified-Since"},
         {Field::IfNoneMatch, "If-None-Match"},
         {Field::IfRange, "If-Range"},
         {Field::IfScheduleTagMatch, "If-Schedule-Tag-Match"},
         {Field::IfUnmodifiedSince, "If-Unmodified-Since"},
         {Field::LastModified, "Last-Modified"},
         {Field::Link, "Link"},
         {Field::Location, "Location"},
         {Field::LockToken, "Lock-Token"},
         {Field::MaxForwards, "Max-Forwards"},
         {Field::MIMEVersion, "MIME-Version"},
         {Field::OrderingType, "Ordering-Type"},
         {Field::Origin, "Origin"},
         {Field::Overwrite, "Overwrite"},
         {Field::Position, "Position"},
         {Field::Pragma, "Pragma"},
         {Field::Prefer, "Prefer"},
         {Field::PreferenceApplied, "Preference-Applied"},
         {Field::ProxyAuthenticate, "Proxy-Authenticate"},
         {Field::ProxyAuthenticationInfo, "Proxy-Authentication-Info"},
         {Field::ProxyAuthorization, "Proxy-Authorization"},
         {Field::PublicKeyPins, "Public-Key-Pins"},
         {Field::PublicKeyPinsReportOnly, "Public-Key-Pins-Report-Only"},
         {Field::Range, "Range"},
         {Field::Referer, "Referer"},
         {Field::RetryAfter, "Retry-After"},
         {Field::ScheduleReply, "Schedule-Reply"},
         {Field::ScheduleTag, "Schedule-Tag"},
         {Field::SecWebSocketAccept, "Sec-WebSocket-Accept"},
         {Field::SecWebSocketExtensions, "Sec-WebSocket-Extensions"},
         {Field::SecWebSocketKey, "Sec-WebSocket-Key"},
         {Field::SecWebSocketProtocol, "Sec-WebSocket-Protocol"},
         {Field::SecWebSocketVersion, "Sec-WebSocket-Version"},
         {Field::Server, "Server"},
         {Field::SetCookie, "Set-Cookie"},
         {Field::SLUG, "SLUG"},
         {Field::StrictTransportSecurity, "Strict-Transport-Security"},
         {Field::TE, "TE"},
         {Field::Timeout, "Timeout"},
         {Field::Topic, "Topic"},
         {Field::Trailer, "Trailer"},
         {Field::TransferEncoding, "Transfer-Encoding"},
         {Field::TTL, "TTL"},
         {Field::Urgency, "Urgency"},
         {Field::Upgrade, "Upgrade"},
         {Field::UserAgent, "User-Agent"},
         {Field::Vary, "Vary"},
         {Field::Via, "Via"},
         {Field::WWWAuthenticate, "WWW-Authenticate"},
         {Field::Warning, "Warning"},
         {Field::XContentTypeOptions, "X-Content-Type-Options"}};

      return FieldText;
   }
   catch (...)
   {
      log::error("An unexpected, unknown exception was thrown: ",
                 type_name(std::current_exception()),
                 _src_loc);
      std::abort();
   }
}

inline void format_arg(fmt::BasicFormatter<char>& f, const char*& /*fmt_str*/, Field field)
{
   f.writer().write("{}", to_string(field));
}

inline std::ostream& operator<<(std::ostream& o, Field f)
{
   auto item = field_text().find(f);

   if (item != field_text().end())
      o << item->second;
   else
      o << "Field: Unkown";

   return o;
}

inline bool operator==(Field f, const std::string& text)
{
   auto item = field_text().find(f);

   if (item != field_text().end())
      return item->second == text;

   return false;
}

inline bool operator==(const std::string& text, Field f)
{
   return operator==(f, text);
}

inline std::string to_string(Field f)
{
   auto item = field_text().find(f);

   if (item != field_text().end())
      return item->second;

   return "Field: Unkown";
}

template<>
inline Field from_string<Field>(const std::string& text)
{
   for (auto& item : field_text())
   {
      if (not iequals(item.second, text))
         continue;

      return item.first;
   }

   throw std::system_error(make_error_code(ErrorCode::FieldTextNotFound));
}

} // namespace http
} // namespace net
} // namespace orion

#endif // ORION_NET_HTTP_UTILS_IPP
