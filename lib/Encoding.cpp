//
// Encoding.h
//
// Copyright 2016 The libdevio Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <orion/Encoding.h>

namespace orion
{
namespace encoding
{
//---------------------------------------------------------------------------------------

uint16_t BigEndian::to_uint16(uint8_t* b)
{
   return uint16_t(b[1]) | uint16_t(b[0]) << 8;
}

uint32_t BigEndian::to_uint32(uint8_t* b)
{
   return uint32_t(b[3]) | uint32_t(b[2]) << 8 | uint32_t(b[1]) << 16 | uint32_t(b[0]) << 24;
}

uint64_t BigEndian::to_uint64(uint8_t* b)
{
   return uint64_t(b[7]) | 
          uint64_t(b[6]) <<  8 | 
          uint64_t(b[5]) << 16 | 
          uint64_t(b[4]) << 24 |
          uint64_t(b[3]) << 32 | 
          uint64_t(b[2]) << 40 | 
          uint64_t(b[1]) << 48 | 
          uint64_t(b[0]) << 56;
}

void BigEndian::put_uint16(uint16_t v, std::array<uint8_t, 2>& b)
{
   b[0] = uint8_t(v >> 8);
   b[1] = uint8_t(v);
}

void BigEndian::put_uint32(uint32_t v, std::array<uint8_t, 4>& b)
{
   b[0] = uint8_t(v >> 24);
   b[1] = uint8_t(v >> 16);
   b[2] = uint8_t(v >>  8);
   b[3] = uint8_t(v);
}

void BigEndian::put_uint64(uint64_t v, std::array<uint8_t, 8>& b)
{
   b[0] = uint8_t(v >> 56);
   b[1] = uint8_t(v >> 48);
   b[2] = uint8_t(v >> 40);
   b[3] = uint8_t(v >> 32);
   b[4] = uint8_t(v >> 24);
   b[5] = uint8_t(v >> 16);
   b[6] = uint8_t(v >>  8);
   b[7] = uint8_t(v);
}

//---------------------------------------------------------------------------------------

uint16_t LittleEndian::to_uint16(uint8_t* b)
{
   return uint16_t(b[0]) | uint16_t(b[1]) << 8;
}

uint32_t LittleEndian::to_uint32(uint8_t* b)
{
   return uint32_t(b[0]) | uint32_t(b[1]) << 8 | uint32_t(b[2]) << 16 | uint32_t(b[3]) << 24;
}

uint64_t LittleEndian::to_uint64(uint8_t* b)
{
   return uint64_t(b[0]) | 
          uint64_t(b[1]) <<  8 | 
          uint64_t(b[2]) << 16 | 
          uint64_t(b[3]) << 24 |
          uint64_t(b[4]) << 32 | 
          uint64_t(b[5]) << 40 | 
          uint64_t(b[6]) << 48 | 
          uint64_t(b[7]) << 56;
}

void LittleEndian::put_uint16(uint16_t v, std::array<uint8_t, 2>& b)
{
   b[0] = uint8_t(v);
   b[1] = uint8_t(v >> 8);
}

void LittleEndian::put_uint32(uint32_t v, std::array<uint8_t, 4>& b)
{
   b[0] = uint8_t(v);
   b[1] = uint8_t(v >>  8);
   b[2] = uint8_t(v >> 16);
   b[3] = uint8_t(v >> 24);
}

void LittleEndian::put_uint64(uint64_t v, std::array<uint8_t, 8>& b)
{
   b[0] = uint8_t(v >>  8);
   b[1] = uint8_t(v >> 16);
   b[2] = uint8_t(v >> 24);
   b[3] = uint8_t(v >> 32);
   b[4] = uint8_t(v >> 40);
   b[5] = uint8_t(v >> 48);
   b[6] = uint8_t(v >> 56);
   b[7] = uint8_t(v);
}

} // namespace encoding
} // namespace orion
