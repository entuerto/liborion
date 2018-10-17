//
// Parser.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <net/http/Parser.h>

#include <orion/Log.h>

#include <iomanip>
#include <iostream>

using namespace orion::log;

namespace orion
{
namespace net
{
namespace http
{
//---------------------------------------------------------------------------------------
int cb_message_begin(http_parser* p)
{
   return static_cast<Parser*>(p->data)->on_message_begin();
}

int cb_url(http_parser* p, const char* at, size_t length)
{
   return static_cast<Parser*>(p->data)->on_url(at, length);
}

int cb_status(http_parser* p, const char* at, size_t length)
{
   return static_cast<Parser*>(p->data)->on_status(at, length);
}

int cb_header_field(http_parser* p, const char* at, size_t length)
{
   return static_cast<Parser*>(p->data)->on_header_field(at, length);
}

int cb_header_value(http_parser* p, const char* at, size_t length)
{
   return static_cast<Parser*>(p->data)->on_header_value(at, length);
}

int cb_headers_complete(http_parser* p)
{
   return static_cast<Parser*>(p->data)->on_headers_complete();
}

int cb_body(http_parser* p, const char* at, size_t length)
{
   return static_cast<Parser*>(p->data)->on_body(at, length);
}

int cb_message_complete(http_parser* p)
{
   return static_cast<Parser*>(p->data)->on_message_complete();
}

int cb_chunk_header(http_parser* p)
{
   return static_cast<Parser*>(p->data)->on_chunk_header();
}

int cb_chunk_complete(http_parser* p)
{
   return static_cast<Parser*>(p->data)->on_chunk_complete();
}
//---------------------------------------------------------------------------------------

Parser::Parser()
   : _settings()
   , _parser()
   , _url()
   , _status()
   , _cur_field()
   , _header()
   , _streambuf(nullptr)
   , _headers_complete(false)
   , _message_complete(false)
{
   http_parser_settings_init(&_settings);

   _settings = {cb_message_begin,
                cb_url,
                cb_status,
                cb_header_field,
                cb_header_value,
                cb_headers_complete,
                cb_body,
                cb_message_complete,
                cb_chunk_header,
                cb_chunk_complete};
}

Parser::~Parser()
{
}

bool Parser::headers_complete() const
{
   return _headers_complete;
}

bool Parser::message_complete() const
{
   return _message_complete;
}

std::error_code Parser::parse(Request& request, asio::const_buffer buffer)
{
   http_parser_init(&_parser, HTTP_REQUEST);
   _parser.data = this;

   _streambuf = request.body_rdbuf();

   auto data = static_cast<const char*>(buffer.data());
   auto length = buffer.size();

   int nparsed = http_parser_execute(&_parser, &_settings, data, length);

   if (_parser.upgrade)
   {
      /* handle new protocol */
   }
   else if (nparsed != length)
   {
      /* Handle error. Usually just close the connection. */
   }

   if (_headers_complete)
   {
      request.method(from_string<Method>(http_method_str(static_cast<http_method>(_parser.method))));
      request.version(Version{_parser.http_major, _parser.http_minor});
      request.url(Url{_url});
      request.header(_header);
      request.should_keep_alive(http_should_keep_alive(&_parser) == 1);
      request.upgrade(_parser.upgrade == 1);
   }

   return std::error_code();
}

std::error_code Parser::parse(Response& response, asio::const_buffer buffer)
{
   http_parser_init(&_parser, HTTP_RESPONSE);
   _parser.data = this;

   _streambuf = response.body_rdbuf();

   auto data = static_cast<const char*>(buffer.data());
   auto length = buffer.size();

   std::size_t nparsed = http_parser_execute(&_parser, &_settings, data, length);

   if (_parser.upgrade)
   {
      /* handle new protocol */
   }
   else if (nparsed != length)
   {
      /* Handle error. Usually just close the connection. */
   }

   if (_headers_complete)
   {
      response.status_code(static_cast<StatusCode>(_parser.status_code));
      response.version(Version{_parser.http_major, _parser.http_minor});
      response.header(_header);
   }

   return std::error_code();
}

int Parser::on_message_begin()
{
   LOG_FUNCTION(Debug2, "Parser::on_message_begin()")

   _url.clear();
   _status.clear();
   _cur_field.clear();
   _header.clear();
   _headers_complete = false;
   _message_complete = false;
   return 0;
}

int Parser::on_url(const char* at, size_t length)
{
   LOG_FUNCTION(Debug2, "Parser::on_url()")

   _url.append(at, length);
   return 0;
}

int Parser::on_status(const char* at, size_t length)
{
   LOG_FUNCTION(Debug2, "Parser::on_status()")

   _status.append(at, length);
   return 0;
}

int Parser::on_header_field(const char* at, size_t length)
{
   LOG_FUNCTION(Debug2, "Parser::on_header_field()")

   _cur_field.append(at, length);
   return 0;
}

int Parser::on_header_value(const char* at, size_t length)
{
   LOG_FUNCTION(Debug2, "Parser::on_header_value()")

   if (not _cur_field.empty())
      _header.emplace(std::make_pair(_cur_field, std::string(at, length)));

   _cur_field.clear();
   return 0;
}

int Parser::on_headers_complete()
{
   LOG_FUNCTION(Debug2, "Parser::on_headers_complete()")

   _headers_complete = true;

   return 0;
}

int Parser::on_body(const char* at, size_t length)
{
   LOG_FUNCTION(Debug2, "Parser::on_body()")

   std::ostream o(_streambuf);

   o << std::string(at, length);

   return 0;
}

int Parser::on_message_complete()
{
   LOG_FUNCTION(Debug2, "Parser::on_message_complete()")

   _message_complete = true;
   return 0;
}

int Parser::on_chunk_header()
{
   LOG_FUNCTION(Debug2, "Parser::on_chunk_header()")
   return 0;
}

int Parser::on_chunk_complete()
{
   LOG_FUNCTION(Debug2, "Parser::on_chunk_complete()")
   return 0;
}

} // http
} // net
} // orion
