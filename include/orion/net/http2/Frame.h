//
// Frame.h
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP2_FRAME_H
#define ORION_NET_HTTP2_FRAME_H

#include <orion/Common.h>

#include <orion/net/http2/Error.h>

namespace orion
{
namespace net
{
namespace http2
{
//-------------------------------------------------------------------------------------------------
// Forward declarations
class Settings;

//-------------------------------------------------------------------------------------------------
// FrameType

enum class FrameType : uint8_t
{
   DATA          = 0x0,
   HEADERS       = 0x1,
   PRIORITY      = 0x2,
   RST_STREAM    = 0x3,
   SETTINGS      = 0x4,
   PUSH_PROMISE  = 0x5,
   PING          = 0x6,
   GOAWAY        = 0x7,
   WINDOW_UPDATE = 0x8,
   CONTINUATION  = 0x9,
   ALTSVC        = 0xa,
   ORIGIN        = 0xc,
   UNKNOWN       = 0xd
};

/// Convert to a string
std::string to_string(FrameType ft);

//-------------------------------------------------------------------------------------------------
// FrameFlags

enum class FrameFlags : uint8_t
{
   NONE        = 0x0,
   ACK         = 0x1,
   END_STREAM  = 0x1,
   END_HEADERS = 0x4,
   PADDED      = 0x8,
   PRIORITY    = 0x20
};

constexpr uint8_t operator|(FrameFlags lhs, FrameFlags rhs);

constexpr FrameFlags operator&(FrameFlags lhs, FrameFlags rhs);
constexpr FrameFlags operator&(uint8_t lhs, FrameFlags rhs);
constexpr FrameFlags operator&(FrameFlags lhs, uint8_t rhs);

constexpr bool operator==(uint8_t lhs, FrameFlags rhs);
constexpr bool operator==(FrameFlags lhs, uint8_t rhs);

constexpr bool operator!=(uint8_t lhs, FrameFlags rhs);
constexpr bool operator!=(FrameFlags lhs, uint8_t rhs);

/// Convert to a string
std::string to_string(FrameFlags ff);

//-------------------------------------------------------------------------------------------------
// Frames

/// All frames begin with a fixed 9-octet header followed by a variable-length payload.
///
/// +-----------------------------------------------+
/// |                 Length (24)                   |
/// +---------------+---------------+---------------+
/// |   Type (8)    |   Flags (8)   |
/// +-+-------------+---------------+-------------------------------+
/// |R|                 Stream Identifier (31)                      |
/// +=+=============================================================+
/// |                   Frame Payload (0...)                      ...
/// +---------------------------------------------------------------+
class Frame
{
public:
   static constexpr const int32_t HeaderSize{9u};
   
   Frame() = default;

   Frame(FrameType t, uint32_t id);

   Frame(FrameType t, uint32_t id, Span<uint8_t> p);

   Frame(FrameType t, uint32_t id, uint8_t flags);

   Frame(FrameType t, uint32_t id, uint8_t flags, Span<uint8_t> p);

   Frame(FrameType t, uint32_t id, FrameFlags flag);

   Frame(FrameType t, uint32_t id, FrameFlags flag, Span<uint8_t> p);

   /// The frame type determines the format and semantics of the frame.
   constexpr FrameType type() const;

   /// Flags are assigned semantics specific to the indicated frame type.
   constexpr uint8_t flags() const;

   /// Set the flags for a frame type.
   constexpr void flags(uint8_t value);
   constexpr void flags(FrameFlags value);

   /// A stream identifier expressed as an unsigned 31-bit integer.
   constexpr uint32_t stream_id() const;

   /// The length of the frame payload expressed as an unsigned 24-bit integer. 
   /// Values greater than 16,384 MUST NOT be sent unless the receiver has set a larger 
   /// value for SETTINGS_MAX_FRAME_SIZE.
   ///
   /// The 9 octets of the frame header are not included in this value.
   constexpr uint32_t length() const;

   /// Set the length of the frame payload
   constexpr void length(uint32_t value);

   void set(Span<const uint8_t> p);

   Span<const uint8_t> get() const;

   /// Encode a frame 
   /// Returns the number of bytes encoded
   static std::size_t encode(Span<uint8_t> b, const Frame& f);

   /// Decode a frame 
   /// Returns the number of bytes decoded
   static std::size_t decode(const Settings& s, Span<const uint8_t> b, Frame& f, std::error_code& ec);

private:
   mutable uint32_t _length{0u};
   FrameType _type{FrameType::UNKNOWN};
   uint8_t _flags{0};
   uint32_t _stream_id{0u};

   std::vector<uint8_t> _payload;
};


Frame make_frame(const Settings& s);

} // namespace http2
} // namespace net
} // namespace orion

#include <orion/net/http2/impl/Frame.ipp>

#endif // ORION_NET_HTTP2_FRAME_H
