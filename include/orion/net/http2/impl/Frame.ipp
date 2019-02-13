//
// Frame.ipp
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP2_FRAME_IPP
#define ORION_NET_HTTP2_FRAME_IPP

#include <orion/Encoding.h>
#include <orion/String.h>

#include <fmt/format.h>

namespace orion
{
namespace net
{
namespace http2
{
//-------------------------------------------------------------------------------------------------

inline std::string to_string(FrameType ft)
{
   switch (ft)
   {
      case FrameType::DATA:
         return "DATA(0x0)";
      case FrameType::HEADERS:
         return "HEADERS(0x1)";
      case FrameType::PRIORITY:
         return "PRIORITY(0x2)";
      case FrameType::RST_STREAM:
         return "RST_STREAM(0x3)";
      case FrameType::SETTINGS:
         return "SETTINGS(0x4)";
      case FrameType::PUSH_PROMISE:
         return "PUSH_PROMISE(0x5)";
      case FrameType::PING:
         return "PING(0x6)";
      case FrameType::GOAWAY:
         return "GOAWAY(0x7)";
      case FrameType::WINDOW_UPDATE:
         return "WINDOW_UPDATE(0x8)";
      case FrameType::CONTINUATION:
         return "CONTINUATION(0x9)";
      case FrameType::UNKNOWN:
         return "UNKNOWN(0xa)";
      default:
         AssertUnreachable("Invalid FrameType");
         break;
   }
   return "Invalid FrameType";
}

//-------------------------------------------------------------------------------------------------

inline constexpr uint8_t operator|(FrameFlags lhs, FrameFlags rhs)
{
   return static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs);
}

inline constexpr bool operator==(uint8_t lhs, FrameFlags rhs)
{
   return static_cast<uint8_t>(rhs) == lhs;
}

inline constexpr bool operator==(FrameFlags lhs, uint8_t rhs)
{
   return static_cast<uint8_t>(lhs) == rhs;
}

inline constexpr bool operator!=(uint8_t lhs, FrameFlags rhs)
{
   return static_cast<uint8_t>(rhs) != lhs;
}

inline constexpr bool operator!=(FrameFlags lhs, uint8_t rhs)
{
   return static_cast<uint8_t>(lhs) != rhs;
}

inline std::string to_string(FrameFlags ff)
{
   uint8_t flag = static_cast<uint8_t>(ff);

   std::vector<std::string> ret;

   if (flag & static_cast<uint8_t>(FrameFlags::ACK))
      ret.emplace_back("ACK(0x1) or END_STREAM(0x1)");

   if (flag & static_cast<uint8_t>(FrameFlags::END_HEADERS))
      ret.emplace_back("END_HEADERS(0x4)");

   if (flag & static_cast<uint8_t>(FrameFlags::PADDED))
      ret.emplace_back("PADDED(0x8)");

   if (flag & static_cast<uint8_t>(FrameFlags::PRIORITY))
      ret.emplace_back("PRIORITY(0x20)");

   return ret.empty() ? "Invalid FrameFlags" : Join{"|"} | ret;
}

//-------------------------------------------------------------------------------------------------
// Frame implementation

inline Frame::Frame(FrameType t, uint32_t id)
   : _type(t)
   , _stream_id(id)
{
}

inline Frame::Frame(FrameType t, uint32_t id, Span<uint8_t> p)
   : _length(HeaderSize + p.size())
   , _type(t)
   , _stream_id(id)
   , _payload(p.begin(), p.end())
{
}

inline Frame::Frame(FrameType t, uint32_t id, uint8_t flags)
   : _type(t)
   , _flags(flags)
   , _stream_id(id)
{
}

inline Frame::Frame(FrameType t, uint32_t id, uint8_t flags, Span<uint8_t> p)
   : _length(HeaderSize + p.size())
   , _type(t)
   , _flags(flags)
   , _stream_id(id)
   , _payload(p.begin(), p.end())
{
}

inline Frame::Frame(FrameType t, uint32_t id, FrameFlags flag)
   : Frame(t, id, static_cast<uint8_t>(flag))
{
}

inline Frame::Frame(FrameType t, uint32_t id, FrameFlags flag, Span<uint8_t> p)
   : Frame(t, id, static_cast<uint8_t>(flag), p)
{
}

inline constexpr FrameType Frame::type() const
{
   return _type;
}

inline constexpr uint8_t Frame::flags() const
{
   return _flags;
}

inline constexpr void Frame::flags(uint8_t value)
{
   _flags = value;
}

inline constexpr void Frame::flags(FrameFlags value)
{
   _flags = static_cast<uint8_t>(value);
}

inline constexpr uint32_t Frame::stream_id() const
{
   return _stream_id;
}

inline constexpr uint32_t Frame::length() const
{
   return _length;
}

inline constexpr void Frame::length(uint32_t value)
{
   _length = value;
}

inline void Frame::set(Span<const uint8_t> p)
{
   _payload.assign(p.begin(), p.end());
}

inline Span<const uint8_t> Frame::get() const
{
   return Span<const uint8_t>{_payload};
}

inline std::size_t Frame::encode(Span<uint8_t> b, const Frame& f)
{
   std::size_t n = 0u;

   // Length
   encoding::BigEndian::put_uint24(Frame::HeaderSize + f._payload.size(), b);
   n += 3;

   // Type
   b[3] = static_cast<uint8_t>(f._type);
   n += 1;

   // Flags
   b[4] = f._flags;
   n += 1;

   // Stream Id
   encoding::BigEndian::put_uint32(f._stream_id, b.subspan(5));
   n += 4;

   if (not f._payload.empty())
   { 
      auto p = b.subspan(Frame::HeaderSize + 1);
      std::copy(std::begin(f._payload), std::end(f._payload), std::begin(p));
      n += f._payload.size();
   }
   return n;
}

inline std::size_t Frame::decode(Span<const uint8_t> b, Frame& f)
{
   std::size_t n = 0u;

   // Length
   f._length = encoding::BigEndian::to_uint24(b);
   n += 3;

   // Type
   f._type = static_cast<FrameType>(b[3]);
    n += 1;

   // Flags
   f._flags = b[4];
    n += 1;

   // Stream Id
   f._stream_id = encoding::BigEndian::to_uint32(b.subspan(5));
   n += 4;

   if (static_cast<std::size_t>(b.size()) > Frame::HeaderSize)
   {
      auto p = b.subspan(Frame::HeaderSize + 1, f._length - Frame::HeaderSize);
      f.set(p);
      n += p.size();
   }

   return n;
}

} // namespace http2
} // namespace net
} // namespace orion

//--------------------------------------------------------------------------------------------------
// Formatters for fmtlib

namespace fmt
{

template<>
struct formatter<orion::net::http2::FrameType>
{
   template<typename ParseContext>
   constexpr auto parse(ParseContext& ctx)
   {
      return ctx.begin();
   }

   template<typename FormatContext>
   auto format(const orion::net::http2::FrameType& ft, FormatContext& ctx)
   {
      return fmt::format_to(ctx.begin(), "{}", orion::net::http2::to_string(ft));
   }
};

template<>
struct formatter<orion::net::http2::FrameFlags>
{
   template<typename ParseContext>
   constexpr auto parse(ParseContext& ctx)
   {
      return ctx.begin();
   }

   template<typename FormatContext>
   auto format(const orion::net::http2::FrameFlags& ff, FormatContext& ctx)
   {
      return fmt::format_to(ctx.begin(), "{}", orion::net::http2::to_string(ff));
   }
};

} // namespace fmt

#endif // ORION_NET_HTTP2_FRAME_IPP
