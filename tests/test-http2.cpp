//
//  test-http2.cpp
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Log.h>
#include <orion/Test.h>
#include <orion/net/http2/Error.h>
#include <orion/net/http2/Frame.h>
#include <orion/net/http2/Server.h>
#include <orion/net/http2/Settings.h>

#include <net/http2/hpack/HPack.h>
#include <net/http2/hpack/Huffman.h>

using namespace orion;
using namespace orion::net;
using namespace orion::net::http2;
using namespace orion::unittest;

using namespace std::string_literals;

Section(OrionNet_http2Address, Label{"HTTPv2"})
{

TestCase("Settings - Default contruction")
{
   Settings s;

   check_eq(s.get<HeaderTableSize>(), DEFAULT_SETTINGS_HEADER_TABLE_SIZE);
   check_eq(s.get<EnablePush>(), DEFAULT_SETTINGS_ENABLE_PUSH);
   check_eq(s.get<MaxConcurrentStreams>(), DEFAULT_SETTINGS_MAX_CONCURRENT_STREAMS);
   check_eq(s.get<InitialWindowSize>(), DEFAULT_SETTINGS_INITIAL_WINDOW_SIZE);
   check_eq(s.get<MaxFrameSize>(), DEFAULT_SETTINGS_MAX_FRAME_SIZE);
   check_eq(s.get<MaxHeaderListSize>(), DEFAULT_SETTINGS_MAX_HEADER_LIST_SIZE);
}

TestCase("Settings - Contruction")
{
   Settings s{HeaderTableSize{5000}};

   check_eq(s.get<HeaderTableSize>(), HeaderTableSize{5000});

   Settings s2{HeaderTableSize{5000}, EnablePush{false}};

   check_eq(s2.get<HeaderTableSize>(), HeaderTableSize{5000});
   check_eq(s2.get<EnablePush>(), EnablePush{false});

   Settings s3{HeaderTableSize{5000}, EnablePush{false}, MaxConcurrentStreams{200}};

   check_eq(s3.get<HeaderTableSize>(), HeaderTableSize{5000});
   check_eq(s3.get<EnablePush>(), EnablePush{false});
   check_eq(s3.get<MaxConcurrentStreams>(), MaxConcurrentStreams{200});
}

TestCase("Settings - Valid InitialWindowSize")
{
   Settings s{};

   check_true(Settings::is_valid(s.get<InitialWindowSize>()));

   s.set(InitialWindowSize{2147483647});

   check_true(Settings::is_valid(s.get<InitialWindowSize>()));

   check_false(Settings::is_valid(InitialWindowSize{2147483648}));
}

TestCase("Settings - Valid MaxFrameSize")
{
   Settings s{};

   check_true(Settings::is_valid(s.get<MaxFrameSize>()));

   s.set(MaxFrameSize{16384});

   check_true(Settings::is_valid(s.get<MaxFrameSize>()));

   check_true(Settings::is_valid(MaxFrameSize{16384}));
   check_true(Settings::is_valid(MaxFrameSize{16777215}));

   check_false(Settings::is_valid(MaxFrameSize{16383}));
   check_false(Settings::is_valid(MaxFrameSize{16777216}));
}

TestCase("Frame - Contruction")
{
   Frame f{FrameType::DATA, 0u};

   check_eq(f.type(), FrameType::DATA);
   check_eq(f.stream_id(), 0u);
   check_eq(f.length(), 0u);

   Frame f2{FrameType::HEADERS, 1u, FrameFlags::ACK};

   check_eq(f2.type(), FrameType::HEADERS);
   check_eq(f2.stream_id(), 1u);
   check_eq(f2.flags(), FrameFlags::ACK);
   check_eq(f2.length(), 0u);

   Frame f3{FrameType::HEADERS, 1u, FrameFlags::ACK | FrameFlags::END_HEADERS};

   check_eq(f3.type(), FrameType::HEADERS);
   check_eq(f3.stream_id(), 1u);
   check_eq(f3.flags(), FrameFlags::ACK | FrameFlags::END_HEADERS);
   check_eq(f3.length(), 0u);
}

TestCase("Frame - fields")
{
   Frame f{FrameType::DATA, 0u};

   check_eq(f.type(), FrameType::DATA);
   check_eq(f.stream_id(), 0u);

   f.length(100u);

   check_eq(f.length(), 100u);

   f.flags(FrameFlags::END_HEADERS);

   check_eq(f.flags(), FrameFlags::END_HEADERS);

   f.flags(FrameFlags::ACK | FrameFlags::END_HEADERS);

   check_eq(f.flags(), FrameFlags::ACK | FrameFlags::END_HEADERS);
}

TestCase("Frame - encode/decode")
{
   std::error_code ec;
   Settings s;
   std::array<uint8_t, 10> data;

   Frame f{FrameType::DATA, 1000u, data};

   std::array<uint8_t, 100> buffer;

   auto n = Frame::encode(buffer, f);

   check_eq(n, 19u);

   Frame f2{};

   n = Frame::decode(s, buffer, f2, ec);

   check_eq(n, 19u);

   check_eq(f.length(), f2.length());
   check_eq(f.type(), f2.type());
   check_eq(f.flags(), f2.flags());
   check_eq(f.stream_id(), f2.stream_id());
   check_eq(f.get().size(), f2.get().size());

}

struct TestHuffmanData
{
   std::string text;
   std::vector<uint8_t> raw;
};

static const std::array<TestHuffmanData, 4> TestHuffmanDecEnc{
   TestHuffmanData{"www.example.com", {0xf1, 0xe3, 0xc2, 0xe5, 0xf2, 0x3a, 0x6b, 0xa0, 0xab, 0x90, 0xf4, 0xff}},
   TestHuffmanData{"no-cache", {0xa8, 0xeb, 0x10, 0x64, 0x9c, 0xbf}},
   TestHuffmanData{"custom-key", {0x25, 0xa8, 0x49, 0xe9, 0x5b, 0xa9, 0x7d, 0x7f}},
   TestHuffmanData{"custom-value", {0x25, 0xa8, 0x49, 0xe9, 0x5b, 0xb8, 0xe8, 0xb4, 0xbf}}
};

TestCase("Huffman - Decode")
{
   hpack::HuffmanDecoder dec;

   std::string value;

   for (const auto& data : TestHuffmanDecEnc)
   {
      dec.reset(); 
      value.clear();

      auto ec = dec.decode(data.raw, value);

      fail_if(ec, DbgSrcLoc);

      check_eq(data.text, value);
   }
}

TestCase("Huffman - Decode invalid input")
{
   const auto expected_ec = http2::make_error_code(http2::ErrorCode::HeaderComp);

   // Invalid 1
   std::array<uint8_t, 1> data{0xff};
   std::string value;

   hpack::HuffmanDecoder dec;

   auto ec = dec.decode(data, value);

   check_eq(expected_ec, ec);

   // Invalid 2
   std::array<uint8_t, 5> data2{0x5f, 0xff, 0xff, 0xff, 0xff};

   dec.reset();
   ec = dec.decode(data2, value);

   check_eq(expected_ec, ec);

   // Invalid 3
   std::array<uint8_t, 5> data3{0x00, 0x3f, 0xff, 0xff, 0xff};

   dec.reset();
   ec = dec.decode(data3, value);

   check_eq(expected_ec, ec);
}

TestCase("Huffman - Encode")
{
   hpack::HuffmanEncoder enc;

   std::vector<uint8_t> value;

   for (const auto& data : TestHuffmanDecEnc)
   {
      value.clear();

      enc.encode(data.text, value);

      check_true(std::equal(std::begin(data.raw), std::end(data.raw), 
                            std::begin(value), std::end(value)));
   }
}

struct TestHPackData
{
   std::vector<uint8_t> raw;
   std::vector<hpack::Header> headers;
};

static const std::array<TestHPackData, 3> TestHPackEncDec
{
   // The header field representation uses a literal name and a literal value.
   TestHPackData
   {
      {0x40, 0x0a, 0x63, 0x75, 0x73, 0x74, 0x6f, 0x6d, 0x2d, 0x6b, 0x65, 0x79, 0x0d,
       0x63, 0x75, 0x73, 0x74, 0x6f, 0x6d, 0x2d, 0x68, 0x65, 0x61, 0x64, 0x65, 0x72},
      {hpack::Header{"custom-key", "custom-header"}}
   },
   // The header field representation uses an indexed name and a literal value.
   TestHPackData
   {
      {0x04, 0x0c, 0x2f, 0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x2f, 0x70, 0x61, 0x74, 0x68},
      {hpack::Header{":path", "/sample/path"}}
   },
   // The header field representation uses an indexed header field, from the static table.
   TestHPackData
   {
      {0x82},
      {hpack::Header{":method", "GET"}}
   }
};

TestCase("HPack - Decode")
{
   for (const auto& data : TestHPackEncDec)
   {
      hpack::Decoder dec;

      auto res = dec.decode(data.raw);

      fail_if(res.error, DbgSrcLoc);

      check_eq(data.headers.size(), res.headers.size());

      check_true(data.headers == res.headers);
   }

}

TestCase("HPack - Decode request no huffman consecutive header sets")
{
   hpack::Decoder dec;

   // Shows several consecutive header sets, corresponding to HTTP requests, on the same connection.

   std::vector<hpack::Header> request_headers{
      hpack::Header{":method", "GET"},
      hpack::Header{":scheme", "http"},
      hpack::Header{":path", "/"},
      hpack::Header{":authority", "www.example.com"}
   };

   std::vector<uint8_t> data = {0x82, 0x86, 0x84, 0x01, 0x0f, 0x77, 0x77, 0x77, 0x2e, 0x65,
                                0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d};

   auto res = dec.decode(data);

   fail_if(res.error, DbgSrcLoc);

   check_eq(request_headers.size(), res.headers.size());

   check_true(request_headers == res.headers);

   // This request takes advantage of the differential encoding of header sets.
   std::vector<hpack::Header> request_headers2{
      hpack::Header{":method", "GET"},
      hpack::Header{":scheme", "http"},
      hpack::Header{":path", "/"},
      hpack::Header{":authority", "www.example.com"},
      hpack::Header{"cache-control", "no-cache"}
   };
   
   data = {0x82, 0x86, 0x84, 0x01, 0x0f, 0x77, 0x77, 0x77, 0x2e, 0x65, 0x78, 0x61, 0x6d, 
           0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d, 0x0f, 0x09, 0x08, 0x6e, 0x6f, 0x2d, 
           0x63, 0x61, 0x63, 0x68, 0x65};

   res = dec.decode(data);

   fail_if(res.error, DbgSrcLoc);

   check_eq(request_headers2.size(), res.headers.size());

   check_true(request_headers2 == res.headers);

   std::vector<hpack::Header> request_headers3{
      hpack::Header{":method", "GET"},
      hpack::Header{":scheme", "https"},
      hpack::Header{":path", "/index.html"},
      hpack::Header{":authority", "www.example.com"},
      hpack::Header{"custom-key", "custom-value"}
   };

   data = {0x82, 0x87, 0x85, 0x01, 0x0f, 0x77, 0x77, 0x77, 0x2e, 0x65, 0x78, 0x61, 0x6d, 
           0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d, 0x40, 0x0a, 0x63, 0x75, 0x73, 0x74, 
           0x6f, 0x6d, 0x2d, 0x6b, 0x65, 0x79, 0x0c, 0x63, 0x75, 0x73, 0x74, 0x6f, 0x6d, 
           0x2d, 0x76, 0x61, 0x6c, 0x75, 0x65};

   res = dec.decode(data);

   fail_if(res.error, DbgSrcLoc);

   check_eq(request_headers3.size(), res.headers.size());

   check_true(request_headers3 == res.headers);
}

TestCase("HPack - Decode request with huffman consecutive header sets")
{
   hpack::Decoder dec;

   // Shows several consecutive header sets, corresponding to HTTP requests, on the same connection.

   std::vector<hpack::Header> request_headers{
      hpack::Header{":method", "GET"},
      hpack::Header{":scheme", "http"},
      hpack::Header{":path", "/"},
      hpack::Header{":authority", "www.example.com"}
   };

   std::vector<uint8_t> data = {0x82, 0x86, 0x84, 0x01, 0x8c, 0xf1, 0xe3, 0xc2, 0xe5, 0xf2, 
                                0x3a, 0x6b, 0xa0, 0xab, 0x90, 0xf4, 0xff};

   auto res = dec.decode(data);

   fail_if(res.error, DbgSrcLoc);

   check_eq(request_headers.size(), res.headers.size());

   check_true(request_headers == res.headers);

   std::vector<hpack::Header> request_headers2{
      hpack::Header{":method", "GET"},
      hpack::Header{":scheme", "http"},
      hpack::Header{":path", "/"},
      hpack::Header{":authority", "www.example.com"},
      hpack::Header{"cache-control", "no-cache"}
   };
   
   data = {0x82, 0x86, 0x84, 0x01, 0x8c, 0xf1, 0xe3, 0xc2, 0xe5, 0xf2, 0x3a, 0x6b, 0xa0, 0xab, 
           0x90, 0xf4, 0xff, 0x0f, 0x09, 0x86, 0xa8, 0xeb, 0x10, 0x64, 0x9c, 0xbf};

   res = dec.decode(data);

   fail_if(res.error, DbgSrcLoc);

   check_eq(request_headers2.size(), res.headers.size());

   check_true(request_headers2 == res.headers);

   std::vector<hpack::Header> request_headers3{
      hpack::Header{":method", "GET"},
      hpack::Header{":scheme", "https"},
      hpack::Header{":path", "/index.html"},
      hpack::Header{":authority", "www.example.com"},
      hpack::Header{"custom-key", "custom-value"}
   };

   data = {0x82, 0x87, 0x85, 0x01, 0x8c, 0xf1, 0xe3, 0xc2, 0xe5, 0xf2, 0x3a, 0x6b, 0xa0, 0xab, 
           0x90, 0xf4, 0xff, 0x40, 0x88, 0x25, 0xa8, 0x49, 0xe9, 0x5b, 0xa9, 0x7d, 0x7f, 0x89, 
           0x25, 0xa8, 0x49, 0xe9, 0x5b, 0xb8, 0xe8, 0xb4, 0xbf};

   res = dec.decode(data);

   fail_if(res.error, DbgSrcLoc);

   check_eq(request_headers3.size(), res.headers.size());

   check_true(request_headers3 == res.headers);
}

TestCase("Server - Contruction")
{
   Server s = make_server();
}

} // TestSuite(OrionNet)
