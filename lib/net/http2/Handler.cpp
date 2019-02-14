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

   switch (frame.type())
   {
      case FrameType::DATA:
      {   
         break;
      }
      case FrameType::HEADERS:
      {   
         break;
      }
      case FrameType::PRIORITY:
      {   
         break;
      }
      case FrameType::RST_STREAM:
      {   
         break;
      }
      case FrameType::SETTINGS:
      {   
         if (auto ec = on_handle_settings(frame); ec)
            log::error(ec, DbgSrcLoc);
         break;
      }
      case FrameType::PUSH_PROMISE:
      {   
         break;
      }
      case FrameType::PING:
      {   
         break;
      }
      case FrameType::GOAWAY:
      {   
         break;
      }
      case FrameType::WINDOW_UPDATE:
      {   
         break;
      }
      case FrameType::CONTINUATION:
      {   
         break;
      }
      default:
         break;
   }
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

   // A SETTINGS frame with a length other than a multiple of 6 octets MUST be treated 
   // as a connection error (Section 5.4.1) of type FRAME_SIZE_ERROR.


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

   // TODO: Apply settings to streams

   return {};
}

} // namespace http2
} // namespace net
} // namespace orion
