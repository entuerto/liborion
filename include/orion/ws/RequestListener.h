/*
 * RequestListener.h
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

#ifndef ORION_WS_REQUESTLISTENER_H
#define ORION_WS_REQUESTLISTENER_H

#include <orion/Orion-Stddefs.h>
#include <orion/MemoryUtils.h>
#include <orion/ws/Response.h>
#include <orion/ws/Request.h>

namespace orion
{
namespace ws
{
//! Provides a simple, programmatically controlled HTTP protocol listener.
/*!
   Using the RequestListener class, you can create a simple HTTP protocol listener that 
   responds to HTTP requests. The listener is active for the lifetime of the RequestListener
   object and runs within your application.
 */
class API_EXPORT RequestListener
{
public:
   NO_COPY(RequestListener)
   NO_MOVE(RequestListener)

   virtual ~RequestListener();

   std::string uri() const;

   virtual std::unique_ptr<Response> on_process_request(const Request* request);

protected:
   RequestListener(const std::string& uri);

   virtual std::unique_ptr<Response> on_get(const Request* request) =0;

   virtual std::unique_ptr<Response> on_post(const Request* request) =0;

   virtual std::unique_ptr<Response> on_put(const Request* request) =0;

   virtual std::unique_ptr<Response> on_delete(const Request* request) =0;

private:
   std::string _uri;
};

} // ws
} // orion

#endif
