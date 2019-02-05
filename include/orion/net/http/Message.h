//
// Message.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP_MESSAGE_H
#define ORION_NET_HTTP_MESSAGE_H

#include <orion/Common.h>

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
/// The base http message class to create requests and responses.
///
class API_EXPORT Message
{
public:
   NO_COPY(Message)

   /// Default constructor
   Message();

   explicit Message(const Version& version);

   Message(const Version& version, const Header& header);

   /// Move constructor
   Message(Message&& other) noexcept;

   /// Default destructor
   virtual ~Message();

   /// Move assignment
   Message& operator=(Message&& other) noexcept;

   /// The protocol version for incoming server requests.
   Version version() const;

   /// Sets the protocol version for a request.
   void version(const Version& v);

   /// Sets the header of the request.
   void header(const Header& header);

   /// Get the header 
   const Header& header() const;

   /// Gets the value associated with the given key.
   std::string header(Field f) const;

   /// Gets the value associated with the given key.
   std::string header(const std::string& name) const;

   /// Adds the key, value pair to the header.
   void header(Field f, const std::string& value);

   /// Adds the key, value pair to the header.
   void header(const std::string& name, const std::string& value);

   /// Body of the message
   std::streambuf* body() const;

   std::size_t body_size() const;

   std::vector<asio::const_buffer> to_buffers();

protected:
   void init_body_buffer() const;
   void init_header_buffer() const;

   virtual void build_header_buffer();

   asio::streambuf* header_buffer() const;

private:
   Version _version;
   Header _header;

   mutable std::unique_ptr<asio::streambuf> _header_streambuf;
   mutable std::unique_ptr<asio::streambuf> _body_streambuf;
};

} // namespace http
} // namespace net
} // namespace orion

#endif