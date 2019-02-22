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
#include <vector>

namespace orion
{
namespace net
{
namespace http2
{
//--------------------------------------------------------------------------------------------------
// Header

struct Header
{
   std::string name;
   std::string value;
   bool indexable{true};

   // Calculates the size of a single entry + 32
   std::size_t size() const { return name.size() + value.size() + 32; }
};

inline bool operator==(const Header& h1, const Header& h2) noexcept
{
   return h1.name == h2.name and h1.value == h2.value and h1.indexable == h2.indexable;
}

inline bool operator!=(const Header& h1, const Header& h2) noexcept
{
   return not (h1 == h2);
}

//--------------------------------------------------------------------------------------------------
// Headers

using Headers = std::vector<Header>;

inline bool operator==(const Headers& h1, const Headers& h2) noexcept
{
   return std::equal(std::begin(h1), std::end(h1), std::begin(h2), std::end(h2));
}

inline bool operator!=(const Headers& h1, const Headers& h2) noexcept
{
   return not (h1 == h2);
}

//--------------------------------------------------------------------------------------------------
// Callback functions

using CloseHandler   = std::function<void()>;
using ConnectHandler = std::function<void(const asio::ip::tcp::endpoint&)>;
using ErrorHandler   = std::function<void(const std::error_code&)>;
using GoAwayHandler  = std::function<void(const std::error_code&, asio::const_buffer)>;
using PingHandler    = std::function<void(asio::const_buffer)>;

} // namespace http2
} // namespace net
} // namespace orion

#endif // ORION_NET_HTTP2_UTILS_H