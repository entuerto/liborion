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

using HandlerFunc = std::function<std::error_code(asio::streambuf&, asio::streambuf&)>;

using ConnectHandler = std::function<void(const std::error_code&)>;
using AcceptHandler  = std::function<void(const std::error_code&)>;
using ReadHandler    = std::function<void(const std::error_code&, asio::streambuf&)>;
using WriteHandler   = std::function<void(const std::error_code&, std::size_t)>;

std::unique_ptr<std::streambuf> make_buffer(std::size_t max_size = 0);

//-------------------------------------------------------------------------------------------------
// Tcp Options
using NoDelay = Option<bool, struct NoDelayTag>;

//-------------------------------------------------------------------------------------------------

class API_EXPORT Handler 
{
public:
   enum class State : uint8_t
   {
      Read,
      Write,
      Close
   };

   virtual ~Handler() = default;
   
   /// Returns true if session wants to receive data from the remote peer.
   bool read_wanted() const;

   /// Returns true if session wants to send data to the remote peer.
   bool write_wanted() const;

   bool should_stop() const;

   State state() const;
   void state(State value);

   virtual void signal_write();
   
   virtual std::error_code on_read(asio::streambuf&) = 0;

   virtual std::error_code on_write(asio::streambuf&) = 0;

private:
   State _state{State::Close};
};

} // tcp
} // net
} // orion
#endif // ORION_NET_TCP_UTILS_H
