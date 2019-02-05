//
// Parser.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP_PARSER_H
#define ORION_NET_HTTP_PARSER_H

#include <orion/Common.h>

#include <orion/net/http/Utils.h>

#include <http-parser/http_parser.h>

#include <system_error>

namespace orion
{
namespace net
{
namespace http
{
// Forward declares
class Request;
class Response;

///
class Parser
{
public:
   NO_COPY(Parser)

   Parser();
   ~Parser();

   bool headers_complete() const;
   bool message_complete() const;

   std::error_code parse(Request& request, asio::const_buffer buffer);
   std::error_code parse(Response& response, asio::const_buffer buffer);

   int on_message_begin();
   int on_url(const char* at, size_t length);
   int on_status(const char* at, size_t length);
   int on_header_field(const char* at, size_t length);
   int on_header_value(const char* at, size_t length);
   int on_headers_complete();
   int on_body(const char* at, size_t length);
   int on_message_complete();
   int on_chunk_header();
   int on_chunk_complete();

private:
   http_parser_settings _settings;
   http_parser _parser;

   std::string _url;
   std::string _status;
   std::string _cur_field;
   Header _header;

   std::streambuf* _streambuf;

   bool _headers_complete;
   bool _message_complete;
};

} // http
} // net
} // orion
#endif // ORION_NET_HTTP_PARSER_H