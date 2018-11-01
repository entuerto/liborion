//
// Message.cpp
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/http/Message.h>

#include <asio.hpp>
#include <fmt/format.h>

#include <ostream>
#include <streambuf>

namespace orion
{
namespace net
{
namespace http
{

Message::Message()
   : _version()
   , _header()
   , _header_streambuf()
   , _body_streambuf()
{
}

Message::Message(const Version& version)
   : _version(version)
   , _header()
   , _header_streambuf()
   , _body_streambuf()
{
}

Message::Message(const Version& version, const Header& header)
   : _version(version)
   , _header(header)
   , _header_streambuf()
   , _body_streambuf()
{
}

Message::Message(Message&& other) noexcept
   : _version(std::move(other._version))
   , _header(std::move(other._header))
   , _header_streambuf(std::move(other._header_streambuf))
   , _body_streambuf(std::move(other._body_streambuf))
{
}

Message::~Message() = default;

Message& Message::operator=(Message&& other) noexcept
{
   _version = std::move(other._version);
   _header  = std::move(other._header);

   _header_streambuf = std::move(other._header_streambuf);
   _body_streambuf   = std::move(other._body_streambuf);
   return *this;
}

Version Message::version() const
{
   return _version;
}

void Message::version(const Version& v)
{
   _version = v;
}

void Message::header(const Header& header)
{
   _header = header;
}

const Header& Message::header() const
{
   return _header;
}

std::string Message::header(Field f) const
{
   return header(to_string(f));
}

std::string Message::header(const std::string& name) const
{
   auto it = _header.find(name);

   if (it != _header.end())
      return it->second;

   return "";
}

void Message::header(Field f, const std::string& value)
{
   _header[to_string(f)] = value;
}

void Message::header(const std::string& name, const std::string& value)
{
   _header[name] = value;
}

std::streambuf* Message::body() const
{
   init_body_buffer();

   return _body_streambuf.get();
}

std::size_t Message::body_size() const
{
   return (_body_streambuf == nullptr) ? 0 : _body_streambuf->size();
}

std::vector<asio::const_buffer> Message::to_buffers()
{
   build_header_buffer();

   if (_body_streambuf == nullptr)
      return {_header_streambuf->data()};

   return {_header_streambuf->data(), _body_streambuf->data()};
}

void Message::init_body_buffer() const
{
   if (_body_streambuf)
      return;

   auto tmp = std::make_unique<asio::streambuf>();
   _body_streambuf.swap(tmp);
}

void Message::init_header_buffer() const
{
   if (_header_streambuf)
      return;
   
   auto tmp = std::make_unique<asio::streambuf>();
   _header_streambuf.swap(tmp);
}

void Message::build_header_buffer()
{
   init_header_buffer();

   if (body_size() != 0)
      header(Field::ContentLength, std::to_string(body_size()));

   std::ostream o(_header_streambuf.get());

   for (const auto& item : _header)
   {
      o << item.first << ": " << item.second << crlf;
   }

   o << crlf;
}

asio::streambuf* Message::header_buffer() const
{
   init_header_buffer();

   return _header_streambuf.get();
}

} // namespace http
} // namespace net
} // namespace orion
