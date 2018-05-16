//
// Protocol.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_RPC_PROTOCOL_H
#define ORION_NET_RPC_PROTOCOL_H

#include <orion/Orion-Stddefs.h>

#include <orion/Utils.h>
#include <orion/net/rpc/Error.h>
#include <orion/net/rpc/Method.h>
#include <orion/net/rpc/Traits.h>

#include <asio.hpp>
#include <fmt/format.h>

#include <atomic>
#include <iostream>
#include <mutex>
#include <system_error>

namespace orion
{
namespace net
{
namespace rpc
{
//
// RPC Message = {Tag Version Content{Request|Reponse}}
// Tag         = ContenType-RPC
// Version     = A String specifying the version of the protocol.
// ContenType  = [RAW|CHAR|JSON]
// Request     = ({Id Method Params}, ...)
// Response    = ({Id [Result|Error]}, ...)
// 
// Result      = {}
// Error       = {Code Message Data}
// 
// Id          = An identifier established by the Client that MUST contain a String or Number.
// Method      = A String containing the name of the method to be invoked. It is REQUIRED.
// Params      = An encoded value that holds the parameter values to be used during the invocation 
//               of the method. MAY be omitted
// 
// Code        = A Number that indicates the error type that occurred. This MUST be an integer.
// Message     = A String providing a short description of the error.
// Data        = Extra information.
//

//--------------------------------------------------------------------------------------------------
// ContentType
//--------------------------------------------------------------------------------------------------

enum class ContentType : uint8_t
{
   Raw,
   Char,
   Json
};

std::istream& operator>>(std::istream& s, ContentType& ct);

std::ostream& operator<<(std::ostream& s, ContentType ct);

//--------------------------------------------------------------------------------------------------

inline void copy_streambuf(asio::streambuf& target, const asio::streambuf& source)
{
   std::size_t bytes_copied =
      asio::buffer_copy(target.prepare(source.size()), // target's output sequence
                        source.data());                // source's input sequence
   // Explicitly move target's output sequence to its input sequence.
   target.commit(bytes_copied);
}

//--------------------------------------------------------------------------------------------------
// Content
//--------------------------------------------------------------------------------------------------

struct Content
{
   ContentType type;
   asio::streambuf data;

   Content() = default;

   Content(const Content& other)
      : type(other.type)
      , data() 
   {
      copy_streambuf(data, other.data);
   }

   Content& operator=(const Content& other)
   {
      if (this == &other)
         return *this;

      type = other.type;

      copy_streambuf(data, other.data);

      return *this;
   }

   Content(Content&&) = default;
   Content& operator=(Content&&) = default;
};

//--------------------------------------------------------------------------------------------------
// Message
//--------------------------------------------------------------------------------------------------

struct Message
{
   std::string tag;
   Version version;
   Content content;
};

std::ostream& operator<<(std::ostream& s, const Message& msg);

//--------------------------------------------------------------------------------------------------
// Request
//--------------------------------------------------------------------------------------------------

template<typename IdT>
struct Request
{
   IdT id;
   std::string method;
   asio::streambuf params;

   Request() = default;
   ~Request() = default;

   explicit Request(IdT identifier);

   // Copy
   Request(const Request& other);
   Request& operator=(const Request& other);

   // Move
   Request(Request&&) = default;
   Request& operator=(Request&&) = default;
};

//--------------------------------------------------------------------------------------------------
// Response
//--------------------------------------------------------------------------------------------------

template<typename IdT>
struct Response 
{
   IdT id;
   asio::streambuf result;

   Response() = default;
   ~Response() = default;

   explicit Response(IdT identifier);

   // Copy
   Response(const Response& other);
   Response& operator=(const Response& other);

   // Move
   Response(Response&&) = default;
   Response& operator=(Response&&) = default;
};

//--------------------------------------------------------------------------------------------------
// Codecs
//--------------------------------------------------------------------------------------------------

struct StringCodec
{
   static rpc::Error decode(asio::streambuf& buf, int& value)
   {
      std::istream input{&buf};

      input >> value;

      return rpc::Error{};
   }

   static rpc::Error decode(asio::streambuf& in_buf, Message& msg)
   {
      std::istream input{&in_buf};

      input >> msg.tag;
      input >> msg.version;

      input >> std::ws;

      std::ostream out{&msg.content.data};
      out << input.rdbuf();

      log::debug(msg);

      return rpc::Error{};
   }

   static rpc::Error encode(asio::streambuf& buf, int value)
   {
      std::ostream output{&buf};

      output << value;

      return rpc::Error{};
   }

   template<typename T>
   static rpc::Error encode(asio::streambuf& buf, const Request<T>& req)
   {
      static const std::string text{"Request Id {} Method {} Params "};

      std::ostream output{&buf};

      output << fmt::format(text, req.id, req.method)
             << const_cast<asio::streambuf*>(&req.params)
             << " ";

      return rpc::Error{};
   }

   template<typename T>
   static rpc::Error encode(asio::streambuf& buf, const Response<T>& res)
   {
      static const std::string text{"Response Id {} Result "};

      std::ostream output{&buf};

      output << fmt::format(text, res.id)
             << const_cast<asio::streambuf*>(&res.result)
             << " ";

      return rpc::Error{};
   }

