//
// Encoding.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_ENCODING_H
#define ORION_ENCODING_H

#include <orion/Common.h>

#include <array>
#include <cstdint>

namespace orion
{
namespace encoding
{
//---------------------------------------------------------------------------------------

struct API_EXPORT BigEndian
{
   static uint16_t to_uint16(Span<const uint8_t> b);
   static uint32_t to_uint32(Span<const uint8_t> b);
   static uint64_t to_uint64(Span<const uint8_t> b);

   static void put_uint16(uint16_t v, Span<uint8_t> b);
   static void put_uint32(uint32_t v, Span<uint8_t> b);
   static void put_uint64(uint64_t v, Span<uint8_t> b);
};

//---------------------------------------------------------------------------------------

struct API_EXPORT LittleEndian
{
   static uint16_t to_uint16(Span<const uint8_t> b);
   static uint32_t to_uint32(Span<const uint8_t> b);
   static uint64_t to_uint64(Span<const uint8_t> b);

   static void put_uint16(uint16_t v, Span<uint8_t> b);
   static void put_uint32(uint32_t v, Span<uint8_t> b);
   static void put_uint64(uint64_t v, Span<uint8_t> b);
};

// __llvm__
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
using ByteOrder = LittleEndian;
#else
using ByteOrder =  BigEndian;
#endif

//---------------------------------------------------------------------------------------

static const int MaxVarintLen16 = 3;
static const int MaxVarintLen32 = 5;
static const int MaxVarintLen64 = 10;

/// Encodes an int into output and returns the number of bytes written.
template<typename T = uint64_t>
int enc_varint(T value, Span<uint8_t> output)
{
   auto uvalue = static_cast<typename std::make_unsigned<T>::type>(value);

   int i = 0;

   while (uvalue >= 0x80)
   {
      output[i] = static_cast<uint8_t>(uvalue) | 0x80;
      // Remove the seven bits we just wrote
      uvalue >>= 7;
      i++;
   }

   output[i] = static_cast<uint8_t>(uvalue);
   return i + 1;
}

/// Decodes a int from input and returns that value and the
/// number of bytes read (> 0).
template<typename T = uint64_t>
T dec_varint(Span<uint8_t> input, int in_size)
{
   T ret = 0;

   for (int i = 0; i < in_size; i++)
   {
      if (input[i] < 0x80)
         return ret | static_cast<T>(input[i]) << (i * 7);

      ret |= static_cast<T>((input[i] & 0x7f)) << (i * 7);
   }

   return ret;
}

} // namespace encoding
} // namespace orion

#endif // ORION_ENCODING_H
