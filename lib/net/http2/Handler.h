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
#include <orion/net/http2/Settings.h>
#include <orion/net/http2/Utils.h>

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

   std::error_code on_read(asio::const_buffer buffer, std::size_t len);

   std::error_code on_write(asio::mutable_buffer& buffer, std::size_t& len);

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
   Statistics& statistics() { return _statistics; }
   /// Returns the handlers running statistics
   const Statistics& statistics() const { return _statistics; }

private:
   asio::io_context& _io_context;
   http::RequestMux& _mux;

   Settings _local_settings;
   Settings _remote_settings;

   Statistics _statistics;
};

} // namespace http2
} // namespace net
} // namespace orion
#endif // ORION_NET_HTTP2_HANDLER_H
