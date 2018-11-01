//
// Request.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP_REQUEST_H
#define ORION_NET_HTTP_REQUEST_H

#include <orion/Orion-Stddefs.h>

#include <orion/log/Record.h>
#include <orion/net/Url.h>
#include <orion/net/http/Message.h>
#include <orion/net/http/Utils.h>

#include <iosfwd>
#include <string>

namespace orion
{
namespace net
{
namespace http
{
///
/// Encapsulates HTTP-request information.
///
class API_EXPORT Request : public Message
{
public:
   NO_COPY(Request)

   /// Default constructor
   Request();

   Request(const Method& method, const Url& url);

   Request(const Method& method, const Url& url, const Version& version, const Header& header);

   /// Move constructor
   Request(Request&& other) noexcept;

   /// Default destructor
   virtual ~Request();

   /// Move assignment
   Request& operator=(Request&& other) noexcept;

   /// method specifies the HTTP method (GET, POST, PUT, etc.).
   const Method& method() const;

   /// Set the HTTP method (GET, POST, PUT, etc.).
   void method(const Method& value);

   /// A reference to the URL of this request.
   Url& url();

   /// A const reference to the URL of this request.
   const Url& url() const;

   /// Sets the URL of this request.
   void url(const Url& u);

private:
   void build_header_buffer() override;

private:
   Method _method;
   Url _url;
};

API_EXPORT std::ostream& operator<<(std::ostream& o, const Request& r);

API_EXPORT orion::log::Record& operator<<(orion::log::Record& rec, const Request& r);

} // namespace http
} // namespace net
} // namespace orion

#endif
