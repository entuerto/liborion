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

   virtual ~Response();

   Response& operator=(Response&& rhs) noexcept;

   virtual StatusCode status_code() const;
   virtual void status_code(StatusCode sc);

   virtual std::string status() const;

   //! E.g. "1.0", "1.1"
   virtual Version version() const;
   virtual void version(const Version& v);

   virtual std::string header(const std::string& name) const;

   virtual void header(const std::string& name, const std::string& value);
   virtual void header(const Header& header);

   virtual std::streambuf* header_rdbuf();
   virtual std::streambuf* body_rdbuf();

   std::vector<asio::const_buffer> to_buffers();

   friend API_EXPORT std::ostream& operator<<(std::ostream& o, const Response& r);

private:
   void build_header_buffer();

private:
   StatusCode _status_code;
   Version _version;
   Header _header;

   asio::streambuf _header_streambuf;
   asio::streambuf _body_streambuf;
};

API_EXPORT const log::Record& operator<<(const log::Record& rec, const Response& r);

} // namespace http
} // namespace net
} // namespace orion

#endif
