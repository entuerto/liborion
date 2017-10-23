//
// Session.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <orion/net/tcp/Session.h>

#include <net/tcp/SessionImpl.h>

namespace orion
{
namespace net
{
namespace tcp
{

Session::Session()
   : _impl(std::make_shared<SessionImpl>())
{
}

Session::Session(Session&& rhs) noexcept
   : _impl(std::move(rhs._impl))
{
}

Session::~Session()
{
}

Session& Session::operator=(Session&& rhs) noexcept
{
   _impl = std::move(rhs._impl);

   return *this;
}

void Session::set_option(const Parameters& params)
{
   _impl->set_option(params);
}

void Session::set_option(Parameters&& params)
{
   _impl->set_option(std::move(params));
}

void Session::set_option(const Timeout& timeout)
{
   _impl->set_option(timeout);
}

bool Session::connected() const
{
   return _impl->connected();
}

void Session::on_connect(ConnectHandler h)
{
   _impl->on_connect(std::move(h));
}

void Session::on_read(ReadHandler h)
{
   _impl->on_read(std::move(h));
}

void Session::on_write(WriteHandler h)
{
   _impl->on_write(std::move(h));
}

void Session::connect(const std::string& addr, int port)
{
   _impl->connect(addr, port);
}

void Session::write(std::streambuf* streambuf)
{
   _impl->write(streambuf);
}

void Session::write(const uint8_t* data, std::size_t len)
{
   _impl->write(data, len);
}

std::error_code Session::close()
{
   return _impl->close();
}

} // tcp
} // net
} // orion
