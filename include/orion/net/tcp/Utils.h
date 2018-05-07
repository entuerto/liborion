//
// Utils.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_TCP_UTILS_H
#define ORION_NET_TCP_UTILS_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/Options.h>

#include <asio.hpp>

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

using Handler = std::function<std::error_code(asio::streambuf&, asio::streambuf&)>;

using ConnectHandler = std::function<void(const std::error_code&)>;
using AcceptHandler  = std::function<void(const std::error_code&)>;
using ReadHandler    = std::function<void(const std::error_code&, asio::streambuf&)>;
using WriteHandler   = std::function<void(const std::error_code&, std::size_t)>;

std::unique_ptr<std::streambuf> make_buffer(std::size_t max_size = 0);

//-------------------------------------------------------------------------------------------------
// Tcp Options
using NoDelay = Option<bool, struct NoDelayTag>;

} // tcp
} // net
} // orion
#endif // ORION_NET_TCP_UTILS_H
