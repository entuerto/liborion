//
// Utils.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_UTILS_H
#define ORION_NET_UTILS_H

#include <orion/Orion-Stddefs.h>

#include <chrono>
#include <initializer_list>
#include <string>
#include <vector>

namespace orion
{
namespace net
{
//-------------------------------------------------------------------------------------------------

struct Parameter
{
   std::string key;
   std::string value;
};

class Parameters
{
public:
   Parameters() = default;
   Parameters(const std::initializer_list<Parameter>& p)
      : _params(p)
   {
   }

   void add(const Parameter& p) { _params.push_back(p); }

   std::vector<Parameter> _params;
};

//-------------------------------------------------------------------------------------------------

struct Timeout
{
   std::chrono::milliseconds ms;

   constexpr operator std::chrono::milliseconds() const noexcept { return ms; }
};

//-------------------------------------------------------------------------------------------------
template <int idx, typename T>
struct Option
{
   T value;

   constexpr operator T() const noexcept { return value; }   
};

//-------------------------------------------------------------------------------------------------
// Socket Options

using KeepAlive               = Option<1, bool>;
using Broadcast               = Option<2, bool>;
using Debug                   = Option<3, bool>;
using DoNotRoute              = Option<4, bool>;
using EnableConnectionAborted = Option<5, bool>;
using ReuseAddress            = Option<6, bool>;
using ReceiveBufferSize       = Option<7, std::size_t>;
using ReceiveLowWatermark     = Option<8, std::size_t>;
using SendBufferSize          = Option<9, std::size_t>;
using SendLowWatermark        = Option<10, std::size_t>;

struct Linger
{
   bool value;
   int  timeout;

   constexpr operator bool() const noexcept { return value; }
};

} // namespace net
} // namespace orion

#endif // ORION_NET_UTILS_H
