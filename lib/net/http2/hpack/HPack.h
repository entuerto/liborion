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

static constexpr const int STATIC_TABLE_SIZE{61};

//--------------------------------------------------------------------------------------------------
// HeaderTableError

class HeaderTableError : public orion::Exception
{
public:
   HeaderTableError(std::string text, SourceLocation src_loc)
      : Exception(std::move(text), std::move(src_loc))
   {
   }
};

//--------------------------------------------------------------------------------------------------
// HeaderTable

/// Implements the combined static and dynamic header table
///
/// See RFC7541 Section 2.3
class API_EXPORT HeaderTable 
{
public:
   /// Default maximum size of the dynamic table. See RFC7540 Section 6.5.2.
   static constexpr const uint32_t DEFAULT_SIZE{4096};

   struct Result
   {
      std::size_t index;
      std::string_view name;
      std::string_view value;
   };

   HeaderTable() = default;

   constexpr uint32_t max_size() const { return _max_size; }

   void max_size(uint32_t value);

   constexpr bool resized() const { return _resized; }

   void resized(bool value);

   // Table size
   uint64_t size() const;

   /// Returns the entry specified by index
   ///
   /// The entry will either be from the static table or the dynamic table depending 
   /// on the value of index.
   const Header& at(std::size_t index) const;

   /// Adds a new entry to the table
   /// We reduce the table size if the entry will make the table size greater than max_size.
   void add(const std::string& name, const std::string& value);

   /// Adds a new entry to the table
   /// We reduce the table size if the entry will make the table size greater than max_size.
   void add(const Header& h);

   /// Searches the table for the entry specified by name and value
   /// returns index
   std::optional<Result> find(const std::string& name, const std::string& value) const;

private:
   /// Shrinks the dynamic table to be at or below max_size
   void shrink();

private:
   uint32_t _max_size{DEFAULT_SIZE};

   uint64_t _current_size{0u};
   bool _resized{false};
	
   std::deque<Header> _entries;
};

//--------------------------------------------------------------------------------------------------
// Encoder

/// An HPACK encoder object. This object takes HTTP headers and emits encoded
/// HTTP/2 header blocks.
class API_EXPORT Encoder
{
public:
   Encoder() = default;

   /// Gets the max size of the HPACK header table.
   constexpr uint32_t header_table_size() const { return _header_table.max_size(); }

   /// Sets the max size of the HPACK header table.
   void header_table_size(uint32_t value);

   /// Takes a set of headers and encodes them into a HPACK-encoded header block.
   std::vector<uint8_t> encode(const Headers& headers, bool use_huffman);

   /// This function takes a header key-value and serializes it.
   std::vector<uint8_t> add(const Header& h, bool is_sensitive, bool use_huffman);

private:
   /// This encodes an integer according to the integer encoding rules defined in the HPACK spec.
   std::vector<uint8_t> encode_integer(uint32_t integer, uint8_t prefix_bits);

   /// Encodes a header using the indexed representation.
   std::vector<uint8_t> encode_indexed(int index);

   /// Encodes a header with a literal name and literal value. If ``indexing``
   /// is True, the header will be added to the header table: otherwise it
   /// will not.
   std::vector<uint8_t> encode_literal(const Header& h, uint8_t indexbit, bool use_huffman);

   /// Encodes a header with an indexed name and a literal value and performs incremental indexing.
   std::vector<uint8_t> encode_indexed_literal(int index,
                                               const std::string& value,
                                               uint8_t indexbit,
                                               bool use_huffman);

   /// Produces the encoded form of all header table size change context updates.
   std::vector<uint8_t> encode_table_size_change();

private:
   HeaderTable _header_table;

   HuffmanEncoder _huffman;

   std::vector<uint32_t> _table_size_changes;
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
