//
// RequestListener.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#include <orion/net/http/RequestListener.h>

#include <orion/net/http/Error.h>
#include <orion/net/http/Utils.h>

namespace orion
{
namespace net
{
namespace http
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

std::error_code RequestListener::on_process_request(const Request& request, Response& response)
{
   if (request.method() == Method::Get)
      return on_get(request, response);
   else if (request.method() == Method::Post)
      return on_post(request, response);
   else if (request.method() == Method::Put)
      return on_put(request, response);
   else if (request.method() == Method::Delete)
      return on_delete(request, response);

   return make_error_code(errc::BadRequest); 
}

std::error_code RequestListener::on_get(const Request& /* request */, Response& /* response */)
{
   return make_error_code(errc::NotImplemented);
}

std::error_code RequestListener::on_post(const Request& /* request */, Response& /* response */)
{
   return make_error_code(errc::NotImplemented);
}

std::error_code RequestListener::on_put(const Request& /* request */, Response& /* response */) 
{
   return make_error_code(errc::NotImplemented);
}

std::error_code RequestListener::on_delete(const Request& /* request */, Response& /* response */)
{
   return make_error_code(errc::NotImplemented);
}

} // http
} // net
} // orion
