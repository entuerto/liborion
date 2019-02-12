//
// Utils.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_TCP_UTILS_H
#define ORION_NET_TCP_UTILS_H

#include <orion/Common.h>

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

using HandlerFunc = std::function<std::error_code(asio::streambuf&, asio::streambuf&)>;

using ConnectHandler = std::function<void(const std::error_code&)>;
using AcceptHandler  = std::function<void(const std::error_code&)>;
using ReadHandler    = std::function<void(const std::error_code&, asio::streambuf&)>;
using WriteHandler   = std::function<void(const std::error_code&, std::size_t)>;

//-------------------------------------------------------------------------------------------------
// Tcp Options
using NoDelay = Option<bool, struct NoDelayTag>;

//-------------------------------------------------------------------------------------------------

class Handler 
   : public std::enable_shared_from_this<Handler>
{
public:
   DEFAULT_COPY(Handler)
   DEFAULT_MOVE(Handler)

   enum class State : uint8_t
   {
      Read,
      Write,
      Closed
   };

   Handler() = default;
   virtual ~Handler() = default;
   
   /// Returns true if the handler wants to receive data from the remote peer.
   constexpr bool read_wanted() const;

   /// Returns true if the handler wants to send data to the remote peer.
   constexpr bool write_wanted() const;

   constexpr bool should_stop() const;

   constexpr State state() const;
   constexpr void state(State value);

   virtual void signal_write();
   
   virtual std::error_code on_read(asio::streambuf&) = 0;

   virtual std::error_code on_write(asio::streambuf&) = 0;

private:
   State _state{State::Closed};
};

} // tcp
} // net
} // orion

#include <orion/net/tcp/impl/Utils.ipp>

#endif // ORION_NET_TCP_UTILS_H
