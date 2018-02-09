//
// StatusCode.ipp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP_STATUSCODE_IPP
#define ORION_NET_HTTP_STATUSCODE_IPP

#include <orion/Orion-Stddefs.h>

#include <orion/Log.h>
#include <orion/Utils.h>

#include <exception>
#include <map>
#include <string>

namespace orion
{
namespace net
{
namespace http
{

static const std::map<StatusCode, std::string>& status_text()
{
   try
   {
      static const std::map<StatusCode, std::string> StatusText{
         {StatusCode::Continue, "Continue"},
         {StatusCode::SwitchingProtocols, "Switching Protocols"},
         {StatusCode::Processing, "Processing"},

         {StatusCode::OK, "OK"},
         {StatusCode::Created, "Created"},
         {StatusCode::Accepted, "Accepted"},
         {StatusCode::NonAuthoritativeInfo, "Non-Authoritative Information"},
         {StatusCode::NoContent, "No Content"},
         {StatusCode::ResetContent, "Reset Content"},
         {StatusCode::PartialContent, "Partial Content"},
         {StatusCode::MultiStatus, "Multi-Status"},
         {StatusCode::AlreadyReported, "Already Reported"},
         {StatusCode::IMUsed, "IM Used"},

         {StatusCode::MultipleChoices, "Multiple Choices"},
         {StatusCode::MovedPermanently, "Moved Permanently"},
         {StatusCode::Found, "Found"},
         {StatusCode::SeeOther, "See Other"},
         {StatusCode::NotModified, "Not Modified"},
         {StatusCode::UseProxy, "Use Proxy"},
         {StatusCode::TemporaryRedirect, "Temporary Redirect"},
         {StatusCode::PermanentRedirect, "Permanent Redirect"},

         {StatusCode::BadRequest, "Bad Request"},
         {StatusCode::Unauthorized, "Unauthorized"},
         {StatusCode::PaymentRequired, "Payment Required"},
         {StatusCode::Forbidden, "Forbidden"},
         {StatusCode::NotFound, "Not Found"},
         {StatusCode::MethodNotAllowed, "Method Not Allowed"},
         {StatusCode::NotAcceptable, "Not Acceptable"},
         {StatusCode::ProxyAuthRequired, "Proxy Authentication Required"},
         {StatusCode::RequestTimeout, "Request Timeout"},
         {StatusCode::Conflict, "Conflict"},
         {StatusCode::Gone, "Gone"},
         {StatusCode::LengthRequired, "Length Required"},
         {StatusCode::PreconditionFailed, "Precondition Failed"},
         {StatusCode::RequestEntityTooLarge, "Request Entity Too Large"},
         {StatusCode::RequestURITooLong, "Request URI Too Long"},
         {StatusCode::UnsupportedMediaType, "Unsupported Media Type"},
         {StatusCode::RequestedRangeNotSatisfiable, "Requested Range Not Satisfiable"},
         {StatusCode::ExpectationFailed, "Expectation Failed"},
         {StatusCode::Teapot, "I'm a teapot"},
         {StatusCode::UnprocessableEntity, "Unprocessable Entity"},
         {StatusCode::Locked, "Locked"},
         {StatusCode::FailedDependency, "Failed Dependency"},
         {StatusCode::UpgradeRequired, "Upgrade Required"},
         {StatusCode::PreconditionRequired, "Precondition Required"},
         {StatusCode::TooManyRequests, "Too Many Requests"},
         {StatusCode::RequestHeaderFieldsTooLarge, "Request Header Fields Too Large"},
         {StatusCode::UnavailableForLegalReasons, "Unavailable For Legal Reasons"},

         {StatusCode::InternalServerError, "Internal Server Error"},
         {StatusCode::NotImplemented, "Not Implemented"},
         {StatusCode::BadGateway, "Bad Gateway"},
         {StatusCode::ServiceUnavailable, "Service Unavailable"},
         {StatusCode::GatewayTimeout, "Gateway Timeout"},
         {StatusCode::HTTPVersionNotSupported, "HTTP Version Not Supported"},
         {StatusCode::VariantAlsoNegotiates, "Variant Also Negotiates"},
         {StatusCode::InsufficientStorage, "Insufficient Storage"},
         {StatusCode::LoopDetected, "Loop Detected"},
         {StatusCode::NotExtended, "Not Extended"},
         {StatusCode::NetworkAuthenticationRequired, "Network Authentication Required"}};
      return StatusText;
   }
   catch (...)
   {
      log::error("An unexpected, unknown exception was thrown: ",
                 type_name(std::current_exception()),
                 _src_loc);
      std::abort();
   }
}

//-------------------------------------------------------------------------------------------------

inline std::string to_string(StatusCode code)
{
   auto item = status_text().find(code);

   if (item != status_text().end())
      return item->second;

   return "StatusCode: Unkown";
}

inline void format_arg(fmt::BasicFormatter<char>& f, const char*& /*fmt_str*/, StatusCode code)
{
   f.writer().write("{}", static_cast<int>(code));
}

inline std::ostream& operator<<(std::ostream& o, StatusCode code)
{
   o << static_cast<int>(code);
   return o;
}

} // namespace http
} // namespace net
} // namespace orion

#endif // ORION_NET_HTTP_STATUSCODE_IPP