   static rpc::Error encode(asio::streambuf& out_buf, const Message& msg)
   {
      std::ostream output{&out_buf};

      output << msg.tag
             << " "
             << msg.version
             << " "
             << const_cast<asio::streambuf*>(&msg.content.data);

      return rpc::Error{};
   }

   static rpc::Error emit_separator(asio::streambuf& buf)
   {
      std::ostream output{&buf};

      output << " ";

      return rpc::Error{};
   }

   static rpc::Error consume_separator(asio::streambuf& buf)
   {
      std::istream input{&buf};

      input >> std::ws;

      return rpc::Error{};
   }
};

//--------------------------------------------------------------------------------------------------
// Protocols
//--------------------------------------------------------------------------------------------------

template<typename Codec>
struct Tcp
{
   using CodecType = Codec;
   using IdType    = int32_t;
   
   using Request  = Request<IdType>;
   using Requests = std::vector<Request>;
   using Response = Response<IdType>;

   static Requests make_requests(const Message& msg)
   {
      Requests reqs;

      // TODO: Validate Tag and Version

      auto& content = msg.content; 

      std::istream input{const_cast<asio::streambuf*>(&content.data)};

      std::string field;

      input >> field; // Request

      Request r;

      input >> field; // Id
      input >> r.id;

      input >> field; // Method
      input >> r.method;

      input >> field; // Params

      copy_streambuf(r.params, content.data);

      reqs.emplace_back(std::move(r));

      return reqs;
   }

   static Response make_response(const Message& msg)
   {
      auto& content = msg.content; 

      // TODO: Validate Tag and Version

      std::istream input{const_cast<asio::streambuf*>(&content.data)};

      std::string field;

      input >> field; // Response

      Response r;

      input >> field; // Id
      input >> r.id;

      input >> field; // Result

      copy_streambuf(r.result, content.data);

      return r;
   }

   static Message make_message(const Requests& reqs)
   {
      Message msg;

      msg.tag     = "CHAR-RPC";
      msg.version = current_version;

      auto& content = msg.content;

      for (const auto& req : reqs)
      {
         CodecType::encode(content.data, req);
      }

      return msg;
   }

   static Message make_message(const Response& res)
   {
      Message msg;

      msg.tag     = "CHAR-RPC";
      msg.version = current_version;

      auto& content = msg.content;

      CodecType::encode(content.data, res);

      return msg;
   }

   static IdType next_id()
   {
      return ++req_id;
   }

   static Request make_request()
   {
      std::lock_guard<std::mutex> lock(request_lock);

      return Request{next_id()};
   }

   static Response make_response(const Request& req)
   {
      std::lock_guard<std::mutex> lock(response_lock);

      return Response{req.id};
   }

   static std::mutex request_lock;
   static std::mutex response_lock;

   static std::atomic<IdType> req_id;

   static Version current_version;
};

template<typename Codec>
std::mutex Tcp<Codec>::request_lock;

template<typename Codec>
std::mutex Tcp<Codec>::response_lock;

template<typename Codec>
std::atomic<typename Tcp<Codec>::IdType> Tcp<Codec>::req_id{0};

template<typename Codec>
Version Tcp<Codec>::current_version{1, 0, 0};

using TcpString = Tcp<StringCodec>;

//--------------------------------------------------------------------------------------------------
// Serializers
//--------------------------------------------------------------------------------------------------

template<>
class Serializer<TcpString>
{
public:
   using CodecType = typename TcpString::CodecType;
   using RequestType = typename TcpString::Request;
   using RequestsType = typename TcpString::Requests;
   using ResponseType = typename TcpString::Response;

   static rpc::Error serialize(asio::streambuf& buf, int value)
   {
      return CodecType::encode(buf, value);
   }

   static rpc::Error serialize(asio::streambuf& out_buf, RequestsType& requests)
   {
      Message msg = TcpString::make_message(requests);

      log::debug(msg);

      return CodecType::encode(out_buf, msg);
   }

   static rpc::Error serialize(asio::streambuf& out_buf, ResponseType& response)
   {
      Message msg = TcpString::make_message(response);

      log::debug(msg);

      return CodecType::encode(out_buf, msg);
   }

   static rpc::Error deserialize(asio::streambuf& buf, int& value)
   {
      return CodecType::decode(buf, value);
   }

   static rpc::Error deserialize(asio::streambuf& in_buf, RequestsType& requests)
   {
      Message msg;

      auto error = CodecType::decode(in_buf, msg);

      requests = TcpString::make_requests(msg);

      return error;
   }

   static rpc::Error deserialize(asio::streambuf& in_buf, ResponseType& response)
   {
      Message msg;

      auto error = CodecType::decode(in_buf, msg);

      response = TcpString::make_response(msg);

      return error;
   }
};

} // rpc
} // net
} // orion

#include <orion/net/rpc/impl/Protocol.ipp>

#endif // ORION_NET_RPC_PROTOCOL_H