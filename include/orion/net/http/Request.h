//
// Request.h
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_NET_HTTP_REQUEST_H
#define ORION_NET_HTTP_REQUEST_H

#include <ostream>
#include <string>
 
#include <orion/Orion-Stddefs.h>
#include <orion/logging/LogRecord.h>
#include <orion/net/IPAddress.h>
#include <orion/net/Url.h>
#include <orion/net/http/Utils.h>

namespace orion
{
namespace net
{
namespace http
{


//! Encapsulates HTTP-request information.
/*!
 */
class API_EXPORT Request
{
public:
   NO_COPY(Request)

   Request();
   Request(const std::string& method, 
           const Url& url, 
           const Version& version, 
           const Header& header,
                 bool should_keep_alive = false,
                 bool upgrade = false);
   Request(Request&& Other);
   virtual ~Request();

   //! "GET", "POST", etc
   virtual std::string method() const; 

   //! URL-decoded URI
   virtual Url url() const;

   //! E.g. "1.0", "1.1"
   virtual Version http_version() const;   
   
   virtual std::string header(const std::string& name) const;

   virtual bool should_keep_alive() const;

   virtual bool upgrade() const;

   virtual std::string content() const =0;

   virtual std::size_t content_lenght() const;

   Request& operator=(Request&& Rhs);
  
   friend std::ostream& operator<<(std::ostream& o, const Request& r);

protected:
   std::string _method;
   Url _url;
   Version _version;
   Header _header;
   bool _should_keep_alive;
   bool _upgrade;
   std::size_t _content_lenght;
};

std::ostream& operator<<(std::ostream& o, const Request& r);

const orion::logging::LogRecord& operator<<(const orion::logging::LogRecord& rec, const Request& r);

} // http
} // net
} // orion

#endif

