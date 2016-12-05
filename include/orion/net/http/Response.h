//
// Response.h
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_NET_HTTP_RESPONSE_H
#define ORION_NET_HTTP_RESPONSE_H

#include <memory>
#include <streambuf>
#include <string>

#include <orion/Orion-Stddefs.h>
#include <orion/logging/LogRecord.h>
#include <orion/net/http/Utils.h>

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

   Response(StatusCode code);
   Response(StatusCode code, const Version& version);
   Response(Response&& Other);
   virtual ~Response();

   virtual StatusCode status_code() const;

   virtual std::string status() const;

   //! E.g. "1.0", "1.1"
   virtual Version http_version() const;

   virtual std::string header(const std::string& name) const;

   virtual void header(const std::string& name, const std::string& value);

   virtual std::string content() const;

   virtual std::streambuf* rdbuf() const =0;

   Response& operator=(Response&& Rhs);

   friend const logging::LogRecord& operator<<(const logging::LogRecord& rec, const Response& r);

protected:
   StatusCode _status_code;
   Version _version;
   Header _header;

};

const logging::LogRecord& operator<<(const logging::LogRecord& rec, const Response& r);
} // http
} // net
} // orion

#endif

