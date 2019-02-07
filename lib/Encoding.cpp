//
// Encoding.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Encoding.h>

namespace orion
{
namespace encoding
{
//---------------------------------------------------------------------------------------

uint16_t BigEndian::to_uint16(Span<const uint8_t> b)
{
   return static_cast<uint16_t>(b[1]) | static_cast<uint16_t>(b[0]) << 8;
}

uint32_t BigEndian::to_uint32(Span<const uint8_t> b)
{
   return static_cast<uint32_t>(b[3]) | static_cast<uint32_t>(b[2]) << 8 |
          static_cast<uint32_t>(b[1]) << 16 | static_cast<uint32_t>(b[0]) << 24;
}

uint64_t BigEndian::to_uint64(Span<const uint8_t> b)
{
   return static_cast<uint64_t>(b[7]) | static_cast<uint64_t>(b[6]) << 8 |
          static_cast<uint64_t>(b[5]) << 16 | static_cast<uint64_t>(b[4]) << 24 |
          static_cast<uint64_t>(b[3]) << 32 | static_cast<uint64_t>(b[2]) << 40 |
          static_cast<uint64_t>(b[1]) << 48 | static_cast<uint64_t>(b[0]) << 56;
}

void BigEndian::put_uint16(uint16_t v, Span<uint8_t> b)
{
   b[0] = static_cast<uint8_t>(v >> 8);
   b[1] = static_cast<uint8_t>(v);
}

void BigEndian::put_uint32(uint32_t v, Span<uint8_t> b)
{
   b[0] = static_cast<uint8_t>(v >> 24);
   b[1] = static_cast<uint8_t>(v >> 16);
   b[2] = static_cast<uint8_t>(v >> 8);
   b[3] = static_cast<uint8_t>(v);
}

void BigEndian::put_uint64(uint64_t v, Span<uint8_t> b)
{
   b[0] = static_cast<uint8_t>(v >> 56);
   b[1] = static_cast<uint8_t>(v >> 48);
   b[2] = static_cast<uint8_t>(v >> 40);
   b[3] = static_cast<uint8_t>(v >> 32);
   b[4] = static_cast<uint8_t>(v >> 24);
   b[5] = static_cast<uint8_t>(v >> 16);
   b[6] = static_cast<uint8_t>(v >> 8);
   b[7] = static_cast<uint8_t>(v);
}

//---------------------------------------------------------------------------------------

uint16_t LittleEndian::to_uint16(Span<const uint8_t> b)
{
   return static_cast<uint16_t>(b[0]) | static_cast<uint16_t>(b[1]) << 8;
}

uint32_t LittleEndian::to_uint32(Span<const uint8_t> b)
{
   return static_cast<uint32_t>(b[0]) | static_cast<uint32_t>(b[1]) << 8 |
          static_cast<uint32_t>(b[2]) << 16 | static_cast<uint32_t>(b[3]) << 24;
}

uint64_t LittleEndian::to_uint64(Span<const uint8_t> b)
{
   return static_cast<uint64_t>(b[0]) | static_cast<uint64_t>(b[1]) << 8 |
          static_cast<uint64_t>(b[2]) << 16 | static_cast<uint64_t>(b[3]) << 24 |
          static_cast<uint64_t>(b[4]) << 32 | static_cast<uint64_t>(b[5]) << 40 |
          static_cast<uint64_t>(b[6]) << 48 | static_cast<uint64_t>(b[7]) << 56;
}

void LittleEndian::put_uint16(uint16_t v, Span<uint8_t> b)
{
   b[0] = static_cast<uint8_t>(v);
   b[1] = static_cast<uint8_t>(v >> 8);
}

void LittleEndian::put_uint32(uint32_t v, Span<uint8_t> b)
{
   b[0] = static_cast<uint8_t>(v);
   b[1] = static_cast<uint8_t>(v >> 8);
   b[2] = static_cast<uint8_t>(v >> 16);
   b[3] = static_cast<uint8_t>(v >> 24);
}

void LittleEndian::put_uint64(uint64_t v, Span<uint8_t> b)
{
   b[0] = static_cast<uint8_t>(v >> 8);
   b[1] = static_cast<uint8_t>(v >> 16);
   b[2] = static_cast<uint8_t>(v >> 24);
   b[3] = static_cast<uint8_t>(v >> 32);
   b[4] = static_cast<uint8_t>(v >> 40);
   b[5] = static_cast<uint8_t>(v >> 48);
   b[6] = static_cast<uint8_t>(v >> 56);
   b[7] = static_cast<uint8_t>(v);
}

} // namespace encoding
} // namespace orion
