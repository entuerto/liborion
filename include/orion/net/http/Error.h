//
// Error.h
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_NET_HTTP_ERROR_H
#define ORION_NET_HTTP_ERROR_H

#include <string>
#include <system_error>

#include <orion/Orion-Stddefs.h>

namespace orion
{
namespace net
{
namespace http
{
///
///
///
enum class errc
{
   BadRequest                    = 400, 
   Unauthorized                  = 401, 
   PaymentRequired               = 402, 
   Forbidden                     = 403, 
   NotFound                      = 404, 
   MethodNotAllowed              = 405, 
   NotAcceptable                 = 406, 
   ProxyAuthRequired             = 407, 
   RequestTimeout                = 408, 
   Conflict                      = 409, 
   Gone                          = 410, 
   LengthRequired                = 411, 
   PreconditionFailed            = 412, 
   RequestEntityTooLarge         = 413, 
   RequestURITooLong             = 414, 
   UnsupportedMediaType          = 415, 
   RequestedRangeNotSatisfiable  = 416, 
   ExpectationFailed             = 417, 
   Teapot                        = 418, 
   UnprocessableEntity           = 422, 
   Locked                        = 423, 
   FailedDependency              = 424, 
   UpgradeRequired               = 426, 
   PreconditionRequired          = 428, 
   TooManyRequests               = 429, 
   RequestHeaderFieldsTooLarge   = 431, 
   UnavailableForLegalReasons    = 451, 
   InternalServerError           = 500, 
   NotImplemented                = 501, 
   BadGateway                    = 502, 
   ServiceUnavailable            = 503, 
   GatewayTimeout                = 504, 
   HTTPVersionNotSupported       = 505, 
   VariantAlsoNegotiates         = 506, 
   InsufficientStorage           = 507, 
   LoopDetected                  = 508, 
   NotExtended                   = 510, 
   NetworkAuthenticationRequired = 511
};   

///
///
///
class ErrorCodeCategory : public std::error_category
{
public:
   const char* name() const noexcept override;

   std::string message(int err_code) const override;
};

///
///
///
inline const std::error_category& get_error_category()
{
   static ErrorCodeCategory res;
   return res;
}

///
///
///
inline std::error_code make_error_code(errc code) noexcept
{
   return std::error_code(static_cast<int>(code), get_error_category());
}

///
///
///
inline std::error_condition make_error_condition(errc code) noexcept
{
   return std::error_condition(static_cast<int>(code), get_error_category());
}

} // namespace http
} // namespace net
} // namespace orion

namespace std
{
   template<> struct is_error_condition_enum<orion::net::http::errc> : true_type {};
}

#endif // ORION_NET_HTTP_ERROR_H