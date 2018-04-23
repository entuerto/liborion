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
#include <orion/net/http/Utils.h>

#include <ostream>
#include <streambuf>
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
class API_EXPORT Request
{
public:
   NO_COPY(Request)

   /// Default constructor
   Request();

   Request(const Method& method, const Url& url, const Version& version, const Header& header);

   /// Move constructor
   Request(Request&& rhs) noexcept;

   /// Default destructor
   virtual ~Request();

   /// Move assignment
   Request& operator=(Request&& rhs) noexcept;

   /// method specifies the HTTP method (GET, POST, PUT, etc.).
   Method method() const;

   /// Set the HTTP method (GET, POST, PUT, etc.).
   void method(Method value);

   //! URL-decoded URI
   Url url() const;
   void url(const Url& u);

   /// The protocol version for incoming server requests.
   Version version() const;

   /// Sets the protocol version for a request.
   void version(const Version& v);

   /// Gets the value associated with the given key.
   std::string header(const std::string& name) const;

   /// Adds the key, value pair to the header.
   void header(Field f, const std::string& value);

    /// Adds the key, value pair to the header.
   void header(const std::string& name, const std::string& value);

   /// Sets the header of the request.
   void header(const Header& header);

   /// Enables should keep alive. By default, keep-alive is always enabled.
   bool should_keep_alive() const;

   /// Returns should keep alive value.
   void should_keep_alive(bool value);

   bool upgrade() const;
   void upgrade(bool value);

   virtual std::streambuf* body_rdbuf() const;

   std::vector<asio::const_buffer> to_buffers();

   friend API_EXPORT std::ostream& operator<<(std::ostream& o, const Request& r);

private:
   void build_header_buffer();

private:
   Method _method;
   Url _url;
   Version _version;
   Header _header;
   bool _should_keep_alive;
   bool _upgrade;

   std::unique_ptr<asio::streambuf> _header_streambuf;
   std::unique_ptr<asio::streambuf> _body_streambuf;
};

API_EXPORT orion::log::Record& operator<<(orion::log::Record& rec, const Request& r);

} // namespace http
} // namespace net
} // namespace orion

#endif
