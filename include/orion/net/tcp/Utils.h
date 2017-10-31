//
// Utils.h
//
//  Created by Tomas Palazuelos on 2017-10-13.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_NET_TCP_UTILS_H
#define ORION_NET_TCP_UTILS_H

#include <orion/Orion-Stddefs.h>

#include <functional>
#include <memory>
#include <streambuf>

namespace orion
{
namespace net
{
namespace tcp
{
//-------------------------------------------------------------------------------------------------

using Handler = std::function<std::error_code(std::streambuf*, std::streambuf*)>;

using ConnectHandler = std::function<void(const std::error_code&)>;
using AcceptHandler  = std::function<void(const std::error_code&)>;
using ReadHandler    = std::function<void(const std::error_code&, std::streambuf*)>;
using WriteHandler   = std::function<void(const std::error_code&, std::size_t)>;

std::unique_ptr<std::streambuf> make_buffer(std::size_t max_size = 0);

//-------------------------------------------------------------------------------------------------
template <int idx, typename T>
struct Option
{
   T value;

   constexpr operator T() const noexcept { return value; }   
};


//-------------------------------------------------------------------------------------------------
// Tcp Options
using NoDelay = Option<0, bool>;

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

} // tcp
} // net
} // orion
#endif // ORION_NET_TCP_UTILS_H
