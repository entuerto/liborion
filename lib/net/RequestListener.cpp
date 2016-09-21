/*
 * RequestListener.cpp
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

#include <orion/net/RequestListener.h>

namespace orion
{
namespace net
{

RequestListener::RequestListener(const std::string& uri) :
   _uri(uri)
{
}

RequestListener::~RequestListener() 
{
}

std::string RequestListener::uri() const
{
   return _uri;
}

std::unique_ptr<Response> RequestListener::on_process_request(const Request* request)
{
   if (request->method() == "GET")
      return on_get(request);
   else if (request->method() == "POST")
      return on_post(request);
   else if (request->method() == "PUT")
      return on_put(request);
   else if (request->method() == "DELETE")
      return on_delete(request);

   return Response::create_400();
}

} // net
} // orion