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
#include <orion/net/http/Message.h>
#include <orion/net/http/StatusCode.h>
#include <orion/net/http/Utils.h>

#include <asio.hpp>

#include <memory>
#include <iosfwd>
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
class API_EXPORT Response : public Message
{
public:
   NO_COPY(Response);

   /// Default constructor
   Response();

   explicit Response(StatusCode code);

   Response(StatusCode code, const Version& version, const Header& header);

   /// Move constructor
   Response(Response&& other) noexcept;

   /// Default destructor
   virtual ~Response();

   /// Move assignment
   Response& operator=(Response&& other) noexcept;

   StatusCode status_code() const;
   void status_code(StatusCode sc);

   std::string status() const;

private:
   void build_header_buffer() override;

private:
   StatusCode _status_code;
};

API_EXPORT std::ostream& operator<<(std::ostream& o, const Response& r);

API_EXPORT log::Record& operator<<(log::Record& rec, const Response& r);

} // namespace http
} // namespace net
} // namespace orion

#endif
