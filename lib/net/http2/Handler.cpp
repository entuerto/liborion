//
// Handler.cpp
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include "Handler.h"

#include <orion/Log.h>
#include <orion/net/http2/Error.h>

#include <fmt/format.h>

namespace orion
{
namespace net
{
namespace http2
{
//--------------------------------------------------------------------------------------------------
// 

//--------------------------------------------------------------------------------------------------
// HandlerImpl

Handler::Handler(asio::io_context& io_context, http::RequestMux& mux)
   : _io_context(io_context)
   , _mux(mux)
{
}

Handler::~Handler()
{
   // Close up all active stream
   // TODO

   _statistics.end_time = std::chrono::high_resolution_clock::now();
}

const Settings& Handler::local_settings() const
{
   return _local_settings;
}

const Settings& Handler::remote_settings() const
{
   return _remote_settings;
}

bool Handler::read_wanted() const
{
   return false;
}

bool Handler::write_wanted() const
{
   return false;
}

bool Handler::should_stop() const
{
   return not read_wanted() and not write_wanted();
}

void Handler::signal_write()
{
   
}

//--------------------------------------------------------------------------------------------------
//

std::error_code Handler::on_read(asio::const_buffer buffer, std::size_t len)
{
   // TODO: Parse buffer

   _statistics.data_received += len;

   return {};
}

std::error_code Handler::on_write(asio::mutable_buffer& buffer, std::size_t& len)
{
   len = 0;

   for (;;)
   {
      auto bytes_read = 0;
      if (bytes_read < 0)
         return make_error_code(bytes_read);

      if (bytes_read == 0)
         break;

      //asio::buffer_copy(buffer, asio::buffer(data, bytes_read));

      len += bytes_read;
      _statistics.data_sent += bytes_read;
   }

   return {};
}

//--------------------------------------------------------------------------------------------------
// Frame handlers


} // namespace http2
} // namespace net
} // namespace orion
