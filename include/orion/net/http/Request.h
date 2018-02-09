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

   Request();
   Request(const Method& method, const Url& url, const Version& version, const Header& header);
   Request(Request&& rhs) noexcept;
   virtual ~Request();

   Request& operator=(Request&& rhs) noexcept;

   //! "GET", "POST", etc
   virtual Method method() const;
   virtual void method(const Method& value);

   //! URL-decoded URI
   virtual Url url() const;
   virtual void url(const Url& u);

   //! E.g. "1.0", "1.1"
   virtual Version version() const;
   virtual void version(const Version& v);

   virtual std::string header(const std::string& name) const;

   virtual void header(const std::string& name, const std::string& value);
   virtual void header(const Header& header);

   virtual bool should_keep_alive() const;
   virtual void should_keep_alive(bool value);

   virtual bool upgrade() const;
   virtual void upgrade(bool value);

   virtual std::streambuf* header_rdbuf() const;
   virtual std::streambuf* body_rdbuf() const;

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

   std::unique_ptr<std::streambuf> _header_streambuf;
   std::unique_ptr<std::streambuf> _body_streambuf;
};

API_EXPORT const orion::log::Record& operator<<(const orion::log::Record& rec, const Request& r);

} // namespace http
} // namespace net
} // namespace orion

#endif
