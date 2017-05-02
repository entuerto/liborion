//
// Utils.h
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_NET_HTTP_UTILS_H
#define ORION_NET_HTTP_UTILS_H

#include <chrono>
#include <map>
#include <vector>
#include <string>
 
#include <orion/Orion-Stddefs.h>

#include <orion/StringUtils.h>
#include <orion/net/http/Error.h>

namespace orion
{
namespace net
{
namespace http
{
//-------------------------------------------------------------------------------------------------

using Header = std::map<std::string, std::string>;

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
   DELETE      = 0,
   GET         = 1,
   HEAD        = 2,
   POST        = 3,
   PUT         = 4,
   // Pathological 
   CONNECT     = 5,
   OPTIONS     = 6,
   TRACE       = 7,
   // WebDAV 
   COPY        = 8, 
   LOCK        = 9, 
   MKCOL       = 10,
   MOVE        = 11,
   PROPFIND    = 12,
   PROPPATCH   = 13,
   SEARCH      = 14,
   UNLOCK      = 15,
   BIND        = 16,
   REBIND      = 17,
   UNBIND      = 18,
   ACL         = 19,
   // Subversion 
   REPORT      = 20,
   MKACTIVITY  = 21,
   CHECKOUT    = 22,
   MERGE       = 23,
   // upnp 
   MSEARCH     = 24, 
   NOTIFY      = 25, 
   SUBSCRIBE   = 26, 
   UNSUBSCRIBE = 27, 
   // RFC-5789 
   PATCH       = 28,
   PURGE       = 29,
   // CalDAV 
   MKCALENDAR  = 30,
   // RFC-2068, section 19.6.1.2 
   LINK        = 31, 
   UNLINK      = 32 
};

static const std::map<Method, std::string> MethodText
{ 
   {Method::DELETE,      "DELETE"},
   {Method::GET,         "GET"},
   {Method::HEAD,        "HEAD"}, 
   {Method::POST,        "POST"},  
   {Method::PUT,         "PUT"},
   {Method::CONNECT,     "CONNECT"},
   {Method::OPTIONS,     "OPTIONS"},
   {Method::TRACE,       "TRACE"},  
   {Method::COPY,        "COPY"},     
   {Method::LOCK,        "LOCK"},     
   {Method::MKCOL,       "MKCOL"},    
   {Method::MOVE,        "MOVE"},     
   {Method::PROPFIND,    "PROPFIND"}, 
   {Method::PROPPATCH,   "PROPPATCH"},
   {Method::SEARCH,      "SEARCH"},   
   {Method::UNLOCK,      "UNLOCK"},   
   {Method::BIND,        "BIND"},     
   {Method::REBIND,      "REBIND"},   
   {Method::UNBIND,      "UNBIND"},   
   {Method::ACL,         "ACL"}, 
   {Method::REPORT,      "REPORT"},    
   {Method::MKACTIVITY,  "MKACTIVITY"},
   {Method::CHECKOUT,    "CHECKOUT"},  
   {Method::MERGE,       "MERGE"},
   {Method::MSEARCH,     "MSEARCH"},    
   {Method::NOTIFY,      "NOTIFY"},     
   {Method::SUBSCRIBE,   "SUBSCRIBE"},  
   {Method::UNSUBSCRIBE, "UNSUBSCRIBE"},
   {Method::PATCH,       "PATCH"},
   {Method::PURGE,       "PURGE"},
   {Method::MKCALENDAR,  "MKCALENDAR"}, 
   {Method::LINK,        "LINK"},  
   {Method::UNLINK,      "UNLINK"}        
};

inline std::ostream& operator<<(std::ostream& o, const Method m)
{
   auto item = MethodText.find(m);

   if (item != MethodText.end())
      o << item->second;
   else
      o << "UNKOWN";

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

   return "UNKOWN";
}

inline Method as_method(const std::string& text)
{
   for (auto& item : MethodText)
   {
      if (not equals_no_case(item.second, text))
         continue;

      return item.first;
   }

   throw std::system_error(make_error_code(ErrorCode::MethodNotAllowed));
}

//-------------------------------------------------------------------------------------------------

const char crlf[] = { '\r', '\n', '\0' };

//-------------------------------------------------------------------------------------------------

struct Parameter 
{
   std::string key;
   std::string value;
};

class Parameters 
{
public:
   Parameters() = default;
   Parameters(const std::initializer_list<Parameter>& p) :
      _params(p) {}

   void add(const Parameter& p)
   {
      _params.push_back(p);
   }

   std::vector<Parameter> _params;
};

//-------------------------------------------------------------------------------------------------

struct Timeout 
{
   std::chrono::milliseconds ms;
};

} // http
} // net
} // orion

#endif // ORION_NET_HTTP_UTILS_H
