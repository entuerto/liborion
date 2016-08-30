/*
 * Response.h
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

#ifndef ORION_WS_RESPONSE_H
#define ORION_WS_RESPONSE_H

#include <string>

#include <orion/Orion-Stddefs.h>
#include <orion/MemoryUtils.h>

namespace orion
{
namespace ws
{

//! Encapsulates HTTP-response information.
/*!
   Response class is used to build HTTP responses from HTTP request..
 */
class API_EXPORT Response
{
public:
   Response(uint32_t code);
   Response(uint32_t code, const std::string& content, const std::string& content_type = "text/plain");
   virtual ~Response();

   virtual uint32_t code() const;

   virtual std::string mimetype() const;
   virtual void set_mimetype(const std::string& value);

   virtual std::string content() const;
   virtual void set_content(const std::string& value);

   virtual std::string to_string() const;

   static std::unique_ptr<Response> create();
   static std::unique_ptr<Response> create(const std::string& content, const std::string& content_type = "text/plain");

   static std::unique_ptr<Response> create_400();
   static std::unique_ptr<Response> create_401();
   static std::unique_ptr<Response> create_403();
   static std::unique_ptr<Response> create_404();
   static std::unique_ptr<Response> create_405();
   static std::unique_ptr<Response> create_406();
   static std::unique_ptr<Response> create_408();
   static std::unique_ptr<Response> create_409();

protected:
   uint32_t _code;
   std::string _mimetype;
   std::string _content;
};

} // ws
} // orion

#endif

