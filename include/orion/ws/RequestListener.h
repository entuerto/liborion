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
class RequestListener
{
public:
   DECLARE_POINTERS(RequestListener)

   virtual ~RequestListener();

   virtual Response::SharedPtr on_process_request(Request::SharedPtr request);

protected:
   RequestListener();

   virtual Response::SharedPtr on_get(Request::SharedPtr request) =0;

   virtual Response::SharedPtr on_post(Request::SharedPtr request) =0;

   virtual Response::SharedPtr on_put(Request::SharedPtr request) =0;

   virtual Response::SharedPtr on_delete(Request::SharedPtr request) =0;
};

} // ws
} // orion

#endif

