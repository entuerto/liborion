/*
 * Response.cpp
 *
 * Copyright 2013 tomas <tomasp@videotron.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include <orion/net/Response.h>

#include <sstream>
#include <orion/StringUtils.h>

namespace orion
{
namespace net
{

static std::string status_code_as_string(uint32_t code)
{
   switch (code)
   {
      case 100: return "Continue";
      case 101: return "Switching Protocols";
      case 200: return "OK";
      case 201: return "Created";
      case 202: return "Accepted";
      case 203: return "Non-Authoritative Information";
      case 204: return "No Content";
      case 205: return "Reset Content";
      case 206: return "Partial Content";
      case 300: return "Multiple Choices";
      case 301: return "Moved Permanently";
      case 302: return "Found";
      case 303: return "See Other";
      case 304: return "Not Modified";
      case 305: return "Use Proxy";
      case 307: return "Temporary Redirect";
      case 400: return "Bad Request";
      case 401: return "Unauthorized";
      case 402: return "Payment Required";
      case 403: return "Forbidden";
      case 404: return "Not Found";
      case 405: return "Method Not Allowed";
      case 406: return "Not Acceptable";
      case 407: return "Proxy Authentication Required";
      case 408: return "Request Time-out";
      case 409: return "Conflict";
      case 410: return "Gone";
      case 411: return "Length Required";
      case 412: return "Precondition Failed";
      case 413: return "Request Entity Too Large";
      case 414: return "Request-URI Too Large";
      case 415: return "Unsupported Media Type";
      case 416: return "Requested range not satisfiable";
      case 417: return "Expectation Failed";
      case 500: return "Internal Server Error";
      case 501: return "Not Implemented";
      case 502: return "Bad Gateway";
      case 503: return "Service Unavailable";
      case 504: return "Gateway Time-out";
      case 505: return "HTTP Version not supported";

      default: return "Internal Server Error";
   }
}

Response::Response(uint32_t code) :
   _code(code),
   _mimetype("text/plain"),
   _content()
{
}

Response::Response(uint32_t code, const std::string& content, const std::string& content_type) :
   _code(code),
   _mimetype(content_type),
   _content(content)
{
}

Response::~Response()
{
}
 
uint32_t Response::code() const
{
   return _code;
}

std::string Response::mimetype() const
{
   return _mimetype;
}

void Response::set_mimetype(const std::string& value)
{
   _mimetype = value;
}

std::string Response::content() const
{
   return _content;
}

void Response::set_content(const std::string& value) 
{
   _content = value;
}

std::string Response::to_string() const
{
   std::ostringstream stream;

   stream << "HTTP/1.1 " << _code << " "
          << status_code_as_string(_code)
          << "\r\n"
          << "Content-Type: " << _mimetype 
          << "\r\n"
          << "Content-Length: " << _content.size() 
          << "\r\n"
          << "\r\n"
          << _content;

   return stream.str();
}

std::unique_ptr<Response> Response::create() 
{
   return std::make_unique<Response>(200);
}

std::unique_ptr<Response> Response::create(const std::string& content, const std::string& content_type) 
{
   return std::make_unique<Response>(200, content, content_type);
}


std::unique_ptr<Response> Response::create_400() 
{
   return std::make_unique<Response>(400, "The browser (or proxy) sent a request that this server could not understand.");
}

std::unique_ptr<Response> Response::create_401() 
{
   return std::make_unique<Response>(401, "The server could not verify that you are authorized to access the URL requested.");
}

std::unique_ptr<Response> Response::create_403() 
{
   return std::make_unique<Response>(403, "You don't have the permission to access the requested resource.");
}

std::unique_ptr<Response> Response::create_404() 
{
   return std::make_unique<Response>(404, "The requested URL was not found on the server.");
}

std::unique_ptr<Response> Response::create_405() 
{
   return std::make_unique<Response>(405, "The method is not allowed for the requested URL.");
}

std::unique_ptr<Response> Response::create_406() 
{
   return std::make_unique<Response>(406, "The resource identified by the request is only capable of "
                                          "generating response entities which have content characteristics "
                                          "not acceptable according to the accept headers sent in the "
                                          "request.");
}

std::unique_ptr<Response> Response::create_408() 
{
   return std::make_unique<Response>(408, "The server closed the network connection because the browser "
                                          "didn't finish the request within the specified time.");
}

std::unique_ptr<Response> Response::create_409() 
{
   return std::make_unique<Response>(409, "A conflict happened while processing the request.  The resource "
                                          "might have been modified while the request was being processed.");
}

} // net
} // orion

