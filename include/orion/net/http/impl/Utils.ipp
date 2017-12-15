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

#include <map>
#include <string>

namespace orion
{
namespace net
{
namespace http
{
static const std::map<Method, std::string> MethodText{{Method::DELETE, "DELETE"},
                                                      {Method::GET, "GET"},
                                                      {Method::HEAD, "HEAD"},
                                                      {Method::POST, "POST"},
                                                      {Method::PUT, "PUT"},
                                                      {Method::CONNECT, "CONNECT"},
                                                      {Method::OPTIONS, "OPTIONS"},
                                                      {Method::TRACE, "TRACE"},
                                                      {Method::COPY, "COPY"},
                                                      {Method::LOCK, "LOCK"},
                                                      {Method::MKCOL, "MKCOL"},
                                                      {Method::MOVE, "MOVE"},
                                                      {Method::PROPFIND, "PROPFIND"},
                                                      {Method::PROPPATCH, "PROPPATCH"},
                                                      {Method::SEARCH, "SEARCH"},
                                                      {Method::UNLOCK, "UNLOCK"},
                                                      {Method::BIND, "BIND"},
                                                      {Method::REBIND, "REBIND"},
                                                      {Method::UNBIND, "UNBIND"},
                                                      {Method::ACL, "ACL"},
                                                      {Method::REPORT, "REPORT"},
                                                      {Method::MKACTIVITY, "MKACTIVITY"},
                                                      {Method::CHECKOUT, "CHECKOUT"},
                                                      {Method::MERGE, "MERGE"},
                                                      {Method::MSEARCH, "MSEARCH"},
                                                      {Method::NOTIFY, "NOTIFY"},
                                                      {Method::SUBSCRIBE, "SUBSCRIBE"},
                                                      {Method::UNSUBSCRIBE, "UNSUBSCRIBE"},
                                                      {Method::PATCH, "PATCH"},
                                                      {Method::PURGE, "PURGE"},
                                                      {Method::MKCALENDAR, "MKCALENDAR"},
                                                      {Method::LINK, "LINK"},
                                                      {Method::UNLINK, "UNLINK"}};

inline void format_arg(fmt::BasicFormatter<char>& f, const char*& fmt_str, const Method m) 
{
   f.writer().write("{}", to_string(m));
}

inline std::ostream& operator<<(std::ostream& o, const Method m)
{
   auto item = MethodText.find(m);

   if (item != MethodText.end())
      o << item->second;
   else
      o << "Method: Unkown";

   return o;
}

inline bool operator==(const Method m, const std::string& text)
{
   auto item = MethodText.find(m);

   if (item != MethodText.end())
      return item->second == text;

   return false;
}

inline bool operator==(const std::string& text, const Method m)
{
   return operator==(m, text);
}

inline std::string to_string(const Method m)
{
   auto item = MethodText.find(m);

   if (item != MethodText.end())
      return item->second;

   return "Method: Unkown";
}

inline Method as_method(const std::string& text)
{
   for (auto& item : MethodText)
   {
      if (not iequals(item.second, text))
         continue;

      return item.first;
   }

   throw std::system_error(make_error_code(ErrorCode::MethodNotAllowed));
}

} // http
} // net
} // orion

#endif // ORION_NET_HTTP_UTILS_IPP
