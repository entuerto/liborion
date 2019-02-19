//
// Huffman.cpp
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include "Huffman.h"

#include "Huffman-tables.h"

#include <orion/Log.h>
#include <orion/net/http2/Error.h>

#include <fmt/format.h>


namespace orion
{
namespace net
{
namespace http2
{
namespace hpack
{

void HuffmanEncoder::encode(std::string_view str, std::vector<uint8_t>& bytes_encoded)
{
   encode(make_span(reinterpret_cast<const uint8_t*>(str.data()), str.size()), bytes_encoded);
}

void HuffmanEncoder::encode(Span<const uint8_t> bytes_to_encode, std::vector<uint8_t>& bytes_encoded)
{
   if (bytes_to_encode.empty())
   {
      return;
   }

   uint64_t code = 0;
   int bits      = 0;

   // Turn each byte into its huffman code. These codes aren't necessarily
   // octet aligned, so keep track of how far through an octet we are. To
   // handle this cleanly, just use a single giant integer.
   for (const auto& c : bytes_to_encode)
   {
      HuffmanSymbol symbol = HUFFMAN_SYMBOL_TABLE[c];

      code  = symbol.code | code << symbol.nbits;
      bits += symbol.nbits;

      if (bits >= 32)
      {
         uint32_t part = code >> (bits -= 32);

         bytes_encoded.emplace_back(static_cast<uint8_t>(part >> 24));
         bytes_encoded.emplace_back(static_cast<uint8_t>(part >> 16));
         bytes_encoded.emplace_back(static_cast<uint8_t>(part >> 8));
         bytes_encoded.emplace_back(static_cast<uint8_t>(part));
      }
   }

   // At most 31 bits left to flush
   for ( ; bits >= 8; bits -= 8)
   {
      bytes_encoded.emplace_back(static_cast<uint8_t>(code >> (bits - 8)));
   }
   
   // The remaining byte must be padded with ones
   if (bits)
   { 
      bytes_encoded.emplace_back(static_cast<uint8_t>(0xff >> bits | code << (8 - bits)));
   }
}

//-------------------------------------------------------------------------------------------------
// Decoder implementation

void HuffmanDecoder::reset()
{
   _ctx.state = 0;
   _ctx.accept = true;
}

std::error_code HuffmanDecoder::decode(Span<const uint8_t> encoded_bytes,
                                       std::string& decoded_bytes,
                                       bool is_final /*  = true */)
{
   if (encoded_bytes.empty())
      return {};

   // We use the decoding algorithm described in http://graphics.ics.uci.edu/pub/Prefix.pdf
   for (const auto& input_byte : encoded_bytes)
   {
      const HuffmanDecodeEntry* table_entry = &HUFFMAN_DECODE_TABLE[_ctx.state][input_byte >> 4];

      if (table_entry->flags & HUFFMAN_FAIL)
      {
         return make_error_code(ErrorCode::HeaderComp);
      }
      if (table_entry->flags & HUFFMAN_SYMBOL)
      {
         decoded_bytes.push_back(table_entry->sym);
      }

      table_entry = &HUFFMAN_DECODE_TABLE[table_entry->state][input_byte & 0xf];
      if (table_entry->flags & HUFFMAN_FAIL)
      {
         return make_error_code(ErrorCode::HeaderComp);
      }
      if (table_entry->flags & HUFFMAN_SYMBOL)
      {
         decoded_bytes.push_back(table_entry->sym);
      }

      _ctx.state  = table_entry->state;
      _ctx.accept = (table_entry->flags & HUFFMAN_ACCEPTED) != 0;
   }

   if (is_final and not _ctx.accept)
   {
      return make_error_code(ErrorCode::HeaderComp);
   }
   return {};
}

} // namespace hpack
} // namespace http2
} // namespace net
} // namespace orion
