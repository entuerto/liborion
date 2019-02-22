//
// Handler.h
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP2_HANDLER_H
#define ORION_NET_HTTP2_HANDLER_H

#include <orion/Common.h>

#include <orion/Chrono.h>
#include <orion/net/http/RequestMux.h>
#include <orion/net/http2/Frame.h>
#include <orion/net/http2/Settings.h>
#include <orion/net/http2/Stream.h>
#include <orion/net/http2/Utils.h>

#include <net/http2/hpack/HPack.h>

#include <map>
#include <string>

namespace orion
{
namespace net
{
namespace http2
{
class Stream;
///
/// Implement the parsing and handling of HTTP v2 streams
///
class Handler : public std::enable_shared_from_this<Handler>
{
public:
   enum class State : uint8_t
   {
      ExpectingPreface,
      Read,
      Write,
      Closed
   };

   Handler(asio::io_context& io_context, http::RequestMux& mux);

   ~Handler();

   const Settings& local_settings() const; 
   const Settings& remote_settings() const; 

   /// Returns true if session wants to receive data from the remote peer.
   bool read_wanted() const;

   /// Returns true if session wants to send data to the remote peer.
   bool write_wanted() const;

   /// Indicates if the connection needs to stop reading and writing.
   /// Connection should close
   bool should_stop() const;

   /// Signals the connection to write data.
   void signal_write();

   State state() const;
   void state(State value);

   void submit(const Frame& frame);

   std::error_code on_read(Span<const uint8_t> buffer, std::size_t len);

   std::error_code on_write(Span<uint8_t> buffer, std::size_t& len);

   struct Statistics
   {
      HighResTimePoint start_time;
      HighResTimePoint end_time;
      uint64_t ping_rtt{0u};
      uint64_t data_sent{0u};
      uint64_t data_received{0u};
      uint32_t frame_count{0u};
      uint32_t frame_sent{0u};
      int32_t stream_count{0};
      std::size_t max_concurrent_streams{0};
      double stream_average_duration{0.0};
   };

   /// Returns the handlers running statistics
   constexpr Statistics& statistics() { return _statistics; }
   /// Returns the handlers running statistics
   constexpr const Statistics& statistics() const { return _statistics; }

private:
   /// Initialise internal value/structures
   void init();

   /// Create a new stream
   Stream* new_stream(uint32_t stream_id);
   /// Get an existing stream
   Stream* get_stream(uint32_t stream_id);

   std::error_code decode_input(Span<const uint8_t> buffer);

   void update_streams_output_window(uint32_t out_window_size);

   // Frame Handlers
   //
   void on_handle_frame(const Frame& frame);

   std::error_code on_handle_data(const Frame& frame);
   std::error_code on_handle_headers(const Frame& frame);
   std::error_code on_handle_priority(const Frame& frame);
   std::error_code on_handle_rst_stream(const Frame& frame);
   std::error_code on_handle_settings(const Frame& frame);
   std::error_code on_handle_push_promise(const Frame& frame);
   std::error_code on_handle_ping(const Frame& frame);
   std::error_code on_handle_goaway(const Frame& frame);
   std::error_code on_handle_window_update(const Frame& frame);
   std::error_code on_handle_continuation(const Frame& frame);
   std::error_code on_handle_altsvc(const Frame& frame);
   std::error_code on_handle_origin(const Frame& frame);

private:
   asio::io_context& _io_context;
   http::RequestMux& _mux;

   State _state{State::ExpectingPreface};

   Settings _local_settings;
   Settings _remote_settings;

   hpack::Decoder _decoder;

   Statistics _statistics;

   std::map<int32_t, Stream> _streams;

   std::array<std::function<std::error_code(const Frame&)>, 13> _frame_dispatch;
};

} // namespace http2
} // namespace net
} // namespace orion
#endif // ORION_NET_HTTP2_HANDLER_H
