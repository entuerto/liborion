//
// Error.cpp
//
// Copyright 2016 tomas <tomasp@videotron.ca>
//

#include <orion/net/http/Error.h>

#include <map>

namespace orion
{
namespace net
{
namespace http
{
static const std::map<ErrorCode, std::string> ErrorText{
   {ErrorCode::BadRequest, "Bad Request"},
   {ErrorCode::Unauthorized, "Unauthorized"},
   {ErrorCode::PaymentRequired, "Payment Required"},
   {ErrorCode::Forbidden, "Forbidden"},
   {ErrorCode::NotFound, "Not Found"},
   {ErrorCode::MethodNotAllowed, "Method Not Allowed"},
   {ErrorCode::NotAcceptable, "Not Acceptable"},
   {ErrorCode::ProxyAuthRequired, "Proxy Authentication Required"},
   {ErrorCode::RequestTimeout, "Request Timeout"},
   {ErrorCode::Conflict, "Conflict"},
   {ErrorCode::Gone, "Gone"},
   {ErrorCode::LengthRequired, "Length Required"},
   {ErrorCode::PreconditionFailed, "Precondition Failed"},
   {ErrorCode::RequestEntityTooLarge, "Request Entity Too Large"},
   {ErrorCode::RequestURITooLong, "Request URI Too Long"},
   {ErrorCode::UnsupportedMediaType, "Unsupported Media Type"},
   {ErrorCode::RequestedRangeNotSatisfiable, "Requested Range Not Satisfiable"},
   {ErrorCode::ExpectationFailed, "Expectation Failed"},
   {ErrorCode::Teapot, "I'm a teapot"},
   {ErrorCode::UnprocessableEntity, "Unprocessable Entity"},
   {ErrorCode::Locked, "Locked"},
   {ErrorCode::FailedDependency, "Failed Dependency"},
   {ErrorCode::UpgradeRequired, "Upgrade Required"},
   {ErrorCode::PreconditionRequired, "Precondition Required"},
   {ErrorCode::TooManyRequests, "Too Many Requests"},
   {ErrorCode::RequestHeaderFieldsTooLarge, "Request Header Fields Too Large"},
   {ErrorCode::UnavailableForLegalReasons, "Unavailable For Legal Reasons"},
   {ErrorCode::InternalServerError, "Internal Server Error"},
   {ErrorCode::NotImplemented, "Not Implemented"},
   {ErrorCode::BadGateway, "Bad Gateway"},
   {ErrorCode::ServiceUnavailable, "Service Unavailable"},
   {ErrorCode::GatewayTimeout, "Gateway Timeout"},
   {ErrorCode::HTTPVersionNotSupported, "HTTP Version Not Supported"},
   {ErrorCode::VariantAlsoNegotiates, "Variant Also Negotiates"},
   {ErrorCode::InsufficientStorage, "Insufficient Storage"},
   {ErrorCode::LoopDetected, "Loop Detected"},
   {ErrorCode::NotExtended, "Not Extended"},
   {ErrorCode::NetworkAuthenticationRequired, "Network Authentication Required"}};

const char* ErrorCodeCategory::name() const noexcept
{
   return "liborion-net http errors";
}

std::string ErrorCodeCategory::message(int err_code) const
{
   auto text = ErrorText.find(static_cast<ErrorCode>(err_code));
   if (text != ErrorText.end())
      return text->second;

   return "Unknown code";
}

} // namespace http
} // namespace net
} // namespace orion
