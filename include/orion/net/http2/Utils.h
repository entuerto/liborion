//
// Utils.h
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP2_UTILS_H
#define ORION_NET_HTTP2_UTILS_H

#include <orion/Config.h>

#include <asio.hpp>

#include <functional>

namespace orion
{
namespace net
{
namespace http2
{

using CloseHandler   = std::function<void()>;
using ConnectHandler = std::function<void(const asio::ip::tcp::endpoint&)>;
using ErrorHandler   = std::function<void(const std::error_code&)>;
using GoAwayHandler  = std::function<void(const std::error_code&, asio::const_buffer)>;
using PingHandler    = std::function<void(asio::const_buffer)>;

} // namespace http2
} // namespace net
} // namespace orion

#endif // ORION_NET_HTTP2_UTILS_H