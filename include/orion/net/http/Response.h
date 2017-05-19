//
// Response.h
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_NET_HTTP_RESPONSE_H
#define ORION_NET_HTTP_RESPONSE_H

#include <orion/Orion-Stddefs.h>
#include <orion/log/Record.h>
#include <orion/net/http/StatusCode.h>
#include <orion/net/http/Utils.h>

#include <memory>
#include <ostream>
#include <streambuf>
#include <string>

namespace orion
{
namespace net
{
namespace http
{
/// Encapsulates HTTP-response information.
///
/// Response class is used to build HTTP responses from a HTTP request..
///
class API_EXPORT Response
{
public:
   NO_COPY(Response);

   Response();
   Response(StatusCode code);
   Response(StatusCode code, const Version& version, const Header& header);
   Response(Response&& rhs);
   virtual ~Response();

   Response& operator=(Response&& rhs);

   virtual StatusCode status_code() const;
   virtual void status_code(StatusCode sc);

   virtual std::string status() const;

   //! E.g. "1.0", "1.1"
   virtual Version http_version() const;
   virtual void http_version(const Version& v);

   virtual std::string header(const std::string& name) const;

   virtual void header(const std::string& name, const std::string& value);
   virtual void header(const Header& header);

   virtual std::streambuf* rdbuf() const;

   friend API_EXPORT std::ostream& operator<<(std::ostream& o, const Response& r);

protected:
   StatusCode _status_code;
   Version _version;
   Header _header;

   std::unique_ptr<std::streambuf> _header_streambuf;
   std::unique_ptr<std::streambuf> _body_streambuf;
};

API_EXPORT std::ostream& operator<<(std::ostream& o, const Response& r);

API_EXPORT const log::Record& operator<<(const log::Record& rec, const Response& r);

} // http
} // net
} // orion

#endif
