//
// Parser.h
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#ifndef ORION_NET_HTTP_PARSER_H
#define ORION_NET_HTTP_PARSER_H

#include <system_error>

#include <orion/Orion-Stddefs.h>
#include <orion/net/http/Utils.h>

#include <net/http/AsioRequest.h>
#include <net/http/AsioResponse.h>

#include <http-parser/http_parser.h>

namespace orion
{
namespace net
{
namespace http
{
///  
class Parser  
{
public:
   NO_COPY(Parser)

   Parser();
   ~Parser();

   bool headers_complete() const;
   bool message_complete() const;

   std::error_code parse(AsioRequest& request, const char* data, std::size_t length);
   std::error_code parse(AsioResponse& response, const char* data, std::size_t length);

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