//
// Huffman.h
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP2_HPACK_HUFFMAN_H
#define ORION_NET_HTTP2_HPACK_HUFFMAN_H

#include <orion/Common.h>

#include <string_view>
#include <vector>

namespace orion
{
namespace net
{
namespace http2
{
namespace hpack
{
//-------------------------------------------------------------------------------------------------
// Huffman decode flags

// FSA accepts this state as the end of huffman encoding sequence. 
static constexpr const uint8_t HUFFMAN_ACCEPTED = 1;
// This state emits symbol 
static constexpr const uint8_t HUFFMAN_SYMBOL = (1 << 1);
// If state machine reaches this state, decoding fails. 
static constexpr const uint8_t  HUFFMAN_FAIL = (1 << 2);

//-------------------------------------------------------------------------------------------------
// Huffman Encoder

class API_EXPORT HuffmanEncoder
{
public:
   HuffmanEncoder() = default;

   void encode(std::string_view str, std::vector<uint8_t>& bytes_encoded);

   void encode(Span<const uint8_t> bytes_to_encode, std::vector<uint8_t>& bytes_encoded);

private:

};

//-------------------------------------------------------------------------------------------------
// Huffman Decoder

class API_EXPORT HuffmanDecoder
{
public:
   HuffmanDecoder() = default;

   void reset();

   std::error_code decode(Span<const uint8_t> encoded_bytes,
                          std::string& decoded_bytes,
                          bool is_final = true);

private:
   struct Context
   {
      // Current huffman decoding state. We stripped leaf nodes, so the
      // value range is [0..255], inclusive.
      uint8_t state{0};
      // nonzero if we can say that the decoding process succeeds at this
      // state
      bool accept{true};
   };

   Context _ctx;
};

} // namespace hpack
} // namespace http2
} // namespace net
} // namespace orion
#endif // ORION_NET_HTTP2_HPACK_HUFFMAN_H