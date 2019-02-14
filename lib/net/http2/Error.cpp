//
// Error.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/http2/Error.h>

#include <map>

namespace orion
{
namespace net
{
namespace http2
{
static const std::map<ErrorCode, std::string> ErrorText{
   {ErrorCode::SettingsInvalidAck, "Invalid SETTINGS frame (+ACK)."},
   {ErrorCode::SettingsInvalidFrame, "Invalid SETTINGS frame."},
   {ErrorCode::SettingsInvalidStreamId, "Invalid stream id in SETTINGS frame."},

   {ErrorCode::InvalidArgument, "Invalid argument passed."},
   {ErrorCode::BufferError, "Out of buffer space."},
   {ErrorCode::UnsupportedVersion, "The specified protocol version is not supported."},
   {ErrorCode::WouldBlock, "Indicate that an operation will block."},
   {ErrorCode::Protocol, "General protocol error."},
   {ErrorCode::InvalidFrame, "The frame is invalid."},
   {ErrorCode::Eof, "The peer performed a shutdown on the connection."},
   {ErrorCode::Deferred, "Data transfer is postponed."},
   {ErrorCode::StreamIdNotAvailable, "Stream ID has reached the maximum value."},
   {ErrorCode::StreamClosed, "The stream is already closed; or the stream ID is invalid."},
   {ErrorCode::StreamClosing, "RST_STREAM has been added to the outbound queue. The stream is in closing state."},
   {ErrorCode::StreamShutWR, "The transmission is not allowed for this stream."},
   {ErrorCode::InvalidStreamId, "The stream ID is invalid."},
   {ErrorCode::InvalidStreamState, "The state of the stream is not valid."},
   {ErrorCode::DeferredDataExist, "Another DATA frame has already been deferred."},
   {ErrorCode::StartStreamNotAllowed, "Starting new stream is not allowed."},
   {ErrorCode::GoawayAlreadySent, "GOAWAY has already been sent."},
   {ErrorCode::InvalidHeaderBlock, "The received frame contains the invalid header block."},
   {ErrorCode::InvalidState, "Indicates that the context is not suitable to perform the requested operation."},
   {ErrorCode::TemporalCallbackFailure, "The user callback function failed due to the temporal error."},
   {ErrorCode::FrameSizeError, "The length of the frame is invalid, either too large or too small."},
   {ErrorCode::HeaderComp, "Header block inflate/deflate error."},
   {ErrorCode::FlowControl, "Flow control error."},
   {ErrorCode::InsuffBufsize, "Insufficient buffer size given to function."},
   {ErrorCode::Pause, "Callback was paused by the application."},
   {ErrorCode::TooManyInflightSettings, "There are too many in-flight SETTING frame and no more transmission of SETTINGS is allowed."},
   {ErrorCode::PushDisabled, "The server push is disabled."},
   {ErrorCode::DataExist, "DATA or HEADERS frame for a given stream has been already submitted and has not been fully processed yet."},
   {ErrorCode::SessionClosing, "The current session is closing due to a connection error or 'terminate session' is called."},
   {ErrorCode::HttpHeader, "Invalid HTTP header field was received and stream is going to be closed."},
   {ErrorCode::HttpMessaging, "Violation in HTTP messaging rule."},
   {ErrorCode::RefusedStream, "Stream was refused."},
   {ErrorCode::Internal, "Unexpected internal error, but recovered."},
   {ErrorCode::Cancel, "Indicates that a processing was canceled."},
   {ErrorCode::SettingsExpected, "When a local endpoint expects to receive SETTINGS frame, it receives an other type of frame."},
   {ErrorCode::Fatal, "Under unexpected condition and processing was terminated."},
   {ErrorCode::NoMem, "Out of memory.  This is a fatal error."},
   {ErrorCode::CallbackFailure, "The user callback function failed.  This is a fatal error."},
   {ErrorCode::BadClientMagic, "Invalid client magic was received and further processing is not possible."},
   {ErrorCode::Flooded, "Possible flooding by peer was detected in this HTTP/2 session."},
   // The codes for the RST_STREAM and GOAWAY frames.
   {ErrorCode::PROTOCOL_ERROR, "Protocol error detected."},
   {ErrorCode::INTERNAL_ERROR, "Implementation fault."},
   {ErrorCode::FLOW_CONTROL_ERROR, "Flow-control limits exceeded."},
   {ErrorCode::SETTINGS_TIMEOUT, "Settings not acknowledged."},
   {ErrorCode::STREAM_CLOSED, "Frame received for closed stream."},
   {ErrorCode::FRAME_SIZE_ERROR, "Frame size incorrect."},
   {ErrorCode::REFUSED_STREAM, "Stream not processed."},
   {ErrorCode::CANCEL, "Stream cancelled."},
   {ErrorCode::COMPRESSION_ERROR, "Compression state not updated."},
   {ErrorCode::CONNECT_ERROR, "TCP connection error for CONNECT method."},
   {ErrorCode::ENHANCE_YOUR_CALM, "Processing capacity exceeded."},
   {ErrorCode::INADEQUATE_SECURITY, "Negotiated TLS parameters not acceptable."},
   {ErrorCode::HTTP_1_1_REQUIRED, "Use HTTP/1.1 for the request."}
};

///
/// Error category class for HTTP v2 errors
///
class ErrorCodeCategory : public std::error_category
{
public:
   const char* name() const noexcept override
   {
      return "liborion-net http2 errors";
   }

   std::string message(int err_code) const override
   {
      auto text = ErrorText.find(static_cast<ErrorCode>(err_code));
      if (text != ErrorText.end())
         return text->second;

      return "Unknown code";
   }
};

///
/// Returns the error category for HTTP v2 errors
const std::error_category& get_error_category()
{
   static ErrorCodeCategory res;
   return res;
}

///
/// Creates an error_code for a HTTP v2 error
std::error_code make_error_code(ErrorCode code) noexcept
{
   return {static_cast<int>(code), get_error_category()};
}

///
/// Creates an error_code for a HTTP v2 error
std::error_code make_error_code(int code) noexcept
{
   return {code, get_error_category()};
}

///
/// Creates an error_condition for a HTTP v2 error
std::error_condition make_error_condition(ErrorCode code) noexcept
{
   return {static_cast<int>(code), get_error_category()};
}

} // namespace http2
} // namespace net
} // namespace orion
