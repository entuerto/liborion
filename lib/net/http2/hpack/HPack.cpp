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

#include <map>

namespace orion
{
namespace net
{
namespace http2
{
namespace hpack
{

static constexpr auto PREFIX_BIT_MAX_NUMBERS =
   std::array<uint8_t, 9>{{0, 1, 3, 7, 15, 31, 63, 127, 255}};

//--------------------------------------------------------------------------------------------------
//
static constexpr const int HPACK_STATIC_TABLE_SIZE{61};

// Constant list of static headers. See RFC7541 Section 2.3.1 and Appendix A
static const std::array<Header, HPACK_STATIC_TABLE_SIZE> STATIC_TABLE = {
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



//--------------------------------------------------------------------------------------------------
// HeaderTable implementation

void HeaderTable::max_size(uint32_t value)
{
   log::debug2(fmt::format("Resizing header table to {} from {}", value, _max_size));

   _max_size = value;

   shrink();
}

uint64_t HeaderTable::size() const
{
   uint64_t s = 0;

   for (const auto& item : _entries)
   {
      uint64_t nl = item.name.length();
      uint64_t vl = item.value.length();
      uint64_t tl = 0;

      if (vl > std::numeric_limits<uint64_t>::max() or
          nl > std::numeric_limits<uint64_t>::max() - vl)
         throw std::runtime_error("HeaderTable::size() Additive integer overflow encountered");

      tl = nl + vl;

      if (tl > std::numeric_limits<uint64_t>::max() - s)
         throw std::runtime_error("HeaderTable::size() Additive integer overflow encountered");

      s += tl;
   }

   return s;
}

const Header& HeaderTable::at(std::size_t idx) const
{
   // Adjust for 0 base arrays
   --idx;

   if (idx > _entries.size())
   {
      if (idx < HPACK_STATIC_TABLE_SIZE)
      {   
         throw std::runtime_error{"HeaderTable::at(): Invalid/out-of-bounds index specified"};
      }

      idx -= HPACK_STATIC_TABLE_SIZE;

      if (idx > _entries.size())
      {
         throw std::runtime_error{"HeaderTable::at(): Invalid/out-of-bounds index specified"};
      }
   }
   return ::orion::at(_entries, idx);
}

const Header& HeaderTable::header(std::size_t idx) const
{
   // Adjust for 0 base arrays
   --idx;

   if (idx < STATIC_TABLE.size())
   {
      return STATIC_TABLE.at(idx);
   }
   
   if (idx > STATIC_TABLE.size() and idx < STATIC_TABLE.size() + _entries.size())
   {
      return _entries.at(idx - STATIC_TABLE.size());
   }

   throw std::runtime_error("HPACK::ringtable_t::get_header(): Invalid index/header not found");
}

void HeaderTable::add(const std::string& name, const std::string& value)
{
   const auto size = name.length() + value.length();

   // We just clear the table if the entry is too big
   if (size > _max_size)
      throw std::runtime_error("HeaderTable::add(): Entry is too big.");

   shrink();

   _entries.emplace_front(Header{name, value});
}

std::optional<int> HeaderTable::find(const std::string& name, const std::string& value) const
{
   int idx = 0;

   for (const auto& item : _entries)
   {
      if (item.name == name and item.value == value)
      {
         return STATIC_TABLE.size() + idx;
      }
      
      if (item.name == name)
      {
         return STATIC_TABLE.size() + idx;
      }
      ++idx;
   }
   return {};
}

void HeaderTable::shrink()
{
   while (size() >= _max_size)
   {
      _entries.pop_back();
   }
}

//--------------------------------------------------------------------------------------------------
// Encoder implementation

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
   if (prefix_bits < 1 or prefix_bits > 8)
   {
      throw std::invalid_argument("ValueError(Prefix bits must be between 1 and 8)");
   }

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

   const auto& h = _header_table.header(index);

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
		throw std::runtime_error("InvalidTableSizeError: Encoder exceeded max allowable table size");
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

		name = _header_table.header(index).name;

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
