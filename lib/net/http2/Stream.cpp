//
// Stream.cpp
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/http2/Stream.h>

namespace orion
{
namespace net
{
namespace http2
{

Stream::Stream(uint32_t id, uint32_t in_window_size, uint32_t out_window_size)
   : _id(id)
   , _outbound_flow_control_window(in_window_size)
   , _inbound_flow_control_window(out_window_size)
{

}

void Stream::send_headers(const Headers& headers, int encoder, bool end_stream /* = false */)
{

}

void Stream::receive_headers(const Headers& headers, bool end_stream)
{

}

void Stream::Close(int32_t code)
{

}

} // namespace http2
} // namespace net
} // namespace orion
