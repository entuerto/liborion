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

TestCase("Server - Contruction")
{
   Server s = make_server();
}

} // TestSuite(OrionNet)
