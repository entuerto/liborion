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
   SettingsInvalidAck          = -501,
   SettingsInvalidFrame        = -502,
   SettingsInvalidStreamId     = -503,
   SettingsFrameSizeError      = -504,
   RstStreamInvalidStreamId    = -505,
   RstStreamFrameSizeError     = -506,
   PushPromiseInvalidStreamId  = -507,
   GoAwayInvalidStreamId       = -508,
   ContinuationInvalidStreamId = -509,
   HeadersInvalidStreamId      = -510,
   TooManyStreams              = -550,

   InvalidArgument         = -601,
   BufferError             = -602,
   UnsupportedVersion      = -603,
   WouldBlock              = -604,
   Protocol                = -605,
   InvalidFrame            = -606, // Used
   Eof                     = -607,
   Deferred                = -608,
   StreamIdNotAvailable    = -609,
   StreamClosed            = -610,
   StreamClosing           = -611,
   StreamShutWR            = -612,
   InvalidStreamId         = -613,
   InvalidStreamState      = -614,
   DeferredDataExist       = -615,
   StartStreamNotAllowed   = -616,
   GoawayAlreadySent       = -617,
   InvalidHeaderBlock      = -618,
   InvalidState            = -619,
   TemporalCallbackFailure = -621,
   FrameSizeError          = -622, // Used
   HeaderComp              = -623, // Used
   FlowControl             = -624,
   InsuffBufsize           = -625, // Used
   Pause                   = -626,
   TooManyInflightSettings = -627,
   PushDisabled            = -628,
   DataExist               = -629,
   SessionClosing          = -630,
   HttpHeader              = -631,
   HttpMessaging           = -632,
   RefusedStream           = -633,
   Internal                = -634,
   Cancel                  = -635,
   SettingsExpected        = -636,
   Fatal                   = -900,
   NoMem                   = -901,
   CallbackFailure         = -902,
   BadClientMagic          = -903, // Used
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
