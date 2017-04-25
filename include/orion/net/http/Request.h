//
// Request.h
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_NET_HTTP_REQUEST_H
#define ORION_NET_HTTP_REQUEST_H

#include <ostream>
#include <streambuf>
#include <string>
 
#include <orion/Orion-Stddefs.h>
#include <orion/log/Record.h>
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
           const Header& header);
   Request(Request&& Other);
   virtual ~Request();

   Request& operator=(Request&& Rhs);

   //! "GET", "POST", etc
   virtual std::string method() const; 
   virtual void method(const std::string& m); 

   //! URL-decoded URI
   virtual Url url() const;
   virtual void url(const Url& u);

   //! E.g. "1.0", "1.1"
   virtual Version http_version() const; 
   virtual void http_version(const Version& v);  
   
   virtual std::string header(const std::string& name) const;

   virtual void header(const std::string& name, const std::string& value);
   virtual void header(const Header& header);

   virtual bool should_keep_alive() const;
   virtual void should_keep_alive(bool value);

   virtual bool upgrade() const;
   virtual void upgrade(bool value);

   virtual std::streambuf* rdbuf() const;
  
   friend API_EXPORT std::ostream& operator<<(std::ostream& o, const Request& r);

protected:
   std::string _method;
   Url _url;
   Version _version;
   Header _header;
   bool _should_keep_alive;
   bool _upgrade;

   std::unique_ptr<std::streambuf> _header_streambuf;
   std::unique_ptr<std::streambuf> _body_streambuf;
};

API_EXPORT std::ostream& operator<<(std::ostream& o, const Request& r);

API_EXPORT const orion::log::Record& operator<<(const orion::log::Record& rec, const Request& r);

} // http
} // net
} // orion

#endif

