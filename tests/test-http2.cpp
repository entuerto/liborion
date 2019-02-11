//
//  test-http2.cpp
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Log.h>
#include <orion/Test.h>

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

} // TestSuite(OrionNet)
