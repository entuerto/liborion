/*
 * Request.h
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

#ifndef ORION_WS_REQUEST_H
#define ORION_WS_REQUEST_H

#include <map>
#include <string>
#include <orion/MemoryUtils.h>
#include <orion/ws/InetAddress.h>

namespace orion
{
namespace ws
{

typedef std::map<std::string, std::string> HeaderMap;
//! Encapsulates HTTP-request information.
/*!
 */
class Request
{
public:
   DECLARE_POINTERS(Request)

   Request();
   virtual ~Request();

   //! "GET", "POST", etc
   virtual std::string method() const =0; 

   //! URL-decoded URI
   virtual std::string uri() const =0;

   //! E.g. "1.0", "1.1"
   virtual std::string http_version() const =0;   
   
   //! URL part after '?', not including '?', or NULL
   virtual std::string query_string() const =0;   

   //! Authenticated user, or NULL if no auth used
   virtual std::string remote_user() const =0;    
   
   //! Client's IP and port address
   virtual InetAddress::SharedPtr remote_address() const;             
   
   //! Host  IP and port address
   virtual InetAddress::SharedPtr host_address() const;            

   virtual bool is_authenticated()  const =0;

   virtual bool is_secure_connection() const =0;

   virtual std::string header(const std::string& name) const;

   virtual std::string content() const;
  

protected:
   InetAddress::SharedPtr _remote_address;
   InetAddress::SharedPtr _host_address;

   HeaderMap   _http_header;
   std::string _data;   

};

} // ws
} // orion

#endif

