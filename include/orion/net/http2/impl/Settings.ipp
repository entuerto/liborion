//
// Settings.ipp
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP2_SETTINGS_IPP
#define ORION_NET_HTTP2_SETTINGS_IPP

#include <orion/net/http2/Error.h>

namespace orion
{
namespace net
{
namespace http2
{

template<>
inline constexpr HeaderTableSize Settings::get<HeaderTableSize>() const
{
   return _header_table_size;
}

template<>
inline constexpr EnablePush Settings::get<EnablePush>() const
{
   return _enable_push;
}

template<>
inline constexpr MaxConcurrentStreams Settings::get<MaxConcurrentStreams>() const
{
   return _max_concurrent_streams;
}

template<>
inline constexpr InitialWindowSize Settings::get<InitialWindowSize>() const
{
   return _initial_window_size;
}

template<>
inline constexpr MaxFrameSize Settings::get<MaxFrameSize>() const
{
   return _max_frame_size;
}

template<>
inline constexpr MaxHeaderListSize Settings::get<MaxHeaderListSize>() const
{
   return _max_header_list_size;
}

/// Allows the sender to inform the remote endpoint of the maximum size of the header 
/// compression table used to decode header blocks, in octets.
inline constexpr void Settings::set(HeaderTableSize value)
{
   _header_table_size = std::move(value);
}

/// This setting can be used to disable server push.
inline constexpr void Settings::set(EnablePush value)
{
   _enable_push = std::move(value);
}

/// There is no limit to this value. It is recommended that this value be no 
/// smaller than 100, so as to not unnecessarily limit parallelism.
inline constexpr void Settings::set(MaxConcurrentStreams value)
{
   _max_concurrent_streams = std::move(value);
}

/// Indicates the sender's initial window size (in octets) for stream-level flow control.
inline constexpr void Settings::set(InitialWindowSize value)
{
   // > 2147483647 = Error
   _initial_window_size = std::move(value);
}

/// Indicates the size of the largest frame payload that the sender is willing 
/// to receive, in octets.
inline constexpr void Settings::set(MaxFrameSize value)
{
   // < 16384 or > 16777215 = Error
   _max_frame_size = std::move(value);
}

/// This advisory setting informs a peer of the maximum size of header list that the 
/// sender is prepared to accept, in octets.
inline constexpr void Settings::set(MaxHeaderListSize value)
{
   _max_header_list_size = std::move(value);
}

inline constexpr bool Settings::is_valid(const InitialWindowSize& value) 
{
   return value <= 2147483647;
}

inline constexpr bool Settings::is_valid(const MaxFrameSize& value) 
{
   return value >= 16384 and value <= 16777215;
}

inline std::size_t Settings::encode(Span<uint8_t> b, const Settings& s)
{
   encoding::BigEndian::put_uint16(s._header_table_size.id(), b);
   encoding::BigEndian::put_uint32(s._header_table_size, b);

   encoding::BigEndian::put_uint16(s._enable_push.id(), b);
   encoding::BigEndian::put_uint32((s._enable_push ? 1 : 0), b);

   encoding::BigEndian::put_uint16(s._max_concurrent_streams.id(), b);
   encoding::BigEndian::put_uint32(s._max_concurrent_streams, b);

   encoding::BigEndian::put_uint16(s._initial_window_size.id(), b);
   encoding::BigEndian::put_uint32(s._initial_window_size, b);

   encoding::BigEndian::put_uint16(s._max_frame_size.id(), b);
   encoding::BigEndian::put_uint32(s._max_frame_size, b);

   encoding::BigEndian::put_uint16(s._max_header_list_size.id(), b);
   encoding::BigEndian::put_uint32(s._max_header_list_size, b);

   return 32;
}

inline std::error_code Settings::update(Span<const uint8_t> b, Settings& s)
{
   for (int i = 0; i < b.size(); i += 6)
   {
      auto value = encoding::BigEndian::to_uint32(b.subspan(i, 4));

      switch (encoding::BigEndian::to_uint16(b.subspan(i, 2)))
      {
         case 0x1:
            s.set(HeaderTableSize{value});
            break;
         case 0x2:
            s.set(EnablePush{value == 1});
            break;
         case 0x3:
            s.set(MaxConcurrentStreams{value});
            break;
         case 0x4:
            s.set(InitialWindowSize{value});
            break;
         case 0x5:
            s.set(MaxFrameSize{value});
            break;
         case 0x6:
            s.set(MaxHeaderListSize{value});
            break;
         default:
            return make_error_code(ErrorCode::SettingsInvalidFrame);
      }
   }
   return {};
}

} // namespace http2
} // namespace net
} // namespace orion

#endif // ORION_NET_HTTP2_SETTINGS_IPP