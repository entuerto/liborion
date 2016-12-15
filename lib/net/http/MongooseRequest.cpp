//
// MongooseRequest.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <net/http/MongooseRequest.h>

#include <orion/Logging.h>

using namespace orion::logging;

namespace orion
{
namespace net
{
namespace http
{
MongooseRequest::MongooseRequest() :
   Request()
{
}

MongooseRequest::MongooseRequest(struct http_message* hm) :
   Request(),
   _body_streambuf(std::make_unique<std::stringbuf>(std::string(hm->body.p, hm->body.len)))
{
   _method  = std::string(hm->method.p, hm->method.len);
   _url     = std::string(hm->uri.p, hm->uri.len) + 
              std::string(hm->query_string.p, hm->query_string.len);
   _version = Version{1, 1};

   for (int i = 0; hm->header_names[i].len > 0; i++) 
   {
      auto h = hm->header_names[i];
      auto v = hm->header_values[i];

      if (h.p == nullptr)
         continue;

      _header.emplace(std::string(h.p, h.len), std::string(v.p, v.len));
   }

   LOG(Debug2) << std::string(hm->message.p, hm->message.len);
}

MongooseRequest::MongooseRequest(MongooseRequest&& Other) :
   Request(std::move(Other)),
   _body_streambuf(std::move(Other._body_streambuf))
{
}

MongooseRequest::~MongooseRequest()
{
}

MongooseRequest& MongooseRequest::operator=(MongooseRequest&& Rhs)
{
   Request::operator=(std::move(Rhs));

   _body_streambuf   = std::move(Rhs._body_streambuf);

   return *this;   
}

std::streambuf* MongooseRequest::rdbuf() const
{
   return _body_streambuf.get();  
}

} // http
} // net
} // orion

