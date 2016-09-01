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

#ifndef ORION_NET_MONGOOSEREQUEST_H
#define ORION_NET_MONGOOSEREQUEST_H

#include <memory>
#include <string>

#include <orion/net/Request.h>
#include <mongoose/mongoose.h>

namespace orion
{
namespace net
{
namespace mongoose
{

class MongooseRequest : public Request
{
public:
   MongooseRequest(struct mg_connection* connection, struct http_message* hm);

   virtual ~MongooseRequest();

   //! "GET", "POST", etc
   virtual std::string method() const override; 

   //! URL-decoded URI
   virtual std::string uri() const override;

   //! E.g. "1.0", "1.1"
   virtual std::string http_version() const override;   
   
   //! URL part after '?', not including '?', or NULL
   virtual std::string query_string() const override;   

   //! Authenticated user, or NULL if no auth used
   virtual std::string remote_user() const override;    
   
   virtual bool is_authenticated()  const override;

   virtual bool is_secure_connection() const override;

   virtual std::string header(const std::string& name) const override;

   static std::unique_ptr<Request> create(struct mg_connection* connection, struct http_message* hm);

private:
   void read_data(struct http_message* hm);

   struct mg_connection* _connection;
   struct http_message* _hm;
   
};

} // mongoose
} // net
} // orion

#endif

