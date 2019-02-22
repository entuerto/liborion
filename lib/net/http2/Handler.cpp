//
// Handler.cpp
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include "Handler.h"

#include <orion/Log.h>
#include <orion/net/http2/Error.h>

#include <fmt/format.h>

namespace orion
{
namespace net
{
namespace http2
{
//--------------------------------------------------------------------------------------------------
//

namespace detail
{

// CLIENT_MAGIC = "PRI * HTTP/2.0\r\n\r\nSM\r\n\r\n"
static const std::array<uint8_t, 24> CLIENT_MAGIC{0x50, 0x52, 0x49, 0x20, 0x2a, 0x20, 0x48, 0x54,
                                                  0x54, 0x50, 0x2f, 0x32, 0x2e, 0x30, 0x0d, 0x0a,
                                                  0x0d, 0x0a, 0x53, 0x4d, 0x0d, 0x0a, 0x0d, 0x0a};

}; // namespace detail

//--------------------------------------------------------------------------------------------------
// HandlerImpl

Handler::Handler(asio::io_context& io_context, http::RequestMux& mux)
   : _io_context(io_context)
   , _mux(mux)
{
   init();
}

Handler::~Handler()
{
   // Close up all active stream
   // TODO

   _statistics.end_time = std::chrono::high_resolution_clock::now();
}

const Settings& Handler::local_settings() const
{
   return _local_settings;
}

const Settings& Handler::remote_settings() const
{
   return _remote_settings;
}

bool Handler::read_wanted() const
{
   return false;
}

bool Handler::write_wanted() const
{
   return false;
}

bool Handler::should_stop() const
{
   return not read_wanted() and not write_wanted();
}

void Handler::signal_write()
{
   
}

Handler::State Handler::state() const
{
   return _state;
}

void Handler::state(State value)
{
   _state = value;
}

void Handler::submit(const Frame& frame)
{

}

void Handler::init()
{
   // Set up the frame dispatch table
   _frame_dispatch[static_cast<int>(FrameType::DATA)] = 
      [&](const Frame& frame) -> std::error_code { return on_handle_data(frame); };
   _frame_dispatch[static_cast<int>(FrameType::HEADERS)] =
      [&](const Frame& frame) -> std::error_code { return on_handle_headers(frame); };
   _frame_dispatch[static_cast<int>(FrameType::PRIORITY)] =
      [&](const Frame& frame) -> std::error_code { return on_handle_priority(frame); };
   _frame_dispatch[static_cast<int>(FrameType::RST_STREAM)] =
      [&](const Frame& frame) -> std::error_code { return on_handle_rst_stream(frame); };
   _frame_dispatch[static_cast<int>(FrameType::SETTINGS)] =
      [&](const Frame& frame) -> std::error_code { return on_handle_settings(frame); };
   _frame_dispatch[static_cast<int>(FrameType::PUSH_PROMISE)] =
      [&](const Frame& frame) -> std::error_code { return on_handle_push_promise(frame); };
   _frame_dispatch[static_cast<int>(FrameType::PING)] = 
      [&](const Frame& frame) -> std::error_code { return on_handle_ping(frame); };
   _frame_dispatch[static_cast<int>(FrameType::GOAWAY)] =
      [&](const Frame& frame) -> std::error_code { return on_handle_goaway(frame); };
   _frame_dispatch[static_cast<int>(FrameType::WINDOW_UPDATE)] =
      [&](const Frame& frame) -> std::error_code { return on_handle_window_update(frame); };
   _frame_dispatch[static_cast<int>(FrameType::CONTINUATION)] =
      [&](const Frame& frame) -> std::error_code { return on_handle_continuation(frame); };
   _frame_dispatch[static_cast<int>(FrameType::ALTSVC)] =
      [&](const Frame& frame) -> std::error_code { return on_handle_altsvc(frame); };
   _frame_dispatch[0xb] = 
      [](const Frame& frame) -> std::error_code { return make_error_code(ErrorCode::PROTOCOL_ERROR); };
   _frame_dispatch[static_cast<int>(FrameType::ORIGIN)] =
      [&](const Frame& frame) -> std::error_code { return on_handle_origin(frame); };

}

Stream* Handler::new_stream(uint32_t stream_id)
{
   Stream s{stream_id,
            _local_settings.get<InitialWindowSize>(),
            _remote_settings.get<InitialWindowSize>()};

   auto r = _streams.emplace(stream_id, std::move(s));

   auto size = _streams.size();
   if (size > _statistics.max_concurrent_streams)
      _statistics.max_concurrent_streams = size;

   // _highest_inbound_stream_id = stream_id;

   log::debug2("Create new stream Id ", stream_id);

   ++_statistics.stream_count;

   return &(*r.first).second;
}

Stream* Handler::get_stream(uint32_t stream_id)
{
   auto it = _streams.find(stream_id);
   if (it == std::end(_streams))
      return nullptr;
   
   return &(*it).second;
}

void Handler::update_streams_output_window(uint32_t out_window_size)
{
/*
   for (auto& [id, stream] : _streams)
   {
      stream.
   }
*/
}

//--------------------------------------------------------------------------------------------------
//

std::error_code Handler::on_read(Span<const uint8_t> buffer, std::size_t len)
{
   // TODO: Parse buffer

   _statistics.data_received += len;

   if (state() == State::ExpectingPreface)
   {
      if (len < detail::CLIENT_MAGIC.size())
         return make_error_code(ErrorCode::BadClientMagic);

      if (make_span(detail::CLIENT_MAGIC) != buffer.subspan(0, 24))
         return make_error_code(ErrorCode::BadClientMagic);

      // Send SETTINGS and Connection-level WINDOW_UPDATE
      submit(make_frame(local_settings()));

      state(State::Read);
      return decode_input(buffer.subspan(24, len - 24));
   }

   return {};
}

std::error_code Handler::on_write(Span<uint8_t> buffer, std::size_t& len)
{
   len = 0;

   for (;;)
   {
      auto bytes_read = 0;
      if (bytes_read < 0)
         return make_error_code(bytes_read);

      if (bytes_read == 0)
         break;

      //asio::buffer_copy(buffer, asio::buffer(data, bytes_read));

      len += bytes_read;
      _statistics.data_sent += bytes_read;
   }

   return {};
}

std::error_code Handler::decode_input(Span<const uint8_t> buffer)
{
   std::error_code ec;

   Frame f;

   int idx = 0;

   auto bytes_decoded = Frame::decode(local_settings(), buffer, f, ec);
   if (ec)
      return ec;

   if (bytes_decoded > 0u)
      on_handle_frame(f);

   while (bytes_decoded > 0u)
   {
      idx += bytes_decoded;

      bytes_decoded = Frame::decode(local_settings(), buffer.subspan(idx), f, ec);
      if (ec)
         return ec;

      if (bytes_decoded > 0u)
         on_handle_frame(f);
   }
   return ec;
}

//--------------------------------------------------------------------------------------------------
// Frame handlers

void Handler::on_handle_frame(const Frame& frame)
{
   log::debug(frame);

   try
   {
       auto func = at(_frame_dispatch, static_cast<int>(frame.type()));

      if (auto ec = func(frame); ec)
         log::error(ec, DbgSrcLoc);
   }
   catch(const std::exception& e)
   {
      log::exception(e);
   }
   
}

std::error_code Handler::on_handle_data(const Frame& frame)
{
   log::debug(fmt::format("Handling data frame for stream {}", frame.stream_id()));

   return {};
}

// The HEADERS frame (type=0x1) is used to open a stream (Section 5.1), and additionally 
// carries a header block fragment. HEADERS frames can be sent on a stream in the "idle", 
// "reserved (local)", "open", or "half-closed (remote)" state.
std::error_code Handler::on_handle_headers(const Frame& frame)
{
   // If a HEADERS frame is received whose stream identifier field is 0x0, the recipient 
   // MUST respond with a connection error (Section 5.4.1) of type PROTOCOL_ERROR
   if (frame.stream_id() == 0) 
   {
      return make_error_code(ErrorCode::HeadersInvalidStreamId);
   }

   log::debug(fmt::format("Handling headers frame for stream {}", frame.stream_id()));

   // Decode the headers
   auto res = _decoder.decode(frame.get());
   if (res.error)
   {
      return res.error;
   }

   Stream* stream = get_stream(frame.stream_id());
   if (stream == nullptr)
   {
      // We can add a new stream so long as we are less than the current
      // maximum on concurrent streams
      uint32_t max_concurrent_streams = _local_settings.get<MaxConcurrentStreams>();
      if (_streams.size() + 1 > max_concurrent_streams)
      {
         return make_error_code(ErrorCode::REFUSED_STREAM);
      }
      stream = new_stream(frame.stream_id());
   }

   stream->receive_headers(res.headers, frame.flags() == FrameFlags::END_STREAM);

   // TODO: Handle priority flag

   return {};
}

std::error_code Handler::on_handle_priority(const Frame& frame)
{
   log::debug(fmt::format("Handling priority frame for stream {}", frame.stream_id()));
   return {};
}

// The RST_STREAM frame (type=0x3) allows for immediate termination of a stream. RST_STREAM is 
// sent to request cancellation of a stream or to indicate that an error condition has occurred.
std::error_code Handler::on_handle_rst_stream(const Frame& frame)
{
   // If a RST_STREAM frame is received with a stream identifier of 0x0, the recipient 
   // MUST treat this as a connection error (Section 5.4.1) of type PROTOCOL_ERROR
   if (frame.stream_id() == 0) 
   {
      return make_error_code(ErrorCode::RstStreamInvalidStreamId);
   }

   log::debug(fmt::format("Handling rst_stream frame for stream {}", frame.stream_id()));

   // A RST_STREAM frame with a length other than 4 octets MUST be treated as a connection 
   // error (Section 5.4.1) of type FRAME_SIZE_ERROR.
   if (frame.length() > 4)
   {
      return make_error_code(ErrorCode::RstStreamFrameSizeError);
   }

   // TODO: Handle RST_STREAM

   return {};
}

// SETTINGS parameters are not negotiated; they describe characteristics of the sending peer, 
// which are used by the receiving peer. Different values for the same parameter can be advertised 
// by each peer. For example, a client might set a high initial flow-control window, whereas a 
// server might set a lower value to conserve resources.
//
// A SETTINGS frame MUST be sent by both endpoints at the start of a connection and MAY be sent 
// at any other time by either endpoint over the lifetime of the connection.
std::error_code Handler::on_handle_settings(const Frame& frame)
{
   // SETTINGS frames always apply to a connection, never a single stream. 
   // The stream identifier for a SETTINGS frame MUST be zero (0x0).
   if (frame.stream_id() != 0) 
   {
      return make_error_code(ErrorCode::SettingsInvalidStreamId);
   }

   log::debug(fmt::format("Handling settings frame for stream {}", frame.stream_id()));

   // A SETTINGS frame with a length other than a multiple of 6 octets MUST be treated 
   // as a connection error (Section 5.4.1) of type FRAME_SIZE_ERROR.
   if (frame.length() % 6 != 0)
   {
      return make_error_code(ErrorCode::SettingsFrameSizeError);
   }

   // When this bit is set, the payload of the SETTINGS frame MUST be empty. Receipt of 
   // a SETTINGS frame with the ACK flag set and a length field value other than 0 MUST 
   // be treated as a connection error (Section 5.4.1)
   if ((frame.flags() & FrameFlags::ACK) == FrameFlags::ACK)
   {
      if (frame.length() != 0)
      {
         return make_error_code(ErrorCode::SettingsInvalidAck);
      }
      return {};
   }
   // Apply remote_settings()
   std::error_code ec = Settings::update(frame.get(), _remote_settings);
   if (ec)
      return ec;

   // Send Ack
   submit(Frame{FrameType::SETTINGS, 0, FrameFlags::ACK});

   // Apply the change to window size (to all the streams but not the connection, 
   // see section 6.9.2
   update_streams_output_window(_remote_settings.get<InitialWindowSize>());

   return {};
}

// The PUSH_PROMISE frame (type=0x5) is used to notify the peer endpoint in advance of 
// streams the sender intends to initiate.
std::error_code Handler::on_handle_push_promise(const Frame& frame)
{
   // The stream identifier of a PUSH_PROMISE frame indicates the stream it is associated with. 
   // If the stream identifier field specifies the value 0x0, a recipient MUST respond with a 
   // connection error (Section 5.4.1) of type PROTOCOL_ERROR
   if (frame.stream_id() == 0) 
   {
      return make_error_code(ErrorCode::PushPromiseInvalidStreamId);
   }

   log::debug(fmt::format("Handling push promise frame for stream {}", frame.stream_id()));

   // TODO: handle push promise

   return {};
}

std::error_code Handler::on_handle_ping(const Frame& frame)
{
   log::debug(fmt::format("Handling ping frame for stream {}", frame.stream_id()));

   return {};
}

// The GOAWAY frame (type=0x7) is used to initiate shutdown of a connection or to signal 
// serious error conditions. GOAWAY allows an endpoint to gracefully stop accepting new 
// streams while still finishing processing of previously established streams. This enables 
// administrative actions, like server maintenance.
//
// Endpoints SHOULD always send a GOAWAY frame before closing a connection so that the 
// remote peer can know whether a stream has been partially processed or not.
std::error_code Handler::on_handle_goaway(const Frame& frame)
{
   // The GOAWAY frame applies to the connection, not a specific stream. An endpoint 
   // MUST treat a GOAWAY frame with a stream identifier other than 0x0 as a connection 
   // error (Section 5.4.1) of type PROTOCOL_ERROR.
   if (frame.stream_id() != 0) 
   {
      return make_error_code(ErrorCode::GoAwayInvalidStreamId);
   }

   log::debug(fmt::format("Handling goaway frame for stream {}", frame.stream_id()));

   // TODO: Handle go away

   return {};
}

std::error_code Handler::on_handle_window_update(const Frame& frame)
{
   log::debug(fmt::format("Handling window update frame for stream {}", frame.stream_id()));
   return {};
}

// The CONTINUATION frame (type=0x9) is used to continue a sequence of header block fragments 
// (Section 4.3). Any number of CONTINUATION frames can be sent, as long as the preceding frame 
// is on the same stream and is a HEADERS, PUSH_PROMISE, or CONTINUATION frame without the 
// END_HEADERS flag set.
std::error_code Handler::on_handle_continuation(const Frame& frame)
{
   // If a CONTINUATION frame is received whose stream identifier field is 0x0, the recipient 
   // MUST respond with a connection error (Section 5.4.1) of type PROTOCOL_ERROR.
   if (frame.stream_id() == 0) 
   {
      return make_error_code(ErrorCode::ContinuationInvalidStreamId);
   }

   log::debug(fmt::format("Handling continuation frame for stream {}", frame.stream_id()));

   // TODO: Handle continuation

   return {};
}

std::error_code Handler::on_handle_altsvc(const Frame& frame)
{
   log::debug(fmt::format("Handling altsvc frame for stream {}", frame.stream_id()));
   return {};
}

std::error_code Handler::on_handle_origin(const Frame& frame)
{
   log::debug(fmt::format("Handling origin frame for stream {}", frame.stream_id()));
   return {};
}


} // namespace http2
} // namespace net
} // namespace orion
