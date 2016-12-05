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
static const std::map<errc, std::string> ErrorText
{
   {errc::BadRequest,                    "Bad Request"},
   {errc::Unauthorized,                  "Unauthorized"},
   {errc::PaymentRequired,               "Payment Required"},
   {errc::Forbidden,                     "Forbidden"},
   {errc::NotFound,                      "Not Found"},
   {errc::MethodNotAllowed,              "Method Not Allowed"},
   {errc::NotAcceptable,                 "Not Acceptable"},
   {errc::ProxyAuthRequired,             "Proxy Authentication Required"},
   {errc::RequestTimeout,                "Request Timeout"},
   {errc::Conflict,                      "Conflict"},
   {errc::Gone,                          "Gone"},
   {errc::LengthRequired,                "Length Required"},
   {errc::PreconditionFailed,            "Precondition Failed"},
   {errc::RequestEntityTooLarge,         "Request Entity Too Large"},
   {errc::RequestURITooLong,             "Request URI Too Long"},
   {errc::UnsupportedMediaType,          "Unsupported Media Type"},
   {errc::RequestedRangeNotSatisfiable,  "Requested Range Not Satisfiable"},
   {errc::ExpectationFailed,             "Expectation Failed"},
   {errc::Teapot,                        "I'm a teapot"},
   {errc::UnprocessableEntity,           "Unprocessable Entity"},
   {errc::Locked,                        "Locked"},
   {errc::FailedDependency,              "Failed Dependency"},
   {errc::UpgradeRequired,               "Upgrade Required"},
   {errc::PreconditionRequired,          "Precondition Required"},
   {errc::TooManyRequests,               "Too Many Requests"},
   {errc::RequestHeaderFieldsTooLarge,   "Request Header Fields Too Large"},
   {errc::UnavailableForLegalReasons,    "Unavailable For Legal Reasons"},
   {errc::InternalServerError,           "Internal Server Error"},
   {errc::NotImplemented,                "Not Implemented"},
   {errc::BadGateway,                    "Bad Gateway"},
   {errc::ServiceUnavailable,            "Service Unavailable"},
   {errc::GatewayTimeout,                "Gateway Timeout"},
   {errc::HTTPVersionNotSupported,       "HTTP Version Not Supported"},
   {errc::VariantAlsoNegotiates,         "Variant Also Negotiates"},
   {errc::InsufficientStorage,           "Insufficient Storage"},
   {errc::LoopDetected,                  "Loop Detected"},
   {errc::NotExtended,                   "Not Extended"},
   {errc::NetworkAuthenticationRequired, "Network Authentication Required"}
};

const char* ErrorCodeCategory::name() const noexcept 
{
   return "liborion-net http errors";
}

std::string ErrorCodeCategory::message(int err_code) const
{
   auto text = ErrorText.find(static_cast<errc>(err_code));
   if (text != ErrorText.end())
      return text->second;

   return "Unknown code";
}

} // namespace http
} // namespace net
} // namespace orion
