//
// Connection.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#include <orion/net/Connection.h>

#include <orion/Log.h>

namespace orion
{
namespace net
{

//---------------------------------------------------------------------------------------
// Connection

Connection::Connection(asio::io_service& io_service)
   : _local_endpoint()
   , _remote_endpoint()
   , _read_deadline()
   , _write_deadline()
   , _read_deadline_timer(io_service)
   , _write_deadline_timer(io_service)
{
}

Connection::~Connection()
{
   _read_deadline_timer.cancel();
   _write_deadline_timer.cancel();
}

const EndPoint& Connection::local_endpoint() const
{
   return _local_endpoint;
}

void Connection::local_endpoint(const EndPoint& value)
{
   _local_endpoint = value;
}

const EndPoint& Connection::remote_endpoint() const
{
   return _remote_endpoint;
}

void Connection::remote_endpoint(const EndPoint& value)
{
   _remote_endpoint = value;
}

std::error_code Connection::deadline(const std::chrono::seconds& sec)
{
   _read_deadline  = sec;
   _write_deadline = sec;
   return std::error_code();
}

std::chrono::seconds Connection::deadline() const
{
   return _read_deadline;
}

std::error_code Connection::read_deadline(const std::chrono::seconds& sec)
{
   _read_deadline = sec;
   return std::error_code();
}

std::chrono::seconds Connection::read_deadline() const
{
   return _read_deadline;
}

std::error_code Connection::write_deadline(const std::chrono::seconds& sec)
{
   _write_deadline = sec;
   return std::error_code();
}

std::chrono::seconds Connection::write_deadline() const
{
   return _write_deadline;
}

void Connection::on_read_timeout(const std::error_code& ec)
{
   if (ec == asio::error::operation_aborted)
      return;

   LOG_FUNCTION(Debug, "Connection::on_read_timeout()")

   log::debug(ec);

   close();
}

void Connection::on_write_timeout(const std::error_code& ec)
{
   if (ec == asio::error::operation_aborted)
      return;

   LOG_FUNCTION(Debug, "Connection::on_write_timeout()")

   log::debug(ec);

   close();
}

void Connection::start_read_timer()
{
   if (read_deadline() == std::chrono::seconds::zero())
      return;

   _read_deadline_timer.expires_from_now(read_deadline());

   _read_deadline_timer.async_wait(std::bind(
      &Connection::on_read_timeout, this, std::placeholders::_1));
}

void Connection::start_write_timer()
{
   if (write_deadline() == std::chrono::seconds::zero())
      return;

   _write_deadline_timer.expires_from_now(write_deadline());

   _write_deadline_timer.async_wait(std::bind(
      &Connection::on_write_timeout, this, std::placeholders::_1));
}

SteadyTimer& Connection::read_deadline_timer()
{
   return _read_deadline_timer;
}

SteadyTimer& Connection::write_deadline_timer()
{
   return _write_deadline_timer;
}


} // net
} // orion
