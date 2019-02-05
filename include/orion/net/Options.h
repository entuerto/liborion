//
// Options.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_OPTIONS_H
#define ORION_NET_OPTIONS_H

#include <orion/Config.h>

#include <chrono>
#include <string>
#include <vector>

namespace orion
{
namespace net
{

struct Timeout
{
   std::chrono::milliseconds ms;

   constexpr operator std::chrono::milliseconds() const noexcept { return ms; }
};


template <typename T, typename Tag>
struct Option
{
   T value;

   constexpr operator T() const noexcept { return value; }   
};

//-------------------------------------------------------------------------------------------------
// Socket Options

using KeepAlive               = Option<bool, struct KeepAliveTag>;
using Broadcast               = Option<bool, struct BroadcastTag>;
using Debug                   = Option<bool, struct DebugTag>;
using DoNotRoute              = Option<bool, struct DoNotRouteTag>;
using EnableConnectionAborted = Option<bool, struct EnableConnectionAbortedTag>;
using ReuseAddress            = Option<bool, struct ReuseAddressTag>;
using ReceiveBufferSize       = Option<std::size_t, struct ReceiveBufferSizeTag>;
using ReceiveLowWatermark     = Option<std::size_t, struct ReceiveLowWatermarkTag>;
using SendBufferSize          = Option<std::size_t, struct SendBufferSizeTag>;
using SendLowWatermark        = Option<std::size_t, struct SendLowWatermarkTag>;

struct Linger
{
   bool value;
   int  timeout;

   constexpr operator bool() const noexcept { return value; }
};

} // namespace net
} // namespace orion

#endif // ORION_NET_OPTIONS_H
