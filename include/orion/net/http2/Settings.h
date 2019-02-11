//
// Settings.h
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP2_SETTINGS_H
#define ORION_NET_HTTP2_SETTINGS_H

#include <orion/Common.h>

namespace orion
{
namespace net
{
namespace http2
{
//--------------------------------------------------------------------------------------------------
// Settings

template<typename T, int ID, typename Tag>
struct Setting
{
   T value;

   constexpr int id() const noexcept { return ID; }
   constexpr operator T() const noexcept { return value; }
};

using HeaderTableSize      = Setting<uint32_t, 0x1, struct HeaderTableSizeTag>;
using EnablePush           = Setting<bool,     0x2, struct EnablePushTag>;
using MaxConcurrentStreams = Setting<uint32_t, 0x3, struct MaxConcurrentStreamsTag>;
using InitialWindowSize    = Setting<uint32_t, 0x4, struct InitialWindowSizeTag>;
using MaxFrameSize         = Setting<uint32_t, 0x5, struct MaxFrameSizeTag>;
using MaxHeaderListSize    = Setting<uint32_t, 0x6, struct MaxHeaderListSizeTag>;

//--------------------------------------------------------------------------------------------------
// Defaults

constexpr static const HeaderTableSize      DEFAULT_SETTINGS_HEADER_TABLE_SIZE{4096};
constexpr static const EnablePush           DEFAULT_SETTINGS_ENABLE_PUSH{true};
constexpr static const MaxConcurrentStreams DEFAULT_SETTINGS_MAX_CONCURRENT_STREAMS{100};
constexpr static const InitialWindowSize    DEFAULT_SETTINGS_INITIAL_WINDOW_SIZE{65535};
constexpr static const MaxFrameSize         DEFAULT_SETTINGS_MAX_FRAME_SIZE{16384};
constexpr static const MaxHeaderListSize    DEFAULT_SETTINGS_MAX_HEADER_LIST_SIZE{65535};

//--------------------------------------------------------------------------------------------------

/// These are the standard HTTP/2 settings initialised with the defaults as specified by the RFC
class Settings
{
public:
   Settings() = default;

   template<typename... Ts>
   Settings(Ts&&... ts)
   {
      set(std::forward<Ts>(ts)...);
   }

   template<typename T>
   constexpr T get() const;

   template<typename T, typename... Ts>
   constexpr void set(T&& t, Ts&&... ts)
   {
      set(std::forward<T>(t));
      set(std::forward<Ts>(ts)...);
   }

   constexpr void set(HeaderTableSize value);
   constexpr void set(EnablePush value);
   constexpr void set(MaxConcurrentStreams value);
   constexpr void set(InitialWindowSize value);
   constexpr void set(MaxFrameSize value);
   constexpr void set(MaxHeaderListSize value);

   static constexpr bool is_valid(const InitialWindowSize& value);
   static constexpr bool is_valid(const MaxFrameSize& value);

private:
   HeaderTableSize _header_table_size{DEFAULT_SETTINGS_HEADER_TABLE_SIZE};
   EnablePush _enable_push{DEFAULT_SETTINGS_ENABLE_PUSH};
   MaxConcurrentStreams _max_concurrent_streams{DEFAULT_SETTINGS_MAX_CONCURRENT_STREAMS};
   InitialWindowSize _initial_window_size{DEFAULT_SETTINGS_INITIAL_WINDOW_SIZE};
   MaxFrameSize _max_frame_size{DEFAULT_SETTINGS_MAX_FRAME_SIZE};
   MaxHeaderListSize _max_header_list_size{DEFAULT_SETTINGS_MAX_HEADER_LIST_SIZE};
};

} // namespace http2
} // namespace net
} // namespace orion

#include <orion/net/http2/impl/Settings.ipp>

#endif // ORION_NET_HTTP2_SETTINGS_H