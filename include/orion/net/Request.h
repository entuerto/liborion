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

#ifndef ORION_NET_REQUEST_H
#define ORION_NET_REQUEST_H

#include <map>
#include <string>
 
#include <orion/Orion-Stddefs.h>
#include <orion/MemoryUtils.h>
#include <orion/net/IPAddress.h>

namespace orion
{
namespace net
{

typedef std::map<std::string, std::string> Header;
//! Encapsulates HTTP-request information.
/*!
 */
class API_EXPORT Request
{
public:
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
   virtual const IPAddress* remote_address() const;             
   
   //! Host  IP and port address
   virtual const IPAddress* host_address() const;            

   virtual bool is_authenticated()  const =0;

   virtual bool is_secure_connection() const =0;

   virtual std::string header(const std::string& name) const;

   virtual std::string content() const;
  

protected:
   std::unique_ptr<IPAddress> _remote_address;
   std::unique_ptr<IPAddress> _host_address;

   Header _header;
   std::string _data;   

};

} // net
} // orion

#endif

