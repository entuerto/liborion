//
// RequestHandler.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#include <orion/net/http/RequestHandler.h>

#include <orion/net/http/Error.h>
#include <orion/net/http/Utils.h>

namespace orion
{
namespace net
{
namespace http
{

RequestHandler::RequestHandler(const std::string& uri)
   : _uri(uri)
{
}

RequestHandler::~RequestHandler()
{
}

std::string RequestHandler::uri() const
{
   return _uri;
}

std::error_code RequestHandler::on_request(const Request& request, Response& response)
{
   switch (request.method())
   {
      case Method::GET:
         return on_get(request, response);
      case Method::HEAD:
         return on_head(request, response);
      case Method::POST:
         return on_post(request, response);
      case Method::PUT:
         return on_put(request, response);
      case Method::PATCH:
         return on_patch(request, response);
      case Method::DELETE:
         return on_delete(request, response);
      case Method::CONNECT:
         return on_connect(request, response);
      case Method::OPTIONS:
         return on_options(request, response);
      case Method::TRACE:
         return on_trace(request, response);
      default:
         break;
   }

   return on_user_defined(request, response);
}

std::error_code RequestHandler::on_connect(const Request& /* request */, Response& /* response */)
{
   return make_error_code(ErrorCode::NotImplemented);
}

std::error_code RequestHandler::on_delete(const Request& /* request */, Response& /* response */)
{
   return make_error_code(ErrorCode::NotImplemented);
}

std::error_code RequestHandler::on_get(const Request& /* request */, Response& /* response */)
{
   return make_error_code(ErrorCode::NotImplemented);
}

std::error_code RequestHandler::on_head(const Request& /* request */, Response& /* response */)
{
   return make_error_code(ErrorCode::NotImplemented);
}

std::error_code RequestHandler::on_options(const Request& /* request */, Response& /* response */)
{
   return make_error_code(ErrorCode::NotImplemented);
}

std::error_code RequestHandler::on_patch(const Request& /* request */, Response& /* response */)
{
   return make_error_code(ErrorCode::NotImplemented);
}

std::error_code RequestHandler::on_post(const Request& /* request */, Response& /* response */)
{
   return make_error_code(ErrorCode::NotImplemented);
}

std::error_code RequestHandler::on_put(const Request& /* request */, Response& /* response */)
{
   return make_error_code(ErrorCode::NotImplemented);
}

std::error_code RequestHandler::on_trace(const Request& /* request */, Response& /* response */)
{
   return make_error_code(ErrorCode::NotImplemented);
}

std::error_code RequestHandler::on_user_defined(const Request& /* request */,
                                                Response& /* response */)
{
   return make_error_code(ErrorCode::MethodNotAllowed);
}

} // http
} // net
} // orion
