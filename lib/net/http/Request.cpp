//
// Request.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <orion/net/http/Request.h>

#include <cstdlib>
#include <iomanip>
#include <sstream>

#include <orion/StringUtils.h>

namespace orion
{
namespace net
{
namespace http
{
Request::Request() :
   _method(),
   _url(),
   _version(),
   _header(),
   _should_keep_alive(false),
   _upgrade(false)
{
}

Request::Request(const std::string& method, 
                 const Url& url, 
                 const Version& version, 
                 const Header& header,
                       bool should_keep_alive,
                       bool upgrade) :
   _method(method),
   _url(url),
   _version(version),
   _header(header),
   _should_keep_alive(should_keep_alive),
   _upgrade(upgrade)
{
}

Request::Request(Request&& Other) :
   _method(std::move(Other._method)),
   _url(std::move(Other._url)),
   _version(std::move(Other._version)),
   _header(std::move(Other._header)),
   _should_keep_alive(std::move(Other._should_keep_alive)),
   _upgrade(std::move(Other._upgrade))
{
}

Request::~Request()
{
}

//! "GET", "POST", etc
std::string Request::method() const
{
   return _method;
}

   //! URL-decoded URI
Url Request::url() const
{
   return _url;
}

//! E.g. "1.0", "1.1"
Version Request::http_version() const
{
   return _version;
}

std::string Request::header(const std::string& name) const
{
   auto it = _header.find(name);

   if (it != _header.end())
      return it->second;

   return "";
}

bool Request::should_keep_alive() const
{
   return _should_keep_alive;
}

bool Request::upgrade() const
{
   return _upgrade;
}

Request& Request::operator=(Request&& Rhs)
{
   _method  = std::move(Rhs._method);
   _url     = std::move(Rhs._url);
   _version = std::move(Rhs._version);
   _header  = std::move(Rhs._header);
   _should_keep_alive  = std::move(Rhs._should_keep_alive);
   _upgrade            = std::move(Rhs._upgrade);
   return *this;   
}

std::ostream& operator<<(std::ostream& o, const Request& r)
{
   static const int w = 18;

   Version v = r._version;

   o << std::left
     << std::boolalpha
     << "Request Record\n";

   o << std::setw(w) << "Method" << " : " << r._method
     << "\n"
     << std::setw(w) << "URL" << " : " << r._url.href()
     << "\n"
     << std::setw(w) << "Version" << " : " << v.major << "." << v.minor
     << "\n";

   for (const auto& item : r._header)
   {
      o << std::setw(w) 
        << item.first
        << " : "
        << item.second
        << "\n";
   }

   o << std::setw(w) << "Should keep alive" << " : " << r._should_keep_alive
     << "\n"
     << std::setw(w) << "Upgrade" << " : " << r._upgrade
     << "\n";
   return o;
}

const orion::logging::LogRecord& operator<<(const orion::logging::LogRecord& rec, const Request& r)
{
   std::ostringstream outs;

   outs << "\n"
        << r;

   const_cast<orion::logging::LogRecord&>(rec).message(outs.str());

   return rec; 
}

} // http
} // net
} // orion

