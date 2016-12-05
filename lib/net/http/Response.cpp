//
// Response.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#include <orion/net/http/Response.h>

#include <iomanip>
#include <sstream>

#include <boost/format.hpp>

namespace orion
{
namespace net
{
namespace http
{

Response::Response(StatusCode code) :
   _status_code(code),
   _version({1, 1}),
   _header()
{
   if (_status_code >= StatusCode::BadRequest)
   {
      header("Connection", "close");
      header("X-Content-Type-Options", "nosniff");
   }
   else
      header("Content-Type", "text/plain; charset=utf-8");
}

Response::Response(StatusCode code, const Version& version) :
   _status_code(code),
   _version(version),
   _header()
{
   if (_status_code >= StatusCode::BadRequest)
   {
      header("Connection", "close");
      header("X-Content-Type-Options", "nosniff");
   }
   else
      header("Content-Type", "text/plain; charset=utf-8");
}

Response::Response(Response&& Other) :
   _status_code(std::move(Other._status_code)),
   _version(std::move(Other._version)),
   _header(std::move(Other._header))
{
}

Response::~Response()
{
}
 
StatusCode Response::status_code() const
{
   return _status_code;
}

std::string Response::status() const
{
   return boost::str(boost::format("%d %s") % _status_code % StatusText.at(_status_code));
}

Version Response::http_version() const
{
   return _version;
}

std::string Response::header(const std::string& name) const
{
   auto it = _header.find(name);

   if (it != _header.end())
      return it->second;

   return "";
}

void Response::header(const std::string& name, const std::string& value)
{
   _header.emplace(name, value);
}

Response& Response::operator=(Response&& Rhs)
{
   _status_code = std::move(Rhs._status_code);
   _version     = std::move(Rhs._version);
   _header      = std::move(Rhs._header);
   return *this;
}

std::string Response::content() const
{
   return "null";
}

const logging::LogRecord& operator<<(const logging::LogRecord& rec, const Response& r)
{
   static const int w = 18;

   std::ostringstream o;

   auto v = r._version;

   auto status_msg = boost::format("HTTP/%d.%d %s") % v.major % v.minor % r.status();

   o << std::left
     << std::boolalpha
     << "\nResponse Header\n";

   o << boost::str(status_msg)
     << "\n";

   for (const auto& item : r._header)
   {
      o << std::setw(w) 
        << item.first
        << " : "
        << item.second
        << "\n";
   }

   o << "\n";

   const_cast<logging::LogRecord&>(rec).message(o.str());

   return rec; 
}

} // http
} // net
} // orion

