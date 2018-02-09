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
static const std::map<Method, std::string>& method_text()
{
   try
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

inline Method as_method(const std::string& text)
{
   for (auto& item : method_text())
   {
      if (not iequals(item.second, text))
         continue;

      return item.first;
   }

   throw std::system_error(make_error_code(ErrorCode::MethodNotAllowed));
}

} // namespace http
} // namespace net
} // namespace orion

#endif // ORION_NET_HTTP_UTILS_IPP
