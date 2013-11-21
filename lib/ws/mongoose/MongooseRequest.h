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

#ifndef MONGOOSEREQUEST_H
#define MONGOOSEREQUEST_H

#include <string>
#include <orion/MemoryUtils.h>
#include <orion/ws/InetAddress.h>
#include <orion/ws/Request.h>
#include <ws/mongoose/mongoose.h>

namespace orion
{
namespace ws
{
namespace mongoose
{

class MongooseRequest : public Request
{
public:
   DECLARE_POINTERS(MongooseRequest)

   MongooseRequest(struct mg_connection* connection);

   virtual ~MongooseRequest();

   //! "GET", "POST", etc
   virtual std::string method() const; 

   //! URL-decoded URI
   virtual std::string uri() const;

   //! E.g. "1.0", "1.1"
   virtual std::string http_version() const;   
   
   //! URL part after '?', not including '?', or NULL
   virtual std::string query_string() const;   

   //! Authenticated user, or NULL if no auth used
   virtual std::string remote_user() const;    
   
   virtual bool is_authenticated()  const;

   virtual bool is_secure_connection() const;

   static Request::SharedPtr create(struct mg_connection* connection);

private:
   HeaderMap make_header_map(const struct mg_request_info* request_info);
   void read_data(struct mg_connection* connection);

   const struct mg_request_info* _request_info;
   
};

} // mongoose
} // ws
} // orion

#endif

