//
// Error.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP_ERROR_H
#define ORION_NET_HTTP_ERROR_H

#include <orion/Orion-Stddefs.h>

#include <string>
#include <system_error>

namespace orion
{
namespace net
{
namespace http
{
///
///
///
enum class ErrorCode
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
inline std::error_code make_error_code(ErrorCode code) noexcept
{
   return std::error_code(static_cast<int>(code), get_error_category());
}

///
///
///
inline std::error_condition make_error_condition(ErrorCode code) noexcept
{
   return std::error_condition(static_cast<int>(code), get_error_category());
}

} // namespace http
} // namespace net
} // namespace orion

namespace std
{
template<>
struct is_error_condition_enum<orion::net::http::ErrorCode> : true_type
{
};
}

#endif // ORION_NET_HTTP_ERROR_H
