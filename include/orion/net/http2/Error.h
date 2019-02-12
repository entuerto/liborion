//
// Error.h
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP2_ERROR_H
#define ORION_NET_HTTP2_ERROR_H

#include <orion/Config.h>

#include <string>
#include <system_error>

namespace orion
{
namespace net
{
namespace http2
{
///
/// HTTP v2 error codes
///
enum class ErrorCode
{
   InvalidArgument         = -501,
   BufferError             = -502,
   UnsupportedVersion      = -503,
   WouldBlock              = -504,
   Protocol                = -505,
   InvalidFrame            = -506,
   Eof                     = -507,
   Deferred                = -508,
   StreamIdNotAvailable    = -509,
   StreamClosed            = -510,
   StreamClosing           = -511,
   StreamShutWR            = -512,
   InvalidStreamId         = -513,
   InvalidStreamState      = -514,
   DeferredDataExist       = -515,
   StartStreamNotAllowed   = -516,
   GoawayAlreadySent       = -517,
   InvalidHeaderBlock      = -518,
   InvalidState            = -519,
   TemporalCallbackFailure = -521,
   FrameSizeError          = -522,
   HeaderComp              = -523,
   FlowControl             = -524,
   InsuffBufsize           = -525,
   Pause                   = -526,
   TooManyInflightSettings = -527,
   PushDisabled            = -528,
   DataExist               = -529,
   SessionClosing          = -530,
   HttpHeader              = -531,
   HttpMessaging           = -532,
   RefusedStream           = -533,
   Internal                = -534,
   Cancel                  = -535,
   SettingsExpected        = -536,
   Fatal                   = -900,
   NoMem                   = -901,
   CallbackFailure         = -902,
   BadClientMagic          = -903,
   Flooded                 = -904,

   // The codes for the RST_STREAM and GOAWAY frames.
   PROTOCOL_ERROR          = 0x1,
   INTERNAL_ERROR          = 0x2,
   FLOW_CONTROL_ERROR      = 0x3,
   SETTINGS_TIMEOUT        = 0x4,
   STREAM_CLOSED           = 0x5,
   FRAME_SIZE_ERROR        = 0x6,
   REFUSED_STREAM          = 0x7,
   CANCEL                  = 0x8,
   COMPRESSION_ERROR       = 0x9,
   CONNECT_ERROR           = 0xa,
   ENHANCE_YOUR_CALM       = 0xb,
   INADEQUATE_SECURITY     = 0xc,
   HTTP_1_1_REQUIRED       = 0xd 
};

///
/// Returns the error category for HTTP v2 errors
API_EXPORT const std::error_category& get_error_category();

///
/// Creates an error_code for a HTTP v2 error
API_EXPORT std::error_code make_error_code(ErrorCode code) noexcept;

///
/// Creates an error_code for a HTTP v2 error
API_EXPORT std::error_code make_error_code(int code) noexcept;

///
/// Creates an error_condition for a HTTP v2 error
API_EXPORT std::error_condition make_error_condition(ErrorCode code) noexcept;

} // namespace http2
} // namespace net
} // namespace orion

namespace std
{
template<>
struct is_error_condition_enum<orion::net::http2::ErrorCode> : true_type
{
};
} // namespace std

#endif // ORION_NET_HTTP2_ERROR_H
