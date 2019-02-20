//
// HPack.cpp
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include "HPack.h"

#include <orion/Log.h>
#include <orion/net/http2/Error.h>

#include <fmt/format.h>

#include <iterator>

namespace orion
{
namespace net
{
namespace http2
{
namespace hpack
{

template<class From, class To>
inline constexpr void insert_back(const From& from, To& to)
{
   std::copy(std::begin(from), std::end(from), std::back_inserter(to));
}

static constexpr const uint8_t INDEX_NEVER = 0x10;
static constexpr const uint8_t INDEX_INCREMENTAL = 0x40;

static constexpr auto PREFIX_BIT_MAX_NUMBERS =
   std::array<uint8_t, 9>{{0, 1, 3, 7, 15, 31, 63, 127, 255}};

//--------------------------------------------------------------------------------------------------
//

// Constant list of static headers. See RFC7541 Section 2.3.1 and Appendix A
static const std::array<Header, STATIC_TABLE_SIZE> STATIC_TABLE = {
   {{":authority", ""},
    {":method", "GET"},
    {":method", "POST"},
    {":path", "/"},
    {":path", "/index.html"},
    {":scheme", "http"},
    {":scheme", "https"},
    {":status", "200"},
    {":status", "204"},
    {":status", "206"},
    {":status", "304"},
    {":status", "400"},
    {":status", "404"},
    {":status", "500"},
    {"accept-charset", ""},
    {"accept-encoding", "gzip, deflate"},
    {"accept-language", ""},
    {"accept-ranges", ""},
    {"accept", ""},
    {"access-control-allow-origin", ""},
    {"age", ""},
    {"allow", ""},
    {"authorization", ""},
    {"cache-control", ""},
    {"content-disposition", ""},
    {"content-encoding", ""},
    {"content-language", ""},
    {"content-length", ""},
    {"content-location", ""},
    {"content-range", ""},
    {"content-type", ""},
    {"cookie", ""},
    {"date", ""},
    {"etag", ""},
    {"expect", ""},
    {"expires", ""},
    {"from", ""},
    {"host", ""},
    {"if-match", ""},
    {"if-modified-since", ""},
    {"if-none-match", ""},
    {"if-range", ""},
    {"if-unmodified-since", ""},
    {"last-modified", ""},
    {"link", ""},
    {"location", ""},
    {"max-forwards", ""},
    {"proxy-authenticate", ""},
    {"proxy-authorization", ""},
    {"range", ""},
    {"referer", ""},
    {"refresh", ""},
    {"retry-after", ""},
    {"server", ""},
    {"set-cookie", ""},
    {"strict-transport-security", ""},
    {"transfer-encoding", ""},
    {"user-agent", ""},
    {"vary", ""},
    {"via", ""},
    {"www-authenticate", ""}}};

//-------------------------------------------------------------------------------------------------

// Calculates the size of a single entry
// 
// The 32 extra bytes are considered the "maximum" overhead that would be required to 
// represent each entry in the table.
//
// See RFC7541 Section 4.1
inline constexpr uint64_t table_entry_size(std::string_view name, std::string_view value)
{
   return name.size() + value.size() + 32;
}

//-------------------------------------------------------------------------------------------------
// HeaderTable implementation

void HeaderTable::max_size(uint32_t value)
{
   log::debug2(fmt::format("Resizing header table to {} from {}", value, _max_size));

   _resized = _max_size != value;

   _max_size = value;

   if (value <= 0)
   {
      _entries.clear();
      _current_size = 0u;
   }

   shrink();
}

uint64_t HeaderTable::size() const
{
   return _current_size;
}

void HeaderTable::resized(bool value)
{
   _resized = value;
}

const Header& HeaderTable::at(std::size_t idx) const
{
   // Adjust for 0 base arrays
   --idx;

   if (idx < STATIC_TABLE.size())
   {
      return STATIC_TABLE.at(idx);
   }
   
   idx -= STATIC_TABLE.size();
   if (idx < _entries.size())
   {
      return _entries.at(idx);
   }

   throw_exception<HeaderTableError>(fmt::format("Invalid table index {}", idx + 1), DbgSrcLoc);
}

void HeaderTable::add(const std::string& name, const std::string& value)
{
   const auto size = table_entry_size(name, value);

   // We just clear the table if the entry is too big
   if (size > _max_size)
   {
      _entries.clear();
      _current_size = 0u;
      return;
   }

   // Add new entry
   _entries.emplace_front(Header{name, value});
   _current_size += size;

   shrink();
}

void HeaderTable::add(const Header& h)
{
   add(h.name, h.value);
}

std::optional<HeaderTable::Result> HeaderTable::find(const std::string& name, const std::string& value) const
{
   std::optional<Result> partial;

   std::size_t idx = 1;

   for (const auto& item : STATIC_TABLE)
   {
      if (item.name == name and item.value == value)
      {
         return Result{idx, item.name, item.value};
      }
      
      if (item.name == name)
      {
         partial = Result{idx, item.name, {}};
         break;
      }
      ++idx;
   }

   idx = STATIC_TABLE_SIZE + 1;
   for (const auto& item : _entries)
   {
      if (item.name == name and item.value == value)
      {
         return Result{idx, item.name, item.value};
      }

      if (item.name == name)
      {
         return Result{idx, item.name, {}};
      }
   }

   return partial;
}

void HeaderTable::shrink()
{
   while (_current_size >= _max_size)
   {
      const auto& h = _entries.back();

      _current_size -= table_entry_size(h.name, h.value);

      log::debug2(fmt::format("Evicting {}: {} from the header table", h.name, h.value));

      _entries.pop_back();
   }
}

//--------------------------------------------------------------------------------------------------
// Encoder implementation

// Controls the size of the HPACK header table.
void Encoder::header_table_size(uint32_t value)
{
   _header_table.max_size(value);

   if (_header_table.resized())
   {
      _table_size_changes.emplace_back(value);
   }
}

/// Takes a set of headers and encodes them into a HPACK-encoded header block.
std::vector<uint8_t> Encoder::encode(const Headers& headers, bool use_huffman)
{
   std::vector<uint8_t> encoded;

   // Before we begin, if the header table size has been changed we need
   // to signal all changes since last emission appropriately.
   if (_header_table.resized())
   {
      insert_back(encode_table_size_change(), encoded);

      _header_table.resized(false);
   }
            
   // Add each header to the header block
   for (const auto& header : headers)
	{
		auto enc_header = add(header, not header.indexable, use_huffman);

		insert_back(enc_header, encoded);
	}

   return encoded;
}

/// This function takes a header key-value and serializes it.
std::vector<uint8_t> Encoder::add(const Header& h, bool is_sensitive, bool use_huffman)
{
   // Set our indexing mode
   const uint8_t indexbit = (is_sensitive) ? INDEX_NEVER : INDEX_INCREMENTAL;

   // Search for a matching header in the header table.
   auto result = _header_table.find(h.name, h.value);

   if (not result)
   {
      // Not in the header table. Encode using the literal syntax,
      // and add it to the header table.
      if (not is_sensitive)
      {
         _header_table.add(h.name, h.value);
      }
      return encode_literal(h, indexbit, use_huffman);
   }

   // The header is in the table, break out the values. If we matched
   // perfectly, we can use the indexed representation: otherwise we
   // can use the indexed literal.
   if (not result->value.empty())
   {
      return encode_indexed(result->index);
   }

   if (not is_sensitive)
   {
      _header_table.add(h.name, h.value);
   }
   return encode_indexed_literal(result->index, h.value, indexbit, use_huffman);
}

// This encodes an integer according to the integer encoding rules defined in the HPACK spec.
std::vector<uint8_t> Encoder::encode_integer(uint32_t integer, uint8_t prefix_bits)
{
   Expects(prefix_bits >= 1 and prefix_bits <= 8);

   const auto& max_number = PREFIX_BIT_MAX_NUMBERS[prefix_bits];

   if (integer < max_number)
   {
      return {static_cast<uint8_t>(integer)};
   }

   std::vector<uint8_t> enc_bytes{max_number};
   integer -= max_number;

   while (integer >= 128)
   {
      enc_bytes.emplace_back((integer & 0x7f) + 0x80);
      integer >>= 7;
   }

   enc_bytes.emplace_back(integer);

   return enc_bytes;
}

/// Encodes a header using the indexed representation.
std::vector<uint8_t> Encoder::encode_indexed(int index)
{
	auto field = encode_integer(index, 7);
	field[0] |= 0x80;

	return field;
}

/// Encodes a header with a literal name and literal value. If ``indexing``
/// is True, the header will be added to the header table: otherwise it
/// will not.
std::vector<uint8_t> Encoder::encode_literal(const Header& h, uint8_t indexbit, bool use_huffman)
{
   std::vector<uint8_t> enc_bytes{indexbit};

   if (use_huffman)
   {
      std::vector<uint8_t> enc_nv;

      // Encode name
      _huffman.encode(h.name, enc_nv);

      auto name_len  = encode_integer(enc_nv.size(), 7);
      name_len[0]  |= 0x80;

      insert_back(name_len, enc_bytes);
      insert_back(enc_nv, enc_bytes);

      enc_nv.clear();

      // Encode value
      _huffman.encode(h.value, enc_nv);

      auto value_len = encode_integer(enc_nv.size(), 7);
      value_len[0] |= 0x80;

      insert_back(value_len, enc_bytes);
      insert_back(enc_nv, enc_bytes);
      
      return enc_bytes;
   }

   auto name_len  = encode_integer(h.name.size(), 7);
   auto value_len = encode_integer(h.value.size(), 7);

   insert_back(name_len, enc_bytes);
   insert_back(h.name, enc_bytes);
   insert_back(value_len, enc_bytes);
   insert_back(h.value, enc_bytes);

   return enc_bytes;
}

/// Encodes a header with an indexed name and a literal value and performs incremental indexing.
std::vector<uint8_t> Encoder::encode_indexed_literal(int index,
                                                     const std::string& value,
                                                     uint8_t indexbit,
                                                     bool use_huffman)
{
   std::vector<uint8_t> prefix =
      (indexbit != INDEX_INCREMENTAL) ? encode_integer(index, 4) : encode_integer(index, 6);

   prefix[0] |= indexbit;

   if (use_huffman)
   {
      std::vector<uint8_t> enc_value;

      _huffman.encode(value, enc_value);

      auto enc_value_len = encode_integer(enc_value.size(), 7);
      enc_value_len[0] |= 0x80;

      insert_back(enc_value_len, prefix);
      insert_back(enc_value, prefix);

      return prefix;
   }

   auto value_len = encode_integer(value.size(), 7);

   insert_back(value_len, prefix);
   insert_back(value, prefix);

   return prefix;
}

/// Produces the encoded form of all header table size change context updates.
std::vector<uint8_t> Encoder::encode_table_size_change()
{
	std::vector<uint8_t> block;

	for (const auto& table_size : _table_size_changes)
	{
		std::vector<uint8_t> size_bytes = encode_integer(table_size, 5);
		size_bytes[0] |= 0x20;

		insert_back(size_bytes, block);
	}

	_table_size_changes.clear();

	return block;
}

//--------------------------------------------------------------------------------------------------
// Decoder implementation

/// This decodes an integer according to the wacky integer encoding rules
/// defined in the HPACK spec.
///
/// Returns a pair of the decoded integer  
///    first  = number decoded
///    second = consumed bytes
std::pair<int, int> Decoder::decode_integer(Span<const uint8_t> data, int prefix_bits)
{
   Expects(prefix_bits >= 1 and prefix_bits <= 8);

   const auto& max_number = PREFIX_BIT_MAX_NUMBERS[prefix_bits];

   int index = 1;
   int shift = 0;

   const uint8_t mask = (0xFF >> (8 - prefix_bits));

   int number = data[0] & mask;

   if (number == max_number)
   {
      while (true)
      {
         const uint8_t& next_byte = data[index];
         index++;

         if (next_byte >= 128)
         {
            number += ((next_byte - 128) << shift);
         }
         else
         {
            number += (next_byte << shift);
            break;
         }

         shift += 7;
      }
   }

   log::debug2(fmt::format("Decoded {}, consumed {} bytes", number, index));

   return {number, index};
}

/// Decodes a header represented using the indexed representation.
/// Returns a pair of the decoded header  
///    first  = hader
///    second = consumed bytes
std::pair<Header, int> Decoder::decode_indexed(Span<const uint8_t> data)
{
   const auto [index, consumed] = decode_integer(data, 7);

   const auto& h = _header_table.at(index);

   //log::debug2(fmt::format("Decoded {}, consumed {}", h, consumed));

   return {h, consumed};
}

Decoder::Result Decoder::decode(Span<const uint8_t> data)
{
   Headers res_headers;

   if (data.empty())
   {
      return {res_headers, {}};
   }

   std::size_t inflated_size = 0;

   int cur_idx = 0;

   while (cur_idx < data.size())
   {
      auto current = data[cur_idx];

      Header header{};
      int consumed = 0;

      // Determine what kind of header we're decoding.
      // If the high bit is 1, it's an indexed field.
      const bool indexed = (current & 0x80);

      // Otherwise, if the second-highest bit is 1 it's a field that does
      // alter the header table.
      const bool literal_index = (current & 0x40);

      // Otherwise, if the third-highest bit is 1 it's an encoding context update.
      const bool encoding_update = (current & 0x20);

      if (indexed)
      {
         auto [h, c] = decode_indexed(data.subspan(cur_idx));
         header = h;
         consumed = c;
      }
      else if (literal_index)
      {
         // It's a literal header that does affect the header table.
         auto [h, c] = decode_literal(data.subspan(cur_idx), true);
         header = h;
         consumed = c;
      }
      else if (encoding_update)
      {
         // It's an update to the encoding context. These are forbidden
         // in a header block after any actual header.
         if (not res_headers.empty())
         {
            return {{}, make_error_code(ErrorCode::HeaderComp)};
         }

         consumed = update_encoding_context(data.subspan(cur_idx));
      }
      else
      {
         // It's a literal header that does not affect the header table.
         auto [h, c] = decode_literal(data.subspan(cur_idx), false);
         header = h;
         consumed = c;
      }

      if (header.size() != 0)
      {
         inflated_size += header.size();

         res_headers.emplace_back(std::move(header));

         if (inflated_size > _max_header_list_size)
         {
            // A header list larger than _max_header_list_size has been received
            return {{}, make_error_code(ErrorCode::HeaderComp)};
         }
      }

      cur_idx += consumed;
   }

   return {res_headers, {}};
}

/// Handles a byte that updates the encoding context.
int Decoder::update_encoding_context(Span<const uint8_t> data)
{
	const auto [new_size, consumed] = decode_integer(data, 5);

	if (new_size > _max_header_list_size)
	{
      throw_exception<HeaderTableError>("Encoder exceeded max allowable table size", DbgSrcLoc);
	}

	_header_table.max_size(new_size);

	return consumed;
}

/// Decodes a header represented with a literal.
///
/// Returns a pair of the decoded header  
///    first  = hader
///    second = consumed bytes
std::pair<Header, int> Decoder::decode_literal(Span<const uint8_t> data, bool should_index)
{
   int total_consumed = 0;
   int name_len       = 0;
   bool indexed_name  = false;
   bool indexable     = true;

   // When should_index is true, if the low six bits of the first byte are
   // nonzero, the header name is indexed.
   // When should_index is false, if the low four bits of the first byte
   // are nonzero the header name is indexed.
   if (should_index)
   {
      name_len      = 6;
      indexed_name  = (data[0] & 0x3f) ? true : false;
      indexable     = true;
   }
   else
   {
      const uint8_t high_byte = data[0];

      name_len      = 4;
      indexed_name  = (high_byte & 0x0f) ? true : false;
      indexable     = (high_byte & 0x10) ? false : true;
   }

	std::string name;
	int length = 0;
	int consumed = 0;

	if (indexed_name)
	{
      // Indexed header name.
		const auto [index, pos] = decode_integer(data, name_len);
		consumed = pos;

		name = _header_table.at(index).name;

		total_consumed = consumed;
	}
	else
	{
      // Literal header name. The first byte was consumed, so we need to move forward
		data = data.subspan(1);

		const auto [number, index] = decode_integer(data, 7);
		length = number;
		consumed = index;

		if ((data[0] & 0x80))
		{
         _huffman.reset();
         _huffman.decode(data.subspan(consumed, length), name);
		}
		else
		{
         auto sp = data.subspan(consumed, length);
			name = std::string{sp.begin(), sp.end()};
		}

		total_consumed = consumed + length + 1;
	}

	data = data.subspan(consumed + length);

	std::string value;

   // The header value is definitely length-based.
	const auto [number, index] = decode_integer(data, 7);
	length = number;
	consumed = index;

   if ((data[0] & 0x80))
   {
      _huffman.reset();
      _huffman.decode(data.subspan(consumed, length), value);
   }
   else
   {
      auto sp = data.subspan(consumed, length);
      value   = std::string{sp.begin(), sp.end()};
   }

   // Updated the total consumed length.
	total_consumed += length + consumed;

   // If we've been asked to index this, add it to the header table.
	if (should_index)
	{
		_header_table.add(name, value);
	}

   return {Header{name, value, indexable}, total_consumed};
}

} // namespace hpack
} // namespace http2
} // namespace net
} // namespace orion
