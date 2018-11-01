//
// Response.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP_RESPONSE_H
#define ORION_NET_HTTP_RESPONSE_H

#include <orion/Orion-Stddefs.h>
#include <orion/log/Record.h>
#include <orion/net/http/StatusCode.h>
#include <orion/net/http/Utils.h>

#include <asio.hpp>

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

   /// Default constructor
   Response();

   explicit Response(StatusCode code);

   Response(StatusCode code, const Version& version, const Header& header);

   /// Move constructor
   Response(Response&& rhs) noexcept;

   /// Default destructor
   virtual ~Response();

   /// Move assignment
   Response& operator=(Response&& rhs) noexcept;

   StatusCode status_code() const;
   void status_code(StatusCode sc);

   std::string status() const;

    /// The protocol version for outgoing server response.
   Version version() const;

   /// Sets the protocol version for a response.
   void version(const Version& v);

   /// Gets the value associated with the given key.
   std::string header(const std::string& name) const;

   /// Adds the key, value pair to the header.
   void header(Field f, const std::string& value);

   /// Adds the key, value pair to the header.
   void header(const std::string& name, const std::string& value);

   /// Sets the header of the request.
   void header(const Header& header);

   virtual std::streambuf* body() const;

   std::vector<asio::const_buffer> to_buffers();

   friend API_EXPORT std::ostream& operator<<(std::ostream& o, const Response& r);

private:
   void init_body_buffer() const;
   void init_header_buffer() const;
   void build_header_buffer();

private:
   StatusCode _status_code;
   Version _version;
   Header _header;

   mutable std::unique_ptr<asio::streambuf> _header_streambuf;
   mutable std::unique_ptr<asio::streambuf> _body_streambuf;
};

API_EXPORT log::Record& operator<<(log::Record& rec, const Response& r);

} // namespace http
} // namespace net
} // namespace orion

#endif
