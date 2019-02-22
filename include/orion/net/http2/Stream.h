//
// Stream.h
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP2_STREAM_H
#define ORION_NET_HTTP2_STREAM_H

#include <orion/Common.h>

#include <orion/Chrono.h>
#include <orion/net/http2/Error.h>
#include <orion/net/http2/Utils.h>

namespace orion
{
namespace net
{
namespace http2
{

//-------------------------------------------------------------------------------------------------
// StreamState

enum class StreamState : uint8_t
{
   Idle,
   ReservedRemote,
   ReservedLocal,
   Open,
   HalfClosedRemote,
   HalfClosedLocal,
   Closed
};


//-------------------------------------------------------------------------------------------------
//

/// A low-level HTTP/2 stream object. This handles building and receiving
/// frames and maintains per-stream state.
class Stream
{
public:
   Stream() = default;

   Stream(uint32_t id, uint32_t in_window_size, uint32_t out_window_size);

   constexpr StreamState state() const { return _state; }

   /// Returns the RST_STREAM code used to close this stream
   constexpr int32_t code() const { return _code; }

   /// Returns the stream identifier for this stream
   constexpr uint32_t id() const { return _id; }

   void send_headers(const Headers& headers, int encoder, bool end_stream = false);

   void receive_headers(const Headers& headers, bool end_stream);

   /// Closes the Stream instance by sending an RST_STREAM frame to the connected HTTP/2 peer.
   void Close(int32_t code);

   struct Statistics
   {
      HighResTimePoint start_time;
      HighResTimePoint end_time;
      HighResTimePoint first_header;    // Time first header was received
      HighResTimePoint first_byte;      // Time first DATA frame byte was received
      HighResTimePoint first_byte_sent; // Time first DATA frame byte was sent
      std::size_t sent_bytes;
      std::size_t received_bytes;
   };

   /// Returns the streams running statistics
   constexpr Statistics& statistics() { return _statistics; }
   /// Returns the streams running statistics
   constexpr const Statistics& statistics() const { return _statistics; }

private:
   StreamState _state{StreamState::Idle};

   uint32_t _id{0u}; // The Stream Identifier
   int32_t _code{0};

   // The current value of the outbound stream flow control window
   uint32_t _outbound_flow_control_window{0};
   // The current value of the inbound stream flow control window
   uint32_t _inbound_flow_control_window{0};

   Statistics _statistics;

   // MaxHeaderListSize _max_header_list_size{DEFAULT_SETTINGS_MAX_HEADER_LIST_SIZE};
};

} // namespace http2
} // namespace net
} // namespace orion

//#include <orion/net/http2/impl/Stream.ipp>

#endif // ORION_NET_HTTP2_STREAM_H