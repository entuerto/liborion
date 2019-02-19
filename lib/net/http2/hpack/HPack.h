//
// HPack.h
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP2_HPACK_HPACK_H
#define ORION_NET_HTTP2_HPACK_HPACK_H

#include <orion/Common.h>

#include "Huffman.h"

#include <deque>
#include <optional>
#include <vector>

namespace orion
{
namespace net
{
namespace http2
{
namespace hpack
{

//--------------------------------------------------------------------------------------------------
// Header

struct Header
{
   std::string name;
   std::string value;
   bool indexable{true};

   std::size_t size() const { return name.size() + value.size(); }
};

inline bool operator==(const Header& h1, const Header& h2) noexcept
{
   return h1.name == h2.name and h1.value == h2.value and h1.indexable == h2.indexable;
}

inline bool operator!=(const Header& h1, const Header& h2) noexcept
{
   return not (h1 == h2);
}

//--------------------------------------------------------------------------------------------------
// Headers

using Headers = std::vector<Header>;

inline bool operator==(const Headers& h1, const Headers& h2) noexcept
{
   return std::equal(std::begin(h1), std::end(h1), std::begin(h2), std::end(h2));
}

inline bool operator!=(const Headers& h1, const Headers& h2) noexcept
{
   return not (h1 == h2);
}

//--------------------------------------------------------------------------------------------------
//

/*
struct Result
{
   std::optional<std::string> value;
   std::error_code error;
};
*/

//--------------------------------------------------------------------------------------------------
// HeaderTable

/// Implements the combined static and dynamic header table
class API_EXPORT HeaderTable 
{
public:
   /// Default maximum size of the dynamic table. See RFC7540 Section 6.5.2.
   static constexpr const uint32_t DEFAULT_SIZE{4096};

   HeaderTable() = default;

   constexpr uint32_t max_size() const { return _max_size; }

   void max_size(uint32_t value);

   // Table size
   uint64_t size() const;

   /// Returns the entry specified by index
   /// The entry will either be from the static table or 
   /// the dynamic table depending on the value of index.
   const Header& at(std::size_t index) const;

   const Header& header(std::size_t index) const;

   /// Adds a new entry to the table
   /// We reduce the table size if the entry will make the table size greater than max_size.
   void add(const std::string& name, const std::string& value);

   /// Searches the table for the entry specified by name and value
   /// returns index
   std::optional<int> find(const std::string& name, const std::string& value) const;

private:
   /// Shrinks the dynamic table to be at or below maxsize
   void shrink();

private:
   uint32_t _max_size{DEFAULT_SIZE};
	
   std::deque<Header> _entries;
};

//--------------------------------------------------------------------------------------------------
// Encoder

/// An HPACK encoder object. This object takes HTTP headers and emits encoded
/// HTTP/2 header blocks.
class API_EXPORT Encoder
{
public:
   using DataIterator = Span<const uint8_t>::iterator;

   Encoder() = default;


private:
   HeaderTable _header_table;
};

//--------------------------------------------------------------------------------------------------
// Decoder

/// An HPACK decoder object.
class API_EXPORT Decoder
{
public:
   struct Result
   {
      Headers headers;
      std::error_code error;
   };

   Decoder() = default;

   /// Decodes the HTTPv2 Header Block contained within the parameter
   ///
   /// @param data The HTTPv2 Header Block
   /// 
   Result decode(Span<const uint8_t> data);

private:
   /// This decodes an integer according to the wacky integer encoding rules
   /// defined in the HPACK spec.
   ///
   /// Returns a pair of the decoded integer  
   ///    first  = number decoded
   ///    second = consumed bytes
   std::pair<int, int> decode_integer(Span<const uint8_t> data, int prefix_bits);

   /// Decodes a header represented using the indexed representation.
   /// Returns a pair of the decoded header  
   ///    first  = hader
   ///    second = consumed bytes
   std::pair<Header, int> decode_indexed(Span<const uint8_t> data);

   /// Decodes a header represented with a literal.
   ///
   /// Returns a pair of the decoded header
   ///    first  = hader
   ///    second = consumed bytes
   std::pair<Header, int> decode_literal(Span<const uint8_t> data, bool should_index);

   /// Handles a byte that updates the encoding context.
   int update_encoding_context(Span<const uint8_t> data);

private:
   uint32_t _max_header_list_size{65535u}; // Change

   HeaderTable _header_table;

   HuffmanDecoder _huffman;
};

} // namespace hpack
} // namespace http2
} // namespace net
} // namespace orion
#endif // ORION_NET_HTTP2_HPACK_HPACK_H
