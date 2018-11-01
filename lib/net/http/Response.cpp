//
// Response.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/http/Response.h>

#include <asio.hpp>
#include <fmt/format.h>

#include <iomanip>
#include <sstream>

namespace orion
{
namespace net
{
namespace http
{

Response::Response()
   : _status_code(StatusCode::OK)
   , _version({1, 1})
   , _header()
   , _header_streambuf()
   , _body_streambuf()
{
}

Response::Response(StatusCode code)
   : _status_code(code)
   , _version({1, 1})
   , _header()
   , _header_streambuf()
   , _body_streambuf()
{
}

Response::Response(StatusCode code, const Version& version, const Header& header)
   : _status_code(code)
   , _version(version)
   , _header(header)
   , _header_streambuf()
   , _body_streambuf()
{
}

Response::Response(Response&& rhs) noexcept
   : _status_code(std::move(rhs._status_code))
   , _version(std::move(rhs._version))
   , _header(std::move(rhs._header))
   , _header_streambuf(std::move(rhs._header_streambuf))
   , _body_streambuf(std::move(rhs._body_streambuf))
{
}

Response::~Response() = default;

Response& Response::operator=(Response&& rhs) noexcept
{
   _status_code = std::move(rhs._status_code);
   _version     = std::move(rhs._version);
   _header      = std::move(rhs._header);

   _header_streambuf = std::move(rhs._header_streambuf);
   _body_streambuf   = std::move(rhs._body_streambuf);
   return *this;
}

StatusCode Response::status_code() const
{
   return _status_code;
}

void Response::status_code(StatusCode sc)
{
   _status_code = sc;
}

std::string Response::status() const
{
   return fmt::format("{0} {1}", _status_code, status_text().at(_status_code));
}

Version Response::version() const
{
   return _version;
}

void Response::version(const Version& v)
{
   _version = v;
}

std::string Response::header(const std::string& name) const
{
   auto it = _header.find(name);

   if (it != _header.end())
      return it->second;

   return "";
}

void Response::header(Field f, const std::string& value)
{
   _header[to_string(f)] = value;
}

void Response::header(const std::string& name, const std::string& value)
{
   _header[name] = value;
}

void Response::header(const Header& header)
{
   _header = header;
}

std::streambuf* Response::body() const
{
   init_body_buffer();

   return _body_streambuf.get();
}

std::vector<asio::const_buffer> Response::to_buffers()
{
   build_header_buffer();

   if (_body_streambuf == nullptr)
      return {_header_streambuf->data()};

   return {_header_streambuf->data(), _body_streambuf->data()};
}

void Response::init_body_buffer() const
{
   if (_body_streambuf)
      return;

   auto tmp = std::make_unique<asio::streambuf>();
   _body_streambuf.swap(tmp);
}

void Response::init_header_buffer() const
{
   if (_header_streambuf)
      return;
   
   auto tmp = std::make_unique<asio::streambuf>();
   _header_streambuf.swap(tmp);
}

void Response::build_header_buffer()
{
   init_header_buffer();

   auto body_size = (_body_streambuf == nullptr) ? 0 : _body_streambuf->size();

   if (_status_code == StatusCode::OK and body_size == 0)
      _status_code = StatusCode::NoContent;

   if (body_size != 0)
      header(Field::ContentLength, std::to_string(body_size));

   std::ostream o(_header_streambuf.get());

   auto v = version();

   auto status_msg = fmt::format("HTTP/{0}.{1} {2}", v.major, v.minor, status());

   o << status_msg << crlf;

   for (const auto& item : _header)
   {
      o << item.first << ": " << item.second << crlf;
   }

   o << crlf;
}

std::ostream& operator<<(std::ostream& o, const Response& r)
{
   auto v = r._version;

   auto status_msg = fmt::format("HTTP/{0}.{1} {2}", v.major, v.minor, r.status());

   o << std::left << std::boolalpha << "\nResponse Header\n";

   o << status_msg << "\n";

   for (const auto& item : r._header)
   {
      o << item.first << ": " << item.second << "\n";
   }

   o << "\n";

   return o;
}

log::Record& operator<<(log::Record& rec, const Response& r)
{
   std::ostringstream outs;

   outs << r;

   rec.message(outs.str());

   return rec;
}

} // http
} // net
} // orion